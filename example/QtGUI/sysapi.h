#ifndef SYSAPI_H
#define SYSAPI_H

#include <QtCore>

//! 显示实时状态
void sysShowStatus(const QString statusInfo);

//! 显示实时进度条状态
void sysShowProgressBar(const bool isRunning);

int writeFile(QString fileName, QString text, bool isAppend = false);

QString readFile(QString fileName);

void removeListSame(QStringList &list);

#endif // SYSAPI_H
