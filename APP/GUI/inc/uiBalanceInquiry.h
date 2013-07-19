#ifndef UIBALANCEINQUIRY_H_
#define UIBALANCEINQUIRY_H_
#include "ui.h"
#include <QAction>
#include <QThread>

#include "uiChooseAccType.h"
#include "uiInputPassword.h"
#include "uiSwipeCard.h"
#include "uiInputPIN.h"
#include "uiTransOnline.h"
#include "uiInputManual.h"
#include "uiShowBalance.h"
#include "transData.h"

class UIBalanceInquiry : public QDialog
{
    Q_OBJECT
public:
    explicit UIBalanceInquiry(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIBalanceInquiry();

    QLabel *lbHead;

private:

    QThread *passThread;
    QThread *typeThread;
    QThread *amountThread;
    QThread *swipeThread;


    UIInputPassword *uiIP;
    UISwipeCard *uiSC;
    UIChooseAccType *uiCAT;
    UIInputPIN *uiIPIN;
    UITransOnline *uiTO;
    UIInputManual *uiIM;
    UIShowBalance *uiSB;

    bool FLAG_InputPassword;
    bool FLAG_AccountType;
    bool FLAG_SwipeCard;
    bool FLAG_InputManual;
    bool FLAG_InputPIN;
    bool FLAG_TransOnline;
    bool FLAG_ShowBalance;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void chooseAccountType(UserType,QString);
    void setAccountType(AccType);
    void swipeCard();
    void inputManual();
    void inputPIN();
    void transOnline();
    void showBalance();

    void inputPinAndExit();
    void quitFromSwipeCard();
    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
