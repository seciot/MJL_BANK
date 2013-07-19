#ifndef UISETTLE_H_
#define UISETTLE_H_
#include "ui.h"
#include <QAction>
#include <QThread>

#include "uiTransOnline.h"
#include "uiPrint.h"

class UISettle : public QDialog
{
    Q_OBJECT
public:
    explicit UISettle(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UISettle();

    QLabel *lbHead;

private:
    UITransOnline *uiTO;
    UIPrint *uiP;

    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void transOnline();
    void printReceipt();

    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
