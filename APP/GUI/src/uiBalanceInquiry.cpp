#include "uiBalanceInquiry.h"
#include "xdata.h"
#include "global.h"

static void TRANS_CleanTransData(void);

UIBalanceInquiry::UIBalanceInquiry(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    TRANS_CleanTransData();
    NormalTransData.transType = TransMode_BalanceInquiry;
    RemoveKeyEventBug();
    xDATA::ReadValidFile(xDATA::DataSaveConstant);
    xDATA::ReadValidFile(xDATA::DataSaveChange);

    FLAG_InputPassword=false;
    FLAG_AccountType=false;
    FLAG_SwipeCard=false;
    FLAG_InputManual=false;
    FLAG_InputPIN=false;
    FLAG_TransOnline=false;
    FLAG_ShowBalance=false;

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
    lbHead->setText(tr("Balance Inquiry"));
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

    uiInPass=new UIInputPassword();
    connect(uiInPass,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(chooseAccountType(UserType,QString)));
    connect(uiInPass,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiInPass, SLOT(exec()));
    passThread->start();
}

UIBalanceInquiry::~UIBalanceInquiry()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIBalanceInquiry::keyPressEvent(QKeyEvent *event)
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

void UIBalanceInquiry::chooseAccountType(UserType ut,QString ID)
{
    qDebug()<<ut<<ID;
    //--------------------------------------- //
    if(g_changeParam.balance.TRANS_ENABLE==false)
    {
        UIMsg::showErrMsgWithAutoClose("Transaction Disabled",g_changeParam.TIMEOUT_ERRMSG);
        return;
    }
    if(g_changeParam.boolCashierLogonFlag==false)
    {
        UIMsg::showErrMsgWithAutoClose("Please Logon",g_changeParam.TIMEOUT_ERRMSG);
        return;
    }
    if(ut==typeCashier)
    {
        FLAG_AccountType=true;
        memcpy(NormalTransData.aucCashier , ID.toAscii().data() , 2);
        uiCAT=new UIChooseAccType();
        connect(uiCAT,SIGNAL(sigChooseTypeComplete(AccType)),this,SLOT(setAccountType(AccType)));
        connect(uiCAT,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
        uiCAT->exec();
    }
    else
    {
        qDebug()<<"不支持柜员以外的用户做交易";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);

        uiInPass->resetLine();

        return;
    }
}

void UIBalanceInquiry::setAccountType(AccType type)
{
    qDebug() << Q_FUNC_INFO<<type;
    // ---------- -------------//
    this->swipeCard();
}

void UIBalanceInquiry::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSwipeCard=new UISwipeCard();
    if(g_changeParam.p2p.MANUAL_ENABLE==false)
    {
            uiSwipeCard->setNoManual();
    }
    connect(uiSwipeCard,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutCard()),this,SLOT(inputAmount()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputPinAndExit()));
    connect(uiSwipeCard,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));


    uiSwipeCard->exec();
}

void UIBalanceInquiry::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiInMan=new UIInputManual();
    connect(uiInMan,SIGNAL(sigInputComplete()),this,SLOT(inputPIN()));
    connect(uiInMan,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiInMan->exec();
}

void UIBalanceInquiry::inputPinAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;
    uiInPIN=new UIInputPIN();
    this->quitFromFlow();
}

void UIBalanceInquiry::inputPIN()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiInPIN=new UIInputPIN();
    if(g_changeParam.balance.PIN_ENABLE==false)
        uiInPIN->slotDisablePIN();
    connect(uiInPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiInPIN,SIGNAL(sigSubmit()),this,SLOT(transOnline()));
    uiInPIN->exec();
}

void UIBalanceInquiry::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTransOn=new UITransOnline();
    uiTransOn->slotStartTrans(NormalTransData.transType);
    connect(uiTransOn,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiTransOn,SIGNAL(sigTransSuccess()),this,SLOT(showBalance()));
//    connect(uiTransOn,SIGNAL(sigTransSuccess()),this,SLOT(finishFromFlow()));

    uiTransOn->exec();

}

void UIBalanceInquiry::showBalance()
{
    qDebug()<<Q_FUNC_INFO;
    FLAG_ShowBalance=true;

    uiSB=new UIShowBalance();
    connect(uiSB,SIGNAL(sigFinishFlow()),this,SLOT(finishFromFlow()));

    uiSB->exec();
}

// ----------------------------------------------------------------------------------------------- //
void UIBalanceInquiry::quitFromSwipeCard()
{
    qDebug()<<Q_FUNC_INFO;

    uiSwipeCard->close();
    uiCAT->close();
    uiInPass->close();

    this->close();
}


void UIBalanceInquiry::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_ShowBalance==true)
        uiSB->close();
    if(FLAG_TransOnline==true)
    {
        uiTransOn->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputPIN==true)
    {
        uiInPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputManual==true)
    {
        uiInMan->close();
        FLAG_InputManual=false;
    }
    if(FLAG_SwipeCard==true)
    {
        uiSwipeCard->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_AccountType==true)
    {
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiInPass->close();
        FLAG_InputPassword=false;
    }

    UIMsg::showErrMsgWithAutoClose(ERR_CANCEL,g_changeParam.TIMEOUT_ERRMSG);

    this->close();
}

void UIBalanceInquiry::finishFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_ShowBalance==true)
        uiSB->close();
    if(FLAG_TransOnline==true)
    {
        uiTransOn->close();
        FLAG_TransOnline=false;
    }
    if(FLAG_InputPIN==true)
    {
        uiInPIN->close();
        FLAG_InputPIN=false;
    }
    if(FLAG_InputManual==true)
    {
        uiInMan->close();
        FLAG_InputManual=false;
    }
    if(FLAG_SwipeCard==true)
    {
        uiSwipeCard->close();
        FLAG_SwipeCard=false;
    }
    if(FLAG_AccountType==true)
    {
        uiCAT->close();
        FLAG_AccountType=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiInPass->close();
        FLAG_InputPassword=false;
    }

    this->close();
}
static void TRANS_CleanTransData(void)
{
    memset(&NormalTransData, 0, sizeof(NormalTransData));
    memset(&ExtraTransData, 0, sizeof(ExtraTransData));
}
