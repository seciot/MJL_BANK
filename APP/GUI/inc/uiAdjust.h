#ifndef UIADJUST_H_
#define UIADJUST_H_
#include "ui.h"
#include <QAction>
#include <QThread>

#include "uiChooseAccType.h"
#include "uiInputAmount.h"
#include "uiInputPassword.h"
#include "uiSwipeCard.h"
#include "uiInputPIN.h"
#include "uiTransOnline.h"
#include "uiInputManual.h"
#include "uiReportDetail.h"
#include "uiPrint.h"

class UIAdjust : public QDialog
{
    Q_OBJECT
public:
    explicit UIAdjust(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIAdjust();

    QLabel *lbHead;

private:

    QThread *passThread;
    QThread *typeThread;
    QThread *amountThread;
    QThread *swipeThread;


    UIInputPassword *uiInPass;
    UIReportDetail *uiRepDetail;
    UIInputAmount *uiInAmt;
    UITransOnline *uiTransOn;
    UIPrint *uiPrint;


    bool FLAG_InputPassword;
    bool FLAG_Detail;
    bool FLAG_InputAmount;
    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;


protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void checkAuth(UserType,QString);
    void inputTraceNo();
    void inputAmount();
    void transOnline();
    void printReceipt();

    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
