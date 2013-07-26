#include "uiReportDetail.h"
#include "transData.h"
#include "xdata.h"
#include "sav.h"
#include "print.h"

#include "global.h"

extern "C"{
void Uart_Printf(char *fmt,...);
}

// type 0 full window
// type 1 intergrate in transaction
UIReportDetail::UIReportDetail(int type, QDialog *parent, Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QObject::installEventFilter(this);
    RemoveKeyEventBug();

    FLAG_NEEDVOID=false;
    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    if(type==1)
    {
        this->setGeometry(0,FRAME420_THVALUE+40,FRAME420_WIDTH,FRAME420_HEIGHT-40);
        this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT-40);
    }
    else
    {
        this->setGeometry(0,FRAME420_THVALUE,FRAME420_WIDTH,FRAME420_HEIGHT);
        this->setFixedSize(FRAME420_WIDTH,FRAME420_HEIGHT);
    }

    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",14);

    this->setStyleSheet("QDialog{border: 3px solid silver;}");

    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setText(tr("Transaction Detail"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    tbDetailList=new QTableWidget();
    tbDetailList->setFrameShape(QFrame::NoFrame);
    tbDetailList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbDetailList->setSelectionMode(QAbstractItemView::SingleSelection);
    tbDetailList->setSelectionBehavior(QAbstractItemView::SelectRows);
    tbDetailList->horizontalHeader()->setVisible(false);
    tbDetailList->horizontalHeader()->setStretchLastSection(true);
    tbDetailList->verticalHeader()->setVisible(false);
    tbDetailList->setShowGrid(false);

    tbDetailList->verticalScrollBar()->setStyleSheet(SCROLL_VERTICAL_STYLE);

    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("VOID"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);
    btnSubmit->hide();

    //    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    //    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    //    v1Lay->addItem(sp1);
    v1Lay->addWidget(tbDetailList);
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
    connect(tbDetailList,SIGNAL(viewportEntered()),this,SLOT(restartTimeOut()));
    connect(tbDetailList,SIGNAL(clicked(QModelIndex)),this,SLOT(restartTimeOut()));
    connect(tbDetailList->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(restartTimeOut()));

    connect(btnSubmit, SIGNAL(clicked()), this, SLOT(slotFunClicked()));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}


UIReportDetail::~UIReportDetail()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIReportDetail::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        this->slotFunClicked();
        break;
    default:
        event->ignore();
        break;
    }
}


void UIReportDetail::slotSetDetailList(QString transType, QString cardNo, QString amount, QString refNo, QString apprNo, QString operatorNo)
{
    QFont fontTitle("Helvetica",14,QFont::Bold);

    QTableWidgetItem *itmTransType=new QTableWidgetItem();
    itmTransType->setText("Trans Type:");
    itmTransType->setFont(fontTitle);

    QTableWidgetItem *itmCardNo=new QTableWidgetItem();
    itmCardNo->setText("Card No:");
    itmCardNo->setFont(fontTitle);
    QTableWidgetItem *itmAmount=new QTableWidgetItem();
    itmAmount->setText("Amount:");
    itmAmount->setFont(fontTitle);
    QTableWidgetItem *itmRefNo=new QTableWidgetItem();
    itmRefNo->setText("Referance Number:");
    itmRefNo->setFont(fontTitle);
    QTableWidgetItem *itmApprNo=new QTableWidgetItem();
    itmApprNo->setText("APPR Code:");
    itmApprNo->setFont(fontTitle);
    QTableWidgetItem *itmOperator=new QTableWidgetItem();
    itmOperator->setText("Operator:");
    itmOperator->setFont(fontTitle);

    QTableWidgetItem *itmTransTypeData=new QTableWidgetItem();
    itmTransTypeData->setText(transType);
    QTableWidgetItem *itmCardNoData=new QTableWidgetItem();
    itmCardNoData->setText(cardNo);
    QTableWidgetItem *itmAmountData=new QTableWidgetItem();
    itmAmountData->setText(amount);
    QTableWidgetItem *itmRefNoData=new QTableWidgetItem();
    itmRefNoData->setText(refNo);
    QTableWidgetItem *itmApprNoData=new QTableWidgetItem();
    itmApprNoData->setText(apprNo);
    QTableWidgetItem *itmOperatorData=new QTableWidgetItem();
    itmOperatorData->setText(operatorNo);


    //set table
    tbDetailList->setRowCount(12);
    tbDetailList->setColumnCount(1);
    // set item
    tbDetailList->setItem(0,0,itmTransType);
    tbDetailList->setItem(1,0,itmTransTypeData);

    tbDetailList->setItem(2,0,itmCardNo);
    tbDetailList->setItem(3,0,itmCardNoData);

    tbDetailList->setItem(4,0,itmAmount);
    tbDetailList->setItem(5,0,itmAmountData);

    tbDetailList->setItem(6,0,itmRefNo);
    tbDetailList->setItem(7,0,itmRefNoData);

    tbDetailList->setItem(8,0,itmApprNo);
    tbDetailList->setItem(9,0,itmApprNoData);

    tbDetailList->setItem(10,0,itmOperator);
    tbDetailList->setItem(11,0,itmOperatorData);
}

void UIReportDetail::slotSetFun(QString text)
{
    FLAG_NEEDVOID=true;

    btnSubmit->setText(text);
    btnSubmit->show();
}

void UIReportDetail::slotFunClicked()
{
    qDebug()<<Q_FUNC_INFO;
    emit sigFun();
}

void UIReportDetail::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIReportDetail::slotQuitMenu()
{
    UIMsg::showNoticeMsgWithAutoCloseNoBeep("TIME OUT",g_changeParam.TIMEOUT_ERRMSG);
    this->close();
}

bool UIReportDetail::eventFilter(QObject *obj, QEvent *event)
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

void UIReportDetail::restartTimeOut()
{
    qDebug()<<Q_FUNC_INFO;
    closeTimer->start(g_changeParam.TIMEOUT_UI);
}
