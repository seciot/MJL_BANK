#ifndef UISETTLE_H_
#define UISETTLE_H_
#include "ui.h"
#include <QAction>

#include <QThread>

#include "TransOnlinePro.h"


class UISettle : public QDialog
{
    Q_OBJECT
public:
    explicit UISettle(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UISettle();

    QLabel *lbHead;
    QLabel *lbGif;
    QLabel *lbHeadNotice;
    QLabel *lbNotice;
    QLabel *lbNotice2;

    QPushButton *btnCancel;


    void startSettle();

private:
    QThread *passThread;

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

    void slotStartSettle();

    void quitFromInputPass();
    void EableKEY_CAN(bool bEnable);
    void UpdateLabelText(const QString &str,const QString &backup);
    void slotUpdateStatus(QString status);
    void ReturnFromThread(unsigned char index);
    void UpdateLabelChange();
    void ReturnWay();
    void slotBtnCancelclicked();
    void slotFinishPrint();
signals:
    void QuickQuit(unsigned char result);

};

#endif
