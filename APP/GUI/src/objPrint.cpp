#include "objPrint.h"
#include "msg.h"
#include "print.h"
#include <QDebug>
#include "global.h"

objPrint::objPrint(QObject *parent) :
    QObject(parent)
{
    qDebug()<<Q_FUNC_INFO;

}

objPrint::~objPrint()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}


void objPrint::printReceipt()
{
    qDebug()<<Q_FUNC_INFO;
    //一联
    PrintTransTicket(&NormalTransData, MerchantTicket, false);
    qDebug()<<"请撕纸";
    emit sigTear();
    //二联
    PrintTransTicket(&NormalTransData, CardHolderTicket, false);
    emit sigPrintComplete();
}

void objPrint::rePrintReceipt()
{
    qDebug()<<Q_FUNC_INFO;
    //一联
    PrintTransTicket(&NormalTransData, MerchantTicket, true);
    qDebug()<<"请撕纸";
    emit sigTear();
    //二联
    PrintTransTicket(&NormalTransData, CardHolderTicket, true);
    emit sigPrintComplete();
}
