#ifndef GROUP_H
#define GROUP_H
#include "ui.h"

class WidgetReport : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetReport(QString Number,QString Date,QString Time,QString TransType, QWidget *parent = 0);


    QLabel *lbNumber;
    QLabel *lbDate;
    QLabel *lbTime;
    QLabel *lbTransType;
    
signals:
    
public slots:
    
};

#endif // GROUP_H
