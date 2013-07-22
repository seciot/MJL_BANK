#ifndef OBJPRINT_H
#define OBJPRINT_H

#include <QObject>
#include "new_drv.h"

class objPrint : public QObject
{
    Q_OBJECT
public:
    explicit objPrint(QObject *parent = 0);
    ~objPrint();


signals:
    void sigPrintComplete();
    void sigTear();
public slots:
    void printReceipt();
    void rePrintReceipt();
    void printSettle();
    void printAudit();

private:
};

#endif
