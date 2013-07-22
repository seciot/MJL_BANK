#include "uiConfigAcq.h"
#include "xdata.h"
#include "commontools.h"
#include "global.h"

UIConfigAcq::UIConfigAcq(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Acquirer Configuration"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);


    //--------------------   define ----------------------//

    lbIndexNum=new QLabel();
    lbHostName=new QLabel();
    lbHostType=new QLabel();
    lbSettleHost=new QLabel();
    lbCarrierTimeOut=new QLabel();
    lbRetry=new QLabel();
    lbNII=new QLabel();
    lbSyncMode=new QLabel();
    lbTerID=new QLabel();
    lbMerID=new QLabel();
    lbBatchNum=new QLabel();
    lbNextBatchNum=new QLabel();
    lbAPN=new QLabel();
    lbProType=new QLabel();
    lbHostAccess=new QLabel();
    lbDomainAddr=new QLabel();
    lbPriIP=new QLabel();
    lbSecIP=new QLabel();
    lbCurCodeNo=new QLabel();
    lbCurCode=new QLabel();
    lbMinTrnsAmt=new QLabel();
    lbMaxTrnsAmt=new QLabel();
    lbCountryCode=new QLabel();

    leIndexNum=new QLineEdit();
    leIndexNum->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    leHostName=new QLineEdit();
    cbHostType=new QComboBox();
    cbSettleHost=new QComboBox();
    cbCarrierTimeOut=new QComboBox();
    leRetry=new QLineEdit();
    leNII=new QLineEdit();
    leNII->setMaxLength(PARAM_NII_LEN);
    leSyncMode=new QLineEdit();
    leTerID=new QLineEdit();
    leMerID=new QLineEdit();
    leBatchNum=new QLineEdit();
    leNextBatchNum=new QLineEdit();
    leAPN=new QLineEdit();
    cbProType=new QComboBox();
    cbHostAccess=new QComboBox();
    leDomainAddr=new QLineEdit();
    lePriIP=new MyIpAddrEdit();
    lePriIP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lePriIPPort=new QLineEdit();
    lePriIPPort->setMaxLength(5);
    leSecIP=new MyIpAddrEdit();
    leSecIP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    leSecIPPort=new QLineEdit();
    leSecIPPort->setMaxLength(5);
    leCurCodeNo=new QLineEdit();
    leCurCode=new QLineEdit();
    leMinTrnsAmt=new QLineEdit();
    leMinTrnsAmt->setMaxLength(7);
    leMaxTrnsAmt=new QLineEdit();
    leMaxTrnsAmt->setMaxLength(7);
    leCountryCode=new QLineEdit();
    chkCashBackCheck=new QCheckBox();

    lbIndexNum->setText(tr("Index Number:"));
    lbHostName->setText(tr("Host Name:"));
    lbHostType->setText(tr("Host Type:"));
    lbSettleHost->setText(tr("Settle Host:"));
    lbCarrierTimeOut->setText(tr("Carrier Time Out:"));
    lbRetry->setText(tr("Retry:"));
    lbNII->setText(tr("NII:"));
    lbSyncMode->setText(tr("Sync Mode:"));
    lbTerID->setText(tr("Terminnal ID:"));
    lbMerID->setText(tr("Merchant ID:"));
    lbBatchNum->setText(tr("Batch Number:"));
    lbNextBatchNum->setText(tr("Next Batch Number:"));
    lbAPN->setText(tr("APN:"));
    lbProType->setText(tr("Protocol Type:"));
    lbHostAccess->setText(tr("Host Access:"));
    lbDomainAddr->setText(tr("Domain Address:"));
    lbPriIP->setText(tr("Primary IP Address:"));
    lbSecIP->setText(tr("Secondary IP Address:"));
    lbCurCodeNo->setText(tr("Currency Code No:"));
    lbCurCode->setText(tr("Currency Code:"));
    lbMinTrnsAmt->setText(tr("Min Trns Amt:"));
    lbMaxTrnsAmt->setText(tr("Max Trns Amt:"));
    lbCountryCode->setText(tr("Country Code:"));
    chkCashBackCheck->setText(tr("Cash Back Check"));

    chkCashBackCheck->setStyleSheet(CHECK_BOX_STYLE);
    chkCashBackCheck->setMinimumHeight(30);

    cbCarrierTimeOut->setStyleSheet(COMBO_BOX_STYLE);
    cbHostAccess->setStyleSheet(COMBO_BOX_STYLE);
    cbHostType->setStyleSheet(COMBO_BOX_STYLE);
    cbProType->setStyleSheet(COMBO_BOX_STYLE);
    cbSettleHost->setStyleSheet(COMBO_BOX_STYLE);
    cbCarrierTimeOut->setMinimumHeight(26);
    cbHostAccess->setMinimumHeight(26);
    cbHostType->setMinimumHeight(26);
    cbProType->setMinimumHeight(26);
    cbSettleHost->setMinimumHeight(26);

    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbIndexNum);
    v1Lay->addWidget(leIndexNum);
    v1Lay->addWidget(lbHostName);
    v1Lay->addWidget(leHostName);
    v1Lay->addWidget(lbHostType);
    v1Lay->addWidget(cbHostType);
    v1Lay->addWidget(lbSettleHost);
    v1Lay->addWidget(cbSettleHost);
    v1Lay->addWidget(lbCarrierTimeOut);
    v1Lay->addWidget(cbCarrierTimeOut);
    v1Lay->addWidget(lbRetry);
    v1Lay->addWidget(leRetry);
    v1Lay->addWidget(lbNII);
    v1Lay->addWidget(leNII);
    v1Lay->addWidget(lbSyncMode);
    v1Lay->addWidget(leSyncMode);
    v1Lay->addWidget(lbTerID);
    v1Lay->addWidget(leTerID);
    v1Lay->addWidget(lbMerID);
    v1Lay->addWidget(leMerID);
    v1Lay->addWidget(lbBatchNum);
    v1Lay->addWidget(leBatchNum);
    v1Lay->addWidget(lbNextBatchNum);
    v1Lay->addWidget(leNextBatchNum);
    v1Lay->addWidget(lbAPN);
    v1Lay->addWidget(leAPN);
    v1Lay->addWidget(lbProType);
    v1Lay->addWidget(cbProType);
    v1Lay->addWidget(lbHostAccess);
    v1Lay->addWidget(cbHostAccess);
    v1Lay->addWidget(lbDomainAddr);
    v1Lay->addWidget(leDomainAddr);

    QGridLayout *g1Lay=new QGridLayout();
    g1Lay->addWidget(lbPriIP,0,0,1,0);
    g1Lay->addWidget(lePriIP,1,0,1,1);
    g1Lay->addWidget(lePriIPPort,1,1,1,1);
    g1Lay->addWidget(lbSecIP,2,0,1,0);
    g1Lay->addWidget(leSecIP,3,0,1,1);
    g1Lay->addWidget(leSecIPPort,3,1,1,1);
    lePriIPPort->setMaximumWidth(50);
    leSecIPPort->setMaximumWidth(50);

    QVBoxLayout *v2Lay=new QVBoxLayout();
    v2Lay->addWidget(lbCurCodeNo);
    v2Lay->addWidget(leCurCodeNo);
    v2Lay->addWidget(lbCurCode);
    v2Lay->addWidget(leCurCode);
    v2Lay->addWidget(lbMinTrnsAmt);
    v2Lay->addWidget(leMinTrnsAmt);
    v2Lay->addWidget(lbMaxTrnsAmt);
    v2Lay->addWidget(leMaxTrnsAmt);
    v2Lay->addWidget(lbCountryCode);
    v2Lay->addWidget(leCountryCode);
    v2Lay->addWidget(chkCashBackCheck);

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
    scArea->setFrameShape(QFrame::NoFrame);
    vBar->setMaximumWidth(5);
    //    vBar->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QVBoxLayout *scLayout=new QVBoxLayout(scWidget);
    scLayout->addLayout(v1Lay);
    scLayout->addLayout(g1Lay);
    scLayout->addLayout(v2Lay);

    scWidget->setMaximumWidth(FRAME420_WIDTH-5);
    scWidget->setFont(font);
    //    scWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
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



    this->initialSettings();
}

UIConfigAcq::~UIConfigAcq()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigAcq::keyPressEvent(QKeyEvent *event)
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

void UIConfigAcq::mousePressEvent(QMouseEvent *event)
{
    //    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();
    //    qDebug()<<leIndexNum->size().width();

}

void UIConfigAcq::mouseMoveEvent(QMouseEvent *event)
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

void UIConfigAcq::initialSettings()
{
    qDebug()<<Q_FUNC_INFO;

    /// NII
    leNII->setText(QString::fromAscii((const char*)g_constantParam.aucNii));


    //    g_constantParam.ulHostIP[0] = 0x74D46CE5;
    //    g_constantParam.ulHostIP[1] = 0x1B938067;
    //    g_constantParam.uiHostPort[0] = 5999;
    //    g_constantParam.uiHostPort[1] = 5999;
    // IP::Port
    QString priIp=FormIpAddressFromArray(g_constantParam.ulHostIP[0]);
    QString secIp=FormIpAddressFromArray(g_constantParam.ulHostIP[1]);
    priIp=priIp.trimmed();
    secIp=secIp.trimmed();
    qDebug()<<priIp<<secIp;
    lePriIP->settext(priIp);
    leSecIP->settext(secIp);

    lePriIPPort->setText(QString::number( g_constantParam.uiHostPort[0]));
    leSecIPPort->setText(QString::number( g_constantParam.uiHostPort[1]));

    // 终端号
    leTerID->setText(QString::fromAscii((const char*)g_constantParam.aucTerminalID));

    // 货币代号
    leCurCode->setText(QString::fromAscii((const char*)g_constantParam.aucCurrencySign));

    // 货币数字代码
    leCurCodeNo->setText(QString::number(g_constantParam.usCurrencyId));

    // 有交易的情况下,以下设置为只读,不可更改,要么先结算
    if(g_transInfo.TransTotal.uiTotalNb>0)
    {
        leTerID->setReadOnly(true);
        leMerID->setReadOnly(true);
        leBatchNum->setReadOnly(true);
        leNextBatchNum->setReadOnly(true);

        connect(leTerID,SIGNAL(editingFinished()),this,SLOT(warning()));
        connect(leTerID,SIGNAL(returnPressed()),this,SLOT(warning()));
        connect(leTerID,SIGNAL(selectionChanged()),this,SLOT(warning()));
    }

    // 最大最小金额
    leMinTrnsAmt->setText(QString::number(g_constantParam.ulMinAmount));
    leMaxTrnsAmt->setText(QString::number(g_constantParam.ulMaxAmount));
}

void UIConfigAcq::warning()
{
    qDebug()<<Q_FUNC_INFO<<sender();
}

void UIConfigAcq::slotSubmitClicked()
{
    // Nii
    QString nii=leNII->text();
    memcpy(g_constantParam.aucNii, nii.toAscii().data(), 4);

    /// ip
    QString priIP=lePriIP->text();
    QString secIP=leSecIP->text();
    unsigned long ulPriIP;
    unsigned long ulSecIP;

    //    g_constantParam.ulHostIP[0] = 0x74D46CE5;
    //    g_constantParam.ulHostIP[1] = 0x1B938067;
    qDebug()<< g_constantParam.ulHostIP[0]<< g_constantParam.ulHostIP[1];
    CONV_IPStringULong((unsigned char *)priIP.toAscii().data(), &ulPriIP);
    CONV_IPStringULong((unsigned char *)secIP.toAscii().data(), &ulSecIP);
    qDebug()<<ulPriIP<<ulSecIP;
    g_constantParam.ulHostIP[0] =  ulPriIP;
    g_constantParam.ulHostIP[1] =  ulSecIP;
    qDebug()<< g_constantParam.ulHostIP[0]<< g_constantParam.ulHostIP[1];

    // 终端号
    QString terminalID=leTerID->text();
    memcpy(g_constantParam.aucTerminalID, terminalID.toAscii().data(), PARAM_TERMINALID_LEN);

    // 货币代号
    QString currencyCode=leCurCode->text();
    memcpy(g_constantParam.aucCurrencySign, currencyCode.toAscii().data(), 2);

    // 货币数字代码
    QString currencyCodeNo=leCurCodeNo->text();
    g_constantParam.usCurrencyId=currencyCodeNo.toAscii().toShort();

    // 最大最小金额
    QString minAmt=leMinTrnsAmt->text();
    QString maxAmt=leMaxTrnsAmt->text();
    if(minAmt.toULong()>maxAmt.toULong())
    {
        UIMsg::showErrMsgWithAutoClose("AMOUNT\nMIN > MAX",g_changeParam.TIMEOUT_ERRMSG);
        return;
    }
    else
    {
        g_constantParam.ulMinAmount=minAmt.toULong();
        g_constantParam.ulMaxAmount=maxAmt.toULong();
    }



    /// 保存设置
    unsigned char ucResult=xDATA::WriteValidFile(xDATA::DataSaveConstant);
    if(!ucResult)
    {
        //成功
        UIMsg::showNoticeMsgWithAutoClose("SAVE SUCCESS",g_changeParam.TIMEOUT_ERRMSG);
    }
    else
    {
        UIMsg::showFileErrMsgWithAutoClose(FileErrIndex(ucResult),g_changeParam.TIMEOUT_ERRMSG);
    }

}
