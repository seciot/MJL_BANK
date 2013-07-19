#include "uiconfigcardtable.h"
#include "uiConfigTer.h"
#include "global.h"

uiConfigCardTable::uiConfigCardTable(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Card Table"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);

    //--------------------   define ----------------------//

    lbSelCardBIN=new QLabel();
    lbSelCardBIN->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);

    lbBINLowVal=new QLabel();
    lbBINHighVal=new QLabel();
    lbIssuerID=new QLabel();
    lbAcquirerID=new QLabel();

    leSelCardBIN=new QLineEdit();
    leBINLowVal=new QLineEdit();
    leBINHighVal=new QLineEdit();
    leIssuerID=new QLineEdit();
    leAcquirerID=new QLineEdit();

    lbSelCardBIN->setText(tr("Select Card BIN:"));
    lbBINLowVal->setText(tr("BIN Low value:"));
    lbBINHighVal->setText(tr("BIN High Value:"));
    lbIssuerID->setText(tr("Issuer ID:"));
    lbAcquirerID->setText(tr("Acquirer ID:"));

    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbSelCardBIN);
    v1Lay->addWidget(leSelCardBIN);
    v1Lay->addWidget(lbBINLowVal);
    v1Lay->addWidget(leBINLowVal);
    v1Lay->addWidget(lbBINHighVal);
    v1Lay->addWidget(leBINHighVal);
    v1Lay->addWidget(lbIssuerID);
    v1Lay->addWidget(leIssuerID);
    v1Lay->addWidget(lbAcquirerID);
    v1Lay->addWidget(leAcquirerID);

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

}

uiConfigCardTable::~uiConfigCardTable()
{
    qDebug() << Q_FUNC_INFO;
}

void uiConfigCardTable::keyPressEvent(QKeyEvent *event)
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

void uiConfigCardTable::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();

}

void uiConfigCardTable::mouseMoveEvent(QMouseEvent *event)
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

