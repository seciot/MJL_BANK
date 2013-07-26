#include "uiAdjust.h"
#include "sav.h"
#include "xdata.h"
#include "commontools.h"
#include "global.h"

static void TRANS_CleanTransData(void);

UIAdjust::UIAdjust(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    TRANS_CleanTransData();
    RemoveKeyEventBug();
    xDATA::ReadValidFile(xDATA::DataSaveChange);
    xDATA::ReadValidFile(xDATA::DataSaveConstant);


    FLAG_InputPassword=false;
    FLAG_Detail=false;
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
    lbHead->setText(tr("Adjust"));
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

UIAdjust::~UIAdjust()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIAdjust::keyPressEvent(QKeyEvent *event)
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

void UIAdjust::checkAuth(UserType ut,QString ID)
{
    // 交易关闭
    qDebug()<<ut<<ID<<g_changeParam.boolCashierLogonFlag;
    if(g_changeParam.transvoid.TRANS_ENABLE==false)
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

        this->inputTraceNo();
    }
    else
    {
        qDebug()<<"不支持柜员以外的用户做交易";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);
        uiInPass->resetLine();

        return;
    }
}

void UIAdjust::inputTraceNo()
{
    qDebug() << Q_FUNC_INFO;
    QString transType;
    QString cardNo;
    QString amount;
    QString refNo;
    QString apprNo;
    QString operatorNo;

    bool ok;
    unsigned int traceNo=UIInput::getInt("TRACE NO","Please Input\nOriginal Trace No. :",REGEX_NUMBER,6,&ok);
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
                    // 显示详细交易信息
                    qDebug()<<traceNo<<"Matched";
                    switch(NormalTransData.transType)
                    {
                    case TransMode_CashDeposit:      //存钱
                        transType="Cash Deposit";
                        NormalTransData.transType=TransMode_DepositAdjust;
                        break;
                    case TransMode_CashAdvance:      //取钱
                        transType="Cash Advance";
                        NormalTransData.transType=TransMode_AdvanceAdjust;
                        break;
                    default:
                        qDebug()<<"不支持调整";
                        UIMsg::showErrMsgWithAutoClose("This Transaction/nNot Support/nFor Adjust",g_changeParam.TIMEOUT_ERRMSG);
                        return;
                    }

                    qDebug()<<"step2"<<transType;

                    //Card No
                    cardNo=QString::fromAscii((const char *)NormalTransData.aucSourceAcc);  // 需要部分隐藏
                    qDebug()<<"card no:"<<cardNo;

                    // Amount
                    unsigned char aucBuf[12];
                    getAmount(aucBuf, NormalTransData.ulAmount, 2);
                    amount=QString::fromAscii((const char *)aucBuf);
                    qDebug()<<"amount:"<<amount;

                    // ref & appr
                    refNo=QString::fromAscii((const char *)NormalTransData.aucRefNum);
                    apprNo=QString::fromAscii((const char *)NormalTransData.aucAuthCode);
                    qDebug()<<"ref & appr:"<<refNo<<apprNo;

                    //operator
                    operatorNo=QString::fromAscii((const char *)NormalTransData.aucCashier);

                    FLAG_Detail=true;
                    uiRepDetail=new UIReportDetail(1);
                    uiRepDetail->slotSetFun("Adjust");
                    uiRepDetail->slotSetDetailList(transType,cardNo,amount,refNo,apprNo,operatorNo);
                    connect(uiRepDetail,SIGNAL(sigFun()),this,SLOT(swipeCard()));
                    uiRepDetail->exec();

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

void UIAdjust::swipeCard()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_SwipeCard=true;
    uiSwipeCard=new UISwipeCard();
    if(g_changeParam.transvoid.MANUAL_ENABLE==false)
    {
        uiSwipeCard->setNoManual();
    }
    connect(uiSwipeCard,SIGNAL(sigQuitTrans()),this,SLOT(quitFromSwipeCard()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutCard()),this,SLOT(inputAmount()));
    connect(uiSwipeCard,SIGNAL(sigFinishPutNotSupportCard()),this,SLOT(inputAmountAndExit()));
    connect(uiSwipeCard,SIGNAL(sigSwitchToManual()),this,SLOT(inputManual()));


    uiSwipeCard->exec();
}

void UIAdjust::inputManual()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputManual=true;
    uiInMan=new UIInputManual();
    connect(uiInMan,SIGNAL(sigInputComplete()),this,SLOT(inputAmount()));
    connect(uiInMan,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));

    uiInMan->exec();
}

void UIAdjust::inputAmount()
{
    qDebug() << Q_FUNC_INFO;

    FLAG_InputAmount=true;
    uiInAmt=new UIInputAmount();
    uiInAmt->slotSetAdjust();
    connect(uiInAmt,SIGNAL(sigAmountInputComplete(QString)),this,SLOT(inputPIN(QString)));
    connect(uiInAmt,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    uiInAmt->exec();
}

void UIAdjust::inputAmountAndExit()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputAmount=true;
    uiInAmt=new UIInputAmount();
    this->quitFromFlow();
}

void UIAdjust::inputPIN(QString strAmt)
{
    qDebug() << Q_FUNC_INFO;
    FLAG_InputPIN=true;

    uiInPIN=new UIInputPIN();
    uiInPIN->slotSetAmount(strAmt);
    if(g_changeParam.transvoid.PIN_ENABLE==false)
        uiInPIN->slotDisablePIN();
    connect(uiInPIN,SIGNAL(sigQuitTrans()),this,SLOT(quitFromFlow()));
    connect(uiInPIN,SIGNAL(sigSubmit()),this,SLOT(transOnline()));
    uiInPIN->exec();
}

void UIAdjust::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTransOn=new UITransOnline();
    uiTransOn->slotStartTrans(NormalTransData.transType);
    connect(uiTransOn,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTransOn,SIGNAL(sigTransSuccess()),this,SLOT(printReceipt()));

    uiTransOn->exec();

}

void UIAdjust::printReceipt()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_PrintReceipt=true;

    uiPrint=new UIPrint();
    connect(uiPrint,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    uiPrint->exec();
}

// ----------------------------------------------------------------------------------------------- //
void UIAdjust::quitFromSwipeCard()
{
    qDebug()<<Q_FUNC_INFO;

    uiSwipeCard->close();
    uiRepDetail->close();
    uiInPass->close();

    this->close();
}


void UIAdjust::quitFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_PrintReceipt==true)
    {
        uiPrint->close();
        FLAG_PrintReceipt=false;
    }
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
    if(FLAG_InputAmount==true)
    {
        FLAG_InputAmount=false;
        uiInAmt->close();
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
    if(FLAG_Detail==true)
    {
        uiRepDetail->close();
        FLAG_Detail=false;
    }
    if(FLAG_InputPassword==true)
    {
        uiInPass->close();
        FLAG_InputPassword=false;
    }

    UIMsg::showErrMsgWithAutoClose(ERR_CANCEL,g_changeParam.TIMEOUT_ERRMSG);

    this->close();
}

void UIAdjust::finishFromFlow()
{
    qDebug()<<Q_FUNC_INFO;

    if(FLAG_PrintReceipt==true)
    {
        uiPrint->close();
        FLAG_PrintReceipt=false;
    }
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
    if(FLAG_InputAmount==true)
    {
        FLAG_InputAmount=false;
        uiInAmt->close();
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
    if(FLAG_Detail==true)
    {
        uiRepDetail->close();
        FLAG_Detail=false;
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
