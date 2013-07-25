#ifndef UIACONFIGTRANSATTR_H_
#define UIACONFIGTRANSATTR_H_
#include "ui.h"
#include <QAction>


class UIConfigTransAttr : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigTransAttr(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigTransAttr();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbCashAd;
    QLabel *lbCashDe;
    QLabel *lbP2P;
    QLabel *lbVOID;
    QLabel *lbAdjust;
    QLabel *lbBalanceInq;
    QLabel *lbPay;
    QLabel *lbPin;

    QCheckBox *chkCashAdTrans;
    QCheckBox *chkCashDeTrans;
    QCheckBox *chkP2PTrans;
    QCheckBox *chkVOIDTrans;
    QCheckBox *chkAdjustTrans;
    QCheckBox *chkBalanceInqTrans;
    QCheckBox *chkPayTrans;
    QCheckBox *chkPinTrans;

    QCheckBox *chkCashAdPin;
    QCheckBox *chkCashDePin;
    QCheckBox *chkP2PPin;
    QCheckBox *chkVOIDPin;
    QCheckBox *chkAdjustPin;
    QCheckBox *chkBalanceInqPin;
    QCheckBox *chkPayPin;
    QCheckBox *chkPinPin;

    QCheckBox *chkCashAdMan;
    QCheckBox *chkCashDeMan;
    QCheckBox *chkP2PMan;
    QCheckBox *chkVOIDMan;
    QCheckBox *chkAdjustMan;
    QCheckBox *chkBalanceInqMan;
    QCheckBox *chkPayMan;
    QCheckBox *chkPinMan;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

private slots:
    void slotSubmitClicked();
private:
    void initialAttr();
    QTimer *closeTimer;
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
public slots:
    void setAutoClose(int timeout);
    void slotQuitCfg();
    void restartTimer();
};

#endif  
