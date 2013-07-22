#ifndef PRINT_H
#define PRINT_H

#include "transData.h"

typedef enum TICKET_TYPE
{
    MerchantTicket      = 1,         //商户联
    CardHolderTicket    = 2          //持卡人联
}TicketType;

void PrintTransTicket(NormalTrans *pNormalTrans, TicketType ucTicketType, bool isReprint);
void PrintSettleTicket(void);
void PrintTransDetailTicket(void);

#endif // PRINT_H
