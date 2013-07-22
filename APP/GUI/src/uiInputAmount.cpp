#include "uiInputAmount.h"
#include "global.h"

UIInputAmount::UIInputAmount(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    qDebug()<<Q_FUNC_INFO;
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
    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Transaction Amount"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//

    lbInput=new QLabel();
    lbPic=new QLabel();
    lbPic->setPixmap(QPixmap(":/icons/amount.png"));
    lbPic->setAlignment(Qt::AlignCenter);

    lbInput->setText("Please Enter The Amount:");
    lbInput->setFont(font);
    lbInput->setAlignment(Qt::AlignCenter);

    // input amount
    leAmount=new QLineEdit();
    leAmount->setAlignment(Qt::AlignCenter);
    leAmount->setFocus();
    leAmount->setFont(font);
    leAmount->setStyleSheet("border: 3px solid silver;border-radius: 6px;");
    leAmount->setMinimumHeight(35);
    leAmount->setMaxLength(10);

    unsigned long ulinitAmt=0;
    unsigned char aucBuf[20] = {0};
    memset(aucBuf,0,sizeof(aucBuf));
    FormAmount(aucBuf,ulinitAmt,2);
    leAmount->setText(QString::fromAscii((const char *)aucBuf));

    //    const QString  REGEX_AMOUNT = "^[-]?[\\d]{1,10}([.]{1}[\\d]{1,2})?$";
    //    QRegExp regx(REGEX_AMOUNT);
    //    QValidator *validator = new QRegExpValidator(regx, leAmount);
    //    leAmount->setValidator(validator);

    //    QSpacerItem *hSpacer;
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

    //    QSpacerItem *sp=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbPic);
    v1Lay->addWidget(lbInput);
    v1Lay->addWidget(leAmount);


    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotQuitTrans()));
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(slotAmountInputComplete()));

    // 金额输入
    connect(leAmount,SIGNAL(textChanged(QString)),this,SLOT(slotSaveAmount(QString)));
    connect(leAmount,SIGNAL(textEdited(QString)),this,SLOT(slotSaveAmount(QString)));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();
}

UIInputAmount::~UIInputAmount()
{
    qDebug() << Q_FUNC_INFO;
}

void UIInputAmount::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->slotQuitTrans();
        break;
    case Qt::Key_Enter:
        this->slotAmountInputComplete();
        break;
        //    case KEY_00_PT:
        //    {
        //        event->ignore();
        //        this->addDot();
        //        break;
        //    }
    case Qt::Key_F9:
    {
        event->ignore();
        this->addDot();
        break;
    }
    default:
        //        qDebug()<<event->key();
        event->ignore();
        break;
    }
}

void UIInputAmount::slotSaveAmount(QString str)
{
    qDebug()<<Q_FUNC_INFO<<str;

    unsigned long ulMaxAmt=g_constantParam.ulMaxAmount*100;
    leAmount->setText(QStringAmountFormular(str,ulMaxAmt));


    // 格式化金额
    AMOUNT=leAmount->text();
    QString formatAmount=AMOUNT;
    if(formatAmount.contains("."))
        formatAmount.remove('.');

    NormalTransData.ulAmount = formatAmount.toULong();
    qDebug()<<"Amount::"<<AMOUNT<< NormalTransData.ulAmount;
}

QString UIInputAmount::slotGetAmount()
{
    return AMOUNT;
}

void UIInputAmount::addDot()
{
    qDebug()<<Q_FUNC_INFO;
    AMOUNT=AMOUNT+".";
    leAmount->setText(AMOUNT);
}

void UIInputAmount::slotAmountInputComplete()
{
    if(AMOUNT!="0" && AMOUNT!="" && !AMOUNT.isEmpty())
    {
        if(AMOUNT.toDouble()<=g_constantParam.ulMaxAmount && AMOUNT.toDouble()>=g_constantParam.ulMinAmount)
            emit sigAmountInputComplete(AMOUNT);
        else
        {
            QString errMsg;
            errMsg.append("KEEP AMOUNT\nIN RANGE\n");
            errMsg.append(QString::number(g_constantParam.ulMinAmount));
            errMsg.append(" TO ");
            errMsg.append(QString::number(g_constantParam.ulMaxAmount));

            UIMsg::showNoticeMsgWithAutoClose(errMsg,g_changeParam.TIMEOUT_ERRMSG);
            return;
        }
    }
    else
        qDebug()<<"Amount Error";
}

void UIInputAmount::slotQuitTrans()
{
    qDebug()<<Q_FUNC_INFO;

    emit sigQuitTrans();
}

void UIInputAmount::FormAmount(unsigned char *pucOut,unsigned long ulAmount,unsigned char ucDecimalPos)
{
    unsigned char   i;
    unsigned char   j;
    unsigned char  aucBuf[10] = {0};
    short space = 0;

    memset(aucBuf,0,sizeof(aucBuf));
    long_str(aucBuf, 9, &ulAmount);//8
    j = ucDecimalPos;
    for (i = 8; j; i--, j--) //7
        aucBuf[i + 1] = aucBuf[i];
    if (ucDecimalPos)
    {
        aucBuf[i + 1] ='.';
        aucBuf[i + 1 + ucDecimalPos + 1] = 0;
    }
    for (j = 0; ((j < i) && (aucBuf[j]=='0')); space++,aucBuf[j++]=' ');

    memcpy(pucOut,&aucBuf[space],strlen((char *)aucBuf)-space);
}


QString UIInputAmount::QStringAmountFormular(const QString &arg1,unsigned long ulMaxAmt)
{
    unsigned char buf[20] = {0};
    unsigned char aucBuf[20] = {0};
    unsigned long TmpAmt = 0;
    memcpy(buf,arg1.toAscii().data(),arg1.toAscii().length());
    Remove(buf,'.');
    TmpAmt = strtoul((char *)buf,0,10);
    if(TmpAmt > ulMaxAmt)
    {
        // new MessageList
        TmpAmt = 0;
    }
    FormAmount(aucBuf,TmpAmt,2);
    return QString::fromAscii((const char *)aucBuf);
}


void UIInputAmount::Remove(unsigned char str[],char remove)
{
    int t=0;
    unsigned char* p=str;
    while (*p)
    {
        if (remove!=*p)
        {
            str[t]=*p;
            t++;
        }
        p++;
    }
    str[t]='\0';
}
