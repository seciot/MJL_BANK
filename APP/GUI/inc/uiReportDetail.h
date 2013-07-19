#ifndef UIREPORTDETAIL_H_
#define UIREPORTDETAIL_H_
#include "ui.h"
//#include "widgetReport.h"

#include <QAction>

class UIReportDetail : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportDetail(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportDetail();

    QLabel *lbHead;

    QTableWidget *tbDetailList;

    QPushButton *btnCancel;
//    QPushButton *btnSubmit;
private:

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotSetDetailList(QString transType, QString cardNo, QString amount, QString refNo, QString apprNo, QString operatorNo);
private slots:

signals:
};

#endif  
