#include <sstream>
#include "GetTaskMessage.h"

GetTaskMessage::GetTaskMessage(MessageTransType transType, PriorityType priority, MessageDistriubite distribute)
    :BaseMessage(Get_Task_Message, transType, priority, distribute)
{

}

std::string GetTaskMessage::toString()
{
    return BaseMessage::toString();
}

/*
 * 查看任务的回应
 */
GetTaskResponse::GetTaskResponse(Json::Value info, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error)
    :BaseResponse(message, error)
{
    mTaskInfo = info;
}

// 获取回应的信息
Json::Value GetTaskResponse::getTaskInfo()
{
    return mTaskInfo;
}

std::string GetTaskResponse::toString()
{
    std::stringstream buf;
    buf << "task info:" << mTaskInfo.toStyledString() << ", " << BaseResponse::toString();
    return buf.str();
}
