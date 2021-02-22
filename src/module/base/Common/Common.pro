QT       -= core gui
CONFIG += c++11 plugin

TARGET = Common
TEMPLATE = lib
DESTDIR = ../../../bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/Common
MOC_DIR = ../../../output/Common

INCLUDEPATH += ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/Qt/QtCore \
            ../../protocol/log/src \
            src \
            src/common \
            src/info \
            src/log \
            src/perftool \
            src/thread \
            src/SystemInfo \
            src/config/StorageConfig \
            src/message

win32 {
LIBS += -L../../../../thirdparty/lib/windows64/curl -llibcurl \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/log4cplus -llog4cplusU \
        #-L../../../../thirdparty/lib/windows64/gperftools -laddressmap_unittest -llibtcmalloc_minimal -llow_level_alloc_unittest \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../../thirdparty/lib/linux64/curl -lcurl \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/log4cplus -llog4cplus \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc \
        -L../../../../thirdparty/lib/linux64/jsoncpp -ljsoncpp
}

HEADERS += \
    src/Base.h \
    src/Common.h \
    src/Log.h \
    src/SystemInfo/CPUStatistic.h \
    src/SystemInfo/DiskStatistic.h \
    src/SystemInfo/MemoryStatistic.h \
    src/SystemInfo/PlatformInfo.h \
    src/SystemInfo/ProcessStatistic.h \
    src/SystemInfo/SystemInfo.h \
    src/common/HttpHelper.h \
    src/common/PriorityQueue.h \
    src/common/Queue.h \
    src/common/SpeedStatisticList.h \
    src/info/ConfigureInfo.h \
    src/info/Error.h \
    src/info/ServiceStatusInfo.h \
    src/log/LogManager.h \
    src/message/BaseMessage.h \
    src/message/BaseWaitMessage.h \
    src/message/ConfigureInfoMessage.h \
    src/message/GetServiceStatusMessage.h \
    src/message/GetTaskMessage.h \
    src/message/ServiceControlMessage.h \
    src/message/StartTaskMessage.h \
    src/message/StopTaskMessage.h \
    src/perftool/perftool.h \
    src/thread/Thread.h \
    src/thread/ThreadHandler.h

SOURCES += \
    src/Common.cpp \
    src/Log.cpp \
    src/SystemInfo/CPUStatistic.cpp \
    src/SystemInfo/DiskStatistic.cpp \
    src/SystemInfo/MemoryStatistic.cpp \
    src/SystemInfo/PlatformInfo.cpp \
    src/SystemInfo/ProcessStatistic.cpp \
    src/SystemInfo/SystemInfo.cpp \
    src/common/HttpHelper.cpp \
    src/common/PriorityQueue.cpp \
    src/common/Queue.cpp \
    src/common/SpeedStatisticList.cpp \
    src/info/ConfigureInfo.cpp \
    src/info/Error.cpp \
    src/info/ServiceStatusInfo.cpp \
    src/log/LogManager.cpp \
    src/message/BaseMessage.cpp \
    src/message/BaseWaitMessage.cpp \
    src/message/ConfigureInfoMessage.cpp \
    src/message/GetServiceStatusMessage.cpp \
    src/message/GetTaskMessage.cpp \
    src/message/ServiceControlMessage.cpp \
    src/message/StartTaskMessage.cpp \
    src/message/StopTaskMessage.cpp \
    src/perftool/perftool.cpp \
    src/thread/Thread.cpp \
    src/thread/ThreadHandler.cpp

