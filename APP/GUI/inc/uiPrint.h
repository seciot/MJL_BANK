#ifndef UIPRINT_H_
#define UIPRINT_H_
#include "ui.h"
#include <QAction>
#include <QThread>
#include "AnimationLabel.h"
#include "objPrint.h"

class UIPrint : public QDialog
{
    Q_OBJECT
public:
    explicit UIPrint(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIPrint();

    QLabel *lbHead;

    QLabel *lbPic;
    QLabel *lbPrintTip;
    QLabel *lbPrintNotice;

    QMovie * movie;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

    QThread *threadPrint;

    objPrint *pPrint;
private:

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotQuitTrans();
    void slotFinishTrans();
    void slotTearPrint();

signals:
    void sigQuitTrans();
    void sigFinishTrans();


};

#endif  
