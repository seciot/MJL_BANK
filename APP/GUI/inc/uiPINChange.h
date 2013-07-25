#ifndef UIPINCHANGE_H_
#define UIPINCHANGE_H_
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

class UIPINChange : public QDialog
{
    Q_OBJECT
public:
    explicit UIPINChange(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIPINChange();

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
    bool FLAG_SwipeCard;
    bool FLAG_InputManual;
    bool FLAG_InputPIN;
    bool FLAG_TransOnline;


protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void checkAuth(UserType,QString);
    void swipeCard();
    void inputManual();
    void inputPIN();
    void inputNewPIN();
    void transOnline();
    void changePINSuccess();

    void inputPINAndExit();
    void quitFromSwipeCard();
    void quitFromFlow();
    void finishFromFlow();

signals:
    void sigErrCancel();
};

#endif  
