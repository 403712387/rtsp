#ifndef RTSP_MODULE_H
#define RTSP_MODULE_H
#include <QMap>
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include "Common.h"
#include "BaseProcess.h"

/*
 * rtsp管理类
 */
class RtspTask;
class RTSPServer;
class ConfigureInfo;
class UsageEnvironment;
class RTSP_EXPORT RtspModule : public BaseProcess
{
public:
    RtspModule(MessageRoute *messageRoute);

    virtual bool init();
    virtual void beginWork();

    // 卸载模块
    virtual void uninit();

    // 处理消息的函数
    virtual std::shared_ptr<BaseResponse> onProcessMessage(std::shared_ptr<BaseMessage> &message);

    // 处理消息的回应
    virtual void onProcessResponse(std::shared_ptr<BaseResponse> &response);

private:
    // 处理配置消息
    std::shared_ptr<BaseResponse> onProcessConfigMessage(std::shared_ptr<BaseMessage> &message);

    // 处理开始任务的消息
    std::shared_ptr<BaseResponse> onProcessStartTaskMessage(std::shared_ptr<BaseMessage> &message);

    // 处理停止任务的消息
    std::shared_ptr<BaseResponse> onProcessStopTaskMessage(std::shared_ptr<BaseMessage> &message);

    // 处理查看任务的消息
    std::shared_ptr<BaseResponse> onProcessGetTaskMessage(std::shared_ptr<BaseMessage> &message);

    // 开始监听
    bool startListen();

    // 停止监听
    bool stopListen();

    // 工作线程
    void rtspListenThread();

    // 添加task
    std::shared_ptr<RtspTask> addTask(std::string fileName);

    // 是否存在task
    bool isExistTask(std::string stream);

    // 根据stream获取task
    std::shared_ptr<RtspTask> getTask(std::string stream);

    // 根据文件名找task
    std::shared_ptr<RtspTask> getTaskByFileName(std::string file);

    // 删除task
    bool removeTask(std::string stream);

private:
    std::shared_ptr<ConfigureInfo>  mConfigureInfo;

    QMap<std::string, std::shared_ptr<RtspTask>>        mMapTask;   // rtsp任务
    std::mutex                                          mMapTaskMutex;

    std::thread                     mRtspThread;        // rtsp的线程类
    RTSPServer                      *mRtspServer = NULL;
    UsageEnvironment                *mRtspEnvironment = NULL;
};

#endif
