#include "uiSwipeCard.h"
#include "global.h"


UISwipeCard::UISwipeCard(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Swipe Card"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbPicTmp=new QLabel();
    lbPic = NULL;
    lbPic=new AnimationLabel(lbPicTmp);
    lbSwipeTip=new QLabel();
    lbSwipeNotice=new QLabel();
    lbManualNotice=new QLabel();

    lbPic->setPixmap(QPixmap(":/icons/swipe.png"));
    lbPic->addPixmap(QPixmap(":/icons/insert.png"));
    lbPic->addPixmap(QPixmap(":/icons/putcard.png"));
    lbPic->addPixmap(QPixmap(":/icons/swipe.png"));
    lbPic->setMinimumSize(120,120);
    lbPic->setAlignment(Qt::AlignCenter);
    lbPic->start();

    lbSwipeTip->setText("Please Swipe Your Card");
    lbSwipeTip->setFont(font);
    lbSwipeTip->setAlignment(Qt::AlignCenter);

    lbSwipeNotice->setText("- Press [CAN] to Cancle -");
    lbSwipeNotice->setFont(font4);
    lbSwipeNotice->setAlignment(Qt::AlignCenter);

    lbManualNotice->setText("- Press [ENT] for Manual Input -");
    lbManualNotice->setFont(font4);
    lbManualNotice->setAlignment(Qt::AlignCenter);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbPic);
    v1Lay->addWidget(lbSwipeTip);
    v1Lay->addWidget(lbSwipeNotice);
    v1Lay->addWidget(lbManualNotice);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

    // ---------- swipe card -------------------//
    threadSwipe=new QThread(this);
    pCard = new objPutCard;
    connect(threadSwipe, SIGNAL(started()), pCard, SLOT(run()));
    connect(pCard,SIGNAL(putCardFinished()),this,SLOT(slotPutCardFinish()));
    connect(pCard,SIGNAL(putCardFailed()),this,SLOT(slotQuitTrans()));
    connect(pCard,SIGNAL(sigManualMode()),this,SLOT(slotSwitchToManual()));

    pCard->moveToThread(threadSwipe);
    threadSwipe->start();
}

void UISwipeCard::setNoManual()
{
    // 没有手动输入
    disconnect(pCard,SIGNAL(sigManualMode()),this,SLOT(slotSwitchToManual()));
    lbManualNotice->hide();
}

UISwipeCard::~UISwipeCard()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UISwipeCard::keyPressEvent(QKeyEvent *event)
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

void UISwipeCard::slotQuitTrans()
{
    qDebug()<<Q_FUNC_INFO;
    lbPic->stop();
    delete pCard;
    threadSwipe->quit();

    emit sigQuitTrans();
}

void UISwipeCard::slotPutCardFinish()
{
    int ei=this->checkCard();
    switch (ei)
    {
    case ERR_CARDSUPPORT:
    {
        lbPic->stop();
        delete pCard;
        threadSwipe->quit();

        emit sigFinishPutNotSupportCard();
        break;
    }
    case SUCCESS_TRACKDATA:
    {
        lbPic->stop();
        delete pCard;
        threadSwipe->quit();
        emit sigFinishPutCard();
        break;
    }
    default:
        break;
    }
}

int UISwipeCard::checkCard()
{
    qDebug()<<Q_FUNC_INFO;
    if(G_EXTRATRANS_uiISO2Len<10)
    {
        qDebug()<<"Not Supported Card";
        return ERR_CARDSUPPORT;
    }
    else
        return SUCCESS_TRACKDATA;
}

void UISwipeCard::slotSwitchToManual()
{
    qDebug()<<Q_FUNC_INFO;

    lbPic->stop();
    delete pCard;
    threadSwipe->quit();

    emit sigSwitchToManual();
}
