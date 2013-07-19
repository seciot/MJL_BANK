#ifndef UICONFIGISSUER_H
#define UICONFIGISSUER_H

#include "ui.h"
#include <QAction>


class uiConfigIssuer : public QDialog
{
    Q_OBJECT
public:
    explicit uiConfigIssuer(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~uiConfigIssuer();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbIndexNumber;
    QLabel *lbCardLabel;

    QLineEdit *leIndexNumber;
    QLineEdit *leCardLabel;

    QCheckBox *chkCheckingOption;
    QCheckBox *chkCashOutOption;
    QCheckBox *chkDefaultAcc;
    QCheckBox *chkFloorLimi;
    QCheckBox *chkFloorLimiEMV;
    QCheckBox *chkEMVPinBypass;
    QCheckBox *chkMagPIN;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif //
