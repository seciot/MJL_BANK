#include "uiLogon.h"
#include <QDebug>
#include <QThread>
#include "sand_lcd.h"
#include "xdata.h"
#include "global.h"

UILogon::UILogon(QDialog *parent,Qt::WindowFlags f) :
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
    this->setGeometry(0,FRAME420_THVALUE,FRAME420_WIDTH,FRAME420_HEIGHT);

    this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT);
    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    lbHead=new QLabel();
    lbHead->setText(tr("Logon"));
    QFont fontH("Helvetica",18,QFont::Bold);
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
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
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

    layout->setContentsMargins(0,0,0,5);

    // -------- input cashier password ------------//
//    uiInPass=new UIInputPassword();
//    connect(uiInPass,SIGNAL(sigLogInSuccess()),this,SLOT(slotStartLogon()));
//    passThread=new QThread(this);
//    connect(passThread, SIGNAL(started()), uiInPass, SLOT(exec()));
//    passThread->start();

    pUpdateTimer = new QTimer(this);

    showColon=0;
    slotStartLogon();
}

UILogon::~UILogon()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
    if(transOnlineProcess != NULL)
    {
        qDebug(" --- Before deleteLater --- !\n");
        transOnlineProcess->deleteLater();
        qDebug(" --- After deleteLater--- !\n");
    }
}

void UILogon::slotStartLogon()
{
    qDebug()<<Q_FUNC_INFO;

//    uiInPass->close();
//    uiInPass=NULL;
//    delete passThread;

    this->startLogon();
}

void UILogon::startLogon()
{
    qDebug()<<Q_FUNC_INFO;
    movie  = new QMovie(":/gif/003.gif");

    transOnlineProcess = new TransOnlinePro(TransMode_DownWK, 0);
    connect(pUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateLabelChange()));
    connect(transOnlineProcess,SIGNAL(EableNotify(bool)),this,SLOT(EableKEY_CAN(bool)));
    connect(transOnlineProcess,SIGNAL(notify(const QString,const QString)),this,SLOT(UpdateLabelText(const QString,const QString)));
    connect(transOnlineProcess,SIGNAL(sendResult(unsigned char)),this,SLOT(ReturnFromThread(unsigned char)));
    connect(transOnlineProcess,SIGNAL(destroyed()),this,SLOT(ReturnWay()));//added by pan.cy 2013/1/10 forbid it being disconnected
    transOnlineProcess->process();
    pUpdateTimer->start(400);
}

void UILogon::ReturnWay()
{
    qDebug()<<Q_FUNC_INFO;
    if (ucShortCutFlag == 2)
    {
        emit QuickQuit(0x00);
        qDebug("shortcutFlag = true  \n");
    }
    qDebug() << Q_FUNC_INFO;
    pUpdateTimer->stop();
    //xDATA::ClearGlobalData();
    //	RemoveKeyEventBug(); //xxb
    this->close();
}



void UILogon::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
//        this->close();
//        this->quitFromInputPass();
        break;
    case Qt::Key_F3:
        //        vBar->setValue(vBar->value()-150);
        break;
    case Qt::Key_F4:
        //        vBar->setValue(vBar->value()+150);
        break;
    default:
        event->ignore();
        break;
    }
}

void UILogon::quitFromInputPass()
{
    qDebug()<<Q_FUNC_INFO;

    this->close();
}

void UILogon::ParamLoadShortcut(int iType)
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
    connect(transOnlineProcess,SIGNAL(destroyed()),this,SLOT(ReturnWay()));//added by pan.cy 2013/1/10 forbid it being disconnected
    transOnlineProcess->process();
    pUpdateTimer->start(400);
}

void UILogon::ReturnFromThread(unsigned char index)
{
    qDebug() << Q_FUNC_INFO;
    disconnect(pUpdateTimer,SIGNAL(timeout()),this,SLOT(UpdateLabelChange()));
    disconnect(transOnlineProcess,SIGNAL(EableNotify(bool)),this,SLOT(EableKEY_CAN(bool)));
    disconnect(transOnlineProcess,SIGNAL(notify(const QString,const QString)),this,SLOT(UpdateLabelText(const QString,const QString)));
    disconnect(transOnlineProcess,SIGNAL(sendResult(unsigned char)),this,SLOT(ReturnFromThread(unsigned char)));

    pUpdateTimer->stop();
    if(movie!=NULL) {movie->stop();delete movie;movie = NULL;}
    Os__gif_stop();
    if(index)
    {
        // 仅供测试使用
        g_changeParam.boolCashierLogonFlag = true;
        xDATA::WriteValidFile(xDATA::DataSaveChange);

        // 签到失败
        UIMsg::showCombineErrMsgWithAutoClose(ErrIndex(index),g_changeParam.TIMEOUT_ERRMSG);
    }
    else
    {
        //签到成功
        g_changeParam.boolCashierLogonFlag = true;
        xDATA::WriteValidFile(xDATA::DataSaveChange);
        UIMsg::showNoticeMsgWithAutoClose(LOGON_SUCCESS,g_changeParam.TIMEOUT_ERRMSG);

    }

    ClearLabelInfo();

    qDebug("----- ReturnFromThread To son_PreClicked ----- \n");
    ReturnWay();
}

void UILogon::ClearLabelInfo()
{
    qDebug()<<Q_FUNC_INFO;

    qsUpdate = "";showColon=0;qsBack = "";backColon=0;
}

void UILogon::UpdateLabelText(const QString &str,const QString &backup)
{
    qDebug() << Q_FUNC_INFO<<str<<backup;
    qsUpdate = str;
    qsBack = backup;
    lbNotice->setText(qsUpdate);
    lbNotice2->setText(qsBack);
    qDebug()<<Q_FUNC_INFO<<qsUpdate;
}

void UILogon::EableKEY_CAN(bool bEnable)
{
    qDebug()<<Q_FUNC_INFO;

    if(bEnable == false)
    {
        //        qDebug("预偶  顺!\n");
        //        ui->ptnPre_4->hide();
        btnCancel->hide();
        movie->stop();
        lbGif->clear();
        //        ui->label_gif->clear();
    }
    else
    {
        //        qDebug("预偶  顺示!\n");
        //        ui->label_gif->setMovie(movie);
        //        ui->ptnPre_4->show();
        lbGif->setMovie(movie);
        movie->start();
        btnCancel->show();
        //sleep(1);//实 时芑岱⑥肥舅诚高的?
    }
}

void UILogon::UpdateLabelChange()
{
    qDebug()<<Q_FUNC_INFO<<qsUpdate;

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
    qDebug()<<temp;
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

void UILogon::slotBtnCancelclicked()
{
    qDebug() << Q_FUNC_INFO;
    if(transOnlineProcess != NULL)
    {
        //bDisable = true;
        transOnlineProcess->ExitFromExCAN(0x02);
    }
}

