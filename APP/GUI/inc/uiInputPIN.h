#ifndef UIINPUTPIN_H_
#define UIINPUTPIN_H_
#include "ui.h"
#include <QAction>


class UIInputPIN : public QDialog
{
    Q_OBJECT
public:
    explicit UIInputPIN(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIInputPIN();

    QLabel *lbHead;

    QLabel *lbAmount;
    QLabel *lbAmountNumber;

    QLabel *lbCardPic;
    QLabel *lbCardNo;
    QLabel *lbPIN;
    QLabel *lbPINNotice;

    QLineEdit *lePIN;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

    QString savePIN;
    QString tempPIN;
    QString showPIN;
private:
    bool FLAG_HASPIN;
protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotQuitTrans();
    void slotSubmitClicked();
    void slotSavePIN(const QString);
    void slotSetAmount(const QString);
    void slotSetCardNo(const QString);
    void slotIdentityCard(const QString);
    void slotDisablePIN();

signals:
    void sigQuitTrans();
    void sigSubmit();
};

#endif  
