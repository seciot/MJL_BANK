#ifndef UIACONFIGSURCHARGE_H_
#define UIACONFIGSURCHARGE_H_
#include "ui.h"
#include <QAction>


class UIConfigSurCharge : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigSurCharge(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigSurCharge();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbCashAd;
    QLabel *lbCashDe;
    QLabel *lbP2PCard;
    QLabel *lbP2PAcc;
    QLabel *lbPay;

    QLabel *lbCashAdChangeAmount;
    QLabel *lbCashDeChangeAmount;
    QLabel *lbP2PCardChangeAmount;
    QLabel *lbP2PAccChangeAmount;
    QLabel *lbPayChangeAmount;

    QLineEdit *leCashAdChangeAmount;
    QLineEdit *leCashDeChangeAmount;
    QLineEdit *leP2PCardChangeAmount;
    QLineEdit *leP2PAccChangeAmount;
    QLineEdit *lePayChangeAmount;

    QCheckBox *chkCashAdSur;
    QCheckBox *chkCashDeSur;
    QCheckBox *chkP2PCardSur;
    QCheckBox *chkP2PAccSur;
    QCheckBox *chkPaySur;

    QCheckBox *chkCashAdFlat;
    QCheckBox *chkCashDeFlat;
    QCheckBox *chkP2PCardFlat;
    QCheckBox *chkP2PAccFlat;
    QCheckBox *chkPayFlat;

    QCheckBox *chkCashAdPer;
    QCheckBox *chkCashDePer;
    QCheckBox *chkP2PCardPer;
    QCheckBox *chkP2PAccPer;
    QCheckBox *chkPayPer;

    QCheckBox *chkCashAdAmountRange;
    QCheckBox *chkCashDeAmountRange;
    QCheckBox *chkP2PCardAmountRange;
    QCheckBox *chkP2PAccAmountRange;
    QCheckBox *chkPayAmountRange;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif  
