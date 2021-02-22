QT       -= core gui
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
            ../../../../thirdparty/include/Qt \
            ../../../../thirdparty/include/Qt/QtCore \
            ../../../../thirdparty/include

win32 {
LIBS += -L../../../bin/rtsp/lib/module -lCommon \
        -L../../../../thirdparty/lib/windows64/Qt -lQt5Core
}

unix {
LIBS += \
        #-L../../../../thirdparty/lib/linux64/gperftools/static -lprofiler -ltcmalloc
        -L../../../bin/rtsp/lib/module -lCommon

}

HEADERS += \
    src/BaseProcess.h \
    src/MessageRoute.h

SOURCES += \
    src/BaseProcess.cpp \
    src/MessageRoute.cpp

