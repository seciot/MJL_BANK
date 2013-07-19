#ifndef UISWIPECARD_H_
#define UISWIPECARD_H_
#include "ui.h"
#include <QAction>
#include <QThread>
#include "AnimationLabel.h"
#include "objPutCard.h"

class UISwipeCard : public QDialog
{
    Q_OBJECT
public:
    explicit UISwipeCard(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UISwipeCard();

    QLabel *lbHead;

    QLabel *lbPicTmp;
    AnimationLabel *lbPic;
    QLabel *lbSwipeTip;
    QLabel *lbSwipeNotice;
    QLabel *lbManualNotice;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

    QThread *threadSwipe;
    objPutCard *pCard;

private:
    unsigned char putCardProcess();
    int checkCard();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotQuitTrans();
    void slotPutCardFinish();
    void slotSwitchToManual();

    void setNoManual();


signals:
    void sigQuitTrans();

    void sigFinishPutCard();
    void sigFinishPutNotSupportCard();

    void sigSwitchToManual();

};

#endif  
