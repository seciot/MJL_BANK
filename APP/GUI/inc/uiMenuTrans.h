#ifndef UIMENUTRANS_H_
#define UIMENUTRANS_H_
#include "ui.h"


class UIMenuTrans : public QDialog
{
    Q_OBJECT
public:
    explicit UIMenuTrans(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIMenuTrans();

    QLabel *lbMenu;
    QLabel *lbMenuIndexText;
    QPushButton *btnCashAd;
    QPushButton *btnCashDp;
    QPushButton *btnP2P;
    QPushButton *btnVOID;
    QPushButton *btnAdjust;
    QPushButton *btnBalance;
    QPushButton *btnPayment;
    QPushButton *btnPIN;
    QPushButton *btnLogOn;

    QPushButton *btnPageOneCancel;
    QPushButton *btnPageTwoCancel;
    QPushButton *btnPageOneNext;
    QPushButton *btnPageTwoBack;

    QStackedWidget *stackWidget;
    QWidget *pageOne;
    QWidget *pageTwo;
    QWidget *pageThree;

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void pageOneNextClicked();
    void pageTwoBackClicked();

    void Cash_Advance_Click();
    void Cash_Deposit_Click();
    void Balance_Inquiry_Click();
    void P2P_Card_Trans_Click();
    void Payment_Click();
    void Logon_Click();
    void VOID_Click();
    void PIN_Change_Click();

private:
    static void styleWidget(QWidget * btn,int iFontSize=13);
    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitMenu();
};

#endif  
