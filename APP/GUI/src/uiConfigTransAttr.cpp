#include "uiConfigTransAttr.h"
#include "xdata.h"
#include "global.h"

UIConfigTransAttr::UIConfigTransAttr(QDialog *parent,Qt::WindowFlags f) :
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
    QFont font3("Helvetica",13,QFont::Bold);

    lbHead=new QLabel();
    lbHead->setText(tr("Transaction Attribute"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);


    //--------------------   define ----------------------//
    lbCashAd=new QLabel();
    lbCashDe=new QLabel();
    lbP2P=new QLabel();
    lbVOID=new QLabel();
    lbAdjust=new QLabel();
    lbBalanceInq=new QLabel();
    lbPay=new QLabel();
    lbPin=new QLabel();

    chkCashAdTrans=new QCheckBox();
    chkCashDeTrans=new QCheckBox();
    chkP2PTrans=new QCheckBox();
    chkVOIDTrans=new QCheckBox();
    chkAdjustTrans=new QCheckBox();
    chkBalanceInqTrans=new QCheckBox();
    chkPayTrans=new QCheckBox();
    chkPinTrans=new QCheckBox();

    chkCashAdPin=new QCheckBox();
    chkCashDePin=new QCheckBox();
    chkP2PPin=new QCheckBox();
    chkVOIDPin=new QCheckBox();
    chkAdjustPin=new QCheckBox();
    chkBalanceInqPin=new QCheckBox();
    chkPayPin=new QCheckBox();
    chkPinPin=new QCheckBox();

    chkCashAdMan=new QCheckBox();
    chkCashDeMan=new QCheckBox();
    chkP2PMan=new QCheckBox();
    chkVOIDMan=new QCheckBox();
    chkAdjustMan=new QCheckBox();
    chkBalanceInqMan=new QCheckBox();
    chkPayMan=new QCheckBox();
    chkPinMan=new QCheckBox();

    lbCashAd->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);
    //--------------------   SET TEXT ----------------------//
    lbCashAd->setText(tr("Cash Advance"));
    lbCashDe->setText(tr("Cash Deposit"));
    lbP2P->setText(tr("P2P Transfer"));
    lbVOID->setText(tr("VOID"));
    lbAdjust->setText(tr("Adjust"));
    lbBalanceInq->setText(tr("Balance Inquiry"));
    lbPin->setText(tr("PIN Change"));
    lbPay->setText(tr("Payment"));

    lbCashAd->setAlignment(Qt::AlignCenter);
    lbCashDe->setAlignment(Qt::AlignCenter);
    lbP2P->setAlignment(Qt::AlignCenter);
    lbVOID->setAlignment(Qt::AlignCenter);
    lbAdjust->setAlignment(Qt::AlignCenter);
    lbBalanceInq->setAlignment(Qt::AlignCenter);
    lbPin->setAlignment(Qt::AlignCenter);
    lbPay->setAlignment(Qt::AlignCenter);

    lbCashAd->setFont(font2);
    lbCashDe->setFont(font2);
    lbP2P->setFont(font2);
    lbVOID->setFont(font2);
    lbAdjust->setFont(font2);
    lbBalanceInq->setFont(font2);
    lbPin->setFont(font2);
    lbPay->setFont(font2);

    lbCashAd->setStyleSheet(HEAD_STYLE);
    lbCashDe->setStyleSheet(HEAD_STYLE);
    lbP2P->setStyleSheet(HEAD_STYLE);
    lbPay->setStyleSheet(HEAD_STYLE);
    lbVOID->setStyleSheet(HEAD_STYLE);
    lbAdjust->setStyleSheet(HEAD_STYLE);
    lbBalanceInq->setStyleSheet(HEAD_STYLE);
    lbPin->setStyleSheet(HEAD_STYLE);

    lbCashAd->setMinimumHeight(30);
    lbCashDe->setMinimumHeight(30);
    lbP2P->setMinimumHeight(30);
    lbPay->setMinimumHeight(30);
    lbVOID->setMinimumHeight(30);
    lbAdjust->setMinimumHeight(30);
    lbBalanceInq->setMinimumHeight(30);
    lbPin->setMinimumHeight(30);

    chkCashAdTrans->setText(tr("Transaction Enable"));
    chkCashDeTrans->setText(tr("Transaction Enable"));
    chkP2PTrans->setText(tr("Transaction Enable"));
    chkVOIDTrans->setText(tr("Transaction Enable"));
    chkAdjustTrans->setText(tr("Transaction Enable"));
    chkBalanceInqTrans->setText(tr("Transaction Enable"));
    chkPayTrans->setText(tr("Transaction Enable"));
    chkPinTrans->setText(tr("Transaction Enable"));

    chkCashAdPin->setText(tr("PIN Entry Enable"));
    chkCashDePin->setText(tr("PIN Entry Enable"));
    chkP2PPin->setText(tr("PIN Entry Enable"));
    chkVOIDPin->setText(tr("PIN Entry Enable"));
    chkAdjustPin->setText(tr("PIN Entry Enable"));
    chkBalanceInqPin->setText(tr("PIN Entry Enable"));
    chkPayPin->setText(tr("PIN Entry Enable"));
    chkPinPin->setText(tr("PIN Entry Enable"));


    chkCashAdMan->setText(tr("Manual Card Entry Enable"));
    chkCashDeMan->setText(tr("Manual Card Entry Enable"));
    chkP2PMan->setText(tr("Manual Card Entry Enable"));
    chkVOIDMan->setText(tr("Manual Card Entry Enable"));
    chkAdjustMan->setText(tr("Manual Card Entry Enable"));
    chkBalanceInqMan->setText(tr("Manual Card Entry Enable"));
    chkPayMan->setText(tr("Manual Card Entry Enable"));
    chkPinMan->setText(tr("Manual Card Entry Enable"));

    //--------------------   LAYOUT ----------------------//
    int space=30;
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbCashAd);
    v1Lay->addWidget(chkCashAdTrans);
    v1Lay->addWidget(chkCashAdPin);
    v1Lay->addWidget(chkCashAdMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbCashDe);
    v1Lay->addWidget(chkCashDeTrans);
    v1Lay->addWidget(chkCashDePin);
    v1Lay->addWidget(chkCashDeMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbP2P);
    v1Lay->addWidget(chkP2PTrans);
    v1Lay->addWidget(chkP2PPin);
    v1Lay->addWidget(chkP2PMan);
    v1Lay->addSpacing(space);


    v1Lay->addWidget(lbVOID);
    v1Lay->addWidget(chkVOIDTrans);
    v1Lay->addWidget(chkVOIDPin);
    v1Lay->addWidget(chkVOIDMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbAdjust);
    v1Lay->addWidget(chkAdjustTrans);
    v1Lay->addWidget(chkAdjustPin);
    v1Lay->addWidget(chkAdjustMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbBalanceInq);
    v1Lay->addWidget(chkBalanceInqTrans);
    v1Lay->addWidget(chkBalanceInqPin);
    v1Lay->addWidget(chkBalanceInqMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbPay);
    v1Lay->addWidget(chkPayTrans);
    v1Lay->addWidget(chkPayPin);
    v1Lay->addWidget(chkPayMan);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbPin);
    v1Lay->addWidget(chkPinTrans);
    v1Lay->addWidget(chkPinPin);
    v1Lay->addWidget(chkPinMan);
    v1Lay->addSpacing(space);


    //--------------------   CANCEL & SUBMIT ----------------------//
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
    connect(vBar,SIGNAL(valueChanged(int)),this,SLOT(restartTimer()));

    this->initialAttr();
    this->setAutoClose(g_changeParam.TIMEOUT_UI);

}

void UIConfigTransAttr::initialAttr()
{
    unsigned char ucResult=xDATA::ReadValidFile(xDATA::DataSaveChange);
//    if(ucResult)
//    UIMsg::showFileErrMsgWithAutoClose(FileErrIndex(ucResult),g_changeParam.TIMEOUT_ERRMSG);

    if(g_changeParam.advance.TRANS_ENABLE==true)
        chkCashAdTrans->setChecked(true);
    if(g_changeParam.advance.PIN_ENABLE==true)
        chkCashAdPin->setChecked(true);
    if(g_changeParam.advance.MANUAL_ENABLE==true)
        chkCashAdMan->setChecked(true);

    if(g_changeParam.deposit.TRANS_ENABLE==true)
        chkCashDeTrans->setChecked(true);
    if(g_changeParam.deposit.PIN_ENABLE==true)
        chkCashDePin->setChecked(true);
    if(g_changeParam.deposit.MANUAL_ENABLE==true)
        chkCashDeMan->setChecked(true);

    if(g_changeParam.balance.TRANS_ENABLE==true)
        chkBalanceInqTrans->setChecked(true);
    if(g_changeParam.balance.PIN_ENABLE==true)
        chkBalanceInqPin->setChecked(true);
    if(g_changeParam.balance.MANUAL_ENABLE==true)
        chkBalanceInqMan->setChecked(true);

    if(g_changeParam.p2p.TRANS_ENABLE==true)
        chkP2PTrans->setChecked(true);
    if(g_changeParam.p2p.PIN_ENABLE==true)
        chkP2PPin->setChecked(true);
    if(g_changeParam.p2p.MANUAL_ENABLE==true)
        chkP2PMan->setChecked(true);

    if(g_changeParam.transvoid.TRANS_ENABLE==true)
        chkVOIDTrans->setChecked(true);
    if(g_changeParam.transvoid.PIN_ENABLE==true)
        chkVOIDPin->setChecked(true);
    if(g_changeParam.transvoid.MANUAL_ENABLE==true)
        chkVOIDMan->setChecked(true);

    if(g_changeParam.adjust.TRANS_ENABLE==true)
        chkAdjustTrans->setChecked(true);
    if(g_changeParam.adjust.PIN_ENABLE==true)
        chkAdjustPin->setChecked(true);
    if(g_changeParam.adjust.MANUAL_ENABLE==true)
        chkAdjustMan->setChecked(true);

    if(g_changeParam.pinchange.TRANS_ENABLE==true)
        chkPinTrans->setChecked(true);
    if(g_changeParam.pinchange.PIN_ENABLE==true)
        chkPinPin->setChecked(true);
    if(g_changeParam.pinchange.MANUAL_ENABLE==true)
        chkPinMan->setChecked(true);
}

void UIConfigTransAttr::slotSubmitClicked()
{
    closeTimer->stop();
    //cash advance
    if(chkCashAdTrans->isChecked())
        g_changeParam.advance.TRANS_ENABLE=true;
    else
        g_changeParam.advance.TRANS_ENABLE=false;
    if(chkCashAdPin->isChecked())
        g_changeParam.advance.PIN_ENABLE=true;
    else
        g_changeParam.advance.PIN_ENABLE=false;
    if(chkCashAdMan->isChecked())
        g_changeParam.advance.MANUAL_ENABLE=true;
    else
        g_changeParam.advance.MANUAL_ENABLE=false;

    // cash deposit
    if(chkCashDeTrans->isChecked())
        g_changeParam.deposit.TRANS_ENABLE=true;
    else
        g_changeParam.deposit.TRANS_ENABLE=false;
    if(chkCashDePin->isChecked())
        g_changeParam.deposit.PIN_ENABLE=true;
    else
        g_changeParam.deposit.PIN_ENABLE=false;
    if(chkCashDeMan->isChecked())
        g_changeParam.deposit.MANUAL_ENABLE=true;
    else
        g_changeParam.deposit.MANUAL_ENABLE=false;

    // balance inquiry
    if(chkBalanceInqTrans->isChecked())
        g_changeParam.balance.TRANS_ENABLE=true;
    else
        g_changeParam.balance.TRANS_ENABLE=false;
    if(chkBalanceInqPin->isChecked())
        g_changeParam.balance.PIN_ENABLE=true;
    else
        g_changeParam.balance.PIN_ENABLE=false;
    if(chkBalanceInqMan->isChecked())
        g_changeParam.balance.MANUAL_ENABLE=true;
    else
        g_changeParam.balance.MANUAL_ENABLE=false;

    // p2p transfer
    if(chkP2PTrans->isChecked())
        g_changeParam.p2p.TRANS_ENABLE=true;
    else
        g_changeParam.p2p.TRANS_ENABLE=false;
    if(chkP2PPin->isChecked())
        g_changeParam.p2p.PIN_ENABLE=true;
    else
        g_changeParam.p2p.PIN_ENABLE=false;
    if(chkP2PMan->isChecked())
        g_changeParam.p2p.MANUAL_ENABLE=true;
    else
        g_changeParam.p2p.MANUAL_ENABLE=false;

    // VOID
    if(chkVOIDTrans->isChecked())
        g_changeParam.transvoid.TRANS_ENABLE=true;
    else
        g_changeParam.transvoid.TRANS_ENABLE=false;
    if(chkVOIDPin->isChecked())
        g_changeParam.transvoid.PIN_ENABLE=true;
    else
        g_changeParam.transvoid.PIN_ENABLE=false;
    if(chkVOIDMan->isChecked())
        g_changeParam.transvoid.MANUAL_ENABLE=true;
    else
        g_changeParam.transvoid.MANUAL_ENABLE=false;

    // Adjust
    if(chkAdjustTrans->isChecked())
        g_changeParam.adjust.TRANS_ENABLE=true;
    else
        g_changeParam.adjust.TRANS_ENABLE=false;
    if(chkAdjustPin->isChecked())
        g_changeParam.adjust.PIN_ENABLE=true;
    else
        g_changeParam.adjust.PIN_ENABLE=false;
    if(chkAdjustMan->isChecked())
        g_changeParam.adjust.MANUAL_ENABLE=true;
    else
        g_changeParam.adjust.MANUAL_ENABLE=false;

    // PIN Change
    if(chkPinTrans->isChecked())
        g_changeParam.pinchange.TRANS_ENABLE=true;
    else
        g_changeParam.pinchange.TRANS_ENABLE=false;
    if(chkPinPin->isChecked())
        g_changeParam.pinchange.PIN_ENABLE=true;
    else
        g_changeParam.pinchange.PIN_ENABLE=false;
    if(chkPinMan->isChecked())
        g_changeParam.pinchange.MANUAL_ENABLE=true;
    else
        g_changeParam.pinchange.MANUAL_ENABLE=false;

    unsigned char ucResult=xDATA::WriteValidFile(xDATA::DataSaveChange);
    if(!ucResult)
        UIMsg::showNoticeMsgWithAutoClose("Save Success",g_changeParam.TIMEOUT_ERRMSG);
}

UIConfigTransAttr::~UIConfigTransAttr()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigTransAttr::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F3:
        vBar->setValue(vBar->value()-150);
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    case Qt::Key_F4:
        vBar->setValue(vBar->value()+150);
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    case Qt::Key_Enter:
        this->slotSubmitClicked();
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIConfigTransAttr::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();
//    qDebug()<<leIndexNum->size().width();

}

void UIConfigTransAttr::mouseMoveEvent(QMouseEvent *event)
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


void UIConfigTransAttr::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitCfg()));
    closeTimer->start(timeout);
}

void UIConfigTransAttr::slotQuitCfg()
{
    UIMsg::showNoticeMsgWithAutoClose("TIME OUT",g_changeParam.TIMEOUT_ERRMSG);
    this->close();
}

void UIConfigTransAttr::restartTimer()
{
    closeTimer->start(g_changeParam.TIMEOUT_UI);
}
