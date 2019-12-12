QT       -= gui
CONFIG += c++11 plugin

TARGET = CoreModule
TEMPLATE = lib
DESTDIR = ../../..//bin/rtsp/lib/module
OBJECTS_DIR = ../../../output/CoreModule
MOC_DIR = ../../../output/CoreModule
DEFINES += QT_NO_VERSION_TAGGING

INCLUDEPATH += ../Common/src \
            ../Common/src/info \
            ../Common/src/message \
            ../../../../thirdparty/include/log4plus/windows \
            ../../../../thirdparty/include

win32 {
LIBS += -L../../../bin/rtsp/lib/module -lCommon
}

unix {
LIBS += \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc
        -L../../../bin/rtsp/lib/module -lCommon

}

HEADERS += \
    src/*.h

SOURCES += \
    src/*.cpp
