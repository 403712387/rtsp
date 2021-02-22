QT       -= core gui
CONFIG += c++11 plugin
TARGET = RtspModule
TEMPLATE = lib
DESTDIR = ../../../bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/RtspModule
MOC_DIR = ../../../output/RtspModule
DEFINES += __STDC_CONSTANT_MACROS

INCLUDEPATH += ../../base/Common/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/Qt/QtCore \
            ../../../../thirdparty/include/ffmpeg \
            ../../../../thirdparty/include/live555/BasicUsageEnvironment/include \
            ../../../../thirdparty/include/live555/liveMedia/include \
            ../../../../thirdparty/include/live555/groupsock/include \
            ../../../../thirdparty/include/live555/UsageEnvironment/include \
            ../../base/Common/src/message \
            ../../base/Common/src/info \
            ../../base/Common/src/common \
            ./src \
            ./src/common \
            ./src/ffmpeg \
            ./src/ffmpeg/subsession \
            ../../base/CoreModule/src

win32 {
DEFINES += LIVE555_EXPORT=__declspec(dllimport)
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/live555 -llive555 \
        -L../../../../thirdparty/lib/windows64/ffmpeg -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
DEFINES += LIVE555_EXPORT=
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp
}

HEADERS += \
    src/RtspModule.h \
    src/RtspTask.h \
    src/common/RtspCommon.h \
    src/ffmpeg/ffmpeg_demux.h \
    src/ffmpeg/ffmpeg_demuxed_elementary_stream.h \
    src/ffmpeg/ffmpeg_media.h \
    src/ffmpeg/ffmpeg_server_demux.h \
    src/ffmpeg/subsession/ffmpeg_aac_server_media_subsession.h \
    src/ffmpeg/subsession/ffmpeg_h264_server_media_subsession.h \
    src/ffmpeg/subsession/ffmpeg_h265_server_media_subsession.h \
    src/ffmpeg/subsession/ffmpeg_mp3_server_media_subsession.h \
    src/ffmpeg/subsession/ffmpeg_mpeg4_server_media_subsession.h \
    src/ffmpeg/subsession/ffmpeg_server_media_subsession.h

SOURCES += \
    src/RtspModule.cpp \
    src/RtspTask.cpp \
    src/common/RtspCommon.cpp \
    src/ffmpeg/ffmpeg_demux.cpp \
    src/ffmpeg/ffmpeg_demuxed_elementary_stream.cpp \
    src/ffmpeg/ffmpeg_server_demux.cpp \
    src/ffmpeg/subsession/ffmpeg_aac_server_media_subsession.cpp \
    src/ffmpeg/subsession/ffmpeg_h264_server_media_subsession.cpp \
    src/ffmpeg/subsession/ffmpeg_h265_server_media_subsession.cpp \
    src/ffmpeg/subsession/ffmpeg_mp3_server_media_subsession.cpp \
    src/ffmpeg/subsession/ffmpeg_mpeg4_server_media_subsession.cpp

