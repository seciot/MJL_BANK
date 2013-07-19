#ifndef UISHOWBALANCE_H_
#define UISHOWBALANCE_H_
#include "ui.h"
#include <QAction>

class UIShowBalance : public QDialog
{
    Q_OBJECT
public:
    explicit UIShowBalance(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIShowBalance();

    QLabel *lbHead;

    QLabel *lbCardNo;
    QLabel *lbBalance;
    QLabel *lbCardNoLine;
    QLabel *lbBalanceLine;

    QPushButton *btnSubmit;
private:
    void setCardNo();
    void setBalance();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotFinishFlow();

signals:
    void sigFinishFlow();
};

#endif  
