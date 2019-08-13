#ifndef XTHREAD_H
#define XTHREAD_H

#include <QThread>
#include <functional>


class XThread : public QThread
{
    Q_OBJECT

public:
    typedef std::function<void(void)> XTHREAD_VOID_FUNC;
    typedef std::function<void(int &)> XTHREAD_INT_FUNC;
    typedef std::function<void(QString &)> XTHREAD_STRING_FUNC;

public:
    XThread(QObject *parent = NULL);

    XThread(XTHREAD_VOID_FUNC func, QObject *parent = NULL);
    XThread(XTHREAD_INT_FUNC func, QObject *parent = NULL);
    XThread(XTHREAD_STRING_FUNC func, QObject *parent = NULL);

    ~XThread();

    static int count();

signals:
    void signalFinishResult(int);
    void signalFinishResult(QString);

public:
    void setCallback(XTHREAD_VOID_FUNC  func);
    void setCallback(XTHREAD_INT_FUNC   func);
    void setCallback(XTHREAD_STRING_FUNC func);

protected:
    virtual void run();

private:
    static int m_count;

    XTHREAD_VOID_FUNC   m_funcVoid;
    XTHREAD_INT_FUNC    m_funcInt;
    XTHREAD_STRING_FUNC m_funcString;
};

#endif // XTHREAD_H
