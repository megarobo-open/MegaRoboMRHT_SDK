QT       -= core gui
TEMPLATE = lib
TARGET = MegaGateway

VERSION = 1.0.0

###打开后只生成静态库
#CONFIG += staticlib

#DEFINES += ARM_LINUX    # 表示arm上的Linux,比如树莓派
#DEFINES += ANDROID      # 表示安卓

unix {
    CONFIG += plugin
    contains(DEFINES, ANDROID) {
        message( "Configuring for android" )
        DEFINES += VXI_NOT_SUPPORT
        DESTDIR = $$PWD/../lib/android
    }
    else {
        contains(DEFINES, ARM_LINUX) {
            message( "Configuring for arm_Linux" )
            DESTDIR = $$PWD/../lib/armlinux
        }
        else{
            message( "Configuring for x86_Linux" )
            DESTDIR = $$PWD/../lib/linux
        }
    }
}

win32 {
    message( "Configuring for x86_windows" )
    message( "Support socket and vxi11" )
    TARGET = MegaGateway-
    DESTDIR = $$PWD/../lib/windows
}

include(MegaGateway.pri)




