#ifndef UITRANSONLINE_H_
#define UITRANSONLINE_H_
#include "ui.h"
#include <QAction>
#include "TransOnlinePro.h"

class UITransOnline : public QDialog
{
    Q_OBJECT
public:
    explicit UITransOnline(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UITransOnline();

    QLabel *lbHead;
    QLabel *lbGif;
    QLabel *lbNotice;
    QLabel *lbNotice2;

    QPushButton *btnCancel;

    void slotStartTrans(TransMode);

    void startTrans(TransMode);
    void ParamLoadShortcut(int iType);	//:- 签退时用到

private:
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

public slots:
//    void slotQuitTrans();


signals:
    void sigQuitTrans();
    void sigTransSuccess();
    void QuickQuit(unsigned char result);

private slots:
    void EableKEY_CAN(bool bEnable);
    void UpdateProcessBar(bool bOpen,int value,int iMin,int iMax,QString format);
    void UpdateLabelText(const QString &str,const QString &backup);
    void ExeAlterMsgBox(const QString &str);
    void ReturnFromThread(unsigned char index);
    void UpdateLabelChange();
    void slotBtnCancelclicked();

};

#endif
