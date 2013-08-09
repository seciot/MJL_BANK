#ifndef OBJPUTCARD_H
#define OBJPUTCARD_H

#include <QObject>
#include "new_drv.h"

class objPutCard : public QObject
{
    Q_OBJECT

public:
    explicit objPutCard(QObject *parent = 0);
    ~objPutCard();

    unsigned char putCardProcess();

    DRV_OUT *pxMagObj;

signals:
    void putCardFinished();
    void putNotSupportCard();
    void putCardFailed();
    void sigCardNum(QString);

    void sigManualMode();

public slots:
    void run();

private:
};

#endif // OBJPUTCARD_H
