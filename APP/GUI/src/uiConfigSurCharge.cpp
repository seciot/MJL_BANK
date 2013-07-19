#include "uiConfigSurCharge.h"
#include "global.h"

UIConfigSurCharge::UIConfigSurCharge(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Sur-Charge"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);


    //--------------------   define ----------------------//
    lbCashAd=new QLabel();
    lbCashDe=new QLabel();
    lbP2PCard=new QLabel();
    lbP2PAcc=new QLabel();
    lbPay=new QLabel();

    lbCashAdChangeAmount=new QLabel();
    lbCashDeChangeAmount=new QLabel();
    lbP2PCardChangeAmount=new QLabel();
    lbP2PAccChangeAmount=new QLabel();
    lbPayChangeAmount=new QLabel();

    leCashAdChangeAmount=new QLineEdit();
    leCashDeChangeAmount=new QLineEdit();
    leP2PCardChangeAmount=new QLineEdit();
    leP2PAccChangeAmount=new QLineEdit();
    lePayChangeAmount=new QLineEdit();

    chkCashAdSur=new QCheckBox();
    chkCashDeSur=new QCheckBox();
    chkP2PCardSur=new QCheckBox();
    chkP2PAccSur=new QCheckBox();
    chkPaySur=new QCheckBox();

    chkCashAdFlat=new QCheckBox();
    chkCashDeFlat=new QCheckBox();
    chkP2PCardFlat=new QCheckBox();
    chkP2PAccFlat=new QCheckBox();
    chkPayFlat=new QCheckBox();

    chkCashAdPer=new QCheckBox();
    chkCashDePer=new QCheckBox();
    chkP2PCardPer=new QCheckBox();
    chkP2PAccPer=new QCheckBox();
    chkPayPer=new QCheckBox();

    chkCashAdAmountRange=new QCheckBox();
    chkCashDeAmountRange=new QCheckBox();
    chkP2PCardAmountRange=new QCheckBox();
    chkP2PAccAmountRange=new QCheckBox();
    chkPayAmountRange=new QCheckBox();

    lbCashAd->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);
    //--------------------   SET TEXT ----------------------//

    lbCashAd->setText(tr("Cash Advance"));
    lbCashDe->setText(tr("Cash Deposit"));
    lbP2PCard->setText(tr("P2P Card Transfer"));
    lbP2PAcc->setText(tr("P2P Account Transfer"));
    lbPay->setText(tr("Payment"));
    lbCashAd->setAlignment(Qt::AlignCenter);
    lbCashDe->setAlignment(Qt::AlignCenter);
    lbP2PCard->setAlignment(Qt::AlignCenter);
    lbP2PAcc->setAlignment(Qt::AlignCenter);
    lbPay->setAlignment(Qt::AlignCenter);
    lbCashAd->setFont(font2);
    lbCashDe->setFont(font2);
    lbP2PCard->setFont(font2);
    lbP2PAcc->setFont(font2);
    lbPay->setFont(font2);
    lbCashAd->setStyleSheet(HEAD_STYLE);
    lbCashDe->setStyleSheet(HEAD_STYLE);
    lbP2PCard->setStyleSheet(HEAD_STYLE);
    lbP2PAcc->setStyleSheet(HEAD_STYLE);
    lbPay->setStyleSheet(HEAD_STYLE);
    lbCashAd->setMinimumHeight(30);
    lbCashDe->setMinimumHeight(30);
    lbP2PCard->setMinimumHeight(30);
    lbP2PAcc->setMinimumHeight(30);
    lbPay->setMinimumHeight(30);

    lbCashAdChangeAmount->setText(tr("Charge Amount:"));
    lbCashDeChangeAmount->setText(tr("Charge Amount:"));
    lbP2PCardChangeAmount->setText(tr("Charge Amount:"));
    lbP2PAccChangeAmount->setText(tr("Charge Amount:"));
    lbPayChangeAmount->setText(tr("Charge Amount:"));

    chkCashAdSur->setText("Sur-Charge Enable");
    chkCashDeSur->setText("Sur-Charge Enable");
    chkP2PCardSur->setText("Sur-Charge Enable");
    chkP2PAccSur->setText("Sur-Charge Enable");
    chkPaySur->setText("Sur-Charge Enable");

    chkCashAdFlat->setText(tr("Flat Amount"));
    chkCashDeFlat->setText(tr("Flat Amount"));
    chkP2PCardFlat->setText(tr("Flat Amount"));
    chkP2PAccFlat->setText(tr("Flat Amount"));
    chkPayFlat->setText(tr("Flat Amount"));

    chkCashAdPer->setText(tr("Percentage Amount"));
    chkCashDePer->setText(tr("Percentage Amount"));
    chkP2PCardPer->setText(tr("Percentage Amount"));
    chkP2PAccPer->setText(tr("Percentage Amount"));
    chkPayPer->setText(tr("Percentage Amount"));

    chkCashAdAmountRange->setText(tr("Amount Range"));
    chkCashDeAmountRange->setText(tr("Amount Range"));
    chkP2PCardAmountRange->setText(tr("Amount Range"));
    chkP2PAccAmountRange->setText(tr("Amount Range"));
    chkPayAmountRange->setText(tr("Amount Range"));



    //--------------------   LAYOUT ----------------------//
    int space=30;
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbCashAd);
    v1Lay->addWidget(chkCashAdSur);
    v1Lay->addWidget(lbCashAdChangeAmount);
    v1Lay->addWidget(leCashAdChangeAmount);
    v1Lay->addWidget(chkCashAdFlat);
    v1Lay->addWidget(chkCashAdPer);
    v1Lay->addWidget(chkCashAdAmountRange);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbCashDe);
    v1Lay->addWidget(chkCashDeSur);
    v1Lay->addWidget(lbCashDeChangeAmount);
    v1Lay->addWidget(leCashDeChangeAmount);
    v1Lay->addWidget(chkCashDeFlat);
    v1Lay->addWidget(chkCashDePer);
    v1Lay->addWidget(chkCashDeAmountRange);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbP2PCard);
    v1Lay->addWidget(chkP2PCardSur);
    v1Lay->addWidget(lbP2PCardChangeAmount);
    v1Lay->addWidget(leP2PCardChangeAmount);
    v1Lay->addWidget(chkP2PCardFlat);
    v1Lay->addWidget(chkP2PCardPer);
    v1Lay->addWidget(chkP2PCardAmountRange);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbP2PAcc);
    v1Lay->addWidget(chkP2PAccSur);
    v1Lay->addWidget(lbP2PAccChangeAmount);
    v1Lay->addWidget(leP2PAccChangeAmount);
    v1Lay->addWidget(chkP2PAccFlat);
    v1Lay->addWidget(chkP2PAccPer);
    v1Lay->addWidget(chkP2PAccAmountRange);
    v1Lay->addSpacing(space);

    v1Lay->addWidget(lbPay);
    v1Lay->addWidget(chkPaySur);
    v1Lay->addWidget(lbPayChangeAmount);
    v1Lay->addWidget(lePayChangeAmount);
    v1Lay->addWidget(chkPayFlat);
    v1Lay->addWidget(chkPayPer);
    v1Lay->addWidget(chkPayAmountRange);

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

}

UIConfigSurCharge::~UIConfigSurCharge()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigSurCharge::keyPressEvent(QKeyEvent *event)
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

void UIConfigSurCharge::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();
//    qDebug()<<leIndexNum->size().width();

}

void UIConfigSurCharge::mouseMoveEvent(QMouseEvent *event)
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
