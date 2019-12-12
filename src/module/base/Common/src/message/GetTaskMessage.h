#ifndef GET_TASK_MESSAGE_H
#define GET_TASK_MESSAGE_H
#include "BaseMessage.h"
#include "jsoncpp/json.h"

/*
 * 查看任务
*/
class RTSP_EXPORT GetTaskMessage : public BaseMessage
{
public:
    GetTaskMessage(MessageTransType transType = Async_Trans_Message, PriorityType priority = Priority_Third, MessageDistriubite distribute = Message_Unicast);

    virtual std::string toString();

private:
};

/*
 * 查看任务的回应
 */
class RTSP_EXPORT GetTaskResponse : public BaseResponse
{
public:
    GetTaskResponse(Json::Value info, std::shared_ptr<BaseMessage> message, std::shared_ptr<Error> error);

    // 获取回应的信息
    Json::Value getTaskInfo();

    virtual std::string toString();

private:
    Json::Value     mTaskInfo;
};
#endif
