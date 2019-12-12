#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <sstream>
#include "Log.h"
#include "Common.h"
#include "RtspTask.h"
#include "liveMedia.hh"
#include "ConfigureInfo.h"
#include "GetTaskMessage.h"
#include "StopTaskMessage.h"
#include "StartTaskMessage.h"
#include "RtspModule.h"
#include "ConfigureInfoMessage.h"
#include "BasicUsageEnvironment.hh"
#include "jsoncpp/json.h"

RtspModule::RtspModule(MessageRoute *messageRoute)
    :BaseProcess(messageRoute, "RtspModule")
{
    // 订阅消息
    subscribeMessage(Start_Task_Message);
    subscribeMessage(Stop_Task_Message);
    subscribeMessage(Get_Task_Message);
    subscribeMessage(Config_Message);
}

bool RtspModule::init()
{
    LOG_I(mClassName, "begin init");
    LOG_I(mClassName, "end init");
    return true;
}

void RtspModule::beginWork()
{
    LOG_I(mClassName, "begin work");
}

// 反初始化
void RtspModule::uninit()
{
    BaseProcess::uninit();
    LOG_I(mClassName, "uninit rtsp module");
}

// 处理消息的函数
std::shared_ptr<BaseResponse> RtspModule::onProcessMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    switch(message->getMessageType())
    {
    case Config_Message:        // 配置信息
        response = onProcessConfigMessage(message);
        break;
    case Start_Task_Message:    // 开始任务
        response = onProcessStartTaskMessage(message);
        break;
    case Stop_Task_Message:     // 停止任务
        response = onProcessStopTaskMessage(message);
        break;
    case Get_Task_Message:  // 查看任务信息
        response = onProcessGetTaskMessage(message);
        break;
    }

    return response;
}

// 处理消息的回应
void RtspModule::onProcessResponse(std::shared_ptr<BaseResponse> &response)
{

}

// 处理配置消息
std::shared_ptr<BaseResponse> RtspModule::onProcessConfigMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    std::shared_ptr<ConfigureInfoMessage> configureMessage = std::dynamic_pointer_cast<ConfigureInfoMessage>(message);
    if (NULL == configureMessage.get())
    {
        LOG_E(mClassName, "receive configure message, but message is NULL, message info:" << message->toString());
        return response;
    }

    mConfigureInfo = configureMessage->getConfigureInfo();

    // 创建线程
    if (!mRtspThread.joinable())
    {
        mRtspThread = std::thread(&RtspModule::rtspListenThread, this);
    }
    return response;
}

// 处理开始任务的消息
std::shared_ptr<BaseResponse> RtspModule::onProcessStartTaskMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    std::shared_ptr<StartTaskMessage> startTask = std::dynamic_pointer_cast<StartTaskMessage>(message);
    if (NULL == startTask.get())
    {
        LOG_E(mClassName, "receive start task message, but message is NULL, message info:" << message->toString());
        return response;
    }

    // rtsp服务是否启动
    if (NULL == mRtspServer || NULL == mRtspEnvironment)
    {
        LOG_E(mClassName, "receive start task message, but rtsp server not start, message info:" << message->toString());
        response = std::make_shared<StartTaskResponse>("", message, Common::getError("rtsp server not start"));
        return response;
    }

    // 文件是否存在
    if (!QFile::exists(startTask->getFileName().c_str()))
    {
        LOG_E(mClassName, "receive start task message, but file not exist, message info:" << message->toString());
        response = std::make_shared<StartTaskResponse>("", message, Common::getError("file not exist"));
        return response;
    }

    // 建立任务
    std::shared_ptr<RtspTask> task = addTask(startTask->getFileName());
    if (NULL == task.get())
    {
        response = std::make_shared<StartTaskResponse>("", message, Common::getError("create rtsp stream fail"));
        return response;
    }

    std::string url = task->getStreamName();
    response = std::make_shared<StartTaskResponse>(url, message, Common::noError());
    return response;
}

// 处理停止任务的消息
std::shared_ptr<BaseResponse> RtspModule::onProcessStopTaskMessage(std::shared_ptr<BaseMessage> &message)
{
    std::shared_ptr<BaseResponse> response;
    std::shared_ptr<StopTaskMessage> stopTask = std::dynamic_pointer_cast<StopTaskMessage>(message);
    if (NULL == stopTask.get())
    {
        LOG_E(mClassName, "receive stop task message, but message is NULL, message info:" << message->toString());
        return response;
    }

    removeTask(stopTask->getStreamName());
    return response;
}

// 处理查看任务的消息
std::shared_ptr<BaseResponse> RtspModule::onProcessGetTaskMessage(std::shared_ptr<BaseMessage> &message)
{
    Json::Value json;

    int index = 0;
    std::unique_lock<std::mutex> autoLock(mMapTaskMutex);
    QMapIterator<std::string, std::shared_ptr<RtspTask>> iter(mMapTask);
    while(iter.hasNext())
    {
        iter.next();
        json["task"][index++] = iter.value()->toJson();
    }

    std::shared_ptr<GetTaskResponse> response = std::make_shared<GetTaskResponse>(json, message, Common::noError());
    return response;
}

// 开始监听
bool RtspModule::startListen()
{
    if (NULL == mConfigureInfo.get())
    {
        LOG_E(mClassName, "rtsp listen faill, configure info is NULL");
        return false;
    }

    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    mRtspEnvironment = BasicUsageEnvironment::createNew(*scheduler);

    mRtspServer = RTSPServer::createNew(*mRtspEnvironment, mConfigureInfo->getRtspPort(), NULL);
    mRtspEnvironment->taskScheduler().doEventLoop();
    return true;
}

// 添加task
std::shared_ptr<RtspTask> RtspModule::addTask(std::string fileName)
{
    std::shared_ptr<RtspTask> task = std::make_shared<RtspTask>(mRtspServer, mRtspEnvironment);
    bool ret = task->startTask(fileName);
    if (!ret)
    {
        LOG_E(mClassName, "start task fail, file name:" << fileName);
        task.reset();
        return task;
    }

    // 任务放入map中
    {
        std::unique_lock<std::mutex> autoLock(mMapTaskMutex);
        mMapTask[task->getStreamName()] = task;
        LOG_I(mClassName, "add task, file name:" << fileName << ", stream name:" << task->getStreamName());
    }

    return task;
}

// 是否存在task
bool RtspModule::isExistTask(std::string stream)
{
    std::unique_lock<std::mutex> autoLock(mMapTaskMutex);
    return mMapTask.count(stream);
}

// 根据stream获取task
std::shared_ptr<RtspTask> RtspModule::getTask(std::string stream)
{
    std::shared_ptr<RtspTask> task;
    std::unique_lock<std::mutex> autoLock(mMapTaskMutex);
    return mMapTask.value(stream, task);
}

// 删除task
bool RtspModule::removeTask(std::string stream)
{
    bool result = false;
    std::unique_lock<std::mutex> autoLock(mMapTaskMutex);
    std::shared_ptr<RtspTask> task = mMapTask.value(stream);
    if (NULL != task.get())
    {
        LOG_I(mClassName, "remove task, stream name:" << stream);
        result = true;
        task->stopTask();
        mMapTask.remove(stream);
    }
    return result;
}

// 工作线程
void RtspModule::rtspListenThread()
{
    LOG_I(mClassName, "begin rtsp thread");
    startListen();
    LOG_I(mClassName, "stop rtsp thread");
}
