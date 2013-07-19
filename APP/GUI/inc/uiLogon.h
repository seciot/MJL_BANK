#ifndef UILOGON_H_
#define UILOGON_H_
#include "ui.h"
#include <QAction>

#include <QThread>

#include "TransOnlinePro.h"
#include "uiInputPassword.h"


class UILogon : public QDialog
{
    Q_OBJECT
public:
    explicit UILogon(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UILogon();

    QLabel *lbHead;
    QLabel *lbGif;
    QLabel *lbNotice;
    QLabel *lbNotice2;

    QPushButton *btnCancel;


    void startLogon();
    void ParamLoadShortcut(int iType);	//:- 签退时用到

private:
    QThread *passThread;
    UIInputPassword *uiIP;

    QMovie * movie;
    TransOnlinePro * transOnlineProcess;
    void ClearLabelInfo();
    int iitype;
    QTimer * pUpdateTimer;

private:
    QString qsUpdate;
    QString qsBack;
    char showColon;
    char backColon;
    char ucShortCutFlag;

protected:
    void keyPressEvent(QKeyEvent *event);
private slots:

    void slotStartLogon();

    void quitFromInputPass();
    void EableKEY_CAN(bool bEnable);
    void UpdateLabelText(const QString &str,const QString &backup);
    void ReturnFromThread(unsigned char index);
    void UpdateLabelChange();
    void ReturnWay();
    void slotBtnCancelclicked();

signals:
    void QuickQuit(unsigned char result);

};

#endif  
