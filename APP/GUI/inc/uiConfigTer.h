#ifndef UICONFIGTER_H_
#define UICONFIGTER_H_
#include "ui.h"
#include <QAction>


class UIConfigTer : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigTer(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigTer();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;
//    QMouseEvent *saveEve;

    QCheckBox *chkTrans;
    QCheckBox *chkCard;


    QLabel *lbSTAN;
    QLineEdit *leSTAN;
    QLabel *lbInvoice;
    QLineEdit *leInvoice;
    QLabel *lbPABXAcCode;
    QLineEdit *lePABXAcCode;
    QLabel *lbPABXDelay;
    QLineEdit *lePABXDelay;
    QLabel *lbDialType;
    QComboBox *cbDialType;
    QLabel *lbTechPass;
    QLineEdit *leTechPass;

    QLabel *lbHostTimeOut;
    QLineEdit *leHostTimeOut;
    QLabel *lbUserTimeOut;
    QLineEdit *leUserTimeOut;

    QLabel *lbErrMsgTimeOut;
    QLineEdit *leErrMsgTimeOut;
    QLabel *lbPaperTearTimeOut;
    QLineEdit *lePaperTearTimeOut;

    QLabel *lbInvoiceLogo;
    QComboBox *cbInvoiceLogo;
    QLabel *lbScreenLogo;
    QComboBox *cbScreenLogo;

    QCheckBox *chkNoBliendDial;
    QCheckBox *chkSetReceipt;
    QCheckBox *chkSelAccEnable;
    QLabel *lbRLine1;
    QLabel *lbRLine2;
    QLabel *lbRLine3;
    QLabel *lbRLine4;
    QLabel *lbALine1;
    QLabel *lbALine2;
    QLabel *lbALine3;
    QLabel *lbALine4;
    QLineEdit *leRLine1;
    QLineEdit *leRLine2;
    QLineEdit *leRLine3;
    QLineEdit *leRLine4;
    QLineEdit *leALine1;
    QLineEdit *leALine2;
    QLineEdit *leALine3;
    QLineEdit *leALine4;

    QLabel *lbCompLb;
    QLineEdit *leCompLb;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);

public slots:
    void initialSettings();
    void slotSubmitClicked();
    void setAutoClose(int timeout);
    void slotQuitCfg();
private slots:
    void restartTimer();
private:
    QTimer *closeTimer;
};

#endif  
