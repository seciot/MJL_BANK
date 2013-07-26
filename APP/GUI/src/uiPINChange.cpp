#include "uiPINChange.h"
#include "sav.h"
#include "xdata.h"
#include "commontools.h"

extern "C"
{
    #include "key.h"
}

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

    uiInPass=new UIInputPassword();
    connect(uiInPass,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(checkAuth(UserType,QString)));
    connect(uiInPass,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiInPass, SLOT(exec()));
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
        uiInPass->resetLine();

        return;
    }
}

void UIPINChange::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSwipeCard=new UISwipeCard();
    if(g_changeParam.pinchange.MANUAL_ENABLE==false)
    {
        qDebug()<<"设置不可手动";
        uiSwipeCard->setNoManual();
    }
    connect(uiSwipeCard,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutCard()),this,SLOT(inputPIN()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputPINAndExit()));
    connect(uiSwipeCard,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));


    uiSwipeCard->exec();
}

void UIPINChange::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiInMan=new UIInputManual();
    connect(uiInMan,SIGNAL(sigInputComplete()),this,SLOT(inputPIN()));
    connect(uiInMan,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiInMan->exec();
}

void UIPINChange::inputPINAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;
    uiInPIN=new UIInputPIN();
    this->quitFromFlow();
}

void UIPINChange::inputPIN()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiInPIN=new UIInputPIN();
    if(g_changeParam.pinchange.PIN_ENABLE==false)
        uiInPIN->slotDisablePIN();
    connect(uiInPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiInPIN,SIGNAL(sigSubmit()),this,SLOT(inputNewPIN()));
    uiInPIN->exec();
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
                unsigned char *ucChangePIN=NULL;
                ucChangePIN  = (unsigned char *)malloc(sizeof(unsigned char)*50);
                memset(ucChangePIN,0,50);
                memcpy(ucChangePIN,newPIN.toAscii().data(),newPIN.length());

                qDebug()<<"ucPIN::"<<ucChangePIN;
                printf("ucPIN:: %s\n",ucChangePIN);

                unsigned char ucResult;
                ucResult=KEY_EncryptPIN_X98(ucChangePIN,G_EXTRATRANS_aucCardPan_UnAssign,0,G_EXTRATRANS_aucChangePINData);
                if(ucResult==ERR_DRIVER)
                {
                    qDebug()<<"加密密码出错";
                }
                else
                {
                    printf("G_EXTRATRANS_aucPINData:: %s\n",G_EXTRATRANS_aucPINData);
                }

                // 上送
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

    uiTransOn=new UITransOnline();
    uiTransOn->slotStartTrans(NormalTransData.transType);
    connect(uiTransOn,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTransOn,SIGNAL(sigTransSuccess()),this,SLOT(changePINSuccess()));

    uiTransOn->exec();
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

    uiSwipeCard->close();
    uiRepDetail->close();
    uiInPass->close();

    this->close();
}


void UIPINChange::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;


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
    if(FLAG_InputPassword==true)
    {
        uiInPass->close();
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
