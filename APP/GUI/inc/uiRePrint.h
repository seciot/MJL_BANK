#ifndef UIREPRINT_H_
#define UIREPRINT_H_
#include "ui.h"
#include "objPrint.h"

class UIRePrint : public QDialog
{
    Q_OBJECT
public:
    explicit UIRePrint(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIRePrint();

    QLabel *lbHead;

    QPushButton *btnRePrintLast;
    QPushButton *btnRePrintWhich;
    QPushButton *btnPrintSettle;
    QPushButton *btnPrintAudit;

    QPushButton *btnCancel;
//    QPushButton *btnSubmit;
private:
    QThread *threadPrint;

    objPrint *pPrint;
protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
private slots:
    void slotReprintLast();
    void slotReprintWhich();
    void slotPrintSettle();
    void slotPrintAudit();

    void slotFinishPrint();
    void slotTearPrint();

signals:
};

#endif  
