#ifndef UICASHADVANCE_H_
#define UICASHADVANCE_H_
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
#include "uiPrint.h"

class UICashAdvance : public QDialog
{
    Q_OBJECT
public:
    explicit UICashAdvance(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UICashAdvance();

    QLabel *lbHead;

private:

    QThread *passThread;
    QThread *typeThread;
    QThread *amountThread;
    QThread *swipeThread;


    UIInputPassword *uiInPass;
    UIInputAmount *uiInAmt;
    UISwipeCard *uiSwipeCard;
    UIChooseAccType *uiCAT;
    UIInputPIN *uiInPIN;
    UITransOnline *uiTransOn;
    UIInputManual *uiInMan;
    UIPrint *uiPrint;


    bool FLAG_InputPassword;
    bool FLAG_AccountType;
    bool FLAG_SwipeCard;
    bool FLAG_InputAmount;
    bool FLAG_InputManual;
    bool FLAG_InputPIN;
    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;


protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void chooseAccountType(UserType,QString);
    void setAccountType(AccType);
    void swipeCard();
    void inputAmount();
    void inputManual();
    void inputPIN(QString);
    void transOnline();
    void printReceipt();

    void inputAmountAndExit();
    void quitFromSwipeCard();
    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
