#include "uiConfigTer.h"
#include "xdata.h"
#include "global.h"

UIConfigTer::UIConfigTer(QDialog *parent,Qt::WindowFlags f) :
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
    QFont font2("Helvetica",14,QFont::Bold);

    lbHead=new QLabel();
    lbHead->setText(tr("Terminal Configuration"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);

    //--------------------   define ----------------------//
    xDATA::ReadValidFile(xDATA::DataSaveChange);

    chkTrans=new QCheckBox();
    chkCard=new QCheckBox();
    chkTrans->setText("模拟通讯");
    chkCard->setText("模拟卡号");
    chkTrans->setFont(fontH);
    chkCard->setFont(fontH);
    if(g_changeParam.simTrans==true)
        chkTrans->setChecked(true);
    if(g_changeParam.simCard==true)
        chkCard->setChecked(true);

    lbSTAN=new QLabel();
    lbSTAN->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    leSTAN=new QLineEdit();
    lbInvoice=new QLabel();
    leInvoice=new QLineEdit();
    lbPABXAcCode=new QLabel();
    lePABXAcCode=new QLineEdit();
    lbPABXDelay=new QLabel();
    lePABXDelay=new QLineEdit();
    lbDialType=new QLabel();
    cbDialType=new QComboBox();
    lbTechPass=new QLabel();
    leTechPass=new QLineEdit();
    lbHostTimeOut=new QLabel();
    leHostTimeOut=new QLineEdit();
    lbUserTimeOut=new QLabel();
    leUserTimeOut=new QLineEdit();
    lbInvoiceLogo=new QLabel();
    cbInvoiceLogo=new QComboBox();
    lbScreenLogo=new QLabel();
    cbScreenLogo=new QComboBox();

    lbSTAN->setText(tr("STAN:"));
    lbInvoice->setText(tr("Invoice:"));
    lbPABXAcCode->setText(tr("PABX Access Code:"));
    lbPABXDelay->setText(tr("PABX Delay:"));
    lbDialType->setText(tr("Dial Type:"));
    lbTechPass->setText(tr("Tech Password:"));
    lbHostTimeOut->setText(tr("Host Time Out:"));
    lbUserTimeOut->setText(tr("User Time Out:"));
    lbInvoiceLogo->setText(tr("Invoice Logo:"));
    lbScreenLogo->setText(tr("Screen Logo:"));

    lbSTAN->setScaledContents(true);
    lbInvoice->setScaledContents(true);
    lbPABXAcCode->setScaledContents(true);
    lbPABXDelay->setScaledContents(true);
    lbDialType->setScaledContents(true);
    lbTechPass->setScaledContents(true);
    lbHostTimeOut->setScaledContents(true);
    lbUserTimeOut->setScaledContents(true);
    lbInvoiceLogo->setScaledContents(true);
    lbScreenLogo->setScaledContents(true);

    chkNoBliendDial=new QCheckBox();
    chkSetReceipt=new QCheckBox();
    chkSelAccEnable=new QCheckBox();
    lbRLine1=new QLabel();
    lbRLine2=new QLabel();
    lbRLine3=new QLabel();
    lbRLine4=new QLabel();
    lbALine1=new QLabel();
    lbALine2=new QLabel();
    lbALine3=new QLabel();
    lbALine4=new QLabel();
    leRLine1=new QLineEdit();
    leRLine2=new QLineEdit();
    leRLine3=new QLineEdit();
    leRLine4=new QLineEdit();
    leALine1=new QLineEdit();
    leALine2=new QLineEdit();
    leALine3=new QLineEdit();
    leALine4=new QLineEdit();

    lbCompLb=new QLabel();
    leCompLb=new QLineEdit();

    chkNoBliendDial->setText(tr("No Bliend Dial"));
    chkSetReceipt->setText(tr("Settle Receipt"));
    chkSelAccEnable->setText(tr("Select Account Enable"));
    lbRLine1->setText(tr("Receipt Line 1:"));
    lbRLine2->setText(tr("Receipt Line 2:"));
    lbRLine3->setText(tr("Receipt Line 3:"));
    lbRLine4->setText(tr("Receipt Line 4:"));
    lbALine1->setText(tr("Agree Line 1:"));
    lbALine2->setText(tr("Agree Line 2:"));
    lbALine3->setText(tr("Agree Line 3:"));
    lbALine4->setText(tr("Agree Line 4:"));
    lbCompLb->setText(tr("Edit Company Copy Label:"));

    chkCard->setStyleSheet(CHECK_BOX_STYLE);
    chkTrans->setStyleSheet(CHECK_BOX_STYLE);
    chkNoBliendDial->setStyleSheet(CHECK_BOX_STYLE);
    chkSelAccEnable->setStyleSheet(CHECK_BOX_STYLE);
    chkSetReceipt->setStyleSheet(CHECK_BOX_STYLE);
    chkCard->setMinimumHeight(30);
    chkTrans->setMinimumHeight(30);
    chkNoBliendDial->setMinimumHeight(30);
    chkSelAccEnable->setMinimumHeight(30);
    chkSetReceipt->setMinimumHeight(30);

    cbDialType->setStyleSheet(COMBO_BOX_STYLE);
    cbInvoiceLogo->setStyleSheet(COMBO_BOX_STYLE);
    cbScreenLogo->setStyleSheet(COMBO_BOX_STYLE);
    cbDialType->setMinimumHeight(26);
    cbInvoiceLogo->setMinimumHeight(26);
    cbScreenLogo->setMinimumHeight(26);
    cbDialType->addItem("GPRS");
    cbDialType->addItem("WIFI");
    cbDialType->addItem("BLUETOOTH");
    cbInvoiceLogo->addItem("LOGO 1");
    cbInvoiceLogo->addItem("LOGO 2");
    cbInvoiceLogo->addItem("LOGO 3");
    cbInvoiceLogo->addItem("LOGO 4");
    cbScreenLogo->addItem("LOGO 1");
    cbScreenLogo->addItem("LOGO 2");
    cbScreenLogo->addItem("LOGO 3");
    cbScreenLogo->addItem("LOGO 4");


    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(chkTrans);
    v1Lay->addWidget(chkCard);
    v1Lay->addWidget(lbSTAN);
    v1Lay->addWidget(leSTAN);
    v1Lay->addWidget(lbInvoice);
    v1Lay->addWidget(leInvoice);

    v1Lay->addWidget(lbPABXAcCode);
    v1Lay->addWidget(lePABXAcCode);
    v1Lay->addWidget(lbPABXDelay);
    v1Lay->addWidget(lePABXDelay);

    v1Lay->addWidget(lbDialType);
    v1Lay->addWidget(cbDialType);
    v1Lay->addWidget(lbTechPass);
    v1Lay->addWidget(leTechPass);

    v1Lay->addWidget(lbHostTimeOut);
    v1Lay->addWidget(leHostTimeOut);
    v1Lay->addWidget(lbUserTimeOut);
    v1Lay->addWidget(leUserTimeOut);

    v1Lay->addWidget(lbInvoiceLogo);
    v1Lay->addWidget(cbInvoiceLogo);
    v1Lay->addWidget(lbScreenLogo);
    v1Lay->addWidget(cbScreenLogo);

    v1Lay->addWidget(chkNoBliendDial);
    v1Lay->addWidget(chkSetReceipt);
    v1Lay->addWidget(chkSelAccEnable);

    v1Lay->addWidget(lbRLine1);
    v1Lay->addWidget(leRLine1);
    v1Lay->addWidget(lbRLine2);
    v1Lay->addWidget(leRLine2);
    v1Lay->addWidget(lbRLine3);
    v1Lay->addWidget(leRLine3);
    v1Lay->addWidget(lbRLine4);
    v1Lay->addWidget(leRLine4);
    v1Lay->addWidget(lbALine1);
    v1Lay->addWidget(leALine1);
    v1Lay->addWidget(lbALine2);
    v1Lay->addWidget(leALine2);
    v1Lay->addWidget(lbALine3);
    v1Lay->addWidget(leALine3);
    v1Lay->addWidget(lbALine4);
    v1Lay->addWidget(leALine4);

    v1Lay->addWidget(lbCompLb);
    v1Lay->addWidget(leCompLb);

    btnCancel=new QPushButton();
    btnSubmit=new QPushButton();
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Submit"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet("color: rgb(255, 255, 255);	background-color: rgb(0, 153, 255);border-radius: 6px;");
    btnSubmit->setStyleSheet("color: rgb(0, 0, 0);	background-color: rgb(0, 255, 0);border-radius: 6px;");


    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(10);
    h1Lay->addWidget(btnCancel);
    h1Lay->addWidget(btnSubmit);
    h1Lay->addSpacing(10);


    // ---------------ScrollArea----------- //
    scArea=new QScrollArea();
    scWidget=new QWidget();
    hBar=new QScrollBar();
    vBar=new QScrollBar();

    QVBoxLayout *scLayout=new QVBoxLayout(scWidget);
    scLayout->addLayout(v1Lay);


    scWidget->setMaximumWidth(FRAME420_WIDTH-5);
    scWidget->setFont(font);
    scWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    vBar->setStyleSheet("QScrollBar:vertical {"
                        "border:0px solid grey;"
                        "width: 3px;"
                        "}"
                        " QScrollBar::handle:vertical {"
                        " background: #8080FF;"
                        " border: 2px solid grey;"
                        " border-radius:5px;"
                        " min-height: 10px;"
                        " }"
                        " QScrollBar::add-line:vertical {"
                        " height: 0px;"
                        " subcontrol-position: bottom;"
                        " }"
                        " QScrollBar::sub-line:vertical {"
                        " height: 0px;"
                        " subcontrol-position: top;"
                        " }"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                        " background: none;"
                        "}"
                        "QScrollArea"
                        "{"
                           "border:0;"
                            "background:rgb(64,64,71);"
                        "}");

    scArea->setVerticalScrollBar(vBar);
    scArea->setHorizontalScrollBar(hBar);
    scArea->setFrameShape(QFrame::NoFrame);
    scArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scArea->setWidget(scWidget);

//    scArea->ensureVisible(240,320,240,320);

    // ------------------------------------------- //

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addWidget(scArea);
    layout->addLayout(h1Lay);
    layout->setContentsMargins(0,0,0,5);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotSubmitClicked()));



}

UIConfigTer::~UIConfigTer()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigTer::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F3:
        vBar->setValue(vBar->value()-150);
        break;
    case Qt::Key_F4:
        vBar->setValue(vBar->value()+150);
        break;
    default:
        event->ignore();
        break;
    }
}

void UIConfigTer::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();

}

void UIConfigTer::mouseMoveEvent(QMouseEvent *event)
{

//    qDebug()<<"mouseMoveEvent"<<event->pos().x()<<event->pos().y()<<vBar->value();
//    qDebug()<<saveVValue;
    if(saveVValue-event->pos().y() > 3)
    {
        vBar->setValue(vBar->value()+13);
        saveVValue=event->pos().y();
    }
    if(saveVValue-event->pos().y() < -3)
    {
        vBar->setValue(vBar->value()-13);
        saveVValue=event->pos().y();
    }
}

void UIConfigTer::slotSubmitClicked()
{
    if(chkCard->isChecked())
        g_changeParam.simCard=true;
    else
        g_changeParam.simCard=false;

    if(chkTrans->isChecked())
        g_changeParam.simTrans=true;
    else
        g_changeParam.simTrans=false;


    xDATA::WriteValidFile(xDATA::DataSaveChange);

    UIMsg::showNoticeMsgWithAutoClose("SAVE SUCCESS",g_changeParam.TIMEOUT_ERRMSG);
}
