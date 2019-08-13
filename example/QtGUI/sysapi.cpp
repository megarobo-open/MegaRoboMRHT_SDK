#include "sysapi.h"
#include "mainwindow.h"

//! 显示实时状态
void sysShowStatus(const QString statusInfo)
{
    MainWindow::showStatus(statusInfo);
}

//! 显示实时进度条状态
void sysShowProgressBar(const bool isRunning)
{
    MainWindow::showProgressBar(isRunning);
}

//! 读文件
QString readFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << QString("Can't ReadOnly open the file: %1").arg(fileName);
        return "";
    }

    QByteArray array = file.readAll();
    file.close();
    return QString(array);
}

//! 写文件
int writeFile(QString fileName, QString text , bool isAppend)
{
    QFile file(fileName);
    bool isOk;
    if(isAppend)
    {
        isOk = file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    }
    else
    {
        isOk = file.open(QIODevice::WriteOnly | QIODevice::Text);
    }
    if(!isOk)
    {
        qDebug() << QString("Can't writeFile open the file: %1").arg(fileName);
        return -1;
    }

    file.write(text.toUtf8());
    file.close();
    return 0;
}

//! 字符串列表去重
void removeListSame(QStringList &list)
{
    for (int i = 0; i < list.count(); i++)
    {
        for (int k = i + 1; k < list.count(); k++)
        {
            if ( list.at(i) ==  list.at(k))
            {
                list.removeAt(k);
                k--;
            }
        }
    }
}
