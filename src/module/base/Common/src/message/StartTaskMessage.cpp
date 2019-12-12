#include <sstream>
#include "StartTaskMessage.h"

StartTaskMessage::StartTaskMessage(std::string file, MessageTransType transType ,PriorityType priority , MessageDistriubite distribute)
    :BaseMessage(Start_Task_Message, transType, priority, distribute)
{
    mFileName = file;
}

// 获取文件名称
std::string StartTaskMessage::getFileName()
{
    return mFileName;
}

std::string StartTaskMessage::toString()
{
    std::stringstream buf;
    buf << "file name:" << mFileName << ", " << BaseMessage::toString();
    return buf.str();
}

/*
 * 开始任务的回应
 */
StartTaskResponse::StartTaskResponse(std::string stream, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error)
    :BaseResponse(message, error)
{
    mStreamName = stream;
}

// 获取流的名称
std::string StartTaskResponse::getStreamName()
{
    return mStreamName;
}

std::string StartTaskResponse::toString()
{
    std::stringstream buf;
    buf << "stream name:" << mStreamName << ", " << BaseResponse::toString();
    return buf.str();
}
