#ifndef UIVOID_H_
#define UIVOID_H_
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

class UIVOID : public QDialog
{
    Q_OBJECT
public:
    explicit UIVOID(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIVOID();

    QLabel *lbHead;
    UIReportDetail *uiRDetail;

private:

    QThread *passThread;
    QThread *typeThread;
    QThread *amountThread;
    QThread *swipeThread;


    UIInputPassword *uiIPass;
    UIInputAmount *uiIA;
    UISwipeCard *uiSC;
    UIChooseAccType *uiCAT;
    UIInputPIN *uiIPIN;
    UITransOnline *uiTO;
    UIInputManual *uiIM;
    UIPrint *uiP;


    bool FLAG_InputPassword;
    bool FLAG_Detail;
    bool FLAG_SwipeCard;
    bool FLAG_InputManual;
    bool FLAG_InputPIN;
    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;


protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void checkAuth(UserType,QString);
    void inputTraceNo();
    void swipeCard();
    void inputManual();
    void inputPIN();
    void transOnline();
    void printReceipt();

    void inputPINAndExit();
    void quitFromSwipeCard();
    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
