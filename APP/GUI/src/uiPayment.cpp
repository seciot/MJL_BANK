#include "uiPayment.h"
#include "global.h"


UIPayment::UIPayment(QDialog *parent,Qt::WindowFlags f) :
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
    QFont font3("Helvetica",8,QFont::Bold);

    lbHead=new QLabel();
    lbHead->setText(tr("Payment"));
    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);

    lbHeadText=new QLabel();
    lbHeadText->setText(tr("Swipe Your Card"));
    lbHeadText->setFont(font3);
    lbHeadText->setMaximumHeight(15);
    lbHeadText->setAlignment(Qt::AlignHCenter);

    //--------------------   define ----------------------//
    lbSelVendor=new QLabel();
    lbSelVendor->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);
    lbPayCardNum=new QLabel();
    lbCHName=new QLabel();
    lbCardExp=new QLabel();
    lbPayerPIN=new QLabel();
    lbInsuranceNum=new QLabel();
    lbInstallNum=new QLabel();
    lbInstallAmount=new QLabel();
    lbMobileNum=new QLabel();

    cmbSelVendor=new QComboBox();
    lePayCardNum=new QLineEdit();
    leCHName=new QLineEdit();
    leCardExp=new QLineEdit();
    lePayerPIN=new QLineEdit();
    leInsuranceNum=new QLineEdit();
    leInstallNum=new QLineEdit();
    leInstallAmount=new QLineEdit();
    leMobileNum=new QLineEdit();

    lbSelVendor->setText(tr("Select Vendor:"));
    lbPayCardNum->setText(tr("Payer Card No:"));
    lbCHName->setText(tr("C/H Name:"));
    lbCardExp->setText(tr("Card Exp Date:"));
    lbPayerPIN->setText(tr("Payer Card PIN:"));
    lbInsuranceNum->setText(tr("Insurance Policy No:"));
    lbInstallNum->setText(tr("Installment No:"));
    lbInstallAmount->setText(tr("Installment Amount:"));
    lbMobileNum->setText(tr("Mobile No:"));

    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbSelVendor);
    v1Lay->addWidget(cmbSelVendor);
    v1Lay->addWidget(lbPayCardNum);
    v1Lay->addWidget(lePayCardNum);
    v1Lay->addWidget(lbCHName);
    v1Lay->addWidget(leCHName);
    v1Lay->addWidget(lbCardExp);
    v1Lay->addWidget(leCardExp);
    v1Lay->addWidget(lbPayerPIN);
    v1Lay->addWidget(lePayerPIN);
    v1Lay->addWidget(lbInsuranceNum);
    v1Lay->addWidget(leInsuranceNum);
    v1Lay->addWidget(lbInstallNum);
    v1Lay->addWidget(leInstallNum);
    v1Lay->addWidget(lbInstallAmount);
    v1Lay->addWidget(leInstallAmount);
    v1Lay->addWidget(lbMobileNum);
    v1Lay->addWidget(leMobileNum);


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
    layout->addWidget(lbHeadText);
    layout->addWidget(scArea);
    layout->addLayout(h1Lay);
    layout->setContentsMargins(0,0,0,5);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

}

UIPayment::~UIPayment()
{
    qDebug() << Q_FUNC_INFO;
}

void UIPayment::keyPressEvent(QKeyEvent *event)
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

void UIPayment::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();

}

void UIPayment::mouseMoveEvent(QMouseEvent *event)
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

//void UIConfigTer::mouseReleaseEvent(QMouseEvent *event)
//{
    //    qDebug()<<"mouseReleaseEvent"<<event->pos().x()<<event->pos().y();
    //    QTimer *timer = new QTimer(this);
    //       connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
    //       timer->start();
    //    if(saveVValue-event->pos().y() > 3)
    //    {
    //        vBar->setValue(vBar->value()-10);
    //        saveVValue=event->pos().y();
    //    }
    //    if(saveVValue-event->pos().y() < -3)
    //    {
    //        vBar->setValue(vBar->value()+10);
    //        saveVValue=event->pos().y();
    //    }

//}
