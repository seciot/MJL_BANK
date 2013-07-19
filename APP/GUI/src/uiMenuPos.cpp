#include "uiMenuPos.h"
#include "uiMenuTrans.h"
#include "uiMenuConfig.h"
#include "uiLogon.h"
#include "uiUserManager.h"
#include "uiReport.h"
#include "uiSettle.h"

#include "global.h"

UIMenuPos::UIMenuPos(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
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

    lbMenu->setText(tr("MENU"));
    lbMenu->setFont(font2);
    lbMenu->setAlignment(Qt::AlignLeft);

    lbMenuIndexText->setText(tr("1/2"));
    lbMenuIndexText->setFont(font2);
    lbMenuIndexText->setAlignment(Qt::AlignRight);
    //    styleWidget(lbMenu);

    btnTrans=new QPushButton;
    btnConfig=new QPushButton;
    btnBatch=new QPushButton;
    btnReceipt=new QPushButton;
    btnVOID=new QPushButton;
    btnSettle=new QPushButton;
    btnCashierManager=new QPushButton;
    btnReport=new QPushButton;
    btnParDown=new QPushButton;
    btnLogon=new QPushButton;

    btnPageOneCancel=new QPushButton;
    btnPageTwoCancel=new QPushButton;
    btnPageOneNext=new QPushButton;
    btnPageTwoBack=new QPushButton;

    styleWidget(btnTrans);
    styleWidget(btnConfig);
    styleWidget(btnBatch);
    styleWidget(btnReceipt);
    styleWidget(btnVOID);
    styleWidget(btnSettle);
    styleWidget(btnCashierManager);
    styleWidget(btnReport);
    styleWidget(btnParDown);
    styleWidget(btnLogon);

    styleWidget(btnPageOneCancel);
    styleWidget(btnPageOneNext);
    styleWidget(btnPageTwoBack);
    styleWidget(btnPageTwoCancel);


    btnTrans->setText(tr("Transactions"));
    btnConfig->setText(tr("Configuration"));
    btnBatch->setText(tr("Batch"));
    btnReceipt->setText(tr("Receipt"));
    btnVOID->setText(tr("VOID"));
    btnSettle->setText(tr("Settlement"));
    btnCashierManager->setText(tr("Cashier Manager"));
    btnReport->setText(tr("Report"));
    btnParDown->setText(tr("Param Download"));
    btnLogon->setText(tr("Logon"));

    btnTrans->setFont(font);
    btnConfig->setFont(font);
    btnBatch->setFont(font);
    btnReceipt->setFont(font);
    btnVOID->setFont(font);
    btnSettle->setFont(font);
    btnCashierManager->setFont(font);
    btnReport->setFont(font);
    btnParDown->setFont(font);
    btnLogon->setFont(font);

    //    btnTrans->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnConfig->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnBatch->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnReceipt->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnVOID->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnSettle->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnCashierManager->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnReport->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnParDown->setIcon(QIcon(":/icons/QCash/menu2.png"));
    //    btnLogon->setIcon(QIcon(":/icons/QCash/menu2.png"));

    btnPageOneCancel->setText(tr("Cancel"));
    btnPageTwoCancel->setText(tr("Cancel"));
    btnPageOneNext->setText(tr("Next"));
    btnPageTwoBack->setText(tr("Back"));
    btnPageOneCancel->setFont(font);
    btnPageTwoCancel->setFont(font);
    btnPageOneNext->setFont(font);
    btnPageTwoBack->setFont(font);

    btnTrans->setStyleSheet(BTN_MENU_STYLE);
    btnConfig->setStyleSheet(BTN_MENU_STYLE);
    btnBatch->setStyleSheet(BTN_MENU_STYLE);
    btnReceipt->setStyleSheet(BTN_MENU_STYLE);
    btnVOID->setStyleSheet(BTN_MENU_STYLE);
    btnSettle->setStyleSheet(BTN_MENU_STYLE);
    btnCashierManager->setStyleSheet(BTN_MENU_STYLE);
    btnReport->setStyleSheet(BTN_MENU_STYLE);
    btnParDown->setStyleSheet(BTN_MENU_STYLE);
    btnLogon->setStyleSheet(BTN_MENU_STYLE);


    btnPageOneCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageTwoCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageOneNext->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnPageTwoBack->setStyleSheet(BTN_MENU_CANCEL_STYLE);

    btnPageOneCancel->setMaximumHeight(30);
    btnPageTwoCancel->setMaximumHeight(30);
    btnPageOneNext->setMaximumHeight(30);
    btnPageTwoBack->setMaximumHeight(30);

    stackWidget=new QStackedWidget();
    pageOne=new QWidget();
    pageTwo=new QWidget();
    //    pageThree=new QWidget();

    QVBoxLayout *pageOneVLayout=new QVBoxLayout(pageOne);
    QVBoxLayout *pageTwoVLayout=new QVBoxLayout(pageTwo);
    QHBoxLayout *pageOneHLayout=new QHBoxLayout;
    QHBoxLayout *pageTwoHLayout=new QHBoxLayout;

    pageOneVLayout->addWidget(btnTrans);
    pageOneVLayout->addWidget(btnConfig);
    pageOneVLayout->addWidget(btnBatch);
    pageOneVLayout->addWidget(btnReceipt);
    pageOneVLayout->addWidget(btnVOID);
    pageOneHLayout->addWidget(btnPageOneCancel);
    pageOneHLayout->addWidget(btnPageOneNext);
    pageOneVLayout->addLayout(pageOneHLayout);
    //    pageOne->setLayout(pageOneVLayout);

    pageTwoVLayout->addWidget(btnSettle);
    pageTwoVLayout->addWidget(btnCashierManager);
    pageTwoVLayout->addWidget(btnReport);
    pageTwoVLayout->addWidget(btnParDown);
    pageTwoVLayout->addWidget(btnLogon);
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

    connect(btnTrans,SIGNAL(clicked()),this,SLOT(showTransMenu()));
    connect(btnConfig,SIGNAL(clicked()),this,SLOT(showConfigMenu()));
    connect(btnCashierManager,SIGNAL(clicked()),this,SLOT(showCashierManager()));
    connect(btnLogon,SIGNAL(clicked()),this,SLOT(showLogon()));
    connect(btnReport,SIGNAL(clicked()),this,SLOT(showReport()));
    connect(btnSettle,SIGNAL(clicked()),this,SLOT(showSettle()));
}

UIMenuPos::~UIMenuPos()
{
    qDebug() << Q_FUNC_INFO;
}

void UIMenuPos::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F1:
        this->pageTwoBackClicked();
        break;
    case Qt::Key_F2:
        this->pageOneNextClicked();
        break;
    default:
        event->ignore();
        break;
    }
}

void UIMenuPos::pageOneNextClicked()
{
    if(stackWidget->currentIndex()==0)
    {
        stackWidget->setCurrentIndex(1);
        lbMenuIndexText->setText("2/2");
    }
}

void UIMenuPos::pageTwoBackClicked()
{
    if(stackWidget->currentIndex()==1)
    {
        stackWidget->setCurrentIndex(0);
        lbMenuIndexText->setText("1/2");
    }
}

void UIMenuPos::styleWidget(QWidget *btn, int iFontSize)
{
    QFont font13;
    if(iFontSize>0)
    {
        font13.setPointSize(iFontSize);
        btn->setFont(font13);
    }
    btn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    btn->setMinimumSize(1,1);
}

void UIMenuPos::showTransMenu()
{
    UIMenuTrans *uiTrans=new UIMenuTrans();
    uiTrans->exec();
}

void UIMenuPos::showConfigMenu()
{
    UIMenuConfig *uiCon=new UIMenuConfig();
    uiCon->exec();

}

void UIMenuPos::showLogon()
{
    UILogon *uiLog=new UILogon();
    uiLog->exec();

}

void UIMenuPos::showCashierManager()
{
    UIUserManager *uiUM=new UIUserManager();
    uiUM->exec();

}

void UIMenuPos::showReport()
{
    UIReport *uiR=new UIReport();
    uiR->exec();

}

void UIMenuPos::showSettle()
{
    qDebug() << Q_FUNC_INFO;
    UISettle *uiStl = new UISettle();
    uiStl->exec();
}
