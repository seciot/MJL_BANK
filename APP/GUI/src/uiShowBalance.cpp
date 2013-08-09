#include "uiShowBalance.h"
#include "transData.h"
#include "xdata.h"
#include "global.h"

UIShowBalance::UIShowBalance(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    RemoveKeyEventBug();

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(0,FRAME420_THVALUE+40,FRAME420_WIDTH,FRAME420_HEIGHT-40);
    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT-40);
    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",14);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Balance"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbCardNo=new QLabel();
    lbCardNoLine=new QLabel();
    lbBalance=new QLabel();
    lbBalanceLine=new QLabel();

    lbCardNo->setText("Card No. :");
    lbBalance->setText("Balance :");

    lbCardNo->setFont(font2);
    lbBalance->setFont(font2);
    lbCardNoLine->setFont(font3);
    lbBalanceLine->setFont(font3);

    lbCardNo->setAlignment(Qt::AlignCenter);
    lbBalance->setAlignment(Qt::AlignCenter);
    lbCardNoLine->setAlignment(Qt::AlignCenter);
    lbBalanceLine->setAlignment(Qt::AlignCenter);

    //    ----------------------------------  //
    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("OK"));
    btnSubmit->setFont(font2);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_GREEN_STYLE);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp3=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbCardNo);
    v1Lay->addWidget(lbCardNoLine);
    v1Lay->addItem(sp2);
    v1Lay->addWidget(lbBalance);
    v1Lay->addWidget(lbBalanceLine);
    v1Lay->addItem(sp3);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotFinishFlow()));

    this->setCardNo();
    this->setBalance();
}

UIShowBalance::~UIShowBalance()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIShowBalance::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->slotFinishFlow();
        break;
    case Qt::Key_Enter:
        this->slotFinishFlow();
        break;
    default:
        event->ignore();
        break;
    }
}

void UIShowBalance::slotFinishFlow()
{
    qDebug()<<Q_FUNC_INFO;
    emit sigFinishFlow();
}

void UIShowBalance::setCardNo()
{
    qDebug()<<Q_FUNC_INFO;

    QString showCardNo=QString::fromLocal8Bit((const char *)G_NORMALTRANS_aucSourceAcc);
//    qDebug()<<showCardNo<<"\n";
    for(int i=0;i<4;i++)
    {
        showCardNo.insert(4*i+i,QString(" "));
    }
    qDebug()<<showCardNo<<"\n";

    lbCardNoLine->setText(showCardNo);
}


void UIShowBalance::setBalance()
{
    qDebug()<<Q_FUNC_INFO;
    QString balance=QString::number(NormalTransData.ulBalance);
    qDebug()<<balance;
    balance.insert(balance.lastIndexOf(balance.right(2)),".");
    qDebug()<<balance;
    lbBalanceLine->setText(balance);
}
