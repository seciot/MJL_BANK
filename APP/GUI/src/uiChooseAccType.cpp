#include "uiChooseAccType.h"
#include "global.h"

UIChooseAccType::UIChooseAccType(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    RemoveKeyEventBug();
    FLAG_SENDER=false;
    FLAG_RECEIVER=false;

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
    QFont font3("Helvetica",8,QFont::Bold);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Account Type"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lbAccountOwner=new QLabel();
    QFont fontAO("Helvetica",16,QFont::Bold);
    lbAccountOwner->setFont(fontAO);
    lbAccountOwner->setAlignment(Qt::AlignCenter);
    lbAccountOwner->setMinimumHeight(30);
    lbAccountOwner->setMaximumHeight(30);
    lbAccountOwner->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lbAccountOwner->hide();

    //--------------define--------------------//
    btnDefault=new QPushButton();
    btnSavings=new QPushButton();
    btnCheck=new QPushButton();
    btnCredit=new QPushButton();

    btnDefault->setText("Default Account");
    btnSavings->setText("Savings Account");
    btnCheck->setText("Check Account");
    btnCredit->setText("Credit Account");

    btnDefault->setFont(font);
    btnSavings->setFont(font);
    btnCheck->setFont(font);
    btnCredit->setFont(font);

    btnDefault->setStyleSheet(BTN_MENU_STYLE);
    btnSavings->setStyleSheet(BTN_MENU_STYLE);
    btnCheck->setStyleSheet(BTN_MENU_STYLE);
    btnCredit->setStyleSheet(BTN_MENU_STYLE);

    btnDefault->setMinimumHeight(30);
    btnSavings->setMinimumHeight(30);
    btnCheck->setMinimumHeight(30);
    btnCredit->setMinimumHeight(30);
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Submit"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);


    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(btnDefault);
    v1Lay->addWidget(btnSavings);
    v1Lay->addWidget(btnCheck);
    v1Lay->addWidget(btnCredit);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addWidget(lbAccountOwner);
    layout->addLayout(v1Lay);


    connect(btnDefault,SIGNAL(clicked()),this,SLOT(btnDefaultClicked()));
    connect(btnSavings,SIGNAL(clicked()),this,SLOT(btnSavingsClicked()));
    connect(btnCheck,SIGNAL(clicked()),this,SLOT(btnCheckClicked()));
    connect(btnCredit,SIGNAL(clicked()),this,SLOT(btnCreditClicked()));

    //Animation
   QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
   animation1->setDuration(100);
   animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
   animation1->setEndValue(mapToParent(QPoint(0, 0)));
   animation1->setEasingCurve(QEasingCurve::OutQuint);
   animation1->start();
}

void UIChooseAccType::setSender()
{
    lbAccountOwner->setText("Sender");
    FLAG_SENDER=true;
    lbAccountOwner->show();
}

void UIChooseAccType::setReceiver()
{
    lbAccountOwner->setText("Receiver");
    FLAG_RECEIVER=true;
    lbAccountOwner->show();
}

UIChooseAccType::~UIChooseAccType()
{
    qDebug() << Q_FUNC_INFO;
}

void UIChooseAccType::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
//        this->close();
        break;
    case Qt::Key_F3:
//        vBar->setValue(vBar->value()-150);
        break;
    case Qt::Key_F4:
//        vBar->setValue(vBar->value()+150);
        break;
    default:
        qDebug()<<"UIChooseAccType key::"<<event->key();

//        event->ignore();
        break;
    }
}

void UIChooseAccType::setAccType(const AccType type)
{
    qDebug() << Q_FUNC_INFO<<type;
    if(FLAG_RECEIVER==true)
    {
        privateAccType=type;
        NormalTransData.toAccType = type;
        emit sigChooseReceiverTypeComplete();
    }
    else
    {
        privateAccType=type;
        NormalTransData.accType = type;
        emit sigChooseTypeComplete(type);
    }
}

AccType UIChooseAccType::getAccType()
{
    return privateAccType;
}

void UIChooseAccType::btnDefaultClicked()
{
    qDebug() << Q_FUNC_INFO;
    setAccType(defaultAcc);

}
void UIChooseAccType::btnSavingsClicked()
{
    qDebug() << Q_FUNC_INFO;
    setAccType(savingsAcc);

}
void UIChooseAccType::btnCheckClicked()
{
    qDebug() << Q_FUNC_INFO;
    setAccType(checkAcc);

}
void UIChooseAccType::btnCreditClicked()
{
    qDebug() << Q_FUNC_INFO;
    setAccType(creditAcc);

}
