#ifndef UIREPORTQUERY_H_
#define UIREPORTQUERY_H_
#include "ui.h"
//#include "widgetReport.h"

#include <QAction>

class UIReportQuery : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportQuery(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportQuery();

    QLabel *lbHead;

    QTableWidget *tbTransList;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:
    QVector<QLabel*> listVector;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
private slots:
    void slotTransClicked();

    void ergodicTrans();

signals:
};

#endif  
