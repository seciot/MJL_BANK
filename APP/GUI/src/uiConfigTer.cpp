#include "uiConfigTer.h"
#include "xdata.h"
#include "global.h"

UIConfigTer::UIConfigTer(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    xDATA::ClearGlobalData();

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
    chkTrans = new QCheckBox();
    chkCard = new QCheckBox();

    chkTrans->setText("Sim COMM");
    chkCard->setText("Sim Test Card");

    chkTrans->setFont(fontH);
    chkCard->setFont(fontH);

    if(g_changeParam.simTrans==true)
        chkTrans->setChecked(true);
    if(g_changeParam.simCard==true)
        chkCard->setChecked(true);

    lbSTAN=new QLabel();
    lbSTAN->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    leSTAN=new QLineEdit();
    leSTAN->setMaxLength(6);
    lbInvoice=new QLabel();
    leInvoice=new QLineEdit();
    leInvoice->setMaxLength(6);
    lePABXDelay=new QLineEdit();
    lbDialType=new QLabel();
    cbDialType=new QComboBox();
    lbTechPass=new QLabel();
    leTechPass=new QLineEdit();
    leTechPass->setMaxLength(8);
    leTechPass->setEchoMode(QLineEdit::Password);
    lbHostTimeOut=new QLabel();
    leHostTimeOut=new QLineEdit();
    lbUserTimeOut=new QLabel();
    leUserTimeOut=new QLineEdit();
    lbErrMsgTimeOut=new QLabel();
    leErrMsgTimeOut=new QLineEdit();
    lbPaperTearTimeOut=new QLabel();
    lePaperTearTimeOut=new QLineEdit();
    lbInvoiceLogo=new QLabel();
    cbInvoiceLogo=new QComboBox();
    lbScreenLogo=new QLabel();
    cbScreenLogo=new QComboBox();

    lbSTAN->setText(tr("STAN:"));
    lbInvoice->setText(tr("Invoice:"));
    lbDialType->setText(tr("Dial Type:"));
    lbTechPass->setText(tr("Tech Password:"));
    lbHostTimeOut->setText(tr("Host Time Out(sec):"));
    lbUserTimeOut->setText(tr("User Time Out(sec):"));
    lbErrMsgTimeOut->setText(tr("Message Box Time Out(sec):"));
    lbPaperTearTimeOut->setText(tr("Receipt Tear Time Out(sec):"));
    lbInvoiceLogo->setText(tr("Invoice Logo:"));
    lbScreenLogo->setText(tr("Screen Logo:"));

    lbSTAN->setScaledContents(true);
    lbInvoice->setScaledContents(true);
    lbDialType->setScaledContents(true);
    lbTechPass->setScaledContents(true);
    lbHostTimeOut->setScaledContents(true);
    lbUserTimeOut->setScaledContents(true);
    lbErrMsgTimeOut->setScaledContents(true);
    lbPaperTearTimeOut->setScaledContents(true);
    lbInvoiceLogo->setScaledContents(true);
    lbScreenLogo->setScaledContents(true);

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
    leRLine1->setMaxLength(PARAM_PRINT_BIG_FONT_LEN);
    leRLine2->setMaxLength(PARAM_PRINT_BIG_FONT_LEN);
    leRLine3->setMaxLength(PARAM_PRINT_BIG_FONT_LEN);
    leRLine4->setMaxLength(PARAM_PRINT_BIG_FONT_LEN);
    leALine1->setMaxLength(PARAM_PRINT_SMALL_FONT_LEN);
    leALine2->setMaxLength(PARAM_PRINT_SMALL_FONT_LEN);
    leALine3->setMaxLength(PARAM_PRINT_SMALL_FONT_LEN);
    leALine4->setMaxLength(PARAM_PRINT_SMALL_FONT_LEN);
    leCompLb->setMaxLength(PARAM_PRINT_BIG_FONT_LEN - 10);

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

    chkSelAccEnable->setStyleSheet(CHECK_BOX_STYLE);
    chkSetReceipt->setStyleSheet(CHECK_BOX_STYLE);

    chkCard->setMinimumHeight(30);
    chkTrans->setMinimumHeight(30);

    chkSelAccEnable->setMinimumHeight(30);
    chkSetReceipt->setMinimumHeight(30);

    chkCard->setFont(font2);
    chkTrans->setFont(font2);

    chkSelAccEnable->setFont(font2);
    chkSetReceipt->setFont(font2);

    cbDialType->setStyleSheet(COMBO_BOX_STYLE);
    cbInvoiceLogo->setStyleSheet(COMBO_BOX_STYLE);
    cbScreenLogo->setStyleSheet(COMBO_BOX_STYLE);
    cbDialType->setMinimumHeight(26);
    cbInvoiceLogo->setMinimumHeight(26);
    cbScreenLogo->setMinimumHeight(26);

    QPixmap pixmap(1,COMBO_HEIGHT);
    pixmap.fill(Qt::transparent);
    QIcon blankicon(pixmap);
    cbDialType->setIconSize(QSize(1, COMBO_HEIGHT));
    cbDialType->addItem(blankicon,"GPRS");
    cbDialType->addItem(blankicon,"WIFI");

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

    v1Lay->addWidget(lbDialType);
    v1Lay->addWidget(cbDialType);
    v1Lay->addWidget(lbTechPass);
    v1Lay->addWidget(leTechPass);

    v1Lay->addWidget(lbHostTimeOut);
    v1Lay->addWidget(leHostTimeOut);
    v1Lay->addWidget(lbUserTimeOut);
    v1Lay->addWidget(leUserTimeOut);

    v1Lay->addWidget(lbErrMsgTimeOut);
    v1Lay->addWidget(leErrMsgTimeOut);
    v1Lay->addWidget(lbPaperTearTimeOut);
    v1Lay->addWidget(lePaperTearTimeOut);

    v1Lay->addWidget(lbInvoiceLogo);
    v1Lay->addWidget(cbInvoiceLogo);
    v1Lay->addWidget(lbScreenLogo);
    v1Lay->addWidget(cbScreenLogo);

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
    vBar->setStyleSheet(SCROLL_VERTICAL_SIMPLE_STYLE);

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
    connect(vBar,SIGNAL(valueChanged(int)),this,SLOT(restartTimer()));


    this->initialSettings();

    this->setAutoClose(g_constantParam.TIMEOUT_UI);

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
    case Qt::Key_Enter:
        focusNextChild();
        break;
    case Qt::Key_F3:
        vBar->setValue(vBar->value()-150);
        closeTimer->start(g_constantParam.TIMEOUT_UI);
        break;
    case Qt::Key_F4:
        vBar->setValue(vBar->value()+150);
        closeTimer->start(g_constantParam.TIMEOUT_UI);
        break;
    default:
        closeTimer->start(g_constantParam.TIMEOUT_UI);
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

void UIConfigTer::initialSettings()
{
    qDebug()<<Q_FUNC_INFO;
    // 模拟通讯 卡号
    qDebug()<<"simCard:"<<g_changeParam.simCard<<"simTrans:"<<g_changeParam.simTrans;
    if(g_changeParam.simCard==true)
        chkCard->setChecked(true);
    if(g_changeParam.simTrans==true)
        chkTrans->setChecked(true);

    // STAN 流水号
    QString originTraceNo=QString::number(g_changeParam.ulTransNum);
    qDebug()<<"originTraceNo:"<<originTraceNo;
    if(originTraceNo.length()<6)
        originTraceNo=originTraceNo.rightJustified(6,'0');
    leSTAN->setText(originTraceNo);
    //    qDebug()<<"g_changeParam.ulTransNum"<<g_changeParam.ulTransNum;
    //    leSTAN->setText(QString::number(g_changeParam.ulTransNum));

    // invoice 账单号
    QString originInvoiceNo=QString::number(g_changeParam.ulInvoiceNum);
    qDebug()<<"originInvoiceNo:"<<originInvoiceNo;
    if(originInvoiceNo.length()<6)
        originInvoiceNo=originInvoiceNo.rightJustified(6,'0');
    leInvoice->setText(originInvoiceNo);
    //    qDebug()<<"g_changeParam.ulInvoiceNum"<<g_changeParam.ulInvoiceNum;
    //    leInvoice->setText(QString::number(g_changeParam.ulInvoiceNum));

    // timeout
    leHostTimeOut->setText(QString::number(g_constantParam.TIMEOUT_HOST/1000));
    leUserTimeOut->setText(QString::number(g_constantParam.TIMEOUT_UI/1000));
    leErrMsgTimeOut->setText(QString::number(g_constantParam.TIMEOUT_ERRMSG/1000));
    lePaperTearTimeOut->setText(QString::number(g_constantParam.TIMEOUT_PAPERTEAR/1000));

    if(g_constantParam.commMode==PARAM_COMMMODE_GPRS)
        cbDialType->setCurrentIndex(0);
    else if(g_constantParam.commMode==PARAM_COMMMODE_WIFI)
        cbDialType->setCurrentIndex(1);

    // flag
    if(g_constantParam.flagSettleReceipt==true)
        chkSetReceipt->setChecked(true);
    if(g_constantParam.flagSelectAccount==true)
        chkSelAccEnable->setChecked(true);

    // print
    leRLine1->setText(QString::fromAscii((const char *)g_constantParam.aucReceiptLine1));
    leRLine2->setText(QString::fromAscii((const char *)g_constantParam.aucReceiptLine2));
    leRLine3->setText(QString::fromAscii((const char *)g_constantParam.aucReceiptLine3));
    leRLine4->setText(QString::fromAscii((const char *)g_constantParam.aucReceiptLine4));
    leALine1->setText(QString::fromAscii((const char *)g_constantParam.aucAgreeLine1));
    leALine2->setText(QString::fromAscii((const char *)g_constantParam.aucAgreeLine2));
    leALine3->setText(QString::fromAscii((const char *)g_constantParam.aucAgreeLine3));
    leALine4->setText(QString::fromAscii((const char *)g_constantParam.aucAgreeLine4));

    QString companyLabel=QString::fromAscii((const char *)g_constantParam.aucCompanyCopyLabel);
    companyLabel.remove(" --- ");
    leCompLb->setText(companyLabel);

    // Tec Pass
    leTechPass->setText(QString::fromAscii((const char *)g_cashier.aucSuperPSW));

    qDebug()<<Q_FUNC_INFO<<"out";

}

void UIConfigTer::slotSubmitClicked()
{
    qDebug()<<"\n\n---------"<<Q_FUNC_INFO<<"---------";
    // 模拟选项
    closeTimer->stop();

    if(chkCard->isChecked())
        g_changeParam.simCard=true;
    else
        g_changeParam.simCard=false;

    if(chkTrans->isChecked())
        g_changeParam.simTrans=true;
    else
        g_changeParam.simTrans=false;


    // STAN
    ulong stan=leSTAN->text().toULong();
    if(stan>=g_changeParam.ulTransNum)
    {
        g_changeParam.ulTransNum=stan;
        qDebug()<<"STAN::"<<leSTAN->text()<<stan<<g_changeParam.ulTransNum;
    }
    else
        UIMsg::showNoticeMsgWithAutoClose("Trace No.\ncan`t reduce",g_constantParam.TIMEOUT_ERRMSG);
    //    g_changeParam.ulTransNum=leSTAN->text().toULong();
    //    qDebug()<<" g_changeParam.ulTransNum"<< g_changeParam.ulTransNum;

    // invoice
    ulong invoice=leInvoice->text().toULong();
    if(invoice>=g_changeParam.ulInvoiceNum)
    {
        g_changeParam.ulInvoiceNum=invoice;
        qDebug()<<"invoice::"<<leInvoice->text()<<invoice<<g_changeParam.ulInvoiceNum;
    }
    else
        UIMsg::showNoticeMsgWithAutoClose("Invoice No\ncan`t reduce",g_constantParam.TIMEOUT_ERRMSG);
    //    g_changeParam.ulInvoiceNum=leInvoice->text().toULong();
    //    qDebug()<<" g_changeParam.ulInvoiceNum"<< g_changeParam.ulInvoiceNum;

    // dial type
    if(cbDialType->currentText()=="GPRS")
    {
        g_constantParam.commMode = PARAM_COMMMODE_GPRS;
    }
    else if(cbDialType->currentText()=="WIFI")
    {
        g_constantParam.commMode = PARAM_COMMMODE_WIFI;
        g_constantParam.ulHostIP[0] = 0xAC100298;
        g_constantParam.ulHostIP[1] = 0xAC100298;
        g_constantParam.uiHostPort[0] = 5999;
        g_constantParam.uiHostPort[1] = 5998;
    }
    else
    {
        g_constantParam.commMode = PARAM_COMMMODE_GPRS;
        g_constantParam.ulHostIP[0] = 0x74D46CE5;
        g_constantParam.ulHostIP[1] = 0x1B938067;
        g_constantParam.uiHostPort[0] = 5999;
        g_constantParam.uiHostPort[1] = 5999;
    }

    // timeout
    g_constantParam.TIMEOUT_HOST=leHostTimeOut->text().toUInt()*1000;
    g_constantParam.TIMEOUT_UI=leUserTimeOut->text().toUInt()*1000;
    g_constantParam.TIMEOUT_ERRMSG=leErrMsgTimeOut->text().toUInt()*1000;
    g_constantParam.TIMEOUT_PAPERTEAR=lePaperTearTimeOut->text().toUInt()*1000;

    // flag
    if(chkSetReceipt->isChecked()==true)
        g_constantParam.flagSettleReceipt=true;
    else
        g_constantParam.flagSettleReceipt=false;

    if(chkSelAccEnable->isChecked()==true)
        g_constantParam.flagSelectAccount=true;
    else
        g_constantParam.flagSelectAccount=false;

    // print
    memcpy(g_constantParam.aucReceiptLine1, leRLine1->text().toAscii().data(), PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine2, leRLine2->text().toAscii().data(), PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine3, leRLine3->text().toAscii().data(), PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine4, leRLine4->text().toAscii().data(), PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine1, leALine1->text().toAscii().data(), PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine2, leALine2->text().toAscii().data(), PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine3, leALine3->text().toAscii().data(), PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine4, leALine4->text().toAscii().data(), PARAM_PRINT_SMALL_FONT_LEN);

    QString companyCopy;
    companyCopy+=" --- ";
    companyCopy+=leCompLb->text();
    companyCopy+=" --- ";
    memcpy(g_constantParam.aucCompanyCopyLabel, companyCopy.toAscii().data(), PARAM_PRINT_BIG_FONT_LEN);


    // Tec pass
    if(!leTechPass->text().isEmpty())
        memcpy(g_cashier.aucSuperPSW,leTechPass->text().toAscii().data(),8);


    xDATA::WriteValidFile(xDATA::DataSaveConstant);
    xDATA::WriteValidFile(xDATA::DataSaveChange);
    UIMsg::showNoticeMsgWithAutoClose("SAVE SUCCESS",g_constantParam.TIMEOUT_ERRMSG);

    qDebug()<<"---------"<<Q_FUNC_INFO<<"out"<<"---------";

}

void UIConfigTer::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitCfg()));
    closeTimer->start(timeout);
}

void UIConfigTer::slotQuitCfg()
{
    UIMsg::showNoticeMsgWithAutoClose("TIME OUT",g_constantParam.TIMEOUT_ERRMSG);
    this->close();
}

void UIConfigTer::restartTimer()
{
    closeTimer->start(g_constantParam.TIMEOUT_UI);
}
