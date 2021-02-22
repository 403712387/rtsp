QT       -= core gui
CONFIG += c++11 plugin
TARGET = HttpModule
TEMPLATE = lib
DESTDIR = ../../../bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/HttpModule
MOC_DIR = ../../../output/HttpModule

INCLUDEPATH += ../Common/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/Qt/QtCore \
            ../Common/src/message \
            ../Common/src/info \
            ../Common/src/common \
            ../Common/src/SystemInfo \
            ./src \
            ./src/helper \
            ../CoreModule/src

win32 {
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/libmicrohttpd -llibmicrohttpd-dll \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule -lCommon \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc \
        -L../../../../thirdparty/lib/linux64/libmicrohttpd  -lmicrohttpd \
        -L../../../../thirdparty/lib/linux64/jsoncpp  -ljsoncpp
}

HEADERS += \
    src/HttpModule.h \
    src/helper/JsonHelper.h

SOURCES += \
    src/HttpModule.cpp \
    src/helper/JsonHelper.cpp

