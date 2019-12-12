#ifndef JSON_HELPER_H
#define JSON_HELPER_H
#include <map>
#include <vector>
#include "Common.h"
#include "jsoncpp/json.h"

class Error;
class ConfigureInfo;
class JsonHelper
{
public:

    // 解析服务控制
    static ServiceOperateType parseServiceControl(std::string &info);

    // 解析配置
    static bool parseConfigure(std::string &info, std::shared_ptr<ConfigureInfo> configure);

    // 解析开始任务
    static std::string parseFilePath(std::string &info);

    // 解析停止任务
    static std::string parseStreamName(std::string &info);

    // 转换为json
    static Json::Value stringToJson(std::string key, std::string value);
private:
    static std::string        mClassName;
};

#endif
