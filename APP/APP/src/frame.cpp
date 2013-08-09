#include "frame.h"

#include "menuconfig.h"

#include "sand_main.h"

#include "msg.h"
#include "global.h"
#include "xdata.h"
#include "key.h"

MainEntryThread *Frame::mainEntryThread = NULL;

Frame::Frame(int argc, char *argv[],QWidget *parent):
    QWidget(parent)
{
    qDebug() << Q_FUNC_INFO;

    KEY_LoadDefaultKey();
	xDATA::CheckFileExist();
    mainEntryThread = new MainEntryThread(argc,argv);
    connect(mainEntryThread,SIGNAL(sysEventSignal(int)),this,SLOT(sysEventSlot(int)));
    mainEntryThread->start();
}

Frame::~Frame()
{
    qDebug() << Q_FUNC_INFO;
    //delete this;
}

void Frame::sysEventSlot(int sysEvent)
{
    qDebug() << Q_FUNC_INFO;

    unsigned char EventType = static_cast<unsigned char>(((struct seven *)sysEvent)->event_type);
    unsigned char EventNum  = static_cast<unsigned char>(((struct seven *)sysEvent)->event_nr);

    ((struct seven *)sysEvent)->pt_eve_out->treatment_status = NOK;
    switch (EventNum + EventType)
    {
        case OSEVENTTYPE_CIR + OSEVENTID_MAGCARD:
//            qDebug("SIGNAL : OSEVENTTYPE_CIR + OSEVENTID_MAGCARD signal get from APP[%d]\n", appnum);
            mainEntryThread->wakeUp();
            break;

        case OSEVENTTYPE_CIR + OSEVENTID_ICC:
//            qDebug("SIGNAL : OSEVENTTYPE_CIR + OSEVENTID_ICC signal get from APP[%d]\n", appnum);
            mainEntryThread->wakeUp();
            break;

        case OSEVENTTYPE_SEL + 0xff:/* 按键进去的界面 */
//            qDebug("SIGNAL : OSEVENTTYPE_SEL + 0xff signal get from APP[%d]\n", appnum);
            ShowMainWindow();
            break;

        case OSEVENTTYPE_CIR + OSEVENTID_MFCCRD:/* 放卡进去的界面 */
//            qDebug("SIGNAL : OSEVENTTYPE_CIR + OSEVENTID_MFCCRD signal get from APP[%d]\n", appnum);
            mainEntryThread->wakeUp();
            break;

        default:
			memset(&ISO8583Data,0 , sizeof(ISO8583));
//            qDebug("SIGNAL : default signal get from APP[%d]\n", appnum);
            mainEntryThread->wakeUp();
            break;
    }
}

void Frame::ShowMainWindow(void)
{
    qDebug() << Q_FUNC_INFO;

    //disconnect(mainEntryThread,SIGNAL(sysEventSignal(int)),this,SLOT(sysEventSlot(int)));
    MenuConfig::mainMenu();
}


void Frame::wakeUpMainThread(void)
{
    qDebug() << Q_FUNC_INFO;

    connect(mainEntryThread,SIGNAL(sysEventSignal(int)),this,SLOT(sysEventSlot(int)));
    mainEntryThread->wakeUp();
}


