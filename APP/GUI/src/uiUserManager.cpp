#include "uiUserManager.h"
#include "xdata.h"
#include "global.h"

UIUserManager::UIUserManager(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QObject::installEventFilter(this);

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
    QFont font4("Helvetica",12);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Cashier Manager"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbUserId=new QLabel();
    lbUserType=new QLabel();
    lbUserNewPass=new QLabel();
    lbUserRePass=new QLabel();

    leUserId=new QLineEdit();
    lbUserTypeText=new QLabel();
    lbNotice=new QLabel();
    leUserNewPass=new QLineEdit();
    leUserRePass=new QLineEdit();

    lbUserId->setText("User ID:");
    lbUserType->setText("User Type:");
    lbNotice->setText("-[Leave Password Blank]-\n-[To Delete the Cashier]-");
    lbUserNewPass->setText("New Password:");
    lbUserRePass->setText("Repeat Password:");
    lbUserTypeText->setText("Cashier");

    lbUserId->setFont(font);
    lbUserType->setFont(font);
    lbUserRePass->setFont(font);
    lbUserNewPass->setFont(font);
    lbUserTypeText->setFont(font);
    leUserId->setFont(font);
    leUserNewPass->setFont(font);
    leUserRePass->setFont(font);

    lbNotice->setAlignment(Qt::AlignCenter);

    // 特殊处理
    leUserId->setMaxLength(2);
    leUserRePass->setMaxLength(CASH_CASHIERPASSLEN);
    leUserNewPass->setMaxLength(CASH_CASHIERPASSLEN);
    leUserRePass->setEchoMode(QLineEdit::Password);
    leUserNewPass->setEchoMode(QLineEdit::Password);

    const QString  REGEX_AMOUNT = "^[0-9]\\d*$";
    QRegExp regx(REGEX_AMOUNT);
    QValidator *validator1 = new QRegExpValidator(regx, leUserNewPass);
    QValidator *validator2 = new QRegExpValidator(regx, leUserRePass);
    leUserNewPass->setValidator(validator1);
    leUserRePass->setValidator(validator2);

    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Submit"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);

    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);
    //    ----------------------------------  //
    QHBoxLayout *hLay=new QHBoxLayout();
    hLay->addWidget(lbUserType);
    hLay->addWidget(lbUserTypeText);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addLayout(hLay);
    v1Lay->addWidget(lbUserId);
    v1Lay->addWidget(leUserId);
    v1Lay->addWidget(lbUserNewPass);
    v1Lay->addWidget(leUserNewPass);
    v1Lay->addWidget(lbUserRePass);
    v1Lay->addWidget(leUserRePass);
    v1Lay->addWidget(lbNotice);

    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(10);
    h1Lay->addLayout(v1Lay);
    h1Lay->addSpacing(10);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(h1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(lbUserTypeText,SIGNAL(currentIndexChanged(int)),this,SLOT(setPasswdWidgetLen(int)));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotProcessUserManager()));
    layout->setContentsMargins(0,0,0,5);

    // ----------------- 验证用户身份----------------- //
    // -------- input cashier password ------------//
    uiInPass=new UIInputPassword();
    connect(uiInPass,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(slotAllowEdit(UserType,QString)));
    connect(uiInPass,SIGNAL(sigFinishTrans()),this,SLOT(quitFromInputPass()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiInPass, SLOT(exec()));
    passThread->start();

    setTabOrder(leUserId,leUserNewPass);
    setTabOrder(leUserNewPass,leUserRePass);
    setTabOrder(leUserRePass,btnSubmit);
    setTabOrder(btnSubmit,btnCancel);

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

UIUserManager::~UIUserManager()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIUserManager::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
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

void UIUserManager::slotProcessUserManager()
{
    qDebug() << Q_FUNC_INFO;
    closeTimer->start(g_changeParam.TIMEOUT_UI);

    unsigned char ucI = 0;
    unsigned char * aucCashierNo = NULL;
    unsigned char * aucCashierPass = NULL;

    aucCashierNo = (unsigned char *)malloc(sizeof(unsigned char)*(CASH_CASHIERNOLEN+1));
    memset(aucCashierNo,0,(CASH_CASHIERNOLEN+1));
    aucCashierPass = (unsigned char *)malloc(sizeof(unsigned char)*(CASH_CASHIERPASSLEN+1));
    memset(aucCashierPass,0,(CASH_CASHIERPASSLEN+1));
    // 如果输入的用户ID已经存在，则更新
    // 如果不存在，则添加这个用户
    // 这尼玛没有删除用户啊, 不需要删除用户，管理员可修改每个柜员的密码
    // leUserId 用户ID
    // leUserNewPass/leUserRePass 用户密码，先确认相同

    // 检查都有输入
    if(leUserId->text().isEmpty())
    {
        qDebug()<<"把东西填满咯";

        UIMsg::showNoticeMsgWithAutoClose(INCOMPLETE_INFORMATION,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    // 检查密码相同
    if(leUserNewPass->text()!=leUserRePass->text())
    {
        qDebug()<<"密码不匹配";

        UIMsg::showNoticeMsgWithAutoClose(PASSWORD_MISMATCHING,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    // 检查用户是否存在
    int userID=leUserId->text().toInt();
    if(userID==0 || userID==99)
    {
        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    int_asc(aucCashierNo,2,(unsigned int *)&userID);

    memset(aucCashierPass,0,(CASH_CASHIERPASSLEN+1));
    memcpy(aucCashierPass,leUserNewPass->text().toAscii().data(),CASH_CASHIERPASSLEN);

    qDebug("aucCashierNo = %s\n",aucCashierNo);
    if( xDATA::CheckCashierNo(&ucI,aucCashierNo) == ERR_CASH_NOTEXIST)
    {
        qDebug()<<"柜员不存在";
        // 柜员不存在则创建
        // 更新用户信息
        xDATA::UpdateCashierData(ucI,aucCashierNo,aucCashierPass);

        UIMsg::showNoticeMsgWithAutoClose(ADD_USER_OK,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }
    else if(leUserNewPass->text()!="" && leUserRePass->text()!="")
    {
        qDebug()<<"柜员存在,则更新柜员的密码";
        xDATA::UpdateCashierData(ucI,aucCashierNo,aucCashierPass);

        UIMsg::showNoticeMsgWithAutoClose(CHANGE_PASSWORD_OK,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }
    else
    {
        qDebug()<<"柜员存在,密码为空,删除柜员";

        memset(aucCashierNo,0,(CASH_CASHIERNOLEN+1));
        memset(aucCashierPass,0,(CASH_CASHIERPASSLEN+1));
        xDATA::UpdateCashierData(ucI,aucCashierNo,aucCashierPass);

        UIMsg::showNoticeMsgWithAutoClose(DELETE_USER_OK,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    return;
}

void UIUserManager::setPasswdWidgetLen(int user)
{
    qDebug() << Q_FUNC_INFO<<user;
    switch(user)
    {
    case typeAdmin:
        qDebug()<<"Admin";
        leUserId->setMaxLength(CASH_SYSCASHIERNOLEN);
        leUserRePass->setMaxLength(CASH_SUPERPASSWDLEN);
        leUserNewPass->setMaxLength(CASH_SUPERPASSWDLEN);
        break;
    case typeManager:
        qDebug()<<"Manager";
        leUserId->setMaxLength(CASH_SYSCASHIERNOLEN);
        leUserRePass->setMaxLength(CASH_SYSCASHIERPASSLEN);
        leUserNewPass->setMaxLength(CASH_SYSCASHIERPASSLEN);
        break;
    case typeCashier:
        qDebug()<<"Cashier";
        leUserId->setMaxLength(CASH_CASHIERNOLEN);
        leUserRePass->setMaxLength(CASH_CASHIERPASSLEN);
        leUserNewPass->setMaxLength(CASH_CASHIERPASSLEN);
        break;
    default:
        break;
    }
}

void UIUserManager::quitFromInputPass()
{
    qDebug()<<Q_FUNC_INFO;

    uiInPass->close();
    this->close();
}


void UIUserManager::slotAllowEdit(UserType ut, QString ID)
{
    qDebug()<<Q_FUNC_INFO;

    if(ut==typeManager)
    {
        uiInPass->close();
    }
    else if(ut==typeCashier)
    {
        leUserId->setText(ID);
        leUserId->setReadOnly(true);
        leUserNewPass->setFocus();
        uiInPass->close();
    }
    else
    {
        qDebug()<<"不支持超级管理员管理员用户做用户设置";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);
        uiInPass->resetLine();

        return;
    }
}

void UIUserManager::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIUserManager::slotQuitMenu()
{
    this->close();
}

bool UIUserManager::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==this)
    {
        if(event->type()==QEvent::WindowActivate)
        {
            qDebug() << Q_FUNC_INFO<<"Start Timer";
            closeTimer->start(g_changeParam.TIMEOUT_UI);
        }
        else if(event->type()==QEvent::WindowDeactivate)
        {
            qDebug() << Q_FUNC_INFO<<"Stop Timer";
            closeTimer->stop();
        }
    }
    return QDialog::eventFilter(obj,event);
}
