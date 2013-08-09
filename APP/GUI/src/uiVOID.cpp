#include "uiVOID.h"
#include "sav.h"
#include "xdata.h"
#include "commontools.h"
#include "sav.h"
#include "global.h"

UIVOID::UIVOID(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 先清数据
    xDATA::ClearGlobalData();
    RemoveKeyEventBug();

    FLAG_InputPassword=false;
    FLAG_Detail=false;
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
    lbHead->setText(tr("VOID"));
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

UIVOID::~UIVOID()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIVOID::keyPressEvent(QKeyEvent *event)
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

void UIVOID::checkAuth(UserType ut,QString ID)
{
    // 交易关闭
    qDebug()<<ut<<ID<<g_changeParam.boolCashierLogonFlag;
    if(g_constantParam.transvoid.TRANS_ENABLE==false)
    {
        UIMsg::showErrMsgWithAutoClose("Transaction Disabled",g_constantParam.TIMEOUT_ERRMSG);

        return;
    }
    if(g_changeParam.boolCashierLogonFlag==false)
    {
        UIMsg::showErrMsgWithAutoClose("Please Logon",g_constantParam.TIMEOUT_ERRMSG);
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

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_constantParam.TIMEOUT_ERRMSG);
        uiInPass->resetLine();

        return;
    }
}

void UIVOID::inputTraceNo()
{
    qDebug() << Q_FUNC_INFO;
    bool ok = false;
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned int traceNo;
    QString transType;
    QString transStatus;
    QString cardNo;
    QString amount;
    QString refNo;
    QString apprNo;
    QString operatorNo;

    traceNo = UIInput::getInt("TRACE NO","Please Input\nOriginal Trace No. :",REGEX_NUMBER,6,&ok);
    qDebug("OK = %x", ok);
    if(!ok)
        ucResult = ERR_CANCEL;
    if(!ucResult)
    {
        qDebug()<<traceNo;
        NormalTransData.ulOrgTraceNumber = traceNo;
        ucResult = SAV_CheckVoidTransIndex(&NormalTransData);
    }
    if(ucResult)
    {
        UIMsg::showErrMsgWithAutoClose((ErrIndex)ucResult,g_constantParam.TIMEOUT_ERRMSG);
        return;
    }


    //显示一下原交易
    if(g_saveTrans.transType == TransMode_CashDeposit)
        transType="Cash Deposit";   //存款
    else if(g_saveTrans.transType == TransMode_CashAdvance)
        transType="Cash Advance";   //取款
    else if(g_saveTrans.transType == TransMode_DepositAdjust)
        transType="Cash Deposit Adjust";
    else if(g_saveTrans.transType == TransMode_AdvanceAdjust)
        transType="Cash Advance Adjust";
    qDebug()<<"step2"<<transType;

    // 交易状态
    transStatus="NORMAL";

    //Card No
    cardNo=QString::fromAscii((const char *)g_saveTrans.aucSourceAcc);  // 需要部分隐藏
    qDebug()<<"card no:"<<cardNo;

    // Amount
    unsigned char aucBuf[12];
    getAmount(aucBuf, g_saveTrans.ulAmount, 2);
    amount=QString::fromAscii((const char *)aucBuf);
    qDebug()<<"amount:"<<amount;

    // ref & appr
    refNo=QString::fromAscii((const char *)g_saveTrans.aucRefNum);
    apprNo=QString::fromAscii((const char *)g_saveTrans.aucAuthCode);
    qDebug()<<"ref & appr:"<<refNo<<apprNo;

    //operator
    operatorNo=QString::fromAscii((const char *)g_saveTrans.aucCashier);

    FLAG_Detail=true;
    uiRepDetail=new UIReportDetail(1);
    uiRepDetail->slotSetFun("VOID");
    uiRepDetail->slotSetDetailList(transType, transStatus, cardNo,amount,refNo,apprNo,operatorNo);
    connect(uiRepDetail,SIGNAL(sigFun()),this,SLOT(confirmVOID()));
    connect(uiRepDetail,SIGNAL(sigClose()),this,SLOT(quitFromFlow()));
    uiRepDetail->exec();
}

void UIVOID::confirmVOID()
{
    if(UIInput::getConfirm("VOID","Really VOID?"))
        this->transOnline();
}

void UIVOID::transOnline()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_TransOnline=true;

    uiTransOn=new UITransOnline();
    uiTransOn->slotStartTrans(NormalTransData.transType);
    connect(uiTransOn,SIGNAL(sigQuitTrans()),this,SLOT(finishFromFlow()));
    connect(uiTransOn,SIGNAL(sigTransSuccess()),this,SLOT(voidSuccess()));

    uiTransOn->exec();

}

void UIVOID::voidSuccess()
{
    qDebug() << Q_FUNC_INFO;

    this->printReceipt();
}

void UIVOID::printReceipt()
{
    qDebug() << Q_FUNC_INFO;
    FLAG_PrintReceipt=true;

    uiPrint=new UIPrint();
    connect(uiPrint,SIGNAL(sigFinishTrans()),this,SLOT(finishFromFlow()));
    uiPrint->exec();
}


void UIVOID::quitFromFlow()
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

    UIMsg::showErrMsgWithAutoClose(ERR_CANCEL,g_constantParam.TIMEOUT_ERRMSG);

    this->close();
}

void UIVOID::finishFromFlow()
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
