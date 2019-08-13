QT += core gui widgets xml network

CONFIG += c++11
TEMPLATE += app

TARGET = MRHT_SDK_Demo
VERSION = 00.01.00.00

#DEFINES += ANDROID      # 表示安卓

unix {
    contains(DEFINES, ANDROID) {
        DEFINES += VXI_NOT_SUPPORT
    }
}
include(../../src/MegaGateway.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

################### QtGUI ###################
INCLUDEPATH += ./mainwindow
INCLUDEPATH += ./widget
INCLUDEPATH += ./dialog
INCLUDEPATH += ./thread

SOURCES +=  \
    mainwindow/mainwindow.cpp   \
    widget/mainwidget.cpp       \
    dialog/TestPanelDialog.cpp \
    dialog/CanDebugDialog.cpp \
    thread/xthread.cpp \
    sysapi.cpp \
    main.cpp

HEADERS += \
    mainwindow/mainwindow.h     \
    widget/mainwidget.h         \
    dialog/TestPanelDialog.h \
    dialog/CanDebugDialog.h \
    thread/xthread.h \
    sysapi.h

FORMS += \
    mainwindow/mainwindow.ui    \
    widget/mainwidget.ui        \
    dialog/TestPanelDialog.ui \
    dialog/CanDebugDialog.ui


RC_ICONS = resource/app.ico

RESOURCES += \
    resource/res.qrc

CONFIG(debug, debug|release) {
    DEFINES += MRG_DEBUG
}
