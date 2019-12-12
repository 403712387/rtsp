#ifndef RTSP_TASK_H
#define RTSP_TASK_H
#include <string>
#include <QDateTime>
#include "Common.h"
#include "liveMedia.hh"
#include "jsoncpp/json.h"

/*
 * rtsp的任务类，每路rtsp流对应一个rtsp任务
*/
class RtspTask
{
public:
    RtspTask(RTSPServer *server, UsageEnvironment *environment);

    // 开始任务
    bool startTask(std::string file);

    // 获取流的名称
    std::string getStreamName();

    // 结束任务
    bool stopTask();

    // 获取文件名称
    std::string getFileName();

    // 是否发送完毕
    bool isFinish();

    Json::Value toJson();

private:
    // 创建rtsp session
    ServerMediaSession* createSession(UsageEnvironment& env, char const* fileName);

private:
    std::string         mClassName = "RtspTask";
    QDateTime           mBirthday = QDateTime::currentDateTime();
    RTSPServer          *mRtspServer = NULL;
    UsageEnvironment    *mEnvironment = NULL;
    ServerMediaSession  *mMediaSession = NULL;

    std::string         mFileName;  // 文件名称
};

#endif
