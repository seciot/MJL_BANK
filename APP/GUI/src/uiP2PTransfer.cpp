#include "uiP2PTransfer.h"
#include "sav.h"
#include "xdata.h"
#include "global.h"

static void TRANS_CleanTransData(void);

UIP2PTransfer::UIP2PTransfer(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    TRANS_CleanTransData();
    NormalTransData.transType = TransMode_CardTransfer;
    RemoveKeyEventBug();
    xDATA::ReadValidFile(xDATA::DataSaveChange);
    xDATA::ReadValidFile(xDATA::DataSaveConstant);


    FLAG_InputPassword=false;
    FLAG_AccountTypeReceive=false;
    FLAG_AccountType=false;
    FLAG_SwipeCard=false;
    FLAG_InputManual=false;
    FLAG_InputAmount=false;
    FLAG_InputReceiveCard=false;
    FLAG_InputPIN=false;
    FLAG_TransOnline=false;
    FLAG_PrintReceipt=false;

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
    lbHead->setText(tr("P2P Transfer"));
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

    uiIP=new UIInputPassword();
    connect(uiIP,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(chooseAccountType(UserType,QString)));
    connect(uiIP,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiIP,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiIP, SLOT(exec()));
    passThread->start();

}

UIP2PTransfer::~UIP2PTransfer()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIP2PTransfer::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F3:
        //        vBar->setValue(vBar->value()-150);
        break;
    case Qt::Key_F4:
        //        vBar->setValue(vBar->value()+150);
        break;
    default:
        event->ignore();
        break;
    }
}

void UIP2PTransfer::chooseAccountType(UserType ut,QString ID)
{
    qDebug()<<ut<<ID;

    if(g_changeParam.p2p.TRANS_ENABLE==false)
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
        FLAG_AccountType=true;
        memcpy(NormalTransData.aucCashier, ID.toAscii().data() , 2);
        uiCAT=new UIChooseAccType();
        uiCAT->setSender();
        connect(uiCAT,SIGNAL(sigChooseTypeComplete(AccType)),this,SLOT(setAccountType(AccType)));
        connect(uiCAT,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
        uiCAT->exec();
    }
    else
    {
        qDebug()<<"不支持柜员以外的用户做交易";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);
        uiIP->resetLine();

        return;
    }
}

void UIP2PTransfer::chooseReceiverAccountType()
{
    FLAG_AccountTypeReceive=true;
    uiCATReceive=new UIChooseAccType();
    uiCATReceive->setReceiver();
    connect(uiCATReceive,SIGNAL(sigChooseReceiverTypeComplete()),this,SLOT(inputReceiverCard()));
    uiCATReceive->exec();
}

void UIP2PTransfer::setAccountType(AccType type)
{
    qDebug() << Q_FUNC_INFO<<type;
    // ---------- -------------//
    this->swipeCard();
}

void UIP2PTransfer::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSC=new UISwipeCard();
    if(g_changeParam.p2p.MANUAL_ENABLE==false)
    {
        uiSC->setNoManual();
    }
    connect(uiSC,SIGNAL(sigFinishPutCard()),this,SLOT(inputAmount()));
    connect(uiSC,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));
    connect(uiSC,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSC,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputAmountAndExit()));


    uiSC->exec();
}
void UIP2PTransfer::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiIMSend=new UIInputManual();
    connect(uiIMSend,SIGNAL(sigInputComplete()),this,SLOT(inputAmount()));
    connect(uiIMSend,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiIMSend->exec();
}

void UIP2PTransfer::inputAmount()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputAmount=true;
    uiIA=new UIInputAmount();
    connect(uiIA,SIGNAL(sigAmountInputComplete(QString)),this,SLOT(inputPIN(QString)));
    connect(uiIA,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    uiIA->exec();
}

void UIP2PTransfer::inputAmountAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputAmount=true;
    uiIA=new UIInputAmount();
    this->quitFromFlow();
}

void UIP2PTransfer::inputPIN(QString strAmt)
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiIPIN=new UIInputPIN();
    uiIPIN->slotSetAmount(strAmt);
    if(g_changeParam.p2p.PIN_ENABLE==false)
        uiIPIN->slotDisablePIN();
    connect(uiIPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiIPIN,SIGNAL(sigSubmit()),this,SLOT(chooseReceiverAccountType()));
    uiIPIN->exec();
}

void UIP2PTransfer::inputReceiverCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputReceiveCard=true;
    uiIMReceive=new UIInputManual();
    uiIMReceive->setEnterReceiver();  // 输入接收账户
    connect(uiIMReceive,SIGNAL(sigInputComplete()),this,SLOT(transOnline()));
    connect(uiIMReceive,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiIMReceive->exec();
}

void UIP2PTransfer::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTO=new UITransOnline();
    uiTO->slotStartTrans(NormalTransData.transType);
    connect(uiTO,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTO,SIGNAL(sigTransSuccess()),this,SLOT(printReceipt()));

    uiTO->exec();

}

void UIP2PTransfer::printReceipt()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_PrintReceipt=true;

    uiP=new UIPrint();
    connect(uiP,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    uiP->exec();
}

// ----------------------------------------------------------------------------------------------- //
void UIP2PTransfer::quitFromSwipeCard()
{
    qDebug()<<Q_FUNC_INFO;

    uiSC->close();
    uiCAT->close();
    uiIP->close();

    this->close();
}


void UIP2PTransfer::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_PrintReceipt==true)
    {
        qDebug()<<"step1";
        uiP->close();
        FLAG_PrintReceipt=false;
    }
    if(FLAG_TransOnline==true)
    {
        qDebug()<<"step2";
        uiTO->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputReceiveCard==true)
    {
        qDebug()<<"step3";
        uiIMReceive->close();
        FLAG_InputReceiveCard=false;
    }
    if(FLAG_AccountTypeReceive==true)
    {
        qDebug()<<"step4";
        uiCATReceive->close();
        FLAG_AccountTypeReceive=false;
    }
    if(FLAG_InputPIN==true)
    {
        qDebug()<<"step5";
        uiIPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputAmount==true)
    {
        qDebug()<<"step6";
        FLAG_InputAmount=false;
        uiIA->close();
    }
    if(FLAG_SwipeCard==true)
    {
        qDebug()<<"step7";
        uiSC->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_InputManual==true)
    {
        qDebug()<<"step8";
        uiIMSend->close();
        FLAG_InputManual=false;
    }
    if(FLAG_AccountType==true)
    {
        qDebug()<<"step9";
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
        qDebug()<<"step10";
        uiIP->close();
        FLAG_InputPassword=false;
    }

    UIMsg::showErrMsgWithAutoClose(ERR_CANCEL,g_changeParam.TIMEOUT_ERRMSG);

    this->close();
}

void UIP2PTransfer::finishFromFlow()
{
    qDebug()<<Q_FUNC_INFO;
    if(FLAG_PrintReceipt==true)
    {
        qDebug()<<"step1";
        uiP->close();
        FLAG_PrintReceipt=false;
    }
    if(FLAG_TransOnline==true)
    {
        qDebug()<<"step2";
        uiTO->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputReceiveCard==true)
    {
        qDebug()<<"step3";
        uiIMReceive->close();
        FLAG_InputReceiveCard=false;
    }
    if(FLAG_AccountTypeReceive==true)
    {
        qDebug()<<"step4";
        uiCATReceive->close();
        FLAG_AccountTypeReceive=false;
    }
    if(FLAG_InputPIN==true)
    {
        qDebug()<<"step5";
        uiIPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputAmount==true)
    {
        qDebug()<<"step6";
        FLAG_InputAmount=false;
        uiIA->close();
    }
    if(FLAG_SwipeCard==true)
    {
        qDebug()<<"step7";
        uiSC->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_InputManual==true)
    {
        qDebug()<<"step8";
        uiIMSend->close();
        FLAG_InputManual=false;
    }
    if(FLAG_AccountType==true)
    {
        qDebug()<<"step9";
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
        qDebug()<<"step10";
        uiIP->close();
        FLAG_InputPassword=false;
    }

    this->close();
}
static void TRANS_CleanTransData(void)
{
    memset(&NormalTransData, 0, sizeof(NormalTransData));
    memset(&ExtraTransData, 0, sizeof(ExtraTransData));
}
