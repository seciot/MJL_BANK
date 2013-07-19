#include "uiCashDeposit.h"
#include "sav.h"
#include "xdata.h"
#include "global.h"

static void TRANS_CleanTransData(void);

UICashDeposit::UICashDeposit(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    TRANS_CleanTransData();
    NormalTransData.transType = TransMode_CashDeposit;
    RemoveKeyEventBug();
    xDATA::ReadValidFile(xDATA::DataSaveChange);
    xDATA::ReadValidFile(xDATA::DataSaveConstant);

    FLAG_InputPassword=false;
    FLAG_AccountType=false;
    FLAG_SwipeCard=false;
    FLAG_InputAmount=false;
    FLAG_InputManual=false;
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
    lbHead->setText(tr("Cash Deposit"));
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

UICashDeposit::~UICashDeposit()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UICashDeposit::keyPressEvent(QKeyEvent *event)
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

void UICashDeposit::chooseAccountType(UserType ut,QString ID)
{
    qDebug()<<ut<<ID;

    if(g_changeParam.deposit.TRANS_ENABLE==false)
    {
        UIMsg::showErrMsg("Transaction Disabled");
        return;
    }
    if(g_changeParam.boolCashierLogonFlag==false)
    {
        UIMsg::showErrMsg("Please Logon");
        return;
    }
    //--------------------------------------- //
    if(ut==typeCashier)
    {
        FLAG_AccountType=true;
        memcpy(NormalTransData.aucCashier , ID.toAscii().data() , 2);
        uiCAT=new UIChooseAccType();
        connect(uiCAT,SIGNAL(sigChooseTypeComplete(AccType)),this,SLOT(setAccountType(AccType)));
        uiCAT->exec();
    }
    else
    {
        qDebug()<<"不支持柜员以外的用户做交易";

        UIMsg::showNoticeMsg(NO_PERMISSION);
        uiIP->resetLine();


        return;
    }
}

void UICashDeposit::setAccountType(AccType type)
{
    qDebug() << Q_FUNC_INFO<<type;
    // ---------- -------------//
    this->swipeCard();
}

void UICashDeposit::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSC=new UISwipeCard();
    if(g_changeParam.deposit.MANUAL_ENABLE==false)
    {
            uiSC->setNoManual();
    }
    connect(uiSC,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSC,SIGNAL(sigFinishPutCard()),this,SLOT(inputAmount()));
    connect(uiSC,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputAmountAndExit()));
    connect(uiSC,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));

    uiSC->exec();
}

void UICashDeposit::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiIM=new UIInputManual();
    connect(uiIM,SIGNAL(sigInputComplete()),this,SLOT(inputAmount()));
    connect(uiIM,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiIM->exec();
}

void UICashDeposit::inputAmount()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputAmount=true;
    uiIA=new UIInputAmount();
    connect(uiIA,SIGNAL(sigAmountInputComplete(QString)),this,SLOT(inputPIN(QString)));
    connect(uiIA,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    uiIA->exec();
}

void UICashDeposit::inputAmountAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputAmount=true;
    uiIA=new UIInputAmount();
    this->quitFromFlow();
}

void UICashDeposit::inputPIN(QString strAmt)
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiIPIN=new UIInputPIN();
    uiIPIN->slotSetAmount(strAmt);
    if(g_changeParam.deposit.PIN_ENABLE==false)
        uiIPIN->slotDisablePIN();
    connect(uiIPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiIPIN,SIGNAL(sigSubmit()),this,SLOT(transOnline()));
    uiIPIN->exec();
}

void UICashDeposit::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTO=new UITransOnline();
    uiTO->slotStartTrans(NormalTransData.transType);
    connect(uiTO,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTO,SIGNAL(sigTransSuccess()),this,SLOT(printReceipt()));

    uiTO->exec();

}

void UICashDeposit::printReceipt()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_PrintReceipt=true;

    uiP=new UIPrint();
    connect(uiP,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    uiP->exec();
}

// ----------------------------------------------------------------------------------------------- //
void UICashDeposit::quitFromSwipeCard()
{
    qDebug()<<Q_FUNC_INFO;

    uiSC->close();
    uiCAT->close();
    uiIP->close();

    this->close();
}


void UICashDeposit::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_PrintReceipt==true)
    {
        uiP->close();
        FLAG_PrintReceipt=false;
    }
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
    if(FLAG_InputAmount==true)
    {
        FLAG_InputAmount=false;
        uiIA->close();
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
    if(FLAG_AccountType==true)
    {
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiIP->close();
        FLAG_InputPassword=false;
    }
    UIMsg::showErrMsg(ERR_CANCEL);

    this->close();
}

void UICashDeposit::finishFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_PrintReceipt==true)
    {
        uiP->close();
        FLAG_PrintReceipt=false;
    }
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
    if(FLAG_InputAmount==true)
    {
        FLAG_InputAmount=false;
        uiIA->close();
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
    if(FLAG_AccountType==true)
    {
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
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
