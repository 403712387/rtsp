#include <malloc.h>
#include <QtCore/QDateTime>
#include "Log.h"
#include "JsonHelper.h"
#include "BaseMessage.h"
#include "ConfigureInfo.h"
#include "jsoncpp/json.h"

std::string JsonHelper::mClassName = "JsonHelper";

// 解析服务控制
ServiceOperateType JsonHelper::parseServiceControl(std::string &info)
{
    ServiceOperateType result = Operate_Exit;
    if (info.empty())
    {
        return result;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(info, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase service control fail");
        return result;
    }

    try
    {
        // 解析type
        if (!readerRoot.isMember("type"))
        {
            return result;
        }

        std::string value = readerRoot["type"].asString();
        if ("exit" == value)
        {
            result = Operate_Exit;
        }
        else if ("reload" == value)
        {
            result = Operate_Reload;
        }
    }
    catch (...)
    {
        LOG_E(mClassName, "prase service control exception");
    }

    return result;
}

// 解析配置
bool JsonHelper::parseConfigure(std::string &info, std::shared_ptr<ConfigureInfo> configure)
{
    if (info.empty())
    {
        LOG_E(mClassName, "parse configure fail, configure is empty");
        return false;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(info, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase json info fail, info:" << info);
        return false;
    }

    try
    {
        // 日志设置
        std::string logKey = "log";
        if (readerRoot.isMember(logKey))
        {
            // 级别
            if (readerRoot[logKey].isMember("level"))
            {
                configure->setLogLevel(Common::getLogLevelFromString(readerRoot[logKey]["level"].asString()));
            }

            // 关键字
            if (readerRoot[logKey].isMember("keyword"))
            {
                configure->setLogKeyword(readerRoot[logKey]["keyword"].asString());
            }
        }

        // http设置
        std::string rtspService = "rtsp_service";
        if (readerRoot.isMember(rtspService))
        {
            // http port
            if (readerRoot[rtspService].isMember("http_port"))
            {
                int httpPort = readerRoot[rtspService]["http_port"].asInt();
                configure->setHttpPort(httpPort);
            }

            // rtsp port
            if (readerRoot[rtspService].isMember("rtsp_port"))
            {
                int rtspPort = readerRoot[rtspService]["rtsp_port"].asInt();
                configure->setRtspPort(rtspPort);
            }
        }
    }
    catch (...)
    {
        LOG_E(mClassName, "prase configure exception");
        return false;
    }

    return true;
}


// 解析开始任务
std::string JsonHelper::parseFilePath(std::string &info)
{
    std::string result;
    if (info.empty())
    {
        return result;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(info, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase file path fail");
        return result;
    }

    try
    {
        // 解析file
        if (readerRoot.isMember("file"))
        {
            result = readerRoot["file"].asString();
        }
    }
    catch (...)
    {
        LOG_E(mClassName, "prase file path exception");
    }

    return result;
}

// 解析停止任务
std::string JsonHelper::parseStreamName(std::string &info)
{
    std::string result;
    if (info.empty())
    {
        return result;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(info, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase stream fail");
        return result;
    }

    try
    {
        // 解析file
        if (readerRoot.isMember("stream"))
        {
            result = readerRoot["stream"].asString();
        }
    }
    catch (...)
    {
        LOG_E(mClassName, "prase stream exception");
    }

    return result;
}

// 转换为json
Json::Value JsonHelper::stringToJson(std::string key, std::string value)
{
    Json::Value result;
    result[key] = value;
    return result;
}
