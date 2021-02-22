#include <QtCore/QFile>
#include <QtCore/QString>
#include <sstream>
#include "ConfigureInfo.h"
#include "ConfigureJson.h"
#include "jsoncpp/json.h"

ConfigureJson::ConfigureJson(std::string file)
{
    mConfigureFile = file;
    loadConfigure();
}

// 从配置文件中加载配置信息
bool ConfigureJson::loadConfigure()
{
    std::string data = Common::readFile(mConfigureFile);
    if (data.empty())
    {
        return false;
    }

    Json::Reader reader;
    Json::Value readerRoot;
    bool ret = reader.parse(data, readerRoot);
    if (false == ret)
    {
        LOG_E(mClassName, "prase json info fail, info:" << data);
        return false;
    }

    std::string rtspService = "rtsp_service";
    if (readerRoot.isMember(rtspService))
    {
        // http port
        if (readerRoot[rtspService].isMember("http_port"))
        {
            mHttpPort = readerRoot[rtspService]["http_port"].asInt();
        }

        // rtsp port
        if (readerRoot[rtspService].isMember("rtsp_port"))
        {
            mRtspPort = readerRoot[rtspService]["rtsp_port"].asInt();
        }
    }

    return true;
}

// 获取http接口
int ConfigureJson::getHttpPort(int defaultPort)
{
    return (mHttpPort > 0) ? mHttpPort : defaultPort;
}

// 设置http接口
void ConfigureJson::setHttpPort(int port)
{
    if (mHttpPort == port)
    {
        return;
    }

    mHttpPort = port;

    // 保存到配置文件
    saveConfigure();
}

// 设置rtsp接口
int ConfigureJson::getRtspPort(int defaultPort)
{
    return (mRtspPort > 0) ? mRtspPort : defaultPort;
}

// 设置rtsp接口
void ConfigureJson::setRtspPort(int port)
{
    if (mRtspPort == port)
    {
        return;
    }

    mRtspPort = port;

    // 保存到配置文件
    saveConfigure();
}

// 保存到配置文件
bool ConfigureJson::saveConfigure()
{
    Json::Value jsonValue;
    std::string rtspService = "rtsp_service";

    // http端口
    jsonValue[rtspService]["http_port"] = mHttpPort;

    // rtsp端口
    jsonValue[rtspService]["rtsp_port"] = mRtspPort;

    // 保存成文件
    std::string configureData = jsonValue.toStyledString();
    return Common::writeFile(mConfigureFile, configureData);
}

std::string ConfigureJson::toString()
{
    std::stringstream buf;
    buf << "http port:" << mHttpPort;
    return buf.str();
}
