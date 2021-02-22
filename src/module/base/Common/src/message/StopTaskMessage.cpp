#include <sstream>
#include "StopTaskMessage.h"
StopTaskMessage::StopTaskMessage(std::string stream, MessageTransType transType ,PriorityType priority , MessageDistriubite distribute)
    :BaseMessage(Stop_Task_Message, transType, priority, distribute)
{
    mStreamName = stream;
}

// 获取流的名称
std::string StopTaskMessage::getStreamName()
{
    return mStreamName;
}

std::string StopTaskMessage::toString()
{
    std::stringstream buf;
    buf << "stream name:" << mStreamName << ", " << BaseMessage::toString();
    return buf.str();
}
