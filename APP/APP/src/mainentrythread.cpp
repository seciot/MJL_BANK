#include "mainentrythread.h"

#include "sand_main.h"

MainEntryThread::MainEntryThread(int c, char **v)
{
    qDebug() << Q_FUNC_INFO;

    argc=c;
    argv=v;
}

MainEntryThread::~MainEntryThread()
{
    qDebug() << Q_FUNC_INFO;
    delete this;
}

void MainEntryThread::run()
{
    qDebug() << Q_FUNC_INFO;

    struct seven   *appevent;
    qt_main_entry_init(argc, argv);

    while(1)
    {
        appevent = (struct seven *)qt_main_entry_readmsg();
        mutex.lock();
        emit sysEventSignal((int)appevent);
        bufferNotDel.wait(&mutex);
        mutex.unlock();
        qt_main_entry_writemsg();
    }
}

void MainEntryThread::wakeUp()
{
    qDebug() << Q_FUNC_INFO;

    mutex.lock();
    bufferNotDel.wakeAll();
    mutex.unlock();
}
