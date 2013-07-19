#ifndef MAINENTRYTHREAD_H
#define MAINENTRYTHREAD_H

#include <QtDebug>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>

class MainEntryThread : public QThread
{
    Q_OBJECT

public:
    MainEntryThread(int c, char **v);
    ~MainEntryThread();

private:
    int argc;
    char **argv;
    QMutex mutex;
    QWaitCondition bufferNotDel;

protected:
    void run();

signals:
    void sysEventSignal(int);
public slots:
    void wakeUp(void);
};

#endif // MAINENTRYTHREAD_H
