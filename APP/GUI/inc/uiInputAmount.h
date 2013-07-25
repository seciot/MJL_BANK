#ifndef UIINPUTAMOUNT_H_
#define UIINPUTAMOUNT_H_
#include "ui.h"
#include <QAction>


class UIInputAmount : public QDialog
{
    Q_OBJECT
public:
    explicit UIInputAmount(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIInputAmount();

    QLabel *lbHead;

    QLabel *lbInput;
    QLabel *lbPic;

    QLineEdit *leAmount;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;


private:
    QString AMOUNT;
    void addDot();
    QTimer *closeTimer;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotSaveAmount(QString);
    QString slotGetAmount();
    void slotAmountInputComplete();
    void slotQuitTrans();
    void FormAmount(unsigned char *pucOut,unsigned long ulAmount,unsigned char ucDecimalPos);
    QString QStringAmountFormular(const QString &arg1,unsigned long ulMaxAmt);
    void Remove(unsigned char str[],char remove);
    void setAutoClose(int timeout);

signals:
    void sigAmountInputComplete(QString);
    void sigQuitTrans();

};

#endif  
