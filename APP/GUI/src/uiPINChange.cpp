#include "uiPINChange.h"
#include "sav.h"
#include "xdata.h"
#include "commontools.h"
#include "global.h"

static void TRANS_CleanTransData(void);

UIPINChange::UIPINChange(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    TRANS_CleanTransData();
    RemoveKeyEventBug();
    NormalTransData.transType=TransMode_PINChange;

    xDATA::ReadValidFile(xDATA::DataSaveChange);
    xDATA::ReadValidFile(xDATA::DataSaveConstant);

    FLAG_InputPassword=false;
    FLAG_SwipeCard=false;
    FLAG_InputManual=false;
    FLAG_InputPIN=false;
    FLAG_TransOnline=false;

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
    QFont font3("Helvetica",8,QFont::Bold);

    lbHead=new QLabel();
    lbHead->setText(tr("PIN CHANGE"));
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QSpacerItem *sItem=new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbHead);
    v1Lay->addItem(sItem);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);

    layout->setContentsMargins(0,0,0,5);

    // -------- input cashier password ------------//
    FLAG_InputPassword=true;

    uiIPass=new UIInputPassword();
    connect(uiIPass,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(checkAuth(UserType,QString)));
    connect(uiIPass,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiIPass,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiIPass, SLOT(exec()));
    passThread->start();
}

UIPINChange::~UIPINChange()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIPINChange::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F3:
        break;
    case Qt::Key_F4:
        break;
    default:
        event->ignore();
        break;
    }
}

void UIPINChange::checkAuth(UserType ut,QString ID)
{
    // 交易关闭
    qDebug()<<ut<<ID<<g_changeParam.boolCashierLogonFlag;
    if(g_changeParam.pinchange.TRANS_ENABLE==false)
    {
        UIMsg::showErrMsgWithAutoClose("Transaction Disabled",g_changeParam.TIMEOUT_ERRMSG);

        return;
    }
    if(g_changeParam.boolCashierLogonFlag==false)
    {
        UIMsg::showErrMsgWithAutoClose("Please Logon",g_changeParam.TIMEOUT_ERRMSG);
        return;
    }
    //--------------------------------------- //
    if(ut==typeCashier)
    {
        memcpy(NormalTransData.aucCashier , ID.toAscii().data() , 2);

        this->swipeCard();
    }
    else
    {
        qDebug()<<"不支持柜员以外的用户做交易";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);
        uiIPass->resetLine();

        return;
    }
}

void UIPINChange::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSC=new UISwipeCard();
    if(g_changeParam.pinchange.MANUAL_ENABLE==false)
    {
        qDebug()<<"设置不可手动";
        uiSC->setNoManual();
    }
    connect(uiSC,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSC,SIGNAL(sigFinishPutCard()),this,SLOT(inputPIN()));
    connect(uiSC,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputPINAndExit()));
    connect(uiSC,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));


    uiSC->exec();
}

void UIPINChange::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiIM=new UIInputManual();
    connect(uiIM,SIGNAL(sigInputComplete()),this,SLOT(inputPIN()));
    connect(uiIM,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiIM->exec();
}

void UIPINChange::inputPINAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;
    uiIPIN=new UIInputPIN();
    this->quitFromFlow();
}

void UIPINChange::inputPIN()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiIPIN=new UIInputPIN();
    if(g_changeParam.pinchange.PIN_ENABLE==false)
        uiIPIN->slotDisablePIN();
    connect(uiIPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiIPIN,SIGNAL(sigSubmit()),this,SLOT(inputNewPIN()));
    uiIPIN->exec();
}

void UIPINChange::inputNewPIN()
{
    bool ok1;
    bool ok2;
    QString newPIN=UIInput::getPIN("New PIN","Please Input\nNew PIN:",REGEX_NUMBER,6,&ok1);
    if(ok1)
    {
        QString newPINConfirm=UIInput::getPIN("Confirm PIN","Please Input\nNew PIN Again:",REGEX_NUMBER,6,&ok2);
        if(ok2)
        {
            // 匹配一样则上送
            int x = QString::compare(newPIN, newPINConfirm, Qt::CaseInsensitive);
            if(x==0)
            {
                this->transOnline();
            }
        }
        else
            this->quitFromFlow();
    }
    else
        this->quitFromFlow();
}

void UIPINChange::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTO=new UITransOnline();
    uiTO->slotStartTrans(NormalTransData.transType);
    connect(uiTO,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTO,SIGNAL(sigTransSuccess()),this,SLOT(changePINSuccess()));

    uiTO->exec();
}

void UIPINChange::changePINSuccess()
{
    UIMsg::showNoticeMsgWithAutoClose("PIN Change/nSuccess",g_changeParam.TIMEOUT_ERRMSG);
    this->finishFromFlow();
}


// ----------------------------------------------------------------------------------------------- //
void UIPINChange::quitFromSwipeCard()
{
    qDebug()<<Q_FUNC_INFO;

    uiSC->close();
    uiRDetail->close();
    uiIPass->close();

    this->close();
}


void UIPINChange::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;


    if(FLAG_TransOnline==true)
    {
        uiTO->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputPIN==true)
    {
        uiIPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputManual==true)
    {
        uiIM->close();
        FLAG_InputManual=false;
    }
    if(FLAG_SwipeCard==true)
    {
        uiSC->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiIPass->close();
        FLAG_InputPassword=false;
    }

    UIMsg::showErrMsgWithAutoClose(ERR_CANCEL,g_changeParam.TIMEOUT_ERRMSG);

    this->close();
}

void UIPINChange::finishFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_TransOnline==true)
    {
        uiTO->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputPIN==true)
    {
        uiIPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputManual==true)
    {
        uiIM->close();
        FLAG_InputManual=false;
    }
    if(FLAG_SwipeCard==true)
    {
        uiSC->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiIPass->close();
        FLAG_InputPassword=false;
    }

    this->close();
}
static void TRANS_CleanTransData(void)
{
    memset(&NormalTransData, 0, sizeof(NormalTransData));
    memset(&ExtraTransData, 0, sizeof(ExtraTransData));
}
