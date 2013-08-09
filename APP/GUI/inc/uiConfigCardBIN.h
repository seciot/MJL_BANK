#ifndef UIACONFIGCARDBIN_H_
#define UIACONFIGCARDBIN_H_
#include "ui.h"
#include <QAction>


class UIConfigCardBIN : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigCardBIN(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigCardBIN();

    QLabel *lbHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;

    QLabel *lbSelectCardBIN;
    QLabel *lbIssuerText;
    QLabel *lbIssuerID;
    QLabel *lbAcquirerID;
    QLabel *lbCardBINText;
    QLabel *lbCardBINLow;
    QLabel *lbCardBINHigh;
    QLabel *lbHostType;
    QLabel *lbIP;
    QLabel *lbDomainName;


    QComboBox *cbSelectCardBIN;
    QLineEdit *leIssuerText;
    QLineEdit *leIssuerID;
    QLineEdit *leAcquirerID;
    QLineEdit *leCardBINText;
    QLineEdit *leCardBINLow;
    QLineEdit *leCardBINHigh;
    QButtonGroup *hostTypeGroup;
    QRadioButton *rdIP;
    QRadioButton *rdDomain;
    MyIpAddrEdit *ipAddr;
    QLineEdit *leIPPort;
    QLineEdit *leDomainName;

    QPushButton *btnApply;
    QPushButton *btnAdd;
    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);

private slots:

    void restartTimer();
private:
    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitCfg();

    void slotApply();
    void slotInitialCardBIN(int index);
    void slotAdd();

    void setEditable(bool);
    void setClear();
    void checkInput();

};

#endif  
