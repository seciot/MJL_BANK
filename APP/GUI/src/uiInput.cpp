#include "uiInput.h"
#include <QDesktopWidget>
#include "global.h"

UIInput::UIInput(QString title, QString showMsg, QDialog *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(20,FRAME420_THVALUE+50,FRAME420_WIDTH,FRAME420_HEIGHT-50);
    this->setFixedSize(FRAME420_WIDTH-40,FRAME420_HEIGHT-80);
    this->setStyleSheet("QDialog{border: 6px solid silver;}");

    //--------------define--------------------//
    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lbHead->setText(title);
    lbHead->setStyleSheet("background-color: rgb(0, 153, 255);");

    lbMsg=new QLabel();
    QFont fontL("Helvetica",14,QFont::Bold);
    lbMsg->setAlignment(Qt::AlignCenter);
    lbMsg->setFont(fontL);
    lbMsg->setText(showMsg);

    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("Apply"));
    btnSubmit->setFont(fontL);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);

    btnCancel=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnCancel->setFont(fontL);
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);


    // -----------layout------------//
    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(6);
    h1Lay->addWidget(lbHead);
    h1Lay->addSpacing(6);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbMsg);
    v1Lay->addItem(sp2);

    QHBoxLayout *hLay=new QHBoxLayout();
    hLay->addSpacing(10);
    hLay->addLayout(v1Lay);
    hLay->addSpacing(10);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addSpacing(6);
    layout->addLayout(h1Lay);
    layout->addLayout(hLay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,10);


    // connect
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(slotConfirm()));
}


UIInput::UIInput(QString title, QString showMsg, QString regExp, int length, QDialog *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(20,FRAME420_THVALUE+50,FRAME420_WIDTH,FRAME420_HEIGHT-50);
    this->setFixedSize(FRAME420_WIDTH-40,FRAME420_HEIGHT-80);
    this->setStyleSheet("QDialog{border: 6px solid silver;}");

    //--------------define--------------------//
    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    lbHead->setText(title);
    lbHead->setStyleSheet("background-color: rgb(0, 153, 255);");

    lbMsg=new QLabel();
    QFont fontL("Helvetica",14,QFont::Bold);
    lbMsg->setAlignment(Qt::AlignCenter);
    lbMsg->setFont(fontL);
    lbMsg->setText(showMsg);

    leInput=new QLineEdit();
    leInput->setFont(fontL);
    leInput->setAlignment(Qt::AlignCenter);
    leInput->setMinimumHeight(35);
    leInput->setStyleSheet("border: 3px solid silver;border-radius: 6px;");
    leInput->setMaxLength(length);
    if(regExp!="0")
    {
        const QString  REGEX_AMOUNT = regExp;
        QRegExp regx(REGEX_AMOUNT);
        QValidator *validator = new QRegExpValidator(regx, leInput);
        leInput->setValidator(validator);
    }

    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("Apply"));
    btnSubmit->setFont(fontL);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_SUBMIT_STYLE);

    btnCancel=new QPushButton;
    btnCancel->setText(tr("Cancel"));
    btnCancel->setFont(fontL);
    btnCancel->setMinimumHeight(30);
    btnCancel->setStyleSheet(BTN_CANCEL_STYLE);


    // -----------layout------------//
    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(6);
    h1Lay->addWidget(lbHead);
    h1Lay->addSpacing(6);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbMsg);
    v1Lay->addWidget(leInput);
    v1Lay->addItem(sp2);

    QHBoxLayout *hLay=new QHBoxLayout();
    hLay->addSpacing(10);
    hLay->addLayout(v1Lay);
    hLay->addSpacing(10);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnCancel);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addSpacing(6);
    layout->addLayout(h1Lay);
    layout->addLayout(hLay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,10);


    // connect
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(slotSubmit()));
    connect(leInput,SIGNAL(textChanged(QString)),this,SLOT(restartTimer()));

    this->setAutoClose(g_changeParam.TIMEOUT_UI);
}

bool UIInput::flagFinish = false;
QString UIInput::inputedText = "";

void UIInput::slotCancel()
{
    flagFinish=false;
    this->close();
}

void UIInput::slotSubmit()
{
    flagFinish=true;
    inputedText=leInput->text();
    if(!inputedText.isEmpty())
    {
        this->close();
    }
    else
    {
        return;
    }
}

void UIInput::slotConfirm()
{
    flagFinish=true;
    this->close();
}

void UIInput::slotSetPINMode()
{
    leInput->setEchoMode(QLineEdit::Password);
}

UIInput::~UIInput()
{
    qDebug() << Q_FUNC_INFO;
}

void UIInput::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->slotCancel();
        break;
    case Qt::Key_Enter:
        this->slotSubmit();
        break;
    default:
        event->ignore();
        break;
    }
}

QString UIInput::getText(QString title, QString showMsg, QString regExp, int length, bool *ok)
{
    if(length==0)
        length=50;
    UIInput *uiInput=new UIInput(title,showMsg,regExp,length);
    uiInput->exec();

    if(flagFinish == true)
        *ok=true;
    else if(flagFinish == false)
        *ok=false;

    QString returnText=inputedText;
    inputedText.clear();
    flagFinish=false;
    return returnText;
}

QString UIInput::getPIN(QString title, QString showMsg, QString regExp, int length, bool *ok)
{
    if(length==0)
        length=50;
    UIInput *uiInput=new UIInput(title,showMsg,regExp,length);
    uiInput->slotSetPINMode();
    uiInput->exec();

    if(flagFinish == true)
        *ok=true;
    else if(flagFinish == false)
        *ok=false;

    QString returnText=inputedText;
    inputedText.clear();
    flagFinish=false;
    return returnText;
}


unsigned int UIInput::getInt(QString title, QString showMsg, QString regExp, int length, bool *ok)
{
    UIInput *uiInput=new UIInput(title,showMsg,regExp,length);
    uiInput->exec();

    if(flagFinish == true)
        *ok=true;
    else if(flagFinish == false)
        *ok=false;

    int returnInt=inputedText.toInt();
    inputedText.clear();
    flagFinish=false;

    return returnInt;
}

bool UIInput::getConfirm(QString title, QString showMsg)
{
    UIInput *uiInput=new UIInput(title,showMsg);
    uiInput->exec();

    bool ok;
    if(flagFinish == true)
        ok=true;
    else if(flagFinish == false)
        ok=false;

    flagFinish=false;
    return ok;
}

void UIInput::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(close()));
    closeTimer->start(timeout);
}

void UIInput::restartTimer()
{
    closeTimer->start(g_changeParam.TIMEOUT_UI);
}
