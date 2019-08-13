#include "xthread.h"
#include <QDebug>

int XThread::m_count = 0;

XThread::XThread(QObject *parent)
    : QThread(parent)
{
    m_count++;

    m_funcVoid = nullptr;
    m_funcInt = nullptr;
    m_funcString = nullptr;

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

XThread::XThread(XTHREAD_VOID_FUNC func, QObject *parent)
    : QThread(parent)
{
    m_count++;

    m_funcVoid = nullptr;
    m_funcInt = nullptr;
    m_funcString = nullptr;

    m_funcVoid = func;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

XThread::XThread(XTHREAD_INT_FUNC func, QObject *parent)
    : QThread(parent)
{
    m_count++;

    m_funcVoid = nullptr;
    m_funcInt = nullptr;
    m_funcString = nullptr;

    m_funcInt = func;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

XThread::XThread(XTHREAD_STRING_FUNC func, QObject *parent)
    : QThread(parent)
{
    m_count++;
    m_funcString = func;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

XThread::~XThread()
{
    m_count--;
}

void XThread::run()
{
    if (m_funcVoid != nullptr)
    {
        m_funcVoid();
    }
    else if (m_funcInt != nullptr)
    {
        int ret = -1;
        m_funcInt( ret );
        emit signalFinishResult(ret);
    }
    else if (m_funcString != nullptr)
    {
        QString str = "";
        m_funcString( str );
        emit signalFinishResult(str);
    }
    else
    {

    }
}

int XThread::count()
{
    return m_count;
}

void XThread::setCallback(XTHREAD_VOID_FUNC func)
{
    m_funcVoid = func;
}

void XThread::setCallback(XTHREAD_INT_FUNC func)
{
    m_funcInt = func;
}

void XThread::setCallback(XTHREAD_STRING_FUNC func)
{
    m_funcString = func;
}


//! eg应用事例
#if 0
void testFunc()
{
    auto func1 = [&]()
    {
        int m_times1 = 20;
        while(m_times1--)
        {
            qDebug() << QString("thread1") << m_times1;
            QThread::msleep(100);
        }
    };

    auto func2 = [&](int &ret)
    {
        int m_times2 = 30;
        while(m_times2--)
        {
            qDebug() << QString("thread2") << m_times2;
            QThread::msleep(100);
        }
        ret = 999999;//线程返回结果
    };

    auto func3 = [&](QString &retStr)
    {
        int m_times3 = 40;
        while(m_times3--)
        {
            qDebug() << QString("thread3") << m_times3;
            QThread::msleep(100);
        }
        retStr = "nieshizhong";//线程返回结果
    };

    //! example 1
    XThread *thread1 = new XThread;
    thread1->setCallback(func1);
    thread1->start();

    //    XThread *thread1 = new XThread(func1);
    //    thread1->start();

    //! example 2
    XThread *thread2 = new XThread(func2);
    connect(thread2,SIGNAL(signalFinishResult(int)),this,SLOT(slotDebug(int)));
    thread2->start();

    //! example 3
    XThread *thread3 = new XThread(func3);
    connect(thread3,SIGNAL(signalFinishResult(QString)),this,SLOT(slotDebug(QString)));
    thread3->start();
}
#endif
