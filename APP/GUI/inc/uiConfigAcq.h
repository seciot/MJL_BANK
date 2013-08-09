#ifndef UIACONFIGACQ_H_
#define UIACONFIGACQ_H_
#include "ui.h"
#include <QAction>


class UIConfigAcq : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigAcq(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigAcq();


    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbIndexNum;
    QLabel *lbHostName;
    QLabel *lbHostType;
    QLabel *lbSettleHost;
    QLabel *lbCarrierTimeOut;
    QLabel *lbRetry;
    QLabel *lbNII;
    QLabel *lbSyncMode;
    QLabel *lbTerID;
    QLabel *lbMerID;
    QLabel *lbBatchNum;
    QLabel *lbNextBatchNum;
    QLabel *lbAPN;
    QLabel *lbHostAccess;
    QLabel *lbDomainAddr;
    QLabel *lbPriIP;
    QLabel *lbSecIP;
    QLabel *lbCurCodeNo;
    QLabel *lbCurCode;
    QLabel *lbMinTrnsAmt;
    QLabel *lbMaxTrnsAmt;
    QLabel *lbCountryCode;


    QLineEdit *leIndexNum;
    QLineEdit *leHostName;
    QComboBox *cbHostType;
    QComboBox *cbSettleHost;
    QComboBox *cbCarrierTimeOut;
    QLineEdit *leRetry;
    QLineEdit *leNII;
    QLineEdit *leSyncMode;
    QLineEdit *leTerID;
    QLineEdit *leMerID;
    QLineEdit *leBatchNum;
    QLineEdit *leNextBatchNum;
    QLineEdit *leAPN;
    QComboBox *cbHostAccess;
    QLineEdit *leDomainAddr;
    MyIpAddrEdit *lePriIP;
    QLineEdit *lePriIPPort;
    MyIpAddrEdit *leSecIP;
    QLineEdit *leSecIPPort;
    QLineEdit *leCurCodeNo;
    QLineEdit *leCurCode;
    QLineEdit *leMinTrnsAmt;
    QLineEdit *leMaxTrnsAmt;
    QLineEdit *leCountryCode;

    QCheckBox *chkCashBackCheck;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void initialSettings();

    void slotSubmitClicked();

    void restartTimer();
private:
    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitCfg();

};

#endif  
