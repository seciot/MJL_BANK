#include "uiConfigCardBIN.h"
#include "xdata.h"
#include "commontools.h"
#include "global.h"

UIConfigCardBIN::UIConfigCardBIN(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Card BIN & Issuer"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);


    //--------------------   define ----------------------//
    lbSelectCardBIN=new QLabel();
    lbSelectCardBIN->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    lbIssuerText=new QLabel();
    lbIssuerText->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    lbIssuerID=new QLabel();
    lbAcquirerID=new QLabel();
    lbCardBINText=new QLabel();
    lbCardBINLow=new QLabel();
    lbCardBINHigh=new QLabel();
    lbHostType=new QLabel();
    lbIP=new QLabel();
    lbDomainName=new QLabel();

    lbSelectCardBIN->setText(tr("Select Card BIN:"));
    lbIssuerText->setText(tr("Issuer Text:"));
    lbIssuerID->setText(tr("Issuer ID:"));
    lbAcquirerID->setText(tr("Acquirer ID:"));
    lbCardBINText->setText(tr("Card BIN Text:"));
    lbCardBINLow->setText(tr("Card BIN Low:"));
    lbCardBINHigh->setText(tr("Card BIN High:"));
    lbHostType->setText(tr("Host Type:"));
    lbIP->setText(tr("IP :: Port"));
    lbDomainName->setText(tr("Domain Name:"));

    QPixmap pixmap(1,COMBO_HEIGHT);
    pixmap.fill(Qt::transparent);
    QIcon blankicon(pixmap);
    cbSelectCardBIN=new QComboBox();
    cbSelectCardBIN->setIconSize(QSize(1, COMBO_HEIGHT));
    cbSelectCardBIN->addItem(blankicon, "Please Select Card BIN");
    cbSelectCardBIN->setStyleSheet(COMBO_BOX_STYLE);

    leIssuerText=new QLineEdit();
    leIssuerID=new QLineEdit();
    leAcquirerID=new QLineEdit();
    leCardBINText=new QLineEdit();
    leCardBINLow=new QLineEdit();
    leCardBINHigh=new QLineEdit();

    rdIP=new QRadioButton();
    rdDomain=new QRadioButton();
    rdIP->setText("IP");
    rdDomain->setText("Domain Name");
    hostTypeGroup=new QButtonGroup(this);
    hostTypeGroup->addButton(rdIP);
    hostTypeGroup->addButton(rdDomain);
    ipAddr=new MyIpAddrEdit();
    ipAddr->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    leIPPort=new QLineEdit();
    leDomainName=new QLineEdit();

    this->setEditable(false);

    //--------------------   LAYOUT ----------------------//
    QHBoxLayout *rdLay=new QHBoxLayout();
    rdLay->addWidget(rdIP);
    rdLay->addWidget(rdDomain);

    QHBoxLayout *ipLay=new QHBoxLayout();
    ipLay->addWidget(ipAddr);
    ipLay->addWidget(leIPPort);
    leIPPort->setMaximumWidth(50);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    //    v1Lay->addWidget(lbSelectCardBIN);
    v1Lay->addWidget(cbSelectCardBIN);
    v1Lay->addWidget(lbIssuerText );
    v1Lay->addWidget(leIssuerText);
    v1Lay->addWidget(lbIssuerID );
    v1Lay->addWidget(leIssuerID );
    v1Lay->addWidget(lbAcquirerID );
    v1Lay->addWidget(leAcquirerID );
    v1Lay->addWidget(lbCardBINText );
    v1Lay->addWidget(leCardBINText);
    v1Lay->addWidget(lbCardBINLow );
    v1Lay->addWidget(leCardBINLow );
    v1Lay->addWidget(lbCardBINHigh );
    v1Lay->addWidget(leCardBINHigh );
    v1Lay->addWidget(lbHostType );
    v1Lay->addLayout(rdLay );
    v1Lay->addWidget(lbIP );
    v1Lay->addLayout(ipLay);
    v1Lay->addWidget(lbDomainName );
    v1Lay->addWidget(leDomainName );

    btnCancel=new QPushButton();
    btnApply=new QPushButton();
    btnAdd=new QPushButton();
    btnSubmit=new QPushButton();
    btnCancel->setText(tr("Cancel"));
    btnApply->setText(tr("Apply"));
    btnAdd->setText(tr("Add"));
    btnSubmit->setText(tr("Use This Card BIN"));
    btnCancel->setFont(font2);
    btnApply->setFont(font2);
    btnAdd->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnApply->setMinimumHeight(30);
    btnAdd->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnApply->setStyleSheet(BTN_GREEN_STYLE);
    btnAdd->setStyleSheet(BTN_BLUE_STYLE);
    btnCancel->setStyleSheet(BTN_GREY_STYLE);
    btnSubmit->setStyleSheet(BTN_BLUE_STYLE);


    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(10);
    h1Lay->addWidget(btnCancel);
    h1Lay->addWidget(btnAdd);
    h1Lay->addWidget(btnApply);
    h1Lay->addSpacing(10);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    // ---------------ScrollArea----------- //
    scArea=new QScrollArea();
    scWidget=new QWidget();
    hBar=new QScrollBar();
    vBar=new QScrollBar();
    scArea->setFrameShape(QFrame::NoFrame);
    vBar->setMaximumWidth(5);

    QVBoxLayout *scLayout=new QVBoxLayout(scWidget);
    scLayout->addLayout(v1Lay);

    scWidget->setMaximumWidth(FRAME420_WIDTH-5);
    scWidget->setFont(font);
    vBar->setStyleSheet(SCROLL_VERTICAL_SIMPLE_STYLE);


    scArea->setVerticalScrollBar(vBar);
    scArea->setHorizontalScrollBar(hBar);
    scArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scArea->setWidget(scWidget);

    // ------------------------------------------- //

    QVBoxLayout *layout=new QVBoxLayout(this);
    //    layout->addWidget(lbHead);
    layout->addWidget(scArea);
    layout->addLayout(h1Lay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,5);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnApply, SIGNAL(clicked()), this, SLOT(slotApply()));
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(slotAdd()));
    connect(cbSelectCardBIN, SIGNAL(activated(int)), this, SLOT(slotInitialCardBIN(int)));
    connect(vBar,SIGNAL(valueChanged(int)),this,SLOT(restartTimer()));

    this->setAutoClose(g_constantParam.TIMEOUT_UI);
}

UIConfigCardBIN::~UIConfigCardBIN()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigCardBIN::keyPressEvent(QKeyEvent *event)
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

void UIConfigCardBIN::mousePressEvent(QMouseEvent *event)
{
    //    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();
    //    qDebug()<<leIndexNum->size().width();

}

void UIConfigCardBIN::mouseMoveEvent(QMouseEvent *event)
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

void UIConfigCardBIN::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitCfg()));
    closeTimer->start(timeout);
}

void UIConfigCardBIN::slotQuitCfg()
{
    UIMsg::showNoticeMsgWithAutoClose("TIME OUT",g_constantParam.TIMEOUT_ERRMSG);
    this->close();
}

void UIConfigCardBIN::slotApply()
{
    qDebug()<<Q_FUNC_INFO;
    this->setEditable(true);
    this->checkInput();

    // issuer text
    QString issuerText=leIssuerText->text();
    memcpy(g_binData.aucIssuerText, issuerText.toAscii().data(), PARAM_ISSUER_TEXT_LEN);

    // issuer id
    QString issuerID=leIssuerID->text();
    g_binData.uiIssuerID=issuerID.toUInt();

    // acquirer id
    QString acquirerID=leAcquirerID->text();
    g_binData.uiAcquirerID=acquirerID.toUInt();

    // card bin
    QString cardBINText=leCardBINText->text();
    memcpy(g_binData.aucCardBINText, cardBINText.toAscii().data(), PARAM_CARDBIN_TEXT_LEN);

    // card bin low
    QString cardBINLow=leCardBINLow->text();
    g_binData.ulCardBINLow=cardBINLow.toULong();

    //card bin high
    QString cardBINHigh=leCardBINHigh->text();
    g_binData.ulCardBINHigh=cardBINHigh.toULong();

    // host type
    if(hostTypeGroup->checkedButton()==rdDomain)
        g_binData.hostType=HOST_DOMAINNAME;
    else if(hostTypeGroup->checkedButton()==rdIP)
        g_binData.hostType=HOST_IP;
    else
        g_binData.hostType=HOST_IP;

    // ip :: port
    QString priIP=ipAddr->text();
    unsigned long ulPriIP;
    qDebug()<< g_constantParam.ulHostIP[0];
    CONV_IPStringULong((unsigned char *)priIP.toAscii().data(), &ulPriIP);
    qDebug()<<ulPriIP;
    g_binData.ulHostIP[0] =  ulPriIP;
    qDebug()<< g_constantParam.ulHostIP[0];
    g_binData.uiHostPort[0]=leIPPort->text().toUInt();
    qDebug()<<g_constantParam.uiHostPort[0];

    // domain
    QString domain=leDomainName->text();
    memcpy(g_binData.aucDomainName, domain.toAscii().data(), domain.length());


    /// 写入文件
}

void UIConfigCardBIN::slotInitialCardBIN(int index)
{
    qDebug()<<Q_FUNC_INFO;
    if(index<=0)
    {
        UIMsg::showErrMsgWithAutoClose("Choose Wrong\nCard BIN",g_constantParam.TIMEOUT_ERRMSG);
        return;
    }
    this->setEditable(false);
    unsigned char ucResult=xDATA::ReadValidFile(xDATA::DataSaveBINStatus);
    if(g_binStatus.status[index-1]==true)
        qDebug()<<"";

}

void UIConfigCardBIN::slotAdd()
{
    qDebug()<<Q_FUNC_INFO;
    this->setClear();
    this->setEditable(true);

}

void UIConfigCardBIN::setEditable(bool flag)
{
    qDebug()<<Q_FUNC_INFO;
    leIssuerText->setReadOnly(flag);
    leIssuerID->setReadOnly(flag);
    leAcquirerID->setReadOnly(flag);
    leCardBINText->setReadOnly(flag);
    leCardBINLow->setReadOnly(flag);
    leCardBINHigh->setReadOnly(flag);
    rdDomain->setEnabled(flag);
    rdIP->setEnabled(flag);
    ipAddr->setReadOnly(flag);
    leIPPort->setReadOnly(flag);
    leDomainName->setReadOnly(flag);
}

void UIConfigCardBIN::setClear()
{
    qDebug()<<Q_FUNC_INFO;
    leIssuerText->clear();
    leIssuerID->clear();
    leAcquirerID->clear();
    leCardBINText->clear();
    leCardBINLow->clear();
    leCardBINHigh->clear();
    rdDomain->setChecked(true);
    rdIP->setChecked(false);
    ipAddr->clearIP();
    leIPPort->clear();
    leDomainName->clear();
}

void UIConfigCardBIN::checkInput()
{
    if(leIssuerText->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Issuer Text",g_constantParam.TIMEOUT_ERRMSG);
    if(leIssuerID->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Issuer ID",g_constantParam.TIMEOUT_ERRMSG);
    if(leAcquirerID->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Acquirer ID",g_constantParam.TIMEOUT_ERRMSG);
    if(leCardBINText->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Card BIN Text",g_constantParam.TIMEOUT_ERRMSG);
    if(leCardBINLow->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Card BIN Low",g_constantParam.TIMEOUT_ERRMSG);
    if(leCardBINHigh->text().isEmpty())
        UIMsg::showNoticeMsgWithAutoClose("Input Card BIN High",g_constantParam.TIMEOUT_ERRMSG);
    if(ipAddr->text().isEmpty() && rdIP->isChecked())
        UIMsg::showNoticeMsgWithAutoClose("Input IP Address",g_constantParam.TIMEOUT_ERRMSG);
    if(leIPPort->text().isEmpty() && rdIP->isChecked())
        UIMsg::showNoticeMsgWithAutoClose("Input IP Port",g_constantParam.TIMEOUT_ERRMSG);
    if(leDomainName->text().isEmpty() && rdDomain->isChecked())
        UIMsg::showNoticeMsgWithAutoClose("Input Domain Name",g_constantParam.TIMEOUT_ERRMSG);

}

void UIConfigCardBIN::restartTimer()
{
    qDebug()<<Q_FUNC_INFO;
    closeTimer->start(g_constantParam.TIMEOUT_UI);
}
