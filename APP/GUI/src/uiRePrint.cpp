#include "uiRePrint.h"
#include "transData.h"
#include "xdata.h"
#include "sand_lcd.h"
#include "global.h"

UIRePrint::UIRePrint(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    RemoveKeyEventBug();

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,FRAME420_THVALUE,FRAME420_WIDTH,FRAME420_HEIGHT);
    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT);
    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",14);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Print"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    btnRePrintLast=new QPushButton();
    btnRePrintWhich=new QPushButton();
    btnPrintAudit=new QPushButton();
    btnPrintSettle=new QPushButton();

    btnRePrintLast->setText("Reprint Last ");
    btnRePrintWhich->setText("Reprint At Will");
    btnPrintAudit->setText("Print Audit");
    btnPrintSettle->setText("Print Settle");

    btnRePrintLast->setFont(font2);
    btnRePrintWhich->setFont(font2);
    btnPrintAudit->setFont(font2);
    btnPrintSettle->setFont(font2);

    btnRePrintLast->setStyleSheet(BTN_MENU_STYLE);
    btnRePrintWhich->setStyleSheet(BTN_MENU_STYLE);
    btnPrintAudit->setStyleSheet(BTN_MENU_STYLE);
    btnPrintSettle->setStyleSheet(BTN_MENU_STYLE);

    btnRePrintLast->setMinimumHeight(40);
    btnRePrintLast->setMaximumHeight(40);
    btnRePrintWhich->setMinimumHeight(40);
    btnRePrintWhich->setMaximumHeight(40);
    btnPrintAudit->setMinimumHeight(40);
    btnPrintAudit->setMaximumHeight(40);
    btnPrintSettle->setMinimumHeight(40);
    btnPrintSettle->setMaximumHeight(40);
    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnCancel->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(btnRePrintLast);
    v1Lay->addWidget(btnRePrintWhich);
    v1Lay->addWidget(btnPrintAudit);
    v1Lay->addWidget(btnPrintSettle);
    v1Lay->addItem(sp2);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);


    connect(btnRePrintLast,SIGNAL(clicked()),this,SLOT(slotReprintLast()));
    connect(btnRePrintWhich,SIGNAL(clicked()),this,SLOT(slotReprintWhich()));
    connect(btnPrintAudit,SIGNAL(clicked()),this,SLOT(slotPrintAudit()));
    connect(btnPrintSettle,SIGNAL(clicked()),this,SLOT(slotPrintSettle()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();
}

UIRePrint::~UIRePrint()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIRePrint::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        break;
    default:
        event->ignore();
        break;
    }
}

void UIRePrint::slotReprintLast()
{
    qDebug()<<Q_FUNC_INFO;

    memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
    int ucResult=xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans,  g_transInfo.ulLastTransNumber);
    if(ucResult!=0)
    {
        UIMsg::showFileErrMsgWithAutoClose((FileErrIndex)ucResult,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }
    memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));
    if(NormalTransData.ulAmount==0)
    {
        UIMsg::showErrMsgWithAutoClose("No Transaction",g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    // 打印线程
    qDebug()<<"start to print";
    threadPrint=new QThread(this);
    pPrint = new objPrint;
    connect(threadPrint, SIGNAL(started()), pPrint, SLOT(rePrintReceipt()));
    connect(pPrint,SIGNAL(sigPrintComplete()),this,SLOT(slotFinishPrint()));
    connect(pPrint,SIGNAL(sigTear()),this,SLOT(slotTearPrint()),Qt::BlockingQueuedConnection);

    pPrint->moveToThread(threadPrint);
    threadPrint->start();
    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);

}

void UIRePrint::slotReprintWhich()
{
    qDebug()<<Q_FUNC_INFO;

    bool ok;
    int traceNo=UIInput::getInt("TRACE NO","Please Input\n Trace No. :",REGEX_NUMBER,6,&ok);
    if(ok)
    {
        qDebug()<<traceNo;

        for(int index = 0; index < ((int)g_constantParam.uiMaxTotalNb); index++)
        {
            qDebug()<<"index:: "<<index;
            if(g_transInfo.auiTransIndex[index])
            {
                //:- 读取数据保存到NormalTransData
                memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
                int ucResult=xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, index);
                if(ucResult!=0)
                {
                    UIMsg::showFileErrMsgWithAutoClose((FileErrIndex)ucResult,g_changeParam.TIMEOUT_ERRMSG);

                    return;
                }
                memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));

                if(traceNo==NormalTransData.ulTraceNumber)
                {
                    // 打印线程
                    qDebug()<<"start to print";
                    threadPrint=new QThread(this);
                    pPrint = new objPrint;
                    connect(threadPrint, SIGNAL(started()), pPrint, SLOT(rePrintReceipt()));
                    connect(pPrint,SIGNAL(sigPrintComplete()),this,SLOT(slotFinishPrint()));
                    connect(pPrint,SIGNAL(sigTear()),this,SLOT(slotTearPrint()),Qt::BlockingQueuedConnection);

                    pPrint->moveToThread(threadPrint);
                    threadPrint->start();
                    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);

                    break;
                }
            }
            else
            {
                UIMsg::showErrMsgWithAutoClose("No Match Trace",g_changeParam.TIMEOUT_ERRMSG);
                return;

            }
        }
    }
    else
    {
        return;
    }
}
void UIRePrint::slotPrintAudit()
{
    qDebug()<<Q_FUNC_INFO;

    // 打印线程
    threadPrint=new QThread(this);
    pPrint = new objPrint;
    connect(threadPrint, SIGNAL(started()), pPrint, SLOT(printAudit()));
    connect(pPrint,SIGNAL(sigPrintComplete()),this,SLOT(slotFinishPrint()));

    pPrint->moveToThread(threadPrint);
    threadPrint->start();
    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);
}

void UIRePrint::slotPrintSettle()
{
    qDebug()<<Q_FUNC_INFO;
    // 打印线程
    threadPrint=new QThread(this);
    pPrint = new objPrint;
    connect(threadPrint, SIGNAL(started()), pPrint, SLOT(printSettle()));
    connect(pPrint,SIGNAL(sigPrintComplete()),this,SLOT(slotFinishPrint()));

    pPrint->moveToThread(threadPrint);
    threadPrint->start();
    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);
}

void UIRePrint::slotFinishPrint()
{
    qDebug()<<Q_FUNC_INFO;
    Os__gif_stop();
    delete pPrint;
    if(threadPrint != NULL)
    {
//        threadPrint->wait();
        qDebug(" --- Before deleteLater --- !\n");
        threadPrint->deleteLater();
        qDebug(" --- After deleteLater--- !\n");
    }
}

// 撕纸
void UIRePrint::slotTearPrint()
{
    qDebug()<<Q_FUNC_INFO;
    Os__gif_stop();
    UIMsg::showNoticeMsgWithAutoClose("Tear Receipt",g_changeParam.TIMEOUT_PAPERTEAR);
    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);
}
