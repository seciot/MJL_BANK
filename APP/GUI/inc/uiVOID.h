#ifndef UIVOID_H_
#define UIVOID_H_
#include "ui.h"
#include <QAction>
#include <QThread>

#include "uiInputPassword.h"
#include "uiTransOnline.h"
#include "uiReportDetail.h"
#include "uiPrint.h"

class UIVOID : public QDialog
{
    Q_OBJECT
public:
    explicit UIVOID(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIVOID();

    QLabel *lbHead;

private:

    QThread *passThread;
    QThread *typeThread;
    QThread *amountThread;
    QThread *swipeThread;


    UIInputPassword *uiInPass;
    UIReportDetail *uiRepDetail;
    UITransOnline *uiTransOn;
    UIPrint *uiPrint;


    bool FLAG_InputPassword;
    bool FLAG_Detail;
    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void checkAuth(UserType,QString);
    void inputTraceNo();
    void confirmVOID();
    void transOnline();
    void voidSuccess();
    void printReceipt();

    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
