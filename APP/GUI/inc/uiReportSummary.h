#ifndef UIREPORTSUMMARY_H_
#define UIREPORTSUMMARY_H_
#include "ui.h"
#include <QAction>

class UIReportSummary : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportSummary(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportSummary();

    QLabel *lbHead;

    QTableWidget *tbSummary;

    QPushButton *btnCancel;
private:


protected:
    void keyPressEvent(QKeyEvent *event);

public slots:

    void setTransTotal();
private slots:



signals:
};

#endif  
