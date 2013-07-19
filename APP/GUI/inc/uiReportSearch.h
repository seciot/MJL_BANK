#ifndef UIREPORTSEARCH_H_
#define UIREPORTSEARCH_H_
#include "ui.h"
#include "uiReportDetail.h"
#include <QAction>

class UIReportSearch : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportSearch(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportSearch();

    QLabel *lbHead;

    QLabel *lbSearchNotice;
    QLineEdit *leSearch;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

    UIReportDetail *uiRDetail;
private:


protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
private slots:
    void slotSearchTransaction();

signals:
};

#endif  
