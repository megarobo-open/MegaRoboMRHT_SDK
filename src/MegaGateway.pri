######################## SRC ##############################
QMAKE_CFLAGS += -Wall
QMAKE_CXXFLAGS += -Wall

INCLUDEPATH += $$PWD

SOURCES +=  \
    $$PWD/bus.c \
    $$PWD/device.c \
    $$PWD/megarobot.c \
    $$PWD/mrqdevice.c \
    $$PWD/project.c \
    $$PWD/storage.c \
    $$PWD/system.c \
    $$PWD/common.c

HEADERS += \
    $$PWD/bus.h \
    $$PWD/device.h \
    $$PWD/MegaGateway.h \
    $$PWD/megarobot.h \
    $$PWD/mrqdevice.h \
    $$PWD/platform.h \
    $$PWD/project.h \
    $$PWD/storage.h \
    $$PWD/system.h \
    $$PWD/common.h \
    $$PWD/megatype.h

################### LIBS ###################
win32 {
INCLUDEPATH += $$PWD/../win
DEPENDPATH += $$PWD/../win

LIBS += -L$$PWD/../win -llibws2_32
PRE_TARGETDEPS += $$PWD/../win/libws2_32.a

LIBS += -L$$PWD/../win/ -llibiphlpapi
PRE_TARGETDEPS += $$PWD/../win/libiphlpapi.a

LIBS += -L$$PWD/../win/ -lvisa32
PRE_TARGETDEPS += $$PWD/../win/visa32.lib

LIBS += -L$$PWD/../win/ -llibmsvcr100
PRE_TARGETDEPS += $$PWD/../win/libmsvcr100.a
}

unix {
    contains(DEFINES, VXI_NOT_SUPPORT) {
        message("Vxi11 not support")
    }
    else {
        message("Support socket and vxi11")
        include(vxi11/vxi11.pri)
    }
}


win32: LIBS += -L$$PWD/msvc/ -llibssh2

INCLUDEPATH += $$PWD/libssh2/
DEPENDPATH += $$PWD/libssh2
