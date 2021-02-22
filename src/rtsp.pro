TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS += \
        ./module/base/Common/Common.pro \
        ./module/base/CoreModule/CoreModule.pro \
        ./module/base/HttpModule/HttpModule.pro \
        ./module/base/ServiceStatusModule/ServiceStatusModule.pro \
        ./module/base/ConfigureModule/ConfigureModule.pro \
        ./module/business/RtspModule/RtspModule.pro \
        ./service/rtsp/rtsp.pro
