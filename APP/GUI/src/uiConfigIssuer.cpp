#include "uiconfigissuer.h"
#include "global.h"

uiConfigIssuer::uiConfigIssuer(QDialog *parent,Qt::WindowFlags f) :
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

    //--------------------   define ----------------------//
    lbHead=new QLabel();
    lbHead->setText(tr("Issuer Configuration"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);

    lbIndexNumber=new QLabel();
    lbIndexNumber->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    lbCardLabel=new QLabel();

    leIndexNumber=new QLineEdit();
    leCardLabel=new QLineEdit();

    chkCheckingOption=new QCheckBox();
    chkCashOutOption=new QCheckBox();
    chkDefaultAcc=new QCheckBox();
    chkFloorLimi=new QCheckBox();
    chkFloorLimiEMV=new QCheckBox();
    chkEMVPinBypass=new QCheckBox();
    chkMagPIN=new QCheckBox();

    lbIndexNumber->setText(tr("Index Number:"));
    lbCardLabel->setText(tr("Card Label:"));

    chkCheckingOption->setText(tr("Checking Option"));
    chkCashOutOption->setText(tr("Cash Out Option"));
    chkDefaultAcc->setText(tr("Default Account"));
    chkFloorLimi->setText(tr("Floor Limit"));
    chkFloorLimiEMV->setText(tr("Floor Limit EMV"));
    chkEMVPinBypass->setText(tr("EMV PIN ByPass"));
    chkMagPIN->setText(tr("Magstripe PIN"));

    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbIndexNumber);
    v1Lay->addWidget(leIndexNumber);
    v1Lay->addWidget(lbCardLabel);
    v1Lay->addWidget(leCardLabel);
    v1Lay->addWidget(chkCheckingOption);
    v1Lay->addWidget(chkCashOutOption);
    v1Lay->addWidget(chkDefaultAcc);
    v1Lay->addWidget(chkFloorLimi);
    v1Lay->addWidget(chkFloorLimiEMV);
    v1Lay->addWidget(chkEMVPinBypass);
    v1Lay->addWidget(chkMagPIN);

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
    // ------------------------------------------- //

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addWidget(scArea);
    layout->addLayout(h1Lay);
    layout->setContentsMargins(0,0,0,5);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

}

uiConfigIssuer::~uiConfigIssuer()
{
    qDebug() << Q_FUNC_INFO;
}

void uiConfigIssuer::keyPressEvent(QKeyEvent *event)
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

void uiConfigIssuer::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();
//    qDebug()<<lbHead->size().height();

}

void uiConfigIssuer::mouseMoveEvent(QMouseEvent *event)
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

