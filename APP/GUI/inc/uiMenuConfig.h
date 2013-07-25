#ifndef UIMENUCONFIG_H_
#define UIMENUCONFIG_H_
#include "ui.h"

#include "uiConfigTer.h"
#include "uiConfigAcq.h"
#include "uiConfigcardTable.h"
#include "uiConfigIssuer.h"
#include "uiConfigPay.h"
#include "uiConfigDateTime.h"
#include "uiConfigSurCharge.h"
#include "uiConfigTransAttr.h"
#include "uiUserManager.h"
#include "uiInputPassword.h"

class UIMenuConfig : public QDialog
{
    Q_OBJECT
public:
    explicit UIMenuConfig(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIMenuConfig();

    QLabel *lbMenu;
    QLabel *lbMenuIndexText;

    QPushButton *btnTerminal;
    QPushButton *btnAcquirer;
    QPushButton *btnCardTable;
    QPushButton *btnIssuer;
    QPushButton *btnPayment;
    QPushButton *btnSetDateTime;
    QPushButton *btnClearReversal;
    QPushButton *btnSuvCharge;
    QPushButton *btnTransAttr;

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

    void Terminal_Config_Click();
    void Acquirer_Config_Click();
    void CardTable_Config_Click();
    void Issuer_Config_Click();
    void Payment_Config_Click();
    void Date_Config_Click();
    void Sur_Config_Click();
    void Trans_Config_Click();

    void quitFromInputPass();
    void slotAllowEdit(UserType,QString);
private:
    UIInputPassword *uiIP;
    QThread *passThread;

    static void styleWidget(QWidget * btn,int iFontSize=13);

    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitMenu();
};

#endif  
