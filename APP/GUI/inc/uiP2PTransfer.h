#ifndef UIP2PTRANSFER_H_
#define UIP2PTRANSFER_H_
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

class UIP2PTransfer : public QDialog
{
    Q_OBJECT
public:
    explicit UIP2PTransfer(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIP2PTransfer();

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
    UIChooseAccType *uiCATReceive;
    UIInputPIN *uiInPIN;
    UITransOnline *uiTransOn;
    UIInputManual *uiIMSend;
    UIInputManual *uiIMReceive;
    UIPrint *uiPrint;


    bool FLAG_InputPassword;
    bool FLAG_AccountType;
    bool FLAG_AccountTypeReceive;
    bool FLAG_SwipeCard;
    bool FLAG_InputManual;
    bool FLAG_InputAmount;
    bool FLAG_InputReceiveCard;
    bool FLAG_InputPIN;
    bool FLAG_TransOnline;
    bool FLAG_PrintReceipt;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void chooseAccountType(UserType,QString);
    void chooseReceiverAccountType();
    void setAccountType(AccType);
    void swipeCard();
    void inputManual();
    void inputAmount();
    void inputReceiverCard();
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
