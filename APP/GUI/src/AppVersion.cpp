#include "AppVersion.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "ui.h"
#include "include.h"

AppVersion::AppVersion(QDialog *parent,Qt::WindowFlags f) :
    QDialog(parent,f)
{
    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setStyleSheet("QDialog{background-image: url(:/images/commonbg.png);}QPushButton{border: 3px groove grey;border-radius:8px;background-color : beige;}QPushButton:hover{border: 3px solid blue}QPushButton:pressed{border: 3px solid red}");
    this->setGeometry(0,FRAME420_THVALUE,FRAME420_WIDTH,FRAME420_HEIGHT);

    QFont font("宋体",14,QFont::Bold);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *SW_HLayout = new QHBoxLayout;
    QLabel *SWTLabel = new QLabel(this);
    SWTLabel->setFont(font);
    SWTLabel->setText(tr("Version:"));
    QLabel *SWCLabel = new QLabel(this);
    SWCLabel->setAlignment(Qt::AlignRight);
    SWCLabel->setFont(font);
    SWCLabel->setText(tr(CURRENT_VER));
    SW_HLayout->addWidget(SWTLabel);
    SW_HLayout->addWidget(SWCLabel);
    layout->addLayout(SW_HLayout);

    QHBoxLayout *DATE_HLayout = new QHBoxLayout;
    QLabel *DATLabel = new QLabel(this);
    DATLabel->setFont(font);
    DATLabel->setText(tr("Last Modify:"));
    QLabel *DACLabel = new QLabel(this);
    DACLabel->setAlignment(Qt::AlignRight);
    DACLabel->setFont(font);
    DACLabel->setText(tr(CURRENT_DATE));
    DATE_HLayout->addWidget(DATLabel);
    DATE_HLayout->addWidget(DACLabel);
    layout->addLayout(DATE_HLayout);

    QHBoxLayout *ID_HLayout = new QHBoxLayout;
    QLabel *AppIDLabel = new QLabel(this);
    AppIDLabel->setFont(font);
    AppIDLabel->setText(tr("App No:"));
    QLabel *AppIDCLabel = new QLabel(this);
    AppIDCLabel->setAlignment(Qt::AlignRight);
    AppIDCLabel->setFont(font);
    AppIDCLabel->setText(tr(CURRENT_APP_SN));
    ID_HLayout->addWidget(AppIDLabel);
    ID_HLayout->addWidget(AppIDCLabel);
    layout->addLayout(ID_HLayout);

    QHBoxLayout *NAME_HLayout = new QHBoxLayout;
    QLabel *AppNameLabel = new QLabel(this);
    AppNameLabel->setFont(font);
    AppNameLabel->setText(tr("Project Name:"));
    QLabel *AppNameCLabel = new QLabel(this);
    AppNameCLabel->setAlignment(Qt::AlignRight);
    AppNameCLabel->setFont(font);
    AppNameCLabel->setText(tr(CURRENT_NAME));
    NAME_HLayout->addWidget(AppNameLabel);
    NAME_HLayout->addWidget(AppNameCLabel);
    layout->addLayout(NAME_HLayout);

    QLabel *AppPrjLabel = new QLabel(this);
    AppPrjLabel->setStyleSheet("QLabel{border-top:1px solid green;border-bottom:1px solid green;}");
    AppPrjLabel->setFont(font);
    AppPrjLabel->setText(tr("Software S/N:"));
    layout->addWidget(AppPrjLabel,Qt::AlignLeft);

    QLabel *AppCodeLabel = new QLabel(this);
    AppCodeLabel->setAlignment(Qt::AlignHCenter);
    AppCodeLabel->setStyleSheet("QLabel{border-top:1px solid green;border-bottom:1px solid green;}");
    AppCodeLabel->setFont(font);
    AppCodeLabel->setText(tr(CURRENT_PRJ));
    layout->addWidget(AppCodeLabel,Qt::AlignLeft);

    layout->addStretch(0);
    QPushButton *pbtn_enter = new QPushButton(this);pbtn_enter->setFont(font);
    pbtn_enter->setText(tr("Confirm"));
    pbtn_enter->setMinimumHeight(35);
    layout->addWidget(pbtn_enter,Qt::AlignHCenter);

//    layout->setMargin(5);
    layout->setContentsMargins(0, 30, 0, 5);
//    layout->setSpacing(5);

    QTimer *pTimer = new QTimer(this);

    connect(pbtn_enter, SIGNAL(clicked()), this, SLOT(close()));
    connect(pTimer, SIGNAL(timeout()), this, SLOT(close()));
    pTimer->start((30*1000));
}

AppVersion::~AppVersion()
{
    qDebug() << Q_FUNC_INFO;
}

void AppVersion::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    default:
        event->ignore();
        break;
    }
}
