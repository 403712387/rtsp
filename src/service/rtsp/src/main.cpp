﻿#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>
#include "Common.h"
#include "RtspModule.h"
#include "MessageRoute.h"
#include "BaseProcess.h"
#include "HttpModule.h"
#include "ConfigureModule.h"
#include "ServiceStatusModule.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    LOG_I("main", "***********Welcome***************");

    // 解析参数
    for (int i = 0; i < argc; ++i)
    {
        std::string param(argv[i]);
        if (param == "-l" && i < (argc - 1))  // 调整日志级别
        {
            common_log::Log_Level logLevel = Common::getLogLevelFromString(argv[i + 1]);
            common_log::setLogLevel(logLevel);
        }
        if (param == "-h" )  // 帮助
        {
            LOG_I("main", "./rtsp -l verbose|debug|info|warn|error|fatal")
        }
    }

    // 启动各个模块
    MessageRoute messageRoute;
    ConfigureModule configureModule(&messageRoute);
    HttpModule httpModule(&messageRoute);
    ServiceStatusModule serviceStatusModule(&messageRoute);
    RtspModule rtspModule(&messageRoute);
    messageRoute.beginWork();

    LOG_I("main", "********************Bye********************");
    return 0;
}
