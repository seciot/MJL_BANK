#include "uiReportQuery.h"
#include "uiReportDetail.h"
#include "transData.h"
#include "xdata.h"
#include "sav.h"
#include "commontools.h"

#include "global.h"

extern "C"{
void Uart_Printf(char *fmt,...);
}

UIReportQuery::UIReportQuery(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QObject::installEventFilter(this);
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
    QFont font3("Helvetica",14);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Transaction List"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    tbTransList=new QTableWidget();
    tbTransList->setFrameShape(QFrame::NoFrame);
    tbTransList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbTransList->setSelectionMode(QAbstractItemView::SingleSelection);
    tbTransList->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbTransList->horizontalHeader()->setVisible(false);
    tbTransList->horizontalHeader()->setDefaultSectionSize(130);
    tbTransList->horizontalHeader()->setStretchLastSection(true);
    tbTransList->verticalHeader()->setVisible(false);
    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Detail"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);

//    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
//    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    //    v1Lay->addItem(sp1);
    v1Lay->addWidget(tbTransList);
    //    v1Lay->addItem(sp2);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotTransClicked()));
    connect(tbTransList,SIGNAL(viewportEntered()),this,SLOT(restartTimeOut()));
    connect(tbTransList,SIGNAL(clicked(QModelIndex)),this,SLOT(restartTimeOut()));
    connect(tbTransList->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(restartTimeOut()));
    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

    this->ergodicTrans();  //遍历交易
    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

void UIReportQuery::ergodicTrans()
{
    qDebug()<<Q_FUNC_INFO;

    QString transType;
    QString traceNumber;
    QString apprCode;
    QString transTime;
    QString transDate;

    for(int index = 0; index < ((int)g_constantParam.uiMaxTotalNb); index++)
    {
        qDebug()<<"index:: "<<index;
        if(g_transInfo.auiTransIndex[index])
        {
            //:- 读取数据保存到NormalTransData
            memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
            xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, index);
            memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));

            //:- 开始监控
            Uart_Printf((char *)"\n->交易[%d]: \n", index);
            Uart_Printf((char *)"TransType: [%02x]\n", NormalTransData.transType);
            Uart_Printf((char *)"Amount: [%lu]\n", NormalTransData.ulAmount);
            Uart_Printf((char *)"TraceNumber: [%lu]\n", NormalTransData.ulTraceNumber);

            unsigned char aucBuf[40];
            // Time
            memset(aucBuf, 0, sizeof(aucBuf));
            getFormTime(NormalTransData.aucTime, aucBuf);
            Uart_Printf((char *)"Time: [%s]\n", aucBuf);
            transTime=QString::fromAscii((const char *)aucBuf);

            //Date
            memset(aucBuf, 0, sizeof(aucBuf));
            getFormDate(NormalTransData.aucDate, aucBuf);
            Uart_Printf((char *)"Date: [%s]\n", aucBuf);
            transDate=QString::fromAscii((const char *)aucBuf);

            switch(NormalTransData.transType)
            {
            case TransMode_CashDeposit:      //存钱
                transType="Cash Deposit";
                break;
            case TransMode_CashAdvance:      //取钱
                transType="Cash Advance";
                break;
            case TransMode_AdvanceVoid:         //撤销
                transType="Cash Advance VOID";
                break;
            case TransMode_DepositVoid:         //撤销
                transType="Cash Deposit VOID";
                break;
            case TransMode_BalanceInquiry:   //查余
                transType="Balance Inquiry";
                break;
            case TransMode_CardTransfer:     //转账
                transType="P2P Transfer";
                break;
            default:
                qDebug()<<"This should not be entered";
                break;
            }

            traceNumber = QString::number(NormalTransData.ulTraceNumber);

            apprCode=QString::fromAscii((const char*)NormalTransData.aucAuthCode);

            // 生成列表
            qDebug()<<transType<<apprCode<<traceNumber<<transDate<<transTime;
            QFont fontL("Helvetica",12,QFont::Bold);

            QLabel *label=new QLabel();
            label->setFont(fontL);
            label->setMinimumHeight(30);
            label->setText(traceNumber+"   "+apprCode+"    "+transType+"\n"+transDate+"    "+transTime);
            qDebug()<<"label text:"<<label->text();
            listVector.append(label);

            //! clear
            //            delete wr;
            transDate.clear();
            transTime.clear();
            transType.clear();
            traceNumber.clear();
        }
        else
        {
            qDebug()<<"end";
            break;
        }
    }

    // 添加到table中

    qDebug()<<listVector.size();
    tbTransList->setRowCount(listVector.size());
    tbTransList->setColumnCount(1);
    for(int i=0;i<listVector.size();i++)
    {
        qDebug()<<"Adding list::"<<i;
        tbTransList->setCellWidget(i,0,listVector.at(i));
    }

    update();
}


UIReportQuery::~UIReportQuery()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIReportQuery::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIReportQuery::slotTransClicked()
{
    qDebug()<<Q_FUNC_INFO<<tbTransList->currentRow();

    QString transType;
    QString cardNo;
    QString amount;
    QString refNo;
    QString apprNo;
    QString operatorNo;

    int index=tbTransList->currentRow();
    if(g_transInfo.auiTransIndex[index])
    {
        //:- 读取数据保存到NormalTransData
        memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
        int ucResult=xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, index);
        if(ucResult!=0)
        {
            UIMsg::showFileErrMsgWithAutoClose((FileErrIndex)ucResult,g_changeParam.TIMEOUT_ERRMSG);

            return;
        }
        memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));

        qDebug()<<"step1";
        switch(NormalTransData.transType)
        {
        case TransMode_CashDeposit:      //存钱
            transType="Cash Deposit";
            break;
        case TransMode_CashAdvance:      //取钱
            transType="Cash Advance";
            break;
        case TransMode_AdvanceVoid:         //撤销
            transType="Cash Advance VOID";
            break;
        case TransMode_DepositVoid:         //撤销
            transType="Cash Deposit VOID";
            break;
        case TransMode_BalanceInquiry:   //查余
            transType="Balance Inquiry";
            break;
        case TransMode_CardTransfer:     //转账
            transType="P2P Transfer";
            break;
        default:
            qDebug()<<"This should not be entered";
            break;
        }
        qDebug()<<"step2"<<transType;

        //Card No
        cardNo=QString::fromAscii((const char *)NormalTransData.aucSourceAcc);  // 需要部分隐藏


        // Amount
        unsigned char aucBuf[12];
        getAmount(aucBuf, NormalTransData.ulAmount, 2);
        amount=QString::fromAscii((const char *)aucBuf);

        // ref & appr
        refNo=QString::fromAscii((const char *)NormalTransData.aucRefNum);
        apprNo=QString::fromAscii((const char *)NormalTransData.aucAuthCode);

        //operator
        operatorNo=QString::fromAscii((const char *)NormalTransData.aucCashier);


        // Display the detail
        UIReportDetail *uiRepDetail=new UIReportDetail();
        uiRepDetail->slotSetDetailList(transType,cardNo,amount,refNo,apprNo,operatorNo);
        uiRepDetail->exec();

    }
}

void UIReportQuery::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIReportQuery::slotQuitMenu()
{
    this->close();
}

bool UIReportQuery::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==this)
    {
        if(event->type()==QEvent::WindowActivate)
        {
            qDebug() << Q_FUNC_INFO<<"Start Timer";
            closeTimer->start(g_changeParam.TIMEOUT_UI);
        }
        else if(event->type()==QEvent::WindowDeactivate)
        {
            qDebug() << Q_FUNC_INFO<<"Stop Timer";
            closeTimer->stop();
        }
    }
    return QDialog::eventFilter(obj,event);
}

void UIReportQuery::restartTimeOut()
{
    qDebug()<<Q_FUNC_INFO;
    closeTimer->start(g_changeParam.TIMEOUT_UI);
}
