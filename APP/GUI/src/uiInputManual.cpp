#include "uiInputManual.h"
#include "transData.h"
#include "xdata.h"
#include "global.h"

#define CARDNO_MAX_LEN 20
#define EXP_MAX_LEN 4

UIInputManual::UIInputManual(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    //    RemoveKeyEventBug();
    NormalTransData.inputMode = INPUTMODE_Manual;
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
    lbHead->setText(tr("Manually Enter"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lbOwner=new QLabel();
    QFont fontO("Helvetica",16,QFont::Bold);
    lbOwner->setText(tr("Receiver"));
    lbOwner->setFont(fontO);
    lbOwner->setAlignment(Qt::AlignCenter);
    lbOwner->setMinimumHeight(40);
    lbOwner->setMaximumHeight(40);
    //    lbOwner->setStyleSheet(HEAD_STYLE);
    lbOwner->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lbOwner->hide();

    //--------------define--------------------//
    lbCardNo=new QLabel();
    lbEXP=new QLabel();
    leCardNo=new QLineEdit();
    leEXP=new QLineEdit();

    lbCardNo->setText("Card Number :");
    lbEXP->setText("Card EXP Date :");

    lbCardNo->setFont(font);
    lbEXP->setFont(font);
    leCardNo->setFont(font);
    leEXP->setFont(font);

    leCardNo->setMaxLength(CARDNO_MAX_LEN);
    leEXP->setMaxLength(EXP_MAX_LEN);

    const QString  REGEX_AMOUNT = "^[0-9]\\d*$";
    QRegExp regx(REGEX_AMOUNT);
    QValidator *validator1 = new QRegExpValidator(regx, leCardNo);
    QValidator *validator2 = new QRegExpValidator(regx, leEXP);
    leCardNo->setValidator(validator1);
    leEXP->setValidator(validator2);

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
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbCardNo);
    v1Lay->addWidget(leCardNo);
    v1Lay->addWidget(lbEXP);
    v1Lay->addWidget(leEXP);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addWidget(lbOwner);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(slotQuitTrans()));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotInputComplete()));
    //    layout->setContentsMargins();


    setTabOrder(leCardNo,leEXP);
    setTabOrder(leEXP,btnSubmit);
    setTabOrder(btnSubmit,btnCancel);

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

}

void UIInputManual::setEnterReceiver()
{
    FLAG_RECEIVER=true;
    lbOwner->show();
    lbEXP->hide();
    leEXP->hide();
}

UIInputManual::~UIInputManual()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIInputManual::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->slotQuitTrans();
        break;
    case Qt::Key_Enter:
        focusNextChild();
        break;
    case Qt::Key_F4:
        //        vBar->setValue(vBar->value()+150);
        break;
    default:
        event->ignore();
        break;
    }
}


void UIInputManual::slotQuitTrans()
{
    emit sigQuitTrans();

}

void UIInputManual::slotInputComplete()
{
    qDebug()<<Q_FUNC_INFO;
    // 检查都有输入
    if(leCardNo->text().isEmpty() && FLAG_RECEIVER==true)
    {
        qDebug()<<"fill in the blank";
        UIMsg::showNoticeMsgWithAutoClose(INCOMPLETE_INFORMATION,g_changeParam.TIMEOUT_ERRMSG);
        return;
    }

    if((leCardNo->text().isEmpty() || leEXP->text().isEmpty()) && FLAG_RECEIVER==false)
    {
        qDebug()<<"fill in the blank";

        UIMsg::showNoticeMsgWithAutoClose(INCOMPLETE_INFORMATION,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    if(leEXP->text().length()<4 && FLAG_RECEIVER==false)
    {
        UIMsg::showNoticeMsgWithAutoClose(WRONG_EXP,g_changeParam.TIMEOUT_ERRMSG);

        return;
    }

    if(FLAG_RECEIVER==false)
    {
        // Normal
        memcpy(NormalTransData.ucExpiredDate, leEXP->text().toAscii().data(), TRANS_DATE_LEN);
        memcpy(NormalTransData.aucSourceAcc, leCardNo->text().toAscii().data(), leCardNo->text().length());
        NormalTransData.ucSourceAccLen=leCardNo->text().length();
        qDebug()<<NormalTransData.ucSourceAccLen;
        qDebug()<<NormalTransData.ucExpiredDate<<NormalTransData.aucSourceAcc;
    }
    else
    {
        //Receiver Account
        memcpy(NormalTransData.aucToAcc, leCardNo->text().toAscii().data(), leCardNo->text().length());
        NormalTransData.ucToAccLen=leCardNo->text().length();
        qDebug()<<NormalTransData.ucToAccLen;
        qDebug()<<NormalTransData.aucToAcc;
    }

    if(g_changeParam.simCard==true)
    {
        //! --------------------test card set 1-----------------------//
        //    NormalTransData.ucSourceAccLen = sprintf((char*)NormalTransData.aucSourceAcc,
        //                                             "%s",(char *)"706001000052170512");
        //    memcpy(NormalTransData.ucExpiredDate, (unsigned char *)"1711", TRANS_DATE_LEN);

        //! --------------------test card set 2-----------------------//
        // send
        NormalTransData.ucSourceAccLen = sprintf((char*)NormalTransData.aucSourceAcc,
                                                 "%s",(char *)"706001000052170512");
        memcpy(NormalTransData.ucExpiredDate, (unsigned char *)"1711", TRANS_DATE_LEN);

        //receive
        NormalTransData.ucToAccLen = sprintf((char*)NormalTransData.aucToAcc,
                                             "%s",(char *)"706001000004340825");

        if(NormalTransData.ucSourceAccLen >= 13)
            asc_bcd(G_NORMALTRANS_aucCardPan_UnAssign,6,&NormalTransData.aucSourceAcc[NormalTransData.ucSourceAccLen - 13], 12);
        else
            asc_bcd(G_NORMALTRANS_aucCardPan_UnAssign,6,NormalTransData.aucSourceAcc,12);
    }
    emit sigInputComplete();
}


