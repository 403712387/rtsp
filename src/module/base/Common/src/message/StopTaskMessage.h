#ifndef STOP_TASK_MESSAGE_H
#define STOP_TASK_MESSAGE_H
#include "BaseMessage.h"

/*
 * 停止任务
*/
class RTSP_EXPORT StopTaskMessage : public BaseMessage
{
public:
    StopTaskMessage(std::string stream, MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取流的名称
    std::string getStreamName();

    virtual std::string toString();

private:
    std::string      mStreamName;
};

#endif
