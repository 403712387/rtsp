#include <sstream>
#include "ConfigureInfo.h"

// 日志级别
void ConfigureInfo::setLogLevel(common_log::Log_Level level)
{
    mLogLevel = level;
    common_log::setLogLevel(mLogLevel);
}

std::string ConfigureInfo::getLogLevel()
{
    return Common::getLogLevelName(mLogLevel);
}

// 日志的关键字
void ConfigureInfo::setLogKeyword(std::string keyword)
{
    mLogKeyword = keyword;
    common_log::setFilterWord(mLogKeyword);
}

std::string ConfigureInfo::getLogKeyword()
{
    return mLogKeyword;
}
// 设置本机监听的端口
void ConfigureInfo::setHttpPort(int port)
{
    mHttpPort = port;
}

int ConfigureInfo::getHttpPort()
{
    return mHttpPort;
}

// 设置rtsp端口
void ConfigureInfo::setRtspPort(int port)
{
    mRtspPort = port;
}

int ConfigureInfo::getRtspPort()
{
    return mRtspPort;
}

std::string ConfigureInfo::toString()
{
    std::stringstream buf;
    buf << "log level:" << Common::getLogLevelName(mLogLevel);
    if (!mLogKeyword.empty())
    {
        buf << ", log keyword:" << mLogKeyword;
    }
    buf << ", http port:" << mHttpPort << ", rtsp port:" << mRtspPort;
    return buf.str();
}

Json::Value ConfigureInfo::toJson()
{
    Json::Value result;

    // log相关的信息
    result["log"]["level"] = getLogLevel();
    if (!mLogKeyword.empty())
    {
        result["log"]["keyword"] = mLogKeyword;
    }

    // 本服务的设置
    std::string videoService = "service";
    result[videoService]["http_port"] = mHttpPort;
    result[videoService]["rtsp_port"] = mRtspPort;

    return result;
}
