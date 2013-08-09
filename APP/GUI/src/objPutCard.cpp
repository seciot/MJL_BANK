#include "objputcard.h"
#include "eventProcess.h"
#include "msg.h"
#include <QDebug>
#include "MIFAREPro.h"
#include "global.h"

#define PUTCARD_DEBUG_FLAG

static void GetTestTrackInfo(void);
static unsigned char GetTrackInfo(unsigned char *pucTrack);

objPutCard::objPutCard(QObject *parent) :
    QObject(parent)
{
    qDebug()<<Q_FUNC_INFO;

}

objPutCard::~objPutCard()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
    RemoveKeyEventBug();
}

unsigned char objPutCard::putCardProcess()
{
    qDebug()<<Q_FUNC_INFO;
    unsigned char ucResult = 0;
    unsigned char aucBuf[20];
    unsigned short usLength = 0;

    while(1)
    {
        ucResult = EventProcess(true, false, true);
        if(ucResult)
            break;

        if(getEventType() == Mag)
        {
            qDebug()<<"Mag is swiped";
            pxMagObj=GetPxMag();
            qDebug("\n+++++ SwipeCard [MAG EVENT] --------+++++++++++++++++\n");
            unsigned char length1 = pxMagObj->xxdata[0];
            unsigned char length2 = pxMagObj->xxdata[length1+2];
            if((length1==0)&&(length2==0))
                return(ERR_MAG_TRACKDATA);
            else
            {
                if(GetTrackInfo((unsigned char*)pxMagObj) != 0)
                    return(ERR_NOTPROC);
            }
            break;
        }
        else if(getEventType() == Untouch)
        {
            memset(aucBuf, 0, sizeof(aucBuf));
            ucResult = MIFARE_readCardNo(GetMifare(), 254, aucBuf, &usLength);
            QString tmp=QString::fromAscii((const char*)aucBuf);
            qDebug()<<"tmp::"<<tmp<<tmp.size();
            if(tmp.size()<5)
                return ERR_MFCDRV;
            qDebug("card is %s", aucBuf);
            qDebug("card len is %d", usLength);
            if(ucResult)
                return ERR_MFCDRV;
            else
            {
                memset(G_NORMALTRANS_aucSourceAcc, 0, sizeof(G_NORMALTRANS_aucSourceAcc));
                memcpy(G_NORMALTRANS_aucSourceAcc, aucBuf, usLength);
                G_NORMALTRANS_ucMainAccountLen = usLength;
                break;
            }
        }
        else if(getEventType() == Key)
        {
            ucResult = KEYDrv_Process();

            if(ucResult==ERR_CANCEL)
            {
                break;
            }
            if(ucResult==ERR_CARDNOACT)
            {
                // 没有手动
                if(NormalTransData.transType==TransMode_CashAdvance && g_constantParam.advance.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_CashDeposit && g_constantParam.deposit.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_BalanceInquiry && g_constantParam.balance.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_CardTransfer && g_constantParam.p2p.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_AdvanceVoid && g_constantParam.transvoid.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_DepositVoid && g_constantParam.transvoid.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }
                if(g_constantParam.adjust.MANUAL_ENABLE == false
                && (NormalTransData.transType == TransMode_AdvanceAdjust
                 || NormalTransData.transType == TransMode_DepositAdjust))
                {
                     ucResult = 0;
                }
                if(NormalTransData.transType==TransMode_PINChange && g_constantParam.pinchange.MANUAL_ENABLE==false)
                {
                     ucResult = 0;
                }


                // 进入手动
                if(NormalTransData.transType==TransMode_CashAdvance && g_constantParam.advance.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_CashDeposit && g_constantParam.deposit.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_BalanceInquiry && g_constantParam.balance.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_CardTransfer && g_constantParam.p2p.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_AdvanceVoid && g_constantParam.transvoid.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_DepositVoid && g_constantParam.transvoid.MANUAL_ENABLE==true)
                {
                    break;
                }
                if(g_constantParam.adjust.MANUAL_ENABLE == true
                && (NormalTransData.transType == TransMode_AdvanceAdjust
                 || NormalTransData.transType == TransMode_DepositAdjust))
                {
                    break;
                }
                if(NormalTransData.transType==TransMode_PINChange && g_constantParam.pinchange.MANUAL_ENABLE==true)
                {
                    break;
                }
            }
        }
        else
        {
            ucResult = ERR_UNKNOWNDRV;
            break;
        }
    }
    RemoveKeyEventBug();
    return ucResult;
}

void objPutCard::run()
{

    int ucResult=this->putCardProcess();
    qDebug()<<Q_FUNC_INFO<<ucResult;

    switch(ucResult)
    {
    case SUCCESS_TRACKDATA:
        emit putCardFinished();
        break;
    case ERR_MAG_TRACKDATA:
    case ERR_NOTPROC:
        emit putNotSupportCard();
        break;
    case ERR_CARDNOACT:
        qDebug()<<"切换为手动";
        emit sigManualMode();
        break;
    default:
        qDebug()<<"ERR_CANCEL";
        emit putCardFailed();
        break;

    }
}

void GetTestTrackInfo(void)
{
    qDebug()<<Q_FUNC_INFO;
#ifdef PUTCARD_DEBUG_FLAG
    //2磁道数据
//    G_EXTRATRANS_uiISO2Len = sprintf((char*)G_EXTRATRANS_aucISO2, "%s",
//                                     (char *)"706001000052170512=1711");

//    G_EXTRATRANS_uiISO2Len = sprintf((char*)G_EXTRATRANS_aucISO2, "%s",
//                                     (char *)"706001000052170512=171152610220524");

    G_EXTRATRANS_uiISO2Len = sprintf((char*)G_EXTRATRANS_aucISO2, "%s",
                                     (char *)"706001000004340825D171152611777295");

#endif
}

unsigned char GetTrackInfo(unsigned char *pucTrack)
{
    qDebug() << Q_FUNC_INFO;
    unsigned short uiI = 0,uiISO2Len = 0,uiISO3Len = 0;
    unsigned char  *pucPtr = NULL;
    unsigned char ucJ = 0;
    unsigned char ucISO2Status = 0,ucISO3Status = 0;
    pucPtr = pucTrack;
    pucPtr += 2;

    /* Track2 information */
    ucISO2Status = *(pucPtr);
    pucPtr ++;
    uiISO2Len = *(pucPtr);
    G_EXTRATRANS_uiISO2Len=*(pucPtr);
    qDebug("\nucISO2Status=%02x",ucISO2Status);
    qDebug("\nuiISO2Len=%d\n",uiISO2Len);
    /***判断二磁道数据是否正确***/
    if(ucISO2Status != SUCCESS_TRACKDATA)
        return ERR_MAG_TRACKDATA;
    if(uiISO2Len > TRANS_ISO2LEN)
        return(ERR_MAG_TRACKDATA);
    pucPtr ++;
    for( uiI=0; uiI<uiISO2Len; uiI++,pucPtr ++)
    {
        G_EXTRATRANS_aucISO2[uiI] =  *(pucPtr) | 0x30;
    }
    qDebug("\nTrack 2 length[%d]Data:[%s]",G_EXTRATRANS_uiISO2Len,G_EXTRATRANS_aucISO2);


    /* Track3 information */
    ucISO3Status = *(pucPtr);
    pucPtr ++;
    uiISO3Len = *(pucPtr);
    pucPtr ++;
    qDebug("\n ucISO3Status=%02x",ucISO3Status);
    qDebug("\n uiISO3Len=%d\n",uiISO3Len);
    for( uiI=0; uiI<uiISO3Len; uiI++,pucPtr ++)
    {
        G_EXTRATRANS_aucISO3[uiI] =  *(pucPtr) | 0x30;
    }
    //G_NORMALTRANS_ucUAAISOLen3 =uiISO3Len;
    G_EXTRATRANS_uiISO3Len=uiISO3Len;
    qDebug("\n Track 3 length[%d]Data:[%s]",G_EXTRATRANS_uiISO3Len,G_EXTRATRANS_aucISO3);
    /***过滤三磁道状态不正确和磁道数据超长***/
    if((ucISO3Status != SUCCESS_TRACKDATA)
            ||(uiISO3Len > TRANS_ISO3LEN))
    {
        G_EXTRATRANS_uiISO3Len = 0;
        qDebug("\nTrack 3 length=%d",G_EXTRATRANS_uiISO3Len);
    }

    // 测试卡号
    if(g_changeParam.simCard==true)
        GetTestTrackInfo();

    for(ucJ=0;ucJ<G_EXTRATRANS_uiISO2Len;ucJ++)
    {
        if(G_EXTRATRANS_aucISO2[ucJ] == '='
        || G_EXTRATRANS_aucISO2[ucJ] == 'd'
        || G_EXTRATRANS_aucISO2[ucJ] == 'D')
        {
            /*************取卡号***************/
            G_NORMALTRANS_ucMainAccountLen = ucJ;
            memcpy(G_NORMALTRANS_aucSourceAcc, G_EXTRATRANS_aucISO2, ucJ);
            memcpy(NormalTransData.ucExpiredDate, &G_EXTRATRANS_aucISO2[ucJ+1], 4);
            if(ucJ>=13)
                asc_bcd(G_EXTRATRANS_aucCardPan_UnAssign,6,&G_EXTRATRANS_aucISO2[ucJ-13],12);
            else
                asc_bcd(G_EXTRATRANS_aucCardPan_UnAssign,6,&G_EXTRATRANS_aucISO2[0],12);
            break;
        }
    }

    qDebug("Card Number = %s", G_NORMALTRANS_aucSourceAcc);
    qDebug("Card Pan = %s", G_EXTRATRANS_aucCardPan_UnAssign);

    return(SUCCESS);
}
