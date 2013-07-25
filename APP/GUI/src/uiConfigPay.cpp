#include "uiConfigPay.h"
#include "global.h"

UIConfigPay::UIConfigPay(QDialog *parent,Qt::WindowFlags f) :
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
    QFont fontH("Helvetica",16,QFont::Bold);

    //--------------------   HEAD ----------------------//
    lbHead=new QLabel();
    lbHead->setText(tr("Payment Configuration"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);

    lbSeHead=new QLabel();
    lbSeHead->setText(tr("Input Fields Configuration"));
    lbSeHead->setFont(fontH);
    lbSeHead->setAlignment(Qt::AlignCenter);
    lbSeHead->setMinimumHeight(40);
    lbSeHead->setStyleSheet(HEAD_STYLE);

    //--------------------   DEFINE ----------------------//
    lbVendorID=new QLabel();
    lbVendorID->setMinimumWidth(FRAME420_WIDTH-RIGHT_WHITE);
    lbVendorName=new QLabel();
    lbVendorAccNo=new QLabel();
    lbText1=new QLabel();
    lbText2=new QLabel();
    lbText3=new QLabel();
    lbText4=new QLabel();
    lbText5=new QLabel();

    leVendorID=new QLineEdit();
    leVendorName=new QLineEdit();
    leVendorAccNo=new QLineEdit();
    leText1=new QLineEdit();
    leText2=new QLineEdit();
    leText3=new QLineEdit();
    leText4=new QLineEdit();
    leText5=new QLineEdit();

    chkText1=new QCheckBox();
    chkText2=new QCheckBox();
    chkText3=new QCheckBox();
    chkText4=new QCheckBox();
    chkText5=new QCheckBox();

    //--------------------   TEXT ----------------------//
    lbVendorID->setText(tr("Vendor ID:"));
    lbVendorName->setText(tr("Vendor Name:"));
    lbVendorAccNo->setText(tr("Vendor Account No:"));
    lbText1->setText(tr("Text Label Input Field #1:"));
    lbText2->setText(tr("Text Label Input Field #2:"));
    lbText3->setText(tr("Text Label Input Field #3:"));
    lbText4->setText(tr("Text Label Input Field #4:"));
    lbText5->setText(tr("Text Label Input Field #5:"));
    chkText1->setText(tr("Display Input Field #1:"));
    chkText2->setText(tr("Display Input Field #2:"));
    chkText3->setText(tr("Display Input Field #3:"));
    chkText4->setText(tr("Display Input Field #4:"));
    chkText5->setText(tr("Display Input Field #5:"));

    //--------------------   LAYOUT ----------------------//
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbVendorID);
    v1Lay->addWidget(leVendorID);
    v1Lay->addWidget(lbVendorName);
    v1Lay->addWidget(leVendorName);
    v1Lay->addWidget(lbVendorAccNo);
    v1Lay->addWidget(leVendorAccNo);
    v1Lay->addWidget(chkText1);
    v1Lay->addWidget(lbText1);
    v1Lay->addWidget(leText1);
    v1Lay->addWidget(chkText2);
    v1Lay->addWidget(lbText2);
    v1Lay->addWidget(leText2);
    v1Lay->addWidget(chkText3);
    v1Lay->addWidget(lbText3);
    v1Lay->addWidget(leText3);
    v1Lay->addWidget(chkText4);
    v1Lay->addWidget(lbText4);
    v1Lay->addWidget(leText4);
    v1Lay->addWidget(chkText5);
    v1Lay->addWidget(lbText5);
    v1Lay->addWidget(leText5);


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

    this->setAutoClose(g_changeParam.TIMEOUT_UI);

}

UIConfigPay::~UIConfigPay()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigPay::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        break;
    case Qt::Key_F3:
        vBar->setValue(vBar->value()-150);
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    case Qt::Key_F4:
        vBar->setValue(vBar->value()+150);
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIConfigPay::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mousePressEvent"<<event->pos().x()<<event->pos().y();
    saveVValue=event->pos().y();

}

void UIConfigPay::mouseMoveEvent(QMouseEvent *event)
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
void UIConfigPay::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitCfg()));
    closeTimer->start(timeout);
}

void UIConfigPay::slotQuitCfg()
{
    UIMsg::showNoticeMsgWithAutoClose("TIME OUT",g_changeParam.TIMEOUT_ERRMSG);
    this->close();
}

