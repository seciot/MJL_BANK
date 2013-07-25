#include "uiPrint.h"
#include "global.h"


UIPrint::UIPrint(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    qDebug()<<Q_FUNC_INFO;
    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,FRAME420_THVALUE+40,FRAME420_WIDTH,FRAME420_HEIGHT-40);
    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT-40);
    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    QFont font("Helvetica",12,QFont::Bold);
    QFont font4("Helvetica",12);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Printing"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbPic=new QLabel();
    lbPrintTip=new QLabel();
    lbPrintNotice=new QLabel();
    lbPic->setAlignment(Qt::AlignCenter);
    movie  = new QMovie(":/gif/printing.gif");
    lbPic->setMovie(movie);

    movie->start();

    lbPrintTip->setText("Transaction Success");
    lbPrintTip->setFont(font);
    lbPrintTip->setAlignment(Qt::AlignCenter);

    lbPrintNotice->setText("- Now Printing -");
    lbPrintNotice->setFont(font4);
    lbPrintNotice->setAlignment(Qt::AlignCenter);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbPic);
    v1Lay->addWidget(lbPrintTip);
    v1Lay->addWidget(lbPrintNotice);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);

    // ---------- print -------------------//
    threadPrint=new QThread(this);
    pPrint = new objPrint;
    connect(threadPrint, SIGNAL(started()), pPrint, SLOT(printReceipt()));
    connect(pPrint,SIGNAL(sigTear()),this,SLOT(slotTearPrint()),Qt::BlockingQueuedConnection);
    connect(pPrint,SIGNAL(sigPrintComplete()),this,SLOT(slotFinishTrans()));

    pPrint->moveToThread(threadPrint);
    threadPrint->start();
}

UIPrint::~UIPrint()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIPrint::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    switch(event->key())
    {
    case Qt::Key_Escape:
        slotQuitTrans();
        break;
    case Qt::Key_F4:
        //        vBar->setValue(vBar->value()+150);
        break;
    default:
        qDebug()<<event->key();
        //        RemoveKeyEventBug();
        //        event->ignore();
        break;
    }
}

void UIPrint::slotQuitTrans()
{
    qDebug()<<Q_FUNC_INFO;
    movie->stop();
    delete pPrint;
    threadPrint->deleteLater();
    emit sigQuitTrans();
}

void UIPrint::slotFinishTrans()
{
    qDebug()<<Q_FUNC_INFO;
    movie->stop();
    delete pPrint;
    threadPrint->deleteLater();
    emit sigFinishTrans();
}

void UIPrint::slotTearPrint()
{
    qDebug()<<Q_FUNC_INFO;
    UIMsg::showNoticeMsgWithAutoCloseNoBeep("Tear Receipt",g_changeParam.TIMEOUT_PAPERTEAR);
}
