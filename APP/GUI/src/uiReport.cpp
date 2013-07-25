#include "uiReport.h"

#include "transData.h"
#include "xdata.h"
#include "global.h"

UIReport::UIReport(QDialog *parent,Qt::WindowFlags f) :
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
    lbHead->setText(tr("Report"));
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //--------------define--------------------//
    btnQueryTransDetail=new QPushButton();
    btnSummary=new QPushButton();
    btnSerachByNumber=new QPushButton();

    btnQueryTransDetail->setText("Query Transaction");
    btnSummary->setText("Summary");
    btnSerachByNumber->setText("Search Transaction");

    btnQueryTransDetail->setFont(font2);
    btnSummary->setFont(font2);
    btnSerachByNumber->setFont(font2);

    btnQueryTransDetail->setStyleSheet(BTN_MENU_STYLE);
    btnSummary->setStyleSheet(BTN_MENU_STYLE);
    btnSerachByNumber->setStyleSheet(BTN_MENU_STYLE);

    btnQueryTransDetail->setMinimumHeight(40);
    btnQueryTransDetail->setMaximumHeight(40);
    btnSummary->setMinimumHeight(40);
    btnSummary->setMaximumHeight(40);
    btnSerachByNumber->setMinimumHeight(40);
    btnSerachByNumber->setMaximumHeight(40);
    //    ----------------------------------  //
    btnCancel=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnCancel->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_MENU_CANCEL_STYLE);

    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(btnQueryTransDetail);
    v1Lay->addWidget(btnSummary);
    v1Lay->addWidget(btnSerachByNumber);
    v1Lay->addItem(sp2);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    //    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    //    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);


    connect(btnQueryTransDetail, SIGNAL(clicked()), this, SLOT(slotShowQueryTrans()));
    connect(btnSummary, SIGNAL(clicked()), this, SLOT(slotShowSummary()));
    connect(btnSerachByNumber, SIGNAL(clicked()), this, SLOT(slotShowSearchTrans()));

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    //Animation
    QPropertyAnimation *animation1 = new QPropertyAnimation(this, "pos");
    animation1->setDuration(100);
    animation1->setStartValue(mapToParent(QPoint(FRAME420_WIDTH, 0)));
    animation1->setEndValue(mapToParent(QPoint(0, 0)));
    animation1->setEasingCurve(QEasingCurve::OutQuint);
    animation1->start();

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

UIReport::~UIReport()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void UIReport::keyPressEvent(QKeyEvent *event)
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

void UIReport::slotShowQueryTrans()
{
    qDebug()<<Q_FUNC_INFO;

    if(g_transInfo.TransTotal.uiTotalNb==0)
    {
        UIMsg::showNoticeMsgWithAutoClose("No Transaction",g_changeParam.TIMEOUT_ERRMSG);
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        return;
    }
    else
    {
        uiRQ=new UIReportQuery();
        uiRQ->exec();
    }
}

void UIReport::slotShowSummary()
{
    qDebug()<<Q_FUNC_INFO;
    uiRSum=new UIReportSummary();
    uiRSum->exec();
}

void UIReport::slotShowSearchTrans()
{
    qDebug()<<Q_FUNC_INFO;
    uiRSearch=new UIReportSearch();
    uiRSearch->exec();
}

void UIReport::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitMenu()));
    closeTimer->start(timeout);
}

void UIReport::slotQuitMenu()
{
    this->close();
}

bool UIReport::eventFilter(QObject *obj, QEvent *event)
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
