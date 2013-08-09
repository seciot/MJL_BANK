#include "uiTransOnline.h"
#include "transData.h"
#include "xdata.h"
#include "global.h"

UITransOnline::UITransOnline(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
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
    QFont fontH("Helvetica",14,QFont::Bold);
    lbHead->setText(tr("Processing"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // -----------define-----------//
    lbGif=new QLabel();
    lbNotice=new QLabel();
    lbNotice2=new QLabel();
    btnCancel=new QPushButton();

    lbGif->setAlignment(Qt::AlignCenter);
    lbNotice->setAlignment(Qt::AlignCenter);
    lbNotice2->setAlignment(Qt::AlignCenter);

    lbGif->setFont(font2);
    lbNotice->setFont(font2);
    lbNotice2->setFont(font2);
    btnCancel->setFont(font2);

    btnCancel->setText("CANCEL");
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_BLUE_STYLE);
    btnCancel->hide();
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slotBtnCancelclicked()));

    QSpacerItem *sItem=new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbHead);
    v1Lay->addWidget(lbGif);
    v1Lay->addItem(sItem);
    v1Lay->addWidget(lbNotice);
    v1Lay->addWidget(lbNotice2);
    v1Lay->addWidget(btnCancel);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);


    pUpdateTimer = new QTimer(this);
    showColon=0;
}


UITransOnline::~UITransOnline()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
    if(transOnlineProcess != NULL)
    {
        transOnlineProcess->wait();
        qDebug(" --- Before deleteLater --- !\n");
        transOnlineProcess->deleteLater();
        qDebug(" --- After deleteLater--- !\n");
    }
    //    delete this;
}

void UITransOnline::slotStartTrans(TransMode transType)
{
    qDebug()<<Q_FUNC_INFO;
    this->startTrans(transType);
}

void UITransOnline::startTrans(TransMode transType)
{
    qDebug()<<Q_FUNC_INFO;
    movie  = new QMovie(":/gif/003.gif");

    transOnlineProcess = new TransOnlinePro(transType, 0);
    connect(pUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateLabelChange()));
    connect(transOnlineProcess,SIGNAL(EableNotify(bool)),this,SLOT(EableKEY_CAN(bool)));
    connect(transOnlineProcess,SIGNAL(notify(const QString,const QString)),this,SLOT(UpdateLabelText(const QString,const QString)));
    connect(transOnlineProcess,SIGNAL(sendResult(unsigned char)),this,SLOT(ReturnFromThread(unsigned char)));
    transOnlineProcess->process();
    pUpdateTimer->start(400);
}



void UITransOnline::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    default:
        event->ignore();
        break;
    }
}

void UITransOnline::ParamLoadShortcut(int iType)
{
    qDebug()<<Q_FUNC_INFO;

    if (iType == TransMode_DownWK)
        ucShortCutFlag = 2;
    this->show();
    //    ui->ptnPre_4->hide();
    //    ui->progressBar_2->hide();
    movie  = new QMovie(":/gif/003.gif");
    transOnlineProcess = new TransOnlinePro(iType, 0);
    connect(pUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateLabelChange()));
    connect(transOnlineProcess,SIGNAL(EableNotify(bool)),this,SLOT(EableKEY_CAN(bool)));
    connect(transOnlineProcess,SIGNAL(notify(const QString,const QString)),this,SLOT(UpdateLabelText(const QString,const QString)));
    connect(transOnlineProcess,SIGNAL(sendResult(unsigned char)),this,SLOT(ReturnFromThread(unsigned char)));
    transOnlineProcess->process();
    pUpdateTimer->start(400);
}

void UITransOnline::ReturnFromThread(unsigned char index)
{
    qDebug() << Q_FUNC_INFO;
    disconnect(pUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateLabelChange()));
    disconnect(transOnlineProcess,SIGNAL(EableNotify(bool)),this,SLOT(EableKEY_CAN(bool)));
    disconnect(transOnlineProcess,SIGNAL(notify(const QString,const QString)),this,SLOT(UpdateLabelText(const QString,const QString)));
    disconnect(transOnlineProcess,SIGNAL(sendResult(unsigned char)),this,SLOT(ReturnFromThread(unsigned char)));

    pUpdateTimer->stop();
    if(movie!=NULL) {movie->stop();delete movie;movie = NULL;}
    Os__gif_stop();
    if(transOnlineProcess != NULL)
    {
        transOnlineProcess->wait();
        qDebug(" --- Before deleteLater --- !\n");
        transOnlineProcess->deleteLater();
        qDebug(" --- After deleteLater--- !\n");
    }
    ClearLabelInfo();

    if(index)
    {
        qDebug()<<"交易失败";
        UIMsg::showCombineErrMsgWithAutoClose(ErrIndex(index),g_constantParam.TIMEOUT_ERRMSG);

        emit sigQuitTrans();
    }
    else
    {
        qDebug()<<"交易成功";
        emit sigTransSuccess();
    }
}

void UITransOnline::ClearLabelInfo()
{
    qDebug()<<Q_FUNC_INFO;

    qsUpdate = "";showColon=0;qsBack = "";backColon=0;
}

void UITransOnline::UpdateLabelText(const QString &str,const QString &backup)
{
    qDebug() << Q_FUNC_INFO;
    qDebug("UPDATE-STR = %s\n",qPrintable(str));qDebug("BACKUP-STR = %s\n",qPrintable(backup));
    qsUpdate = str;qsBack = backup;
    lbNotice->setText(qsUpdate);
    lbNotice2->setText(qsBack);
}

void UITransOnline::UpdateProcessBar(bool bOpen,int value,int iMin,int iMax,QString format)
{
    qDebug()<<Q_FUNC_INFO;

    bOpen = true; value = 1; iMin = 1; iMax = 1; format = " ";
}

void UITransOnline::ExeAlterMsgBox(const QString &str)
{
    qDebug() << Q_FUNC_INFO;
    QString str2;
    str2 = str;
}

void UITransOnline::EableKEY_CAN(bool bEnable)
{
    qDebug()<<Q_FUNC_INFO;

    if(bEnable == false)
    {
        btnCancel->hide();
        movie->stop();
        lbGif->clear();
    }
    else
    {
        lbGif->setMovie(movie);
        movie->start();
        btnCancel->show();
    }
}

void UITransOnline::UpdateLabelChange()
{
    qDebug()<<Q_FUNC_INFO;

    QString temp = "";
    if(qsUpdate.length())
    {
        switch(showColon)
        {
        case 0:
            temp = qsUpdate +" . . .";
            showColon = 1;
            break;
        case 1:
            temp = qsUpdate +" . .";
            showColon = 2;
            break;
        case 2:
            temp = qsUpdate +" .";
            showColon = 3;
            break;
        case 3:
            temp = qsUpdate;
            showColon = 0;
            break;
        default:
            break;
        }
    }
    lbNotice->clear();
    lbNotice->setText(temp);
    temp = "";
    if(qsBack.length())
    {
        switch(backColon)
        {
        case 0:
            temp = qsBack +" . . .";
            backColon = 1;
            break;
        case 1:
            temp = qsBack +" . .";
            backColon = 2;
            break;
        case 2:
            temp = qsBack +" .";
            backColon = 3;
            break;
        case 3:
            temp = qsBack;
            backColon = 0;
            break;
        default:
            break;
        }
    }
    lbNotice2->clear();
    lbNotice2->setText(temp);

}

void UITransOnline::slotBtnCancelclicked()
{
    qDebug() << Q_FUNC_INFO;
    if(transOnlineProcess != NULL)
    {
        //bDisable = true;
        transOnlineProcess->ExitFromExCAN(0x02);
    }
}

