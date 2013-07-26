#include "uiInputPassword.h"
#include "transData.h"
#include "xdata.h"
#include "sav.h"
#include "global.h"

UIInputPassword::UIInputPassword(QDialog *parent,Qt::WindowFlags f) :
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
    this->setGeometry(0,FRAME420_THVALUE+40,FRAME420_WIDTH,FRAME420_HEIGHT-40);
    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT-40);
    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Login"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbCashier=new QLabel();
    lbPass=new QLabel();
    leCashier=new QLineEdit();
    lePass=new QLineEdit();

    lbCashier->setText("Input Cashier No. :");
    lbPass->setText("Input Password :");

    lbCashier->setFont(font);
    lbPass->setFont(font);
    leCashier->setFont(font);
    lePass->setFont(font);


    // 特殊处理
    lePass->setEchoMode(QLineEdit::Password);
    leCashier->setMaxLength(CASH_SYSCASHIERNOLEN);
    lePass->setMaxLength(CASH_SUPERPASSWDLEN);

    const QString  REGEX_AMOUNT = "^[0-9]\\d*$";
    QRegExp regx(REGEX_AMOUNT);
    QValidator *validator1 = new QRegExpValidator(regx, lePass);
    QValidator *validator2 = new QRegExpValidator(regx, leCashier);
    lePass->setValidator(validator1);
    leCashier->setValidator(validator2);
    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Login"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    
    
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbCashier);
    v1Lay->addWidget(leCashier);
    v1Lay->addWidget(lbPass);
    v1Lay->addWidget(lePass);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotFinishTrans()));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(startAuthorize()));
    //    layout->setContentsMargins();

    setTabOrder(leCashier,lePass);
    setTabOrder(lePass,btnSubmit);
    setTabOrder(btnSubmit,btnCancel);

   this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

UIInputPassword::~UIInputPassword()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIInputPassword::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->slotFinishTrans();
        break;
    case Qt::Key_Enter:
        focusNextChild();
        break;
    case Qt::Key_F4:
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

// 验证用户权限
void UIInputPassword::startAuthorize()
{
    qDebug() << Q_FUNC_INFO;
    closeTimer->stop();

    // 检查交易是否已满
    unsigned char ucResult=SAV_CheckNormalTransIndex();
    if(ucResult!=SUCCESS)
    {
        UIMsg::showCombineErrMsgWithAutoClose(ErrIndex(ucResult),g_changeParam.TIMEOUT_ERRMSG);
        slotFinishTrans();

        closeTimer->start(g_changeParam.TIMEOUT_UI);
        return;
    }

    unsigned char ucIndex = 0;
    unsigned char * aucCashierNo = NULL;
    aucCashierNo = (unsigned char *)malloc(sizeof(unsigned char)*(CASH_CASHIERNOLEN+1));
    memset(aucCashierNo,0,(CASH_CASHIERNOLEN+1));

    // 检查都有输入
    if(leCashier->text().isEmpty() || lePass->text().isEmpty())
    {
        qDebug()<<"把东西填满咯";

        UIMsg::showNoticeMsgWithAutoClose(INCOMPLETE_INFORMATION,g_changeParam.TIMEOUT_ERRMSG);

        this->resetLine();

        closeTimer->start(g_changeParam.TIMEOUT_UI);

        return;
    }

    int inputUserNo=leCashier->text().toInt();
    QString inputPass=lePass->text();

    int_asc(aucCashierNo,2,(unsigned int *)&inputUserNo);
    qDebug("aucCashierNo = %s\n",aucCashierNo);

    // 获取超级管理员密码
    int superPassUserNo = (int)asc_long(g_cashier.aucSuperNo,CASH_MAXSUPERNO);

    // 判断超级管理员99
    if( inputUserNo==superPassUserNo)
    {
        if(!memcmp(inputPass.toAscii().data(),g_cashier.aucSuperPSW,CASH_SUPERPASSWDLEN))
        {
            qDebug()<<"给予系统管理员权限";
            emit sigLogInSuccess(typeAdmin,leCashier->text());

            return;
        }
        else
        {
            qDebug()<<"系统管理员密码错误";

            UIMsg::showErrMsgWithAutoClose(ERR_CASH_PASS,g_changeParam.TIMEOUT_ERRMSG);
            this->resetLine();
            closeTimer->start(g_changeParam.TIMEOUT_UI);

            return;
        }
    }
    else if(inputUserNo!=superPassUserNo)
    {
        // 判断管理员00
        if(inputUserNo == ((int)asc_long(g_cashier.aucSYSCashierNo,CASH_SYSCASHIERNOLEN)))
        {
            if(!memcmp(inputPass.toAscii().data(),g_cashier.aucSYSCashierPass,6))
            {
                qDebug()<<"超级密码正确,给予管理员权限";
                emit sigLogInSuccess(typeManager,leCashier->text());

                return;
            }
            else
            {
                qDebug()<<"超级密码错误";

                UIMsg::showErrMsgWithAutoClose(ERR_CASH_PASS,g_changeParam.TIMEOUT_ERRMSG);
                this->resetLine();
                closeTimer->start(g_changeParam.TIMEOUT_UI);

                return;
            }
        }
        // 普通柜员
        else
        {
            if(xDATA::CheckCashierNo(&ucIndex,aucCashierNo) == ERR_CASH_NOTEXIST)
            {
                qDebug()<<"柜员不存在";

                UIMsg::showErrMsgWithAutoClose(ERR_CASH_NOTEXIST,g_changeParam.TIMEOUT_ERRMSG);
                this->resetLine();
                closeTimer->start(g_changeParam.TIMEOUT_UI);

                return;
            }
            else
            {
                qDebug()<<"柜员存在,验证密码";
                if(!memcmp(inputPass.toAscii().data(),g_cashier.aucCashierPass[ucIndex],CASH_CASHIERPASSLEN))
                {
                    qDebug()<<"密码正确,给予柜员权限";
                    emit sigLogInSuccess(typeCashier,leCashier->text());
                    return;
                }
                else
                {
                    qDebug()<<"密码错误";

                    UIMsg::showErrMsgWithAutoClose(ERR_CASH_PASS,g_changeParam.TIMEOUT_ERRMSG);
                    this->resetLine();
                    closeTimer->start(g_changeParam.TIMEOUT_UI);

                    return;
                }
            }
        }
    }
}

void UIInputPassword::slotFinishTrans()
{
    emit sigFinishTrans();
}

void UIInputPassword::resetLine()
{
    leCashier->clear();
    lePass->clear();
    leCashier->setFocus();
}

void UIInputPassword::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotFinishTrans()));
    closeTimer->start(timeout);
}
