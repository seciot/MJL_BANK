#include "iccsmart.h"
#include "sand_icc.h"

ICC_ORDER IccOrder;
union ORDER_IN_TYPE     OrderType;
union AS_ORDER_TYPE     *pAsType;
SYNC_ORDER_TYPE         *pSyncOrder;
ICC_ANSWER              *pAnswer;

unsigned char SMART_Detect(unsigned char ucReader)
{
    /* Card Detection */
    if (Os__ICC_detect(ucReader) != 0)
    {
        return SMART_ABSENT;
    }
    return SMART_PRESENT;
}

unsigned char SMART_SelectProtocol(unsigned char ucReader, unsigned char ucProtocol)
{
    /* Protocol Type */
    IccOrder.order = ASYNC_PROTOCOL;
    IccOrder.pt_order_in = &OrderType;
    IccOrder.pt_order_in->async_protocol = 0;
    if(ucProtocol == PROT_T1)
    {
        IccOrder.pt_order_in->async_protocol = 1;
    }

    /* Send Command */
    pAnswer = Os__ICC_command(ucReader, &IccOrder);

    /* Answer Command */
    if(pAnswer->drv_status != OK)
    {
        return (SMART_DRIVER_ERROR);
    }
    if(pAnswer->card_status != ASY_OK)
    {
        return (SMART_CARD_ERROR);
    }
    return (SMART_OK);
}

unsigned char SMART_ISO(unsigned char ucReader,
                        unsigned char ucCardType,
                        unsigned char* pucIn,
                        unsigned char* pucOut,
                        unsigned short* puiLengthOut,
                        unsigned char ucOrderType)
{
    unsigned char ucLength;
    unsigned short uiAdd;

    /* Order to execute */
    IccOrder.ptout = pucOut;
    IccOrder.pt_order_in = &OrderType;
    if(ucCardType == SMART_ASYNC)
    {
        IccOrder.order = ASYNC_ORDER;
        IccOrder.pt_order_in->async_order.order_type = ucOrderType;
        pAsType = &IccOrder.pt_order_in->async_order.as_order;
        IccOrder.pt_order_in->async_order.NAD = 0x00;
        switch(ucOrderType)
        {
        case TYPE0:
            pAsType->order_type0.Lc = *pucIn;       /* Length of Data transmitted */
            pAsType->order_type0.ptin = pucIn+1;    /* Data transmitted           */
            break;
        case TYPE1:
            pAsType->order_type1.CLA = *pucIn;
            pAsType->order_type1.INS = *(pucIn+1);
            pAsType->order_type1.P1  = *(pucIn+2);
            pAsType->order_type1.P2  = *(pucIn+3);
            break;
        case TYPE2:
            pAsType->order_type2.CLA = *pucIn;
            pAsType->order_type2.INS = *(pucIn+1);
            pAsType->order_type2.P1  = *(pucIn+2);
            pAsType->order_type2.P2  = *(pucIn+3);
            pAsType->order_type2.Le  = *(pucIn+4);  /* Length of data received   */
            break;
        case TYPE3:
            pAsType->order_type3.CLA = *pucIn;
            pAsType->order_type3.INS = *(pucIn+1);
            pAsType->order_type3.P1  = *(pucIn+2);
            pAsType->order_type3.P2  = *(pucIn+3);
            pAsType->order_type3.Lc  = *(pucIn+4);  /* Length of data transmitted*/
            pAsType->order_type3.ptin = pucIn+5;    /* Data transmitted          */
            break;
        case TYPE4:
            pAsType->order_type4.CLA = *pucIn;
            pAsType->order_type4.INS = *(pucIn+1);
            pAsType->order_type4.P1  = *(pucIn+2);
            pAsType->order_type4.P2  = *(pucIn+3);
            pAsType->order_type4.Lc  = *(pucIn+4);  /* Length of data transmitted*/
            pAsType->order_type4.ptin = pucIn+5;    /* Data transmitted          */
                                                    /* Length of data received   */
            pAsType->order_type4.Le  = *(pucIn+5+ *(pucIn+4));
            break;
        default:
            return (SMART_CARD_ERROR);
        }
    }
    else
    {
        IccOrder.order = SYNC_ORDER;
        pSyncOrder = &IccOrder.pt_order_in->sync_order;

        switch(*pucIn)                                  /* Address            */
        {
        case 0x05:                                      /* SLE4418 ou SLE4428 */
        case 0x06:                                      /* SLE4432 ou SLE4442 */
            uiAdd = (*(pucIn+2) << 8) | (*(pucIn+3) & 0x00FF);
            uiAdd *= 8;                                 /* Convert to bits    */
            pSyncOrder->ADDH = uiAdd >> 8;             /* P1                 */
            pSyncOrder->ADDL = uiAdd & 0x00FF;         /* P2                 */
            break;
        default:                                        /* Other cards        */
            pSyncOrder->ADDH = *(pucIn+2);             /* P1                 */
            pSyncOrder->ADDL = *(pucIn+3);             /* P2                 */
            break;
        }

        switch(*pucIn)                                  /* Data lenght        */
        {
        case 0x04:                                      /* GFM2K              */
        case 0x05:                                      /* SLE4418 ou SLE4428 */
        case 0x06:                                      /* SLE4432 ou SLE4442 */
            ucLength = *(pucIn+4);
            ucLength *= 8;                              /* Length * 8         */
            break;
        default:                                        /* Other cards        */
            ucLength = *(pucIn+4);
            break;
        }

        switch(ucOrderType)
        {
        case TYPE2:
            pSyncOrder->card_type = *pucIn;     /* Synchronous Type           */
            pSyncOrder->INS = *(pucIn+1);       /* INS                        */
            /* P1                         */
            /* P2                         */
            pSyncOrder->Len = ucLength;         /* Length of data received    */
            break;
        case TYPE3:
            pSyncOrder->card_type = *pucIn;     /* Synchronous Type           */
            pSyncOrder->INS = *(pucIn+1);       /* INS                        */
            /* P1                         */
            /* P2                         */
            pSyncOrder->Len = ucLength;         /* Length of data transmitted */
            pSyncOrder->ptin = pucIn+4;         /* Data transmitted           */
            break;
        default:
            return (SMART_CARD_ERROR);
        }
    }

    /* Send Command */
    pAnswer = Os__ICC_command(ucReader, &IccOrder);

    /* Answer Command */
    /******************/
    if(pAnswer->drv_status != OK)
    {
        return (SMART_DRIVER_ERROR);
    }
    if(pAnswer->card_status != ASY_OK)
    {
        return (SMART_CARD_ERROR);
    }
    if(pAnswer->Len > *puiLengthOut)
    {
        return (SMART_OWERFLOW);
    }
    *puiLengthOut = pAnswer->Len;
    return (SMART_OK);
}

unsigned char SMART_PowerOff(unsigned char ucReader)
{
    /* Power Off */
    IccOrder.order = POWER_OFF;

    /* Send Command */
    pAnswer = Os__ICC_command(ucReader, &IccOrder);

    /* Answer Command */
    if(pAnswer->drv_status != OK)
    {
        return (SMART_DRIVER_ERROR);
    }
    if(pAnswer->card_status != ASY_OK)
    {
        return (SMART_CARD_ERROR);
    }
    return (SMART_OK);
}

unsigned char SMART_Remove(unsigned char ucReader)
{
    unsigned char ucRet;

    /* Remove Card */
    if((ucRet = SMART_PowerOff(ucReader)) == SMART_OK)
    {
        while(SMART_Detect(ucReader) == SMART_PRESENT)
        {/*NOP*/;}
    }
    return (ucRet);
}

unsigned char SMART_WarmReset(unsigned char ucReader,
                              unsigned char ucCardStandard,
                              unsigned char *pucReset,
                              unsigned short *puiLength)
{
    unsigned char ucCardType;

    /* Wait for a Card */
    IccOrder.pt_order_in = &OrderType;
    IccOrder.ptout = pucReset;
    IccOrder.order = NEW_WARM_RESET;
    IccOrder.pt_order_in->new_power.card_standards = ucCardStandard;
    IccOrder.pt_order_in->new_power.GR_class_byte_00 = 1;
    IccOrder.pt_order_in->new_power.preferred_protocol = 0xFF;

    /* Send Command */
    pAnswer = Os__ICC_command(ucReader, &IccOrder);/*发送读写卡命令SMART卡或内部SIM卡*/
    /* Answer Command */
    if(pAnswer->drv_status != OK)
    {
        return (SMART_DRIVER_ERROR);
    }
    switch(pAnswer->card_status)
    {
    case ICC_ASY:
    case ICC_SYN:
        ucCardType = pAnswer->card_status;
        break;
    default:
        return (SMART_CARD_ERROR);
    }
    if(pAnswer->Len > *puiLength)
    {
        return (SMART_OWERFLOW);
    }
    *puiLength = pAnswer->Len;
    return (ucCardType);
}
