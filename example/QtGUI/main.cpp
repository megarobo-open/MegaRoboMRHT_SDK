#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <QDebug>

#define VERSION "00.01.00.00"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //解决中文字符乱码的问题，使用下面转化方式
    //! QString->char * ==> str.toLocal8Bit().data()
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    a.setApplicationVersion(VERSION);

    MainWindow w;
    w.show();

    return a.exec();
}
