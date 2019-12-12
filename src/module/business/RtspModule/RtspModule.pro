QT       += core
QT       -= gui
CONFIG += c++11 plugin
TARGET = RtspModule
TEMPLATE = lib
DESTDIR = ../../../bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/RtspModule
MOC_DIR = ../../../output/RtspModule

INCLUDEPATH += ../../base/Common/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/live555/BasicUsageEnvironment/include \
            ../../../../thirdparty/include/live555/liveMedia/include \
            ../../../../thirdparty/include/live555/groupsock/include \
            ../../../../thirdparty/include/live555/UsageEnvironment/include \
            ../../base/Common/src/message \
            ../../base/Common/src/info \
            ../../base/Common/src/common \
            ./src \
            ./src/common \
            ../../base/CoreModule/src

win32 {
DEFINES += LIVE555_EXPORT=__declspec(dllimport)
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/live555 -llive555 \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
DEFINES += LIVE555_EXPORT=
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp
}

HEADERS += \
    src/*.h \
    src/common/*.h

SOURCES += \
    src/*.cpp \
    src/common/*.cpp
