#include "uiInputPIN.h"

extern "C"
{
#include "key.h"
}
#include "global.h"

UIInputPIN::UIInputPIN(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    //    RemoveKeyEventBug();

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
    QFont font3("Helvetica",15);
    QFont font4("Helvetica",12);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("PIN"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    lbAmount=new QLabel();
    lbAmountNumber=new QLabel();
    lbPIN=new QLabel();
    lbPINNotice=new QLabel();
    lePIN=new QLineEdit();

    lbCardNo=new QLabel();
    lbCardPic=new QLabel();
    this->slotSetCardNo(QString::fromLocal8Bit((const char *)G_NORMALTRANS_aucSourceAcc));
    this->slotIdentityCard(QString::fromLocal8Bit((const char *)G_NORMALTRANS_aucSourceAcc));
    //    lbCardNo->setText(QString::fromLocal8Bit((const char *)G_NORMALTRANS_aucSourceAcc));

    lbCardPic->setFont(font2);
    lbCardNo->setFont(font3);
    lbCardPic->setAlignment(Qt::AlignCenter);
    lbCardNo->setAlignment(Qt::AlignCenter);

    lbAmount->setText("Amount :");
    //    lbAmountNumber->setText("Input Password :");
    lbPIN->setText("Please Enter The PIN :");

    lbAmount->setFont(font2);
    lbAmountNumber->setFont(font3);
    lbPIN->setFont(font);
    lePIN->setFont(font);
    lePIN->setStyleSheet("border: 3px solid silver;border-radius: 6px;");
    lePIN->setEchoMode(QLineEdit::Password);
    lePIN->setMinimumHeight(35);
    lePIN->setMaxLength(10);
    //    lePIN->setMaximumWidth(200);

    lbAmount->setAlignment(Qt::AlignCenter);
    lbAmountNumber->setAlignment(Qt::AlignCenter);
    lbPIN->setAlignment(Qt::AlignCenter);
    lePIN->setAlignment(Qt::AlignCenter);

    const QString  REGEX_AMOUNT = "^[0-9]\\d*$";
    QRegExp regx(REGEX_AMOUNT);
    QValidator *validator = new QRegExpValidator(regx, lePIN);
    lePIN->setValidator(validator);

    lbAmount->hide();
    lbAmountNumber->hide();

    lbPINNotice->setText("- Press [ENT] if No PIN -");
    lbPINNotice->setFont(font4);
    lbPINNotice->setAlignment(Qt::AlignCenter);
    //    ----------------------------------  //
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

    //    QHBoxLayout *h1Lay=new QHBoxLayout();
    //    h1Lay->addWidget(lbCard);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbAmount);
    v1Lay->addWidget(lbAmountNumber);
    v1Lay->addWidget(lbCardPic);
    v1Lay->addWidget(lbCardNo);
    v1Lay->addItem(sp2);
    v1Lay->addWidget(lbPIN);
    v1Lay->addWidget(lePIN);
    v1Lay->addWidget(lbPINNotice);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotQuitTrans()));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotSubmitClicked()));

    connect(lePIN,SIGNAL(textChanged(QString)),this,SLOT(slotSavePIN(QString)));
    connect(lePIN,SIGNAL(textEdited(QString)),this,SLOT(slotSavePIN(QString)));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

    FLAG_HASPIN=true;

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

UIInputPIN::~UIInputPIN()
{
    qDebug() << Q_FUNC_INFO;
}

void UIInputPIN::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        emit sigQuitTrans();
        break;
    case Qt::Key_Enter:
        slotSubmitClicked();
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIInputPIN::slotQuitTrans()
{
    emit sigQuitTrans();
}

void UIInputPIN::slotSubmitClicked()
{
    qDebug()<<Q_FUNC_INFO;
    closeTimer->stop();
    // 完成输入密码
    if(savePIN.isEmpty() || savePIN.length()==0 || FLAG_HASPIN==false)
    {
        // 没密码
        qDebug()<<"No Password";
        NormalTransData.isInputPin=false;
    }
    else
    {
        // 有密码
        qDebug()<<"Has Password";
        NormalTransData.isInputPin=true;

        unsigned char *ucPIN=NULL;
        ucPIN  = (unsigned char *)malloc(sizeof(unsigned char)*50);
        memset(ucPIN,0,50);
        memcpy(ucPIN,savePIN.toAscii().data(),savePIN.length());

        qDebug()<<"ucPIN::"<<ucPIN;
        printf("ucPIN:: %s\n",ucPIN);

        unsigned char ucResult;
        ucResult=KEY_EncryptPIN_X98(ucPIN,G_NORMALTRANS_aucCardPan_UnAssign,0,G_EXTRATRANS_aucPINData);
        if(ucResult==ERR_DRIVER)
        {
            qDebug()<<"加密密码出错";
        }
        else
        {
            printf("G_EXTRATRANS_aucPINData:: %s\n",G_EXTRATRANS_aucPINData);
        }
    }

    emit sigSubmit();
}

void UIInputPIN::slotSavePIN(const QString str)
{
    qDebug()<<Q_FUNC_INFO<<str;
    savePIN=str;
}

void UIInputPIN::slotSetAmount(const QString strAmt)
{
    QString showAmt;
    QStringList list;
    //    if(strAmt.length()==16)
    //    {
    list = strAmt.split(QRegExp("[0-9]\\d{3}(?!\\d)"),QString::SkipEmptyParts);
    qDebug()<<"list::"<<list;
    for(int i=0;i<list.size();i++)
    {
        showAmt.append(list.at(i));
        showAmt.append(" ");
    }
    qDebug()<<"showamt ::"<<showAmt;
    //    }

    lbAmountNumber->setText(strAmt);
    lbAmount->show();
    lbAmountNumber->show();
}

void UIInputPIN::slotSetCardNo(const QString str)
{
    qDebug()<<Q_FUNC_INFO<<str.length()%16;

    QString showCardNo=str;
    for(int i=0;i<str.length()/4;i++)
    {
        showCardNo.insert(4*i+i,QString(" "));
    }
    qDebug()<<showCardNo<<"\n";

    lbCardNo->setText(showCardNo);
}

void UIInputPIN::slotIdentityCard(const QString str)
{
    QString cardBIN=str.left(6);
    int intCardBin=cardBIN.toInt();

    if(intCardBin>=510000 && intCardBin<= 559999)
    {
        qDebug()<<"Master Card";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/mastercard.png"));
    }
    else if( (intCardBin>=340000 && intCardBin<= 349999) || (intCardBin>=370000 && intCardBin<= 379999) )
    {
        qDebug()<<"American Express";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/amex.png"));
    }
    else if(intCardBin>=400000 && intCardBin<= 499999)
    {
        qDebug()<<"VISA";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/visa.png"));
    }
    else if( (intCardBin>=300000 && intCardBin<= 305999) || (intCardBin>=309500 && intCardBin<= 309599)
             ||  (intCardBin>=360000 && intCardBin<= 369999) || (intCardBin>=380000 && intCardBin<= 399999) )
    {
        qDebug()<<"DinersClub";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/dinersclub.png"));
    }
    else if(intCardBin>=352800 && intCardBin<= 358999)
    {
        qDebug()<<"JCB";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/jcb.png"));
    }
    else if(intCardBin>=620000 && intCardBin<= 629999)
    {
        qDebug()<<"China UnionPay";
        lbCardPic->setPixmap(QPixmap(":/icons/CardBIN/chinaunionpay.png"));
    }
    else
    {
        qDebug()<<"Unregonized Card";
        lbCardPic->setText("Card No. :");
    }
}

void UIInputPIN::slotDisablePIN()
{
    lePIN->setMaxLength(20);
    lePIN->setEchoMode(QLineEdit::Normal);
    lePIN->setText("PIN Disabled");
    lePIN->setReadOnly(true);
    lePIN->setDisabled(true);

    btnSubmit->setFocus();
    FLAG_HASPIN=false;
}

void UIInputPIN::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitTrans()));
    closeTimer->start(timeout);
}
