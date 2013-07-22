#include "uiMenuTrans.h"
#include "uiCashAdvance.h"
#include "uiCashDeposit.h"
#include "uiBalanceInquiry.h"
#include "uiP2PTransfer.h"
#include "uiPayment.h"
#include "uiLogon.h"
#include "global.h"

UIMenuTrans::UIMenuTrans(QDialog *parent,Qt::WindowFlags f) :
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

    lbMenu->setText(tr("TRANSACTIONS"));
    lbMenu->setFont(font2);
    lbMenu->setAlignment(Qt::AlignLeft);
    lbMenuIndexText->setText(tr("1/2"));
    lbMenuIndexText->setFont(font2);
    lbMenuIndexText->setAlignment(Qt::AlignRight);
//    styleWidget(lbMenu);

    btnCashAd=new QPushButton;
    btnCashDp=new QPushButton;
    btnP2P=new QPushButton;
    btnVOID=new QPushButton;
    btnAdjust=new QPushButton;
    btnBalance=new QPushButton;
    btnPayment=new QPushButton;
    btnPIN=new QPushButton;
    btnLogOn=new QPushButton;

    btnPageOneCancel=new QPushButton;
    btnPageTwoCancel=new QPushButton;
    btnPageOneNext=new QPushButton;
    btnPageTwoBack=new QPushButton;

    styleWidget(btnCashAd);
    styleWidget(btnCashDp);
    styleWidget(btnP2P);
    styleWidget(btnVOID);
    styleWidget(btnAdjust);
    styleWidget(btnBalance);
    styleWidget(btnPayment);
    styleWidget(btnPIN);
    styleWidget(btnLogOn);

    btnCashAd->setText(tr("Cash Advance"));
    btnCashDp->setText(tr("Cash Deposit"));
    btnP2P->setText(tr("P2P Transfer"));
    btnVOID->setText(tr("VOID"));
    btnAdjust->setText(tr("Adjust"));
    btnBalance->setText(tr("Balance Inquiry"));
    btnPayment->setText(tr("Payment"));
    btnPIN->setText(tr("PIN Change"));
    btnLogOn->setText(tr("Log On"));

    btnCashAd->setFont(font);
    btnCashDp->setFont(font);
    btnP2P->setFont(font);
    btnVOID->setFont(font);
    btnAdjust->setFont(font);
    btnBalance->setFont(font);
    btnPayment->setFont(font);
    btnPIN->setFont(font);
    btnLogOn->setFont(font);

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

    btnCashAd->setStyleSheet(BTN_MENU_STYLE);
    btnCashDp->setStyleSheet(BTN_MENU_STYLE);
    btnP2P->setStyleSheet(BTN_MENU_STYLE);
    btnVOID->setStyleSheet(BTN_MENU_STYLE);
    btnAdjust->setStyleSheet(BTN_MENU_STYLE);
    btnBalance->setStyleSheet(BTN_MENU_STYLE);
    btnPayment->setStyleSheet(BTN_MENU_STYLE);
    btnPIN->setStyleSheet(BTN_MENU_STYLE);
    btnLogOn->setStyleSheet(BTN_MENU_STYLE);

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
    pageOneVLayout->addWidget(btnCashAd);
    pageOneVLayout->addWidget(btnCashDp);
    pageOneVLayout->addWidget(btnP2P);
    pageOneVLayout->addWidget(btnVOID);
    pageOneVLayout->addWidget(btnAdjust);
    pageOneVLayout->addItem(sp1);
    pageOneHLayout->addWidget(btnPageOneCancel);
    pageOneHLayout->addWidget(btnPageOneNext);
    pageOneVLayout->addLayout(pageOneHLayout);
//    pageOne->setLayout(pageOneVLayout);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    pageTwoVLayout->addWidget(btnBalance);
    pageTwoVLayout->addWidget(btnPayment);
    pageTwoVLayout->addWidget(btnPIN);
    pageTwoVLayout->addWidget(btnLogOn);
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


    // ------------------------TRANS--------------------- //
    connect(btnCashAd,SIGNAL(clicked()),this,SLOT(Cash_Advance_Click()));
    connect(btnCashDp,SIGNAL(clicked()),this,SLOT(Cash_Deposit_Click()));
    connect(btnBalance,SIGNAL(clicked()),this,SLOT(Balance_Inquiry_Click()));
    connect(btnP2P,SIGNAL(clicked()),this,SLOT(P2P_Card_Trans_Click()));
    connect(btnPayment,SIGNAL(clicked()),this,SLOT(Payment_Click()));
    connect(btnLogOn,SIGNAL(clicked()),this,SLOT(showLogon()));
}

UIMenuTrans::~UIMenuTrans()
{
    qDebug() << Q_FUNC_INFO;
}

void UIMenuTrans::keyPressEvent(QKeyEvent *event)
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

void UIMenuTrans::pageOneNextClicked()
{
    if(stackWidget->currentIndex()==0)
    {
        stackWidget->setCurrentIndex(1);
        lbMenuIndexText->setText("2/2");
    }
}


void UIMenuTrans::pageTwoBackClicked()
{
    if(stackWidget->currentIndex()==1)
    {
        stackWidget->setCurrentIndex(0);
        lbMenuIndexText->setText("1/2");
    }
}

void UIMenuTrans::styleWidget(QWidget *btn, int iFontSize)
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

void UIMenuTrans::Cash_Advance_Click()
{
    qDebug() << Q_FUNC_INFO;

    UICashAdvance *uiCashAd = new UICashAdvance();
    uiCashAd->exec();
}

void UIMenuTrans::Cash_Deposit_Click()
{
    qDebug() << Q_FUNC_INFO;

    UICashDeposit *uiCashDp = new UICashDeposit();
    uiCashDp->exec();
}

void UIMenuTrans::Balance_Inquiry_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIBalanceInquiry *uiBI = new UIBalanceInquiry();
    uiBI->exec();
}

void UIMenuTrans::P2P_Card_Trans_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIP2PTransfer *uiPCT = new UIP2PTransfer();
    uiPCT->exec();
}

void UIMenuTrans::Payment_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIPayment *uiP = new UIPayment();
    uiP->exec();
}

void UIMenuTrans::showLogon()
{
    UILogon *uiLog=new UILogon();
    uiLog->exec();

}
