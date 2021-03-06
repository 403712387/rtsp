QT       -= core gui
CONFIG += c++11 plugin
TARGET = ConfigureModule
TEMPLATE = lib
DESTDIR = ../../../bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/ConfigureModule
MOC_DIR = ../../../output/ConfigureModule

INCLUDEPATH += ../Common/src \
            ../Common/src/info \
            ./src/common \
            ../../protocol/log/src \
            ../../../../thirdparty/include \
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/Qt/QtCore \
            ../Common/src/message \
            ../CoreModule/src
win32 {
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core \
        -L../../../../thirdparty/lib/windows64/jsoncpp -ljsoncpp
}

unix {
LIBS += -L../../../bin/rtsp/lib/module -lCommon -lCoreModule \
        -L../../../../thirdparty/lib/linux64/Qt -lQt5Core \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc \
        -L../../../../thirdparty/lib/linux64/jsoncpp -ljsoncpp
}

HEADERS += \
    src/ConfigureModule.h \
    src/common/ConfigureJson.h

SOURCES += \
    src/ConfigureModule.cpp \
    src/common/ConfigureJson.cpp

