QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#DEFINES += ANDROID      # 表示安卓

INCLUDEPATH += "G:/work/mct/source/device/MegaRoboMRHT_SDK/src"
win32:INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include"

win32:LIBS += -L"C:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc"
win32:LIBS += -L"G:/work/mct/lib"
win32:LIBS += -L"G:/work/mct/3rdlib"

win32:LIBS += -lmrx-device

win32:LIBS += -llibws2_32 -llibiphlpapi

win32:LIBS += -lvisa32

unix {
    contains(DEFINES, ANDROID) {
        DEFINES += VXI_NOT_SUPPORT
    }
}
#include(../../src/MegaGateway.pri)

SOURCES += \
    main.cpp
