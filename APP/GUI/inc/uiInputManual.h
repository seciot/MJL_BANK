#ifndef UIINPUTMANUAL_H_
#define UIINPUTMANUAL_H_
#include "ui.h"
#include <QAction>



class UIInputManual : public QDialog
{
    Q_OBJECT
public:
    explicit UIInputManual(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIInputManual();

    QLabel *lbHead;
    QLabel *lbOwner;

    QLabel *lbCardNo;
    QLabel *lbEXP;
    QLineEdit *leCardNo;
    QLineEdit *leEXP;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:
    bool FLAG_SENDER;
    bool FLAG_RECEIVER;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotQuitTrans();
    void slotInputComplete();

    void setEnterReceiver();

signals:
    void sigInputComplete();
    void sigQuitTrans();
};

#endif  
