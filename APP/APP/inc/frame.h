#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

#include "menuBase.h"
#include "mainentrythread.h"

class Frame: public QWidget
{
    Q_OBJECT
public:
    explicit Frame(int argc, char *argv[],QWidget *parent = 0);
    ~Frame();
    static MainEntryThread *mainEntryThread;

private slots:
    void sysEventSlot(int);

    void wakeUpMainThread(void);

private:    
    MenuBase *mainMenu;

    void ShowMainWindow(void);

};

#endif // FRAME_H
