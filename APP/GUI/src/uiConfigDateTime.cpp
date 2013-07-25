#include "uiConfigDateTime.h"
#include "global.h"

UIConfigDateTime::UIConfigDateTime(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
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
    this->setFont(font);
    QFont font2("Helvetica",14,QFont::Bold);

    QFont fontH("Helvetica",16,QFont::Bold);
    lbHead=new QLabel();
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setStyleSheet(HEAD_STYLE);
    lbHead->setMaximumHeight(40);
    lbHead->setText(tr("Set Date & Time"));

    lbDate=new QLabel();
    lbTime=new QLabel();
    dateEdit=new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    timeEdit=new QTimeEdit(QTime::currentTime());

    lbDate->setText(tr("Set Date:"));
    lbTime->setText(tr("Set Time:"));

    btnCancel=new QPushButton;
    btnSubmit=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnSubmit->setText(tr("Submit"));
    btnCancel->setFont(font2);
    btnSubmit->setFont(font2);
    btnCancel->setMinimumHeight(30);
    btnSubmit->setMinimumHeight(30);
    btnCancel->setStyleSheet("color: rgb(255, 255, 255);	background-color: rgb(0, 153, 255);border-radius: 6px;");
    btnSubmit->setStyleSheet("color: rgb(0, 0, 0);	background-color: rgb(0, 255, 0);border-radius: 6px;");

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addWidget(lbDate);
    v1Lay->addWidget(dateEdit);
    v1Lay->addWidget(lbTime);
    v1Lay->addWidget(timeEdit);

    QHBoxLayout *hSpaceLay=new QHBoxLayout();
    hSpaceLay->addSpacing(10);
    hSpaceLay->addLayout(v1Lay);
    hSpaceLay->addSpacing(10);

    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(10);
    h1Lay->addWidget(btnCancel);
    h1Lay->addWidget(btnSubmit);
    h1Lay->addSpacing(10);

    QSpacerItem *sp=new QSpacerItem(0,10,QSizePolicy::Minimum,QSizePolicy::Expanding);
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(lbHead);
    layout->addLayout(hSpaceLay);
    layout->addItem(sp);
    layout->addLayout(h1Lay);

    layout->setContentsMargins(0,0,0,5);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(setDateTime()));

    this->setAutoClose(g_changeParam.TIMEOUT_UI);

}

UIConfigDateTime::~UIConfigDateTime()
{
    qDebug() << Q_FUNC_INFO;
}

void UIConfigDateTime::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_Enter:
        break;
    case Qt::Key_F3:
        break;
    case Qt::Key_F4:
        break;
    default:
        closeTimer->start(g_changeParam.TIMEOUT_UI);
        event->ignore();
        break;
    }
}

void UIConfigDateTime::setDateTime()
{
    qDebug() << Q_FUNC_INFO;
    closeTimer->stop();

    QString dateEd=dateEdit->dateTime().toString("ddMMyy");
    QString timeEd=timeEdit->time().toString("hhmm");

    qWarning("%s: %s", qPrintable(dateEd), qPrintable(timeEd));

    qDebug()<<"OS fun";
    Os__write_time(qstring_auc(timeEd));
    Os__write_date(qstring_auc(dateEd));

}

unsigned char *UIConfigDateTime::qstring_auc(QString str)
{
    char*  ch;
    QByteArray ba = str.toLatin1();
    ch=ba.data();
    unsigned char* ptr;
    ptr=(unsigned char*)ch;
    return ptr;
}

void UIConfigDateTime::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotQuitCfg()));
    closeTimer->start(timeout);
}

void UIConfigDateTime::slotQuitCfg()
{
    UIMsg::showNoticeMsgWithAutoClose("TIME OUT",g_changeParam.TIMEOUT_ERRMSG);
    this->close();
}
