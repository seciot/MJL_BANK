#ifndef UICHOOSEACCTYPE_H_
#define UICHOOSEACCTYPE_H_
#include "ui.h"
#include <QAction>
#include "transData.h"

class UIChooseAccType : public QDialog
{
    Q_OBJECT
public:
    explicit UIChooseAccType(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIChooseAccType();

    QLabel *lbHead;
    QLabel *lbAccountOwner;

    QPushButton *btnDefault;
    QPushButton *btnSavings;
    QPushButton *btnCheck;
    QPushButton *btnCredit;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;


    AccType privateAccType;

    void setAccType(const AccType);
    AccType getAccType();
private:

    bool FLAG_SENDER;
    bool FLAG_RECEIVER;

    QTimer *closeTimer;
protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void btnDefaultClicked();
    void btnSavingsClicked();
    void btnCheckClicked();
    void btnCreditClicked();
    void setAutoClose(int timeout);

    void setSender();
    void setReceiver();

    void slotQuitTrans();

signals:
    void sigChooseReceiverTypeComplete();
    void sigChooseTypeComplete(AccType);

    void sigQuitTrans();
};

#endif
