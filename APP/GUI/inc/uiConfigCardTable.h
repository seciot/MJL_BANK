#ifndef UICONFIGCARDTABLE_H
#define UICONFIGCARDTABLE_H

#include "ui.h"
#include <QAction>


class uiConfigCardTable : public QDialog
{
    Q_OBJECT
public:
    explicit uiConfigCardTable(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~uiConfigCardTable();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbSelCardBIN;
    QLabel *lbBINLowVal;
    QLabel *lbBINHighVal;
    QLabel *lbIssuerID;
    QLabel *lbAcquirerID;

    QLineEdit *leSelCardBIN;
    QLineEdit *leBINLowVal;
    QLineEdit *leBINHighVal;
    QLineEdit *leIssuerID;
    QLineEdit *leAcquirerID;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif // UICONFIGCARDTABLE_H
