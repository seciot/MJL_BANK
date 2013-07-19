#include "widgetReport.h"
#include <QLayout>
#include <QGridLayout>

WidgetReport::WidgetReport(QString Number, QString Date, QString Time, QString TransType, QWidget *parent):
    QWidget(parent)
{
    lbNumber=new QLabel;
    lbDate=new QLabel;
    lbTime=new QLabel;
    lbTransType=new QLabel;

    QFont font("Helvetica",7,QFont::Bold);
    lbNumber->setFont(font);
    lbDate->setFont(font);
    lbTime->setFont(font);
    lbTransType->setFont(font);

    qDebug()<<Number<<Date<<Time<<TransType;
    lbNumber->setText(Number);
    lbDate->setText(Date);
    lbTime->setText(Time);
    lbTransType->setText(TransType);

    lbNumber->setStyleSheet("color: #1C1C1C;");
    lbDate->setStyleSheet("color: #000080;");
    lbTime->setStyleSheet("color: #00FF00;");
    lbTransType->setStyleSheet("color: #1C1C1C;");

    QHBoxLayout *layout1=new QHBoxLayout();
    QHBoxLayout *layout2=new QHBoxLayout();
    layout1->addWidget(lbNumber);
    layout1->addWidget(lbTransType);
    layout2->addWidget(lbDate);
    layout2->addWidget(lbTime);

    QVBoxLayout *vl=new QVBoxLayout(this);
    vl->addLayout(layout1);
    vl->addLayout(layout2);


    setContentsMargins(0,0,0,0);
}
