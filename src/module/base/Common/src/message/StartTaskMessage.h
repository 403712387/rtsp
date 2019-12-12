#ifndef START_TASK_MESSAGE_H
#define START_TASK_MESSAGE_H
#include "BaseMessage.h"

/*
 * 开始任务
*/
class RTSP_EXPORT StartTaskMessage : public BaseMessage
{
public:
    StartTaskMessage(std::string file, MessageTransType transType = Async_Trans_Message,
                         PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    // 获取文件名称
    std::string getFileName();

    virtual std::string toString();

private:
    std::string      mFileName;
};

/*
 * 开始任务的回应
 */
class RTSP_EXPORT StartTaskResponse : public BaseResponse
{
public:
    StartTaskResponse(std::string stream, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error);

    // 获取流的名称
    std::string getStreamName();

    virtual std::string toString();

private:
    std::string     mStreamName;
};

#endif
