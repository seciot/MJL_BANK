#include "uiMenuConfig.h"
#include "global.h"



UIMenuConfig::UIMenuConfig(QDialog *parent,Qt::WindowFlags f) :
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
    QFont font2("Helvetica",16,QFont::Bold);

    lbMenu=new QLabel;
    lbMenuIndexText=new QLabel;

    lbMenu->setText(tr("Configuration"));
    lbMenu->setFont(font2);
    lbMenu->setAlignment(Qt::AlignLeft);
    lbMenuIndexText->setText(tr("1/2"));
    lbMenuIndexText->setFont(font2);
    lbMenuIndexText->setAlignment(Qt::AlignRight);
    //    styleWidget(lbMenu);


    btnTerminal=new QPushButton;
    btnAcquirer=new QPushButton;
    btnCardTable=new QPushButton;
    btnIssuer=new QPushButton;
    btnPayment=new QPushButton;
    btnSetDateTime=new QPushButton;
    btnClearReversal=new QPushButton;
    btnSuvCharge=new QPushButton;
    btnTransAttr=new QPushButton;

    btnPageOneCancel=new QPushButton;
    btnPageTwoCancel=new QPushButton;
    btnPageOneNext=new QPushButton;
    btnPageTwoBack=new QPushButton;

    styleWidget(btnTerminal);
    styleWidget(btnAcquirer);
    styleWidget(btnCardTable);
    styleWidget(btnIssuer);
    styleWidget(btnPayment);
    styleWidget(btnSetDateTime);
    styleWidget(btnClearReversal);
    styleWidget(btnSuvCharge);
    styleWidget(btnTransAttr);

    btnTerminal->setText(tr("Terminal"));
    btnAcquirer->setText(tr("Acquirer"));
    btnCardTable->setText(tr("Card Table"));
    btnIssuer->setText(tr("Issuer"));
    btnPayment->setText(tr("Payment"));
    btnSetDateTime->setText(tr("Set Date Time"));
    btnClearReversal->setText(tr("Clear Reversal"));
    btnSuvCharge->setText(tr("Sur-Charge"));
    btnTransAttr->setText(tr("Transaction Attribute"));

    btnTerminal->setFont(font);
    btnAcquirer->setFont(font);
    btnCardTable->setFont(font);
    btnIssuer->setFont(font);
    btnPayment->setFont(font);
    btnSetDateTime->setFont(font);
    btnClearReversal->setFont(font);
    btnSuvCharge->setFont(font);
    btnTransAttr->setFont(font);

    styleWidget(btnPageOneCancel);
    styleWidget(btnPageOneNext);
    styleWidget(btnPageTwoBack);
    styleWidget(btnPageTwoCancel);

    btnPageOneCancel->setText(tr("Cancel"));
    btnPageTwoCancel->setText(tr("Cancel"));
    btnPageOneNext->setText(tr("Next"));
    btnPageTwoBack->setText(tr("Back"));
    btnPageOneCancel->setFont(font);
    btnPageTwoCancel->setFont(font);
    btnPageOneNext->setFont(font);
    btnPageTwoBack->setFont(font);

    btnTerminal->setStyleSheet(BTN_MENU_STYLE);
    btnAcquirer->setStyleSheet(BTN_MENU_STYLE);
    btnCardTable->setStyleSheet(BTN_MENU_STYLE);
    btnIssuer->setStyleSheet(BTN_MENU_STYLE);
    btnPayment->setStyleSheet(BTN_MENU_STYLE);
    btnSetDateTime->setStyleSheet(BTN_MENU_STYLE);
    btnClearReversal->setStyleSheet(BTN_MENU_STYLE);
    btnSuvCharge->setStyleSheet(BTN_MENU_STYLE);
    btnTransAttr->setStyleSheet(BTN_MENU_STYLE);

    btnPageOneCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageTwoCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageOneNext->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageTwoBack->setStyleSheet(BTN_MENU_CANCEL_STYLE);

    btnPageOneCancel->setFixedHeight(30);
    btnPageTwoCancel->setFixedHeight(30);
    btnPageOneNext->setFixedHeight(30);
    btnPageTwoBack->setFixedHeight(30);

    stackWidget=new QStackedWidget();
    pageOne=new QWidget();
    pageTwo=new QWidget();
    //    pageThree=new QWidget();

    QVBoxLayout *pageOneVLayout=new QVBoxLayout(pageOne);
    QVBoxLayout *pageTwoVLayout=new QVBoxLayout(pageTwo);
    QHBoxLayout *pageOneHLayout=new QHBoxLayout;
    QHBoxLayout *pageTwoHLayout=new QHBoxLayout;

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    pageOneVLayout->addWidget(btnTerminal);
    pageOneVLayout->addWidget(btnAcquirer);
    pageOneVLayout->addWidget(btnCardTable);
    pageOneVLayout->addWidget(btnIssuer);
    pageOneVLayout->addWidget(btnPayment);
    pageOneVLayout->addItem(sp1);

    pageOneHLayout->addWidget(btnPageOneCancel);
    pageOneHLayout->addWidget(btnPageOneNext);
    pageOneVLayout->addLayout(pageOneHLayout);
    //    pageOne->setLayout(pageOneVLayout);

    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    pageTwoVLayout->addWidget(btnSetDateTime);
    pageTwoVLayout->addWidget(btnClearReversal);
    pageTwoVLayout->addWidget(btnSuvCharge);
    pageTwoVLayout->addWidget(btnTransAttr);
    pageTwoVLayout->addItem(sp2);

    pageTwoHLayout->addWidget(btnPageTwoCancel);
    pageTwoHLayout->addWidget(btnPageTwoBack);
    pageTwoVLayout->addLayout(pageTwoHLayout);
    //    pageTwo->setLayout(pageTwoVLayout);

    stackWidget->addWidget(pageOne);
    stackWidget->addWidget(pageTwo);

    QHBoxLayout *hlayout=new QHBoxLayout;
    hlayout->addWidget(lbMenu);
    hlayout->addWidget(lbMenuIndexText);
    QVBoxLayout *vLayout=new QVBoxLayout(this);
    vLayout->addLayout(hlayout);
    vLayout->addWidget(stackWidget);


    connect(btnPageOneNext,SIGNAL(clicked()),this,SLOT(pageOneNextClicked()));
    connect(btnPageTwoBack,SIGNAL(clicked()),this,SLOT(pageTwoBackClicked()));
    connect(btnPageOneCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnPageTwoCancel, SIGNAL(clicked()), this, SLOT(close()));

    // ----------------------- CONFIG MENU-----------------------//
    connect(btnTerminal,SIGNAL(clicked()),this,SLOT(Terminal_Config_Click()));
    connect(btnAcquirer,SIGNAL(clicked()),this,SLOT(Acquirer_Config_Click()));
    connect(btnCardTable,SIGNAL(clicked()),this,SLOT(CardTable_Config_Click()));
    connect(btnIssuer,SIGNAL(clicked()),this,SLOT(Issuer_Config_Click()));
    connect(btnPayment,SIGNAL(clicked()),this,SLOT(Payment_Config_Click()));
    connect(btnSetDateTime,SIGNAL(clicked()),this,SLOT(Date_Config_Click()));
    connect(btnSuvCharge,SIGNAL(clicked()),this,SLOT(Sur_Config_Click()));
    connect(btnTransAttr,SIGNAL(clicked()),this,SLOT(Trans_Config_Click()));

    // ----------------- 验证用户身份----------------- //
    // -------- input cashier password ------------//
    uiIP=new UIInputPassword();
    connect(uiIP,SIGNAL(sigLogInSuccess(UserType,QString)),this,SLOT(slotAllowEdit(UserType,QString)));
    connect(uiIP,SIGNAL(sigFinishTrans()),this,SLOT(quitFromInputPass()));
    passThread=new QThread(this);
    connect(passThread, SIGNAL(started()), uiIP, SLOT(exec()));
    passThread->start();

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

UIMenuConfig::~UIMenuConfig()
{
    qDebug() << Q_FUNC_INFO;
}

void UIMenuConfig::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F1:
        this->pageTwoBackClicked();
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    case Qt::Key_F2:
        this->pageOneNextClicked();
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIMenuConfig::pageOneNextClicked()
{
    if(stackWidget->currentIndex()==0)
    {
        stackWidget->setCurrentIndex(1);
        lbMenuIndexText->setText("2/2");
    }
}


void UIMenuConfig::pageTwoBackClicked()
{
    if(stackWidget->currentIndex()==1)
    {
        stackWidget->setCurrentIndex(0);
        lbMenuIndexText->setText("1/2");
    }
}

void UIMenuConfig::styleWidget(QWidget *btn, int iFontSize)
{
    QFont font13;
    if(iFontSize>0)
    {
        font13.setPointSize(iFontSize);
        btn->setFont(font13);
    }
    btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btn->setFixedHeight(35);
}

void UIMenuConfig::Terminal_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigTer *uiCT = new UIConfigTer();
    uiCT->exec();
}

void UIMenuConfig::Acquirer_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigAcq *uiCA = new UIConfigAcq();
    uiCA->exec();
}

void UIMenuConfig::CardTable_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigCardTable *uiCCT = new UIConfigCardTable();
    uiCCT->exec();
}

void UIMenuConfig::Issuer_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigIssuer *uiCI= new UIConfigIssuer();
    uiCI->exec();
}

void UIMenuConfig::Payment_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigPay *uiCP= new UIConfigPay();
    uiCP->exec();
}

void UIMenuConfig::Date_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigDateTime *uiCDT= new UIConfigDateTime();
    uiCDT->exec();
}

void UIMenuConfig::Sur_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigSurCharge *uiCSC= new UIConfigSurCharge();
    uiCSC->exec();
}

void UIMenuConfig::Trans_Config_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIConfigTransAttr *uiCTA= new UIConfigTransAttr();
    uiCTA->exec();
}


void UIMenuConfig::quitFromInputPass()
{
    qDebug()<<Q_FUNC_INFO;

    uiIP->close();
    this->close();
}

void UIMenuConfig::slotAllowEdit(UserType ut,QString ID)
{
    qDebug()<<Q_FUNC_INFO;
    qDebug()<<ut<<ID;


    if(ut==typeAdmin)
    {
        uiIP->close();
    }
    else
    {
        qDebug()<<"不支持系统管理员以外的用户做系统设置";

        UIMsg::showNoticeMsgWithAutoClose(NO_PERMISSION,g_changeParam.TIMEOUT_ERRMSG);
        return;
    }
}

void UIMenuConfig::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIMenuConfig::slotQuitMenu()
{
    this->close();
}

bool UIMenuConfig::eventFilter(QObject *obj, QEvent *event)
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
