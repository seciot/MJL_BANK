#ifndef UICONFIGCARDTABLE_H
#define UICONFIGCARDTABLE_H

#include "ui.h"
#include <QAction>


class UIConfigCardTable : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigCardTable(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigCardTable();


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
private:
    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitCfg();

};

#endif // UICONFIGCARDTABLE_H
