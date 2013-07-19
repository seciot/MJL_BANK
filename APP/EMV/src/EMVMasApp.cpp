#include <stddef.h>
#include "msg.h"

extern "C"
{
#include "ostools.h"
#include "iccsmart.h"
#include "EMVInterface.h"
#include "EMVGlobal.h"
unsigned char PROCESSRESTRICT_Process();
}


#if 1
#define  TRANS_CARDTYPE_INTERNAL            0
#define  TRANS_CARDTYPE_VISA                1
#define  TRANS_CARDTYPE_MASTER              2
#define  TRANS_CARDTYPE_JCB                 3
#define  TRANS_CARDTYPE_DINERS              4
#define  TRANS_CARDTYPE_AE                  5
#define  TRANS_CARDTYPE_FOREIGN             6
#define  TRANS_CARDTYPE_INTERNALSMART       10

unsigned char ucCardType;
unsigned char aucAID[EMVAIDLEN];
unsigned char ucSourceAccLen;
unsigned char aucSourceAcc[19];

#define G_NORMALTRANS_aucAID            aucAID
#define G_NORMALTRANS_ucCardType        ucCardType
#define G_NORMALTRANS_aucSourceAcc      aucSourceAcc
#define G_NORMALTRANS_ucSourceAccLen    ucSourceAccLen

#endif

#define MAXDISPROWS         8

unsigned char IccIsoCommand(unsigned char slot, EMVICCIN *pEMVICCIn, EMVICCOUT *pEMVICCOut)
{
    unsigned char ucResult;
    unsigned char ucReader, ucCase;
    unsigned short uiLen = 0;
    unsigned char aucInBuf[300];
    unsigned char aucOutBuf[300];

    ucReader = slot;
    memset(aucOutBuf, 0, sizeof(aucOutBuf));
    memset(aucInBuf, 0, sizeof(aucInBuf));
    memcpy(aucInBuf, &pEMVICCIn->ucCla, 4);

    if(pEMVICCIn->uiLc)
    {
        aucInBuf[4] = pEMVICCIn->uiLc;
        memcpy(&aucInBuf[5], pEMVICCIn->aucDataIn, pEMVICCIn->uiLc);
        aucInBuf[5 + pEMVICCIn->uiLc] = pEMVICCIn->uiLe%256;
    }
    else
    {
        aucInBuf[4] = pEMVICCIn->uiLe%256;
    }

    if(pEMVICCIn->uiLc)
    {
        if(pEMVICCIn->uiLe)
            ucCase = TYPE4;
        else
            ucCase = TYPE3;
    }
    else
    {
        if(pEMVICCIn->uiLe)
            ucCase = TYPE2;
        else
            ucCase = TYPE1;
    }

    uiLen = sizeof(aucOutBuf);
    ucResult = SMART_ISO(ucReader, SMART_ASYNC, aucInBuf, aucOutBuf, &uiLen, ucCase);
    if(ucResult != SMART_OK)
        return(ucResult);

    memset(pEMVICCOut->aucDataOut, 0, 300);
    pEMVICCOut->uiLenOut = uiLen-2;
    memcpy(pEMVICCOut->aucDataOut, aucOutBuf, pEMVICCOut->uiLenOut);
    pEMVICCOut->ucSWA = aucOutBuf[uiLen-2];
    pEMVICCOut->ucSWB = aucOutBuf[uiLen-1];

    return(SMART_OK);
}

unsigned char EMVICC(EMVICCIN *pEMVICCIn ,EMVICCOUT *pEMVICCOut)
{
    return IccIsoCommand(0, pEMVICCIn, pEMVICCOut);
}

unsigned char CardHolderConfirmApp(PCANDIDATELIST pCandidateList, PUCHAR pucAppIndex)
{
    unsigned char ucI, ucCurPage, ucBeginIndex;
    unsigned char aucBuff[30], ucKey;
    PCANDIDATEAPP pCandidateApp;

    ucCurPage = 0;
/*
    bDispFlag = TRUE;
    for(ucI=0;ucI<pCandidateList->CandidateNums;ucI++)
    {
        pCandidateApp =&pCandidateList->CandidateApp[ucI];
        if(pCandidateApp->ucADFNameLen*2 > MAXDISPCHAR-2)
        {
            bDispFlag = FALSE;
            break;
        }
    }
*/
#if 0
    do
    {
        ucBeginIndex =ucCurPage*MAXDISPROWS;
        Os__clr_display(255);
        for(ucI =0; ucI <MAXDISPROWS; ucI++)
        {
            if(ucI+ucBeginIndex >= pCandidateList->CandidateNums) break;
            pCandidateApp =&pCandidateList->CandidateApp[ucI+ucBeginIndex];
            memset(aucBuff,0x00,sizeof(aucBuff));
            aucBuff[0] = ucI+ucBeginIndex+0x31;
            aucBuff[1] = '.';
            if(pCandidateApp->bICTIExist
               &&(pCandidateApp->ucICTI ==0x01)
               &&pCandidateApp->ucAppPreferNameLen)
                memcpy(&aucBuff[2],pCandidateApp->aucAppPreferName,pCandidateApp->ucAppPreferNameLen);
            else if(pCandidateApp->ucAppLabelLen)
                memcpy(&aucBuff[2],pCandidateApp->aucAppLabel,pCandidateApp->ucAppLabelLen);
            else if(pCandidateApp->ucADFNameLen)
                CONV_HexAsc(&aucBuff[2],pCandidateApp->aucADFName,pCandidateApp->ucADFNameLen*2);

//            if(bDispFlag)
//                Os__GB2312_display(ucI,0,aucBuff);
//            else
                OSMMI_DisplayASCII(0x30,ucI,0,aucBuff);
        }

        ucKey = Os__xget_key();
        if(ucKey ==KEY_ENTER)
        {
            *pucAppIndex =0;
            return EMVERROR_SUCCESS;
        }
        else if(ucKey == KEY_CLEAR)
            return EMVERROR_CANCEL;
        else if((ucKey>'0')&&(ucKey < pCandidateList->CandidateNums +'1'))
        {
            *pucAppIndex = ucKey -0x31;
            return EMVERROR_SUCCESS;
        }

        if(ucKey == KEY_F3 &&ucCurPage)
            ucCurPage --;
        if(ucKey == KEY_F4 && (ucCurPage+1)*MAXDISPROWS<pCandidateList->CandidateNums)
            ucCurPage++;
    }while(1);
#endif
}

unsigned char MasAPP_CheckAID(void)
{
    unsigned char ucResult, ucBuf[EMVAIDLEN];
    unsigned short uiLen;

    const TERMSUPPORTAPP TermSupportAID[] =
    {
        {0,5,"\xD1\x56\x00\x00\x01"},           //CUP
        {0,5,"\xA0\x00\x00\x03\x33"},           //CUP
        {0,7,"\xA0\x00\x00\x00\x03\x10\x10"},   //Visa - VSDC,also used in EMV test.
        {0,7,"\xA0\x00\x00\x00\x99\x90\x90"},   //EMV - test
        {0,7,"\xA0\x00\x00\x00\x03\x20\x10"},   //Visa - Electron
        {0,7,"\xA0\x00\x00\x00\x04\x10\x10"},   //Mastercard - M/Chip
        {0,7,"\xA0\x00\x00\x00\x04\x30\x60"},   //Mastercard - Maestro
        {0,7,"\xA0\x00\x00\x00\x04\x60\x00"},   //Mastercard - Cirrus
        {0,7,"\xA0\x00\x00\x00\x04\x60\x10"},   //Mastercard - app
        {0,7,"\xA0\x00\x00\x00\x10\x10\x30"},   //Mastercard - eurocheque
        {0,7,"\xA0\x00\x00\x00\x65\x10\x10"}    //JCB - J/Smart AID.
    };

    memset(ucBuf, 0, sizeof(ucBuf));
    uiLen = EMVAIDLEN;
    if(ucResult = EMVTRANSTAG_GetTagValue(ALLPHASETAG, (PUCHAR)"\x4F", ucBuf, &uiLen))
        return ucResult;

    memcpy(G_NORMALTRANS_aucAID, ucBuf, uiLen);
    if(!memcmp(TermSupportAID[0].aucAID,G_NORMALTRANS_aucAID,5)
        ||!memcmp(TermSupportAID[1].aucAID,G_NORMALTRANS_aucAID,5))
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_INTERNAL;//内卡
    }else if(!memcmp(TermSupportAID[2].aucAID,G_NORMALTRANS_aucAID,5))
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_VISA;//VISA
    }
    else if(!memcmp(TermSupportAID[5].aucAID,G_NORMALTRANS_aucAID,5))
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_MASTER;//MASTER
    }
    else if(!memcmp(TermSupportAID[10].aucAID,G_NORMALTRANS_aucAID,TermSupportAID[10].ucAIDLen))
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_JCB;//JCB
    }
    else if(!memcmp(TermSupportAID[9].aucAID,G_NORMALTRANS_aucAID,7))
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_MASTER;//MASTER
    }
    else
    {
        G_NORMALTRANS_ucCardType = TRANS_CARDTYPE_FOREIGN;//外卡
    }
}

unsigned int CalcLogTransAmount(void)
{
#if 0
    ULONG uiLogAmount;
    UCHAR ucResult, aucCardNo[20];
    USHORT uiLen, uiIndex;
    NORMALTRANS tempNormorTrans;

    ucResult = SUCCESS;
    uiLogAmount = 0;

    memcpy(&tempNormorTrans,&NormalTransData,sizeof(NORMALTRANS));
    memset(aucCardNo,0,sizeof(aucCardNo));
    uiLen = G_NORMALTRANS_ucSourceAccLen;
    memcpy(aucCardNo,G_NORMALTRANS_aucSourceAcc,G_NORMALTRANS_ucSourceAccLen);
    for(uiIndex=0;uiIndex< DataSave0.TransInfoData.ulTransNumber;uiIndex++)
    {
        GetOldTransData(uiIndex);
        if(!memcmp(aucCardNo,G_NORMALTRANS_aucSourceAcc,(uiLen+1)/2))
        {
            if(G_NORMALTRANS_ucOptOutFlag)
            {
                uiLogAmount += G_NORMALTRANS_ulAmount;
            }
            else
            {
                uiLogAmount += G_NORMALTRANS_ulTrueAmount;
            }
        }
    }
    memcpy(&NormalTransData, &tempNormorTrans, sizeof(NORMALTRANS));
    return uiLogAmount;
#endif
}

BOOL TransCapture(TRANSRESULT enTransResult)
{

    if(enTransResult == OFFLINEAPPROVED
        ||enTransResult == UNABLEONLINE_OFFLINEAPPROVED)
    {
//        G_NORMALTRANS_ucTransType = TRANS_OFFPURCHASE;
        MasAPP_CheckAID();
    }

    Uart_Printf((char *)"\nenTransResult=%d\n", enTransResult);
    switch(enTransResult)
    {
    case ONLINEAPPROVED:
    case OFFLINEAPPROVED:
    case UNABLEONLINE_OFFLINEAPPROVED:
        return TRUE;
    case OFFLINEDECLINED:
    case ONLINEDECLINED:
    case UNABLEONINE_OFFLINEDECLINED:
        return FALSE;
    }
}

unsigned char MASAPP_WaitReadIC(unsigned char ucInputMode)
{
    unsigned char ucResult;
#if 0
    unsigned int uiTimesOut;
    unsigned char ucKey;
    unsigned char ucICCardType;

    DRV_OUT *pxIcc;
    DRV_OUT *pxKey;
    UCHAR aucResetBuf[50],ucResetBufLen;

    Os__abort_drv(drv_mmi);
    Os__abort_drv(drv_mag);
    Os__abort_drv(drv_icc);

    while(1)
    {
         MAG_DisplaySwipeCardMode( G_NORMALTRANS_ucTransType, TRANS_INPUTMODE_INSERTCARD);
        pxKey = Os__get_key();
        if(ucInputMode& TRANS_INPUTMODE_INSERTCARD)
        {
            pxIcc = Os__ICC_insert();/*等待插卡并对卡上电*/
        }
         G_NORMALTRANS_ucFallBackFlag = FALSE;

        ucResult = SUCCESS;
        uiTimesOut = 45;
        uiTimesOut = uiTimesOut*100;

        //SEL_AccumulatRunTime();

        Os__timer_start(&uiTimesOut);
        while(uiTimesOut)
        {
            if(pxKey->gen_status == DRV_ENDED)
                break;
            if((ucInputMode& TRANS_INPUTMODE_INSERTCARD)
                &&(pxIcc->gen_status == DRV_ENDED))
                break;
        }
        Os__timer_stop(&uiTimesOut);
        if(!uiTimesOut)
        {
            Os__abort_drv(drv_mmi);
            Os__abort_drv(drv_icc);
            Os__light_off();
            Os__light_off_pp();
            return(ERR_CANCEL);
        }

        if((ucInputMode& TRANS_INPUTMODE_INSERTCARD)
                    &&(pxIcc->gen_status== DRV_ENDED))
        {
            Os__abort_drv(drv_mmi);
            ucICCardType =pxIcc->xxdata[1];
            ucResetBufLen=sizeof(aucResetBuf);
            if(ucICCardType==0x39)
                ucICCardType=SMART_WarmReset(0,0,aucResetBuf,(unsigned short*)&ucResetBufLen);
            Uart_Printf("ucICCardType --1101--= %02x\n",ucICCardType);
            /*==========
            if(ucICCardType!=ICC_ASY)
            {
                return ERR_ICCARD;
            }
            =============*/
            if(ucICCardType!=ICC_ASY
                &&ucICCardType!=ICC_SYN)/*同步或异步SMART卡*/
            {
                ucInputMode&=~ TRANS_INPUTMODE_INSERTCARD;
                 G_NORMALTRANS_ucFallBackFlag =2;
                return ERR_FALLBACK;
            }else
            if(ucICCardType!=ICC_SYN)
            {
                 G_NORMALTRANS_ucInputMode= TRANS_INPUTMODE_INSERTCARD;
                 G_NORMALTRANS_euCardSpecies=CARDSPECIES_EMV;
                break;
            }
            else
            {
                return( ERR_ICCARD);
            }
            break;
        }else if(!ucResult&&(pxKey->gen_status == DRV_ENDED))
        {
            Uart_Printf("exit================\n");
            Os__abort_drv(drv_icc);
            ucKey = pxKey->xxdata[1];
            if(ucKey == KEY_CLEAR)
            {
                ucResult = ERR_CANCEL;
                break;
            }
        }
    }
    Uart_Printf("插入卡ucResult=%02x\n",ucResult);
#endif
    return ucResult;
}

unsigned char MasApp_CARDPAN(unsigned char *pstrCardNo, unsigned char *pucCardNoLen)
{
    unsigned char aucBuff_asc[20], aucBuff_bcd[10], ucI;
    unsigned short uslen;

    //modify date:080318 13:50
    memset(aucBuff_bcd, 0x00, sizeof(aucBuff_bcd));
    memset(aucBuff_asc, 0x00, sizeof(aucBuff_asc));
    uslen = MAXPANDATALEN;
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x5A", aucBuff_bcd, &uslen);
    bcd_asc(aucBuff_asc, aucBuff_bcd, uslen*2);

    for(ucI = 0; ucI < uslen*2; ucI++)
    {
        if(aucBuff_asc[ucI] == 0x3F)
        {
            aucBuff_asc[ucI] = 0x00;
            break;
        }
    }
    *pucCardNoLen = ucI;
    memcpy(pstrCardNo, aucBuff_asc, *pucCardNoLen);
    return SUCCESS_TRACKDATA;
}

void MasApp_SetEMVTransInfo(void)
{
    UCHAR ucResult , ucBuf[256];
    USHORT uiLen;
    unsigned char ucI;

    memset( ucBuf, 0, sizeof(ucBuf));
    MasApp_CARDPAN(ucBuf, &G_NORMALTRANS_ucSourceAccLen);
    asc_bcd(G_NORMALTRANS_aucSourceAcc,(G_NORMALTRANS_ucSourceAccLen+1)/2,ucBuf,((G_NORMALTRANS_ucSourceAccLen+1)/2)*2);
//    asc_bcd(G_NORMALTRANS_aucCardPan,6,ucBuf+G_NORMALTRANS_ucSourceAccLen-12-1,12);
    memset( ucBuf, 0, sizeof(ucBuf));
    uiLen =1;
    if(!(ucResult = EMVTRANSTAG_GetTagValue(ALLPHASETAG, (PUCHAR)"\x5F\x34", ucBuf, &uiLen)))
    {
//        G_NORMALTRANS_ucPANSeq = ucBuf[0];
//        G_NORMALTRANS_ucPANSeqExist = true;
    }
    memset(ucBuf, 0, sizeof(ucBuf));
    uiLen = 37;
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x57", ucBuf, &uiLen);
//    bcd_asc(G_EXTRATRANS_aucISO2, ucBuf, uiLen*2);
//    for(ucI = 0; ucI < uiLen*2; ucI++)
//    {
//        if(G_EXTRATRANS_aucISO2[ucI] == 0x3F)
//        {
//            G_EXTRATRANS_aucISO2[ucI] = 0x00;
//            break;
//        }
//    }
//    G_NORMALTRANS_ucISO2Len = G_EXTRATRANS_uiISO2Len = ucI;

//    asc_bcd(G_NORMALTRANS_aucISO2, (G_NORMALTRANS_ucISO2Len+1)/2,
//            G_EXTRATRANS_aucISO2, ((G_NORMALTRANS_ucISO2Len+1)/2)*2);

//    Uart_Printf("G_EXTRATRANS_uiISO2Len = %d\n", G_EXTRATRANS_uiISO2Len);

    uiLen = sizeof(ucBuf);
    memset(ucBuf, 0, sizeof(ucBuf));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG ,(PUCHAR)"\x5F\x20", ucBuf, &uiLen);
//    memcpy(G_NORMALTRANS_aucCardHolderName, ucBuf, uiLen);

    memset(ucBuf, 0, sizeof(ucBuf));
    uiLen = 3;
    ucResult = EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x5F\x24", ucBuf, &uiLen);
    if(!ucResult)
    {
//        bcd_asc(&G_NORMALTRANS_ucExpiredDate[0], ucBuf, 4);
        //memcpy( &G_NORMALTRANS_ucExpiredDate[0], ucBuf , 3);
        //Uart_Printf("G_NORMALTRANS_ucExpiredDate: %02x%02x%02x\n",ucBuf[0],ucBuf[1],ucBuf[2]);
    }
}

unsigned char  MasApp_CopytEMVTransInfoToNormalTrans(void)
{
    unsigned char ucBuf[256];
    USHORT uiLen;
#if 1
    //---------- AC ------------
    uiLen = sizeof(ucBuf);
    memset(ucBuf, 0, sizeof(ucBuf));
    if(EMVTransInfo.enTransResult == ONLINEAPPROVED)
        EMVTRANSTAG_GetTagValue(SECONDGACPHASETAG, (PUCHAR)"\x9F\x26", ucBuf, &uiLen);
    else if(EMVTransInfo.enTransResult == OFFLINEAPPROVED)
        EMVTRANSTAG_GetTagValue(FIRSTGACPHASETAG, (PUCHAR)"\x9F\x26", ucBuf, &uiLen);
    else if(EMVTransInfo.enTransResult == UNABLEONLINE_OFFLINEAPPROVED)
        EMVTRANSTAG_GetTagValue(SECONDGACPHASETAG, (PUCHAR)"\x9F\x26", ucBuf, &uiLen);

//    if(uiLen)
//        memcpy(G_NORMALTRANS_AppCrypt, ucBuf, 8);
    //---------- AID -------------------
    uiLen = EMVAIDLEN-1;
    memset(ucBuf, 0, sizeof(ucBuf));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x4F", ucBuf, &uiLen);
//    G_NORMALTRANS_ucAIDLen = uiLen;
//    if(uiLen)
//        memcpy(G_NORMALTRANS_aucAID, ucBuf, G_NORMALTRANS_ucAIDLen);
    //---------- TVR ------------
//    memset(G_NORMALTRANS_TVR, 0,sizeof(G_NORMALTRANS_TVR));
//    memcpy(G_NORMALTRANS_TVR, EMVTransInfo.EMVTVR, 5);
    //---------- TSI ------------
//    memcpy(G_NORMALTRANS_TSI , EMVTransInfo.EMVTSI, 5);
    //---------- ATC  ------------
    uiLen = sizeof(ucBuf);
    memset( ucBuf,  0, sizeof(ucBuf));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x9F\x36",ucBuf,&uiLen);
//    if(uiLen)
//        memcpy( G_NORMALTRNAS_ATC, ucBuf, 2);
    //---------- CVM -------	-----
//    memcpy(&G_NORMALTRNAS_CVM[0] , &EMVTransInfo.CVMResult.ucCVMCode, sizeof(uchar));
//    memcpy(&G_NORMALTRNAS_CVM[1] , &EMVTransInfo.CVMResult.ucConditionCode, sizeof(uchar));
//    memcpy(&G_NORMALTRNAS_CVM[2] , &EMVTransInfo.CVMResult.ucCVMResult, sizeof(uchar));
    //---------- App Label ------------
    uiLen = sizeof(ucBuf);
    memset( ucBuf,  0, sizeof(ucBuf));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x50",ucBuf,&uiLen);
//    G_NORMALTRANS_ucAppLabelLen = uiLen;
//    if(uiLen)
//        memcpy(G_NORMALTRANS_aucAppLabel, ucBuf, 16);
    //---------- Application Prefer Name  ------------
    uiLen = sizeof(ucBuf);
    memset( ucBuf,  0, sizeof(ucBuf));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x9F\x12",ucBuf,&uiLen);
//    G_NORMALTRANS_ucAppPreferNameLen = uiLen;
//    if(uiLen)
//        memcpy(G_NORMALTRANS_aucAppPreferName, ucBuf, 16);

    memset(ucBuf, 0, sizeof(ucBuf));
    uiLen = sizeof(ucBuf);
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x89",ucBuf,&uiLen);//Auth  Code

    Uart_Printf("\n\n *********************************************** ");
    Uart_Printf("\n|   AUTH CODE: %02x",ucBuf[0]);
    Uart_Printf("\n| 	EMVTransInfo.enTransResult : %02X",EMVTransInfo.enTransResult);
    memset( ucBuf,  0, sizeof(ucBuf));
//    hex_asc(ucBuf,G_NORMALTRANS_AppCrypt,sizeof(G_NORMALTRANS_AppCrypt)*2);
    Uart_Printf("\n| 	TC:  %s ",ucBuf);
    memset( ucBuf,  0, sizeof(ucBuf));
//    hex_asc(ucBuf,G_NORMALTRANS_aucAID,G_NORMALTRANS_ucAIDLen*2);
#if 0
    Uart_Printf("\n| 	AID: %s",ucBuf);
    Uart_Printf("\n| 	TVR: %02X %02X %02X %02X %02X",EMVTransInfo.EMVTVR[0],EMVTransInfo.EMVTVR[1],
                                                    EMVTransInfo.EMVTVR[2],EMVTransInfo.EMVTVR[3],
                                                    EMVTransInfo.EMVTVR[4]);
    Uart_Printf("\n| 	TSI: %02X %02X ",EMVTransInfo.EMVTSI[0],EMVTransInfo.EMVTSI[1]);
    Uart_Printf("\n| 	ATC: %02X %02X",G_NORMALTRNAS_ATC[0],G_NORMALTRNAS_ATC[1]);
    Uart_Printf("\n| 	CVM: %02X %02X %02X",EMVTransInfo.CVMResult.ucCVMCode,EMVTransInfo.CVMResult.ucConditionCode,EMVTransInfo.CVMResult.ucCVMResult);
    Uart_Printf("\n| 	App Label: %s",G_NORMALTRANS_aucAppLabel);
    Uart_Printf("\n| 	App Prefer Name: %s",G_NORMALTRANS_aucAppPreferName);
    Uart_Printf("\n *********************************************** \n\n");
#endif
    //if(DataSave0.ConstantParamData.uICCInfo=='0')
        return SUCCESS_TRACKDATA;
#if 0
    //----- LCD DISPLAY -----
    Os__clr_display(255);
    OSMMI_DisplayASCII(0x30,0,0, (uchar *)"--- IC Trans Info -----");

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "AID:",4 );
    hex_asc(&ucBuf[4], G_NORMALTRANS_aucAID, G_NORMALTRANS_ucAIDLen*2);
    OSMMI_DisplayASCII(0x30,1,0, ucBuf);

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "TVR:",4 );
    hex_asc(&ucBuf[4], EMVTransInfo.EMVTVR, 10);
    OSMMI_DisplayASCII(0x30,2,0, ucBuf);

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "TSI:",4 );
    hex_asc(&ucBuf[4], EMVTransInfo.EMVTSI, 4);
    OSMMI_DisplayASCII(0x30,3,0, ucBuf);

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "CVM:",4 );
    hex_asc(&ucBuf[4], (uchar *)EMVTransInfo.CVMResult.ucCVMCode, 2);
    hex_asc(&ucBuf[6], (uchar *)EMVTransInfo.CVMResult.ucConditionCode, 2);
    hex_asc(&ucBuf[8], (uchar *)EMVTransInfo.CVMResult.ucCVMResult, 2);
    OSMMI_DisplayASCII(0x30,4,0, ucBuf);

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "App Label:",9 );
    memcpy(&ucBuf[9] ,G_NORMALTRANS_aucAppLabel , 16);
    OSMMI_DisplayASCII(0x30,5,0, ucBuf);

    memset( ucBuf,  0, sizeof(ucBuf));
    memcpy( ucBuf, "App Prefer Name:",16 );
    OSMMI_DisplayASCII(0x30,6,0, (uchar *)"App Prefer Name:");
    memcpy(&ucBuf[0] ,G_NORMALTRANS_aucAppPreferName , 16);
    OSMMI_DisplayASCII(0x30,7,0, ucBuf);

    MSG_WaitKey(300);
#endif
#endif
}

#if 0
unsigned char EMV_DisplayAIDParam(void)
{
    unsigned char ucResult,ucI,ucJ,aucBuff[50],aucAmount[15];

    if(!ucTermAIDNum)
    {
        Os__clr_display(255);
        if(CONFIG_GetLanguage() == CN)
        {
            Os__GB2312_display(2,0,(unsigned char *)"POS没有AID信息");
            Os__GB2312_display(3,0,(unsigned char *)"请手工下载!");
        }
        else
        {
            Os__GB2312_display(2,0,(unsigned char *)"NO AID INFO");
            Os__GB2312_display(3,0,(unsigned char *)"Pls Manual Dwn!");
        }
        Os__xget_key();
    }
    else
    {
        for(ucI=0;ucI<ucTermAIDNum;ucI++)
        {
            if(!memcmp("\xA0\x00\x00\x00\x03", TermAID[ucI].aucAID, 5))//VISA
            {
                memcpy(&AIDParam[ucI].aucTACDenial, "\x00\x10\x00\x00\x00",5);
                memcpy(&AIDParam[ucI].aucTACOnline, "\xD8\x40\x04\xF8\x00" ,5);
                memcpy(&AIDParam[ucI].aucTACDefault , "\xD8\x40\x00\xA8\x00",5);
                memcpy(&AIDParam[ucI].aucAppVer, "\x00\x8C",2);
                AIDParam[ucI].ulFloorLimit = 1000;
                AIDParam[ucI].ulThreshold = 1000;
            }
            else if(!memcmp("\xA0\x00\x00\x00\x04", TermAID[ucI].aucAID, 5))//MASTER
            {
                memcpy(&AIDParam[ucI].aucTACDenial, "\x04\x00\x00\x00\x00",5);
                memcpy(&AIDParam[ucI].aucTACOnline, "\xF8\x50\xA8\xF8\x00" ,5);
                memcpy(&AIDParam[ucI].aucTACDefault, "\xFC\x50\xA8\xA0\x00",5);
                memcpy(&AIDParam[ucI].aucAppVer, "\x00\x02",2);
                AIDParam[ucI].ulFloorLimit = 1000;
                AIDParam[ucI].ulThreshold = 1000;
            }
            else if(!memcmp("\xA0\x00\x00\x00\x65", TermAID[ucI].aucAID, 5))//JCB
            {
                memcpy(&AIDParam[ucI].aucTACDenial, "\x00\x10\x00\x00\x00",5);
                memcpy(&AIDParam[ucI].aucTACOnline, "\xFC\x60\xAC\xF8\x00" ,5);
                memcpy(&AIDParam[ucI].aucTACDefault, "\xFC\x60\x24\xA8\x00",5);
                memcpy(&AIDParam[ucI].aucAppVer, "\x02\x00",2);
                AIDParam[ucI].ulFloorLimit = 1000;
                AIDParam[ucI].ulThreshold = 1000;
            }

            Os__clr_display(255);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"AID:");
            hex_asc(aucBuff+strlen((char*)aucBuff),TermAID[ucI].aucAID,TermAID[ucI].ucAIDLen*2);
            OSMMI_DisplayASCII(0x30,0,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"ASI:");
            char_asc(aucBuff+strlen((char*)aucBuff),2,&TermAID[ucI].ucASI);
            OSMMI_DisplayASCII(0x30,1,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"APPVer:");
            hex_asc(aucBuff+strlen((char*)aucBuff),AIDParam[ucI].aucAppVer,sizeof(AIDParam[ucI].aucAppVer)*2);
            OSMMI_DisplayASCII(0x30,2,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"ATCDefault:");
            hex_asc(aucBuff+strlen((char*)aucBuff),AIDParam[ucI].aucTACDefault,sizeof(AIDParam[ucI].aucTACDefault)*2);
            OSMMI_DisplayASCII(0x30,3,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff," TACOnline:");
            hex_asc(aucBuff+strlen((char*)aucBuff),AIDParam[ucI].aucTACOnline,sizeof(AIDParam[ucI].aucTACOnline)*2);
            OSMMI_DisplayASCII(0x30,4,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff," TACDenial:");
            hex_asc(aucBuff+strlen((char*)aucBuff),AIDParam[ucI].aucTACDenial,sizeof(AIDParam[ucI].aucTACDenial)*2);
            OSMMI_DisplayASCII(0x30,5,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"Fllimit:");
            memset(aucAmount,0x00,sizeof(aucAmount));
            UTIL_Form_Montant(aucAmount,AIDParam[ucI].ulFloorLimit,2);
            ucJ=0;
            while(aucAmount[ucJ]==' ') ucJ++;
            memcpy(aucBuff+strlen((char*)aucBuff),aucAmount+ucJ,strlen((char*)aucAmount)-ucJ);
            OSMMI_DisplayASCII(0x30,6,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"Thrhold:");
            memset(aucAmount,0x00,sizeof(aucAmount));
            UTIL_Form_Montant(aucAmount,AIDParam[ucI].ulThreshold,2);
            ucJ=0;
            while(aucAmount[ucJ]==' ') ucJ++;
            memcpy(aucBuff+strlen((char*)aucBuff),aucAmount+ucJ,strlen((char*)aucAmount)-ucJ);
            OSMMI_DisplayASCII(0x30,7,0,aucBuff);
            Os__xget_key();

#if 1
            Os__clr_display(255);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"MaxPercent:");
            Uart_Printf("\nAIDParam[%d].ucMaxTargetPercent %d",ucI,AIDParam[ucI].ucMaxTargetPercent);
            bcd_asc(aucBuff+strlen((char*)aucBuff),&AIDParam[ucI].ucMaxTargetPercent,sizeof(AIDParam[ucI].ucMaxTargetPercent)*2);
            OSMMI_DisplayASCII(0x30,0,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"Percent:");
            bcd_asc(aucBuff+strlen((char*)aucBuff),&AIDParam[ucI].ucTargetPercent,sizeof(AIDParam[ucI].ucTargetPercent)*2);
            OSMMI_DisplayASCII(0x30,1,0,aucBuff);
            Uart_Printf("\nAIDParam[ucI].ucTargetPercent %d",AIDParam[ucI].ucTargetPercent);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"DDOL:");
            hex_asc(aucBuff+strlen((char*)aucBuff),AIDParam[ucI].aucDDOL,AIDParam[ucI].ucDDOLLen*2);
            OSMMI_DisplayASCII(0x30,2,0,aucBuff);
            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"PINCapab:");
            char_asc(aucBuff+strlen((char*)aucBuff),1,&AIDParam[ucI].ucOnlinePINCapab);
            OSMMI_DisplayASCII(0x30,3,0,aucBuff);

            if(Os__xget_key() == KEY_CLEAR)
                break;
#endif
        }
    }
    return OK;
}
#endif

#if 0
unsigned char EMV_DisplayPKInfo(void)
{
    unsigned char ucI,ucJ,ucKey;
    unsigned char aucBuff[50];

    if(!ucTermCAPKNum)
    {
        Os__clr_display(255);
        if(CONFIG_GetLanguage() == CN)
        {
            Os__GB2312_display(1,0,(UCHAR *)"POS没有公钥信息");
            Os__GB2312_display(2,0,(UCHAR *)"请手工下载!");
        }
        else
        {
            Os__GB2312_display(1,0,(unsigned char *)"NO CAPK INFO");
            Os__GB2312_display(2,0,(unsigned char *)"Pls Manual Dwn!");
        }
        Os__xget_key();
        return SUCCESS_TRACKDATA;
    }
    else
    {
        if(ucTermCAPKNum >= MAXTERMCAPKNUMS)
        {
            Os__clr_display(255);
            Os__GB2312_display(1,0,(UCHAR *)" 公钥数量过大");
            Os__GB2312_display(2,0,(UCHAR *)" 怀疑数据出错");
            Os__xget_key();
            return SUCCESS_TRACKDATA;
        }
        for(ucI=0;ucI<ucTermCAPKNum;ucI++)
        {
            Os__clr_display(255);

            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"RID:");
            hex_asc(aucBuff+strlen((char*)aucBuff),TermCAPK[ucI].aucRID,sizeof(TermCAPK[ucI].aucRID)*2);
            OSMMI_DisplayASCII(0x30, 0,0, aucBuff);

            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"CAPKI:");
            hex_asc(aucBuff+strlen((char*)aucBuff),&TermCAPK[ucI].ucCAPKI,sizeof(TermCAPK[ucI].ucCAPKI)*2);
            OSMMI_DisplayASCII(0x30,1,0,aucBuff);

            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"EXP:");
            hex_asc(aucBuff+strlen((char*)aucBuff),TermCAPK[ucI].aucExponent,TermCAPK[ucI].ucExponentLen*2);
            OSMMI_DisplayASCII(0x30,2,0,aucBuff);

            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"EXPIRE:");
            memcpy(aucBuff+strlen((char*)aucBuff),TermCAPK[ucI].aucExpireDate,sizeof(TermCAPK[ucI].aucExpireDate));
            OSMMI_DisplayASCII(0x30,3,0,aucBuff);

            memset(aucBuff,0x00,sizeof(aucBuff));
            strcpy((char*)aucBuff,"MOUDLEN:");
            char_asc(aucBuff+strlen((char*)aucBuff),3,(unsigned char*)&TermCAPK[ucI].ucModulLen);
            OSMMI_DisplayASCII(0x30,4,0,aucBuff);

            for(ucJ=0;ucJ<3;ucJ++)
            {
                memset(aucBuff,0x00,sizeof(aucBuff));
                hex_asc(aucBuff,(unsigned char*)TermCAPK[ucI].aucModul+ucJ*10,20);
                OSMMI_DisplayASCII(0x30,5+ucJ,0,aucBuff);
            }
            if(Os__xget_key()==KEY_CLEAR)
                break;
        }
    }
    return SUCCESS_TRACKDATA;
}
#endif

unsigned char EMV_SimpleProcess(TRANSTYPE enTransType,UINT uiAmount,UINT uiOtherAmount)
{
    UCHAR	ucResult;
    UINT		uiLogSumAmount;
    BOOL	bConfirm;
    UCHAR	ucAppIndex;
    CANDIDATELIST	CandidateList;
    UCHAR	aucBuff[30];
    USHORT  uiLen;

//    MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//    MSG_DisplayMsg(0,2,1,MSG_DATA_PROCESSED);

    EMVConfig.ucCAPKNums =ucTermCAPKNum;
    EMVConfig.pTermSupportCAPK =TermCAPK;
    EMVConfig.ucTermSupportAppNum = ucTermAIDNum;
    EMVConfig.pTermSupportApp = TermAID;
    EMVConfig.ucIPKRevokeNum =ucIPKRevokeNum;
    EMVConfig.pIPKRevoke =IPKRevoke;
    EMVConfig.ucExceptFileNum = ucExceptFileNum;
    EMVConfig.pExceptPAN = ExceptFile;
    EMVConfig.pfnEMVICC = EMVICC;
//    EMVConfig.pfnOfflinePIN = UTIL_OfflinePIN;
//    EMVConfig.pfnOnlinePIN = UTIL_InputEncryptPIN;//UTIL_OnlinePIN;;
//    EMVConfig.pfnEMVInformMsg =UTIL_EMVInformMsg;
//    EMVConfig.bPBOC20 =DataSave0.ChangeParamData.bPBOC20;
//    EMVConfig.pfnEMVIDCardVerify =UTIL_IDCardVerify;

    Uart_Printf("\nEMVConfig.uiFloorLimit = %d\n",EMVConfig.uiFloorLimit);

#ifdef EMVTEST
    EMVDATA_TermInit(&EMVConfig);
#else
    EMVDATA_TermInit(&EMVConfig);
#endif
  memset(&CandidateList,0,sizeof(CANDIDATELIST));
    ucResult = APPSELECT_CreateAppCandidate(&CandidateList);
    Uart_Printf("---0100----ucResult=%02x\n",ucResult);
    bConfirm =FALSE;
    if(CandidateList.CandidateNums ==1 &&CandidateList.CandidateApp[0].bAppPriorityExist)
    {
        if(CandidateList.CandidateApp[0].ucAppPriorityIndicator &APPNEEDCONFIRM)
            bConfirm = TRUE;
    }
    else
    if (CandidateList.CandidateNums >1)
        bConfirm = TRUE;

    while(!ucResult && CandidateList.CandidateNums)
    {
        Uart_Printf("CandidateList.CandidateNums = %02X\n",CandidateList.CandidateNums);
        ucAppIndex = 0;

        if(bConfirm)
        {
            ucResult = CardHolderConfirmApp(&CandidateList,&ucAppIndex);
            Uart_Printf("---0101----ucResult=%02x\n",ucResult);
        }
        if(!ucResult)
        {
            ucResult = APPSELECT_FinalSelect(ucAppIndex,&CandidateList);
            Uart_Printf("---0102----ucResult=%02x\n",ucResult);
            if(ucResult == EMVERROR_RESELECT)
            {
//                MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//                MSG_DisplayMsg(0,2,1,MSG_Retry);
//                Os__xget_key();
                ucResult = EMVERROR_SUCCESS;
                continue;
            }
        }
        if(!ucResult)
        {
//            UTIL_SavEMVTransData();
            //UTIL_IncreaseTraceNumber(0);
            EMVDATA_TransInit(&TransReqInfo);
        }
        if(!ucResult)
        {
            if(ucResult = EMV_SetEMVConfig()) return ucResult;
        }

        if(!ucResult)
        {
//            MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//            MSG_DisplayMsg(0,2,1,MSG_DATA_PROCESSED);
            ucResult = INTIAPP_GPO();
            Uart_Printf("---0103----ucResult=%02x\n",ucResult);
            if(ucResult == EMVERROR_RESELECT)
            {
                ucResult = APPSELECT_GetAppCandidate(&CandidateList);
                Uart_Printf("---0104----ucResult=%02x\n",ucResult);
//                MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//                MSG_DisplayMsg(0,2,1,MSG_Retry);
//                Os__xget_key();
                continue;
            }else
            if(ucResult != EMVERROR_SUCCESS)
            {
//                G_NORMALTRANS_ucFallBackFlag = 2;
                break;
            }
            else
                break;
        }
    }

    if(!ucResult)
        ucResult = READAPP_ReadData();

    if(!ucResult)
    {
        uiLogSumAmount = CalcLogTransAmount();
    }

    if(!ucResult)
    {
        ucResult = TERMRISKMANAGE_Process(uiLogSumAmount);
    }

    if(!ucResult)
    {
        MasApp_SetEMVTransInfo();
    }
    //取AID
    memset( aucBuff,  0, sizeof(aucBuff));
    uiLen = sizeof(aucBuff);
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x4F",aucBuff,&uiLen);
//    G_NORMALTRANS_ucAIDLen = uiLen;
//    if(uiLen) memcpy( G_NORMALTRANS_aucAID, aucBuff, G_NORMALTRANS_ucAIDLen);
    //根据AID判卡别
    MasAPP_CheckAID();

    if(ucResult)
    {
        if((ucResult == EMVERROR_FALLBACK)||(ucResult == EMVERROR_NOSUPPORTAPP)
            ||(ucResult == EMVERROR_ICCDATA) )
//            G_NORMALTRANS_ucFallBackFlag=2;
        EMVERROR_DisplayMsg(ucResult);
        ucResult = ERR_CANCEL;
    }
    return ucResult;
}

UCHAR	EMVTransProcess(TRANSTYPE enTransType,UINT uiAmount,UINT uiOtherAmount)
{
    UCHAR	ucResult,ucAPPResult;
    CRYPTTYPE	CryptType;
    unsigned long uiLogSumAmount;
    BOOL	bConfirm,bConnectSuccess;
    UCHAR	ucAppIndex;
    CANDIDATELIST	CandidateList;
    UCHAR	aucRecvBuff[512],ucRstBuf[3];
    USHORT  uiRecvLen,uiLen,ucJ;
    UCHAR ucHostResult;
    UCHAR aucBuf[512];
    UCHAR pucCardFind;
    UCHAR ucErrorType = 0;

    ucResult = EMVERR_SUCCESS;
    ucAPPResult = EMVERR_SUCCESS;
    ucHostResult = EMVERR_SUCCESS ;
//    MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//    MSG_DisplayMsg(0,2,1,MSG_DATA_PROCESSED);

    EMVConfig.ucCAPKNums =ucTermCAPKNum;
    EMVConfig.pTermSupportCAPK =TermCAPK;
    EMVConfig.ucTermSupportAppNum = ucTermAIDNum;
    EMVConfig.pTermSupportApp = TermAID;
    EMVConfig.ucIPKRevokeNum =ucIPKRevokeNum;
    EMVConfig.pIPKRevoke =IPKRevoke;
    EMVConfig.ucExceptFileNum = ucExceptFileNum;
    EMVConfig.pExceptPAN = ExceptFile;
    EMVConfig.pfnEMVICC = EMVICC;
//    EMVConfig.pfnOfflinePIN = UTIL_OfflinePIN;
//    EMVConfig.pfnOnlinePIN = UTIL_InputEncryptPIN;//UTIL_OnlinePIN;
//    EMVConfig.pfnEMVInformMsg =UTIL_EMVInformMsg;
//    EMVConfig.bPBOC20 = DataSave0.ChangeParamData.bPBOC20;
//    EMVConfig.pfnEMVIDCardVerify =UTIL_IDCardVerify;

    Uart_Printf("\nEMVConfig.uiFloorLimit = %d,%d\n",EMVConfig.uiFloorLimit,EMVConfig.ucCAPKNums);

    for(uiLen=0;uiLen<EMVConfig.ucCAPKNums;uiLen++)
    {
        Uart_Printf("%02X ",EMVConfig.pTermSupportCAPK[uiLen].ucCAPKI);
        //UTIL_TestDispStr(unsigned char * pucInBuf, unsigned int uiLen, unsigned char * DispInfo, unsigned char flag)
    }
    Uart_Printf("\n---------------aid--------------\n");

    memset(aucBuf,0,sizeof(aucBuf));
    uiLen = sizeof(aucBuf);
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\xDF\x01",aucBuf,&uiLen);

    Uart_Printf("aucBuf:%02x,%02x\n",aucBuf[0],aucBuf[1]);
    Uart_Printf("\nEMVConfig.pTermSupportApp.Len:%d\n",EMVConfig.pTermSupportApp[0].ucAIDLen);
    Uart_Printf("\nEMVConfig.pTermSupportApp.ucASI:%d\n",EMVConfig.pTermSupportApp->ucASI);

    //EMV 内核配置初始化
    EMVDATA_TermInit(&EMVConfig);
    //#endif
    //建立终端与卡片共同支持的应用.
    memset(&CandidateList,0,sizeof(CANDIDATELIST));
    ucResult = APPSELECT_CreateAppCandidate(&CandidateList);
    memset(aucBuf,0,sizeof(aucBuf));
    uiLen = sizeof(aucBuf);
    Uart_Printf("---0100----ucResult=%02x\n",ucResult);

    bConfirm =FALSE;
    if(CandidateList.CandidateNums ==1 &&CandidateList.CandidateApp[0].bAppPriorityExist)
    {
        if(CandidateList.CandidateApp[0].ucAppPriorityIndicator &APPNEEDCONFIRM)
            bConfirm = TRUE;
    }
    else
    if (CandidateList.CandidateNums >1)
        bConfirm = TRUE;

    while(!ucResult && CandidateList.CandidateNums)
    {
        Uart_Printf("CandidateList.CandidateNums = %02X\n",CandidateList.CandidateNums);
        ucAppIndex = 0;

        if(bConfirm)
        {
            ucResult = CardHolderConfirmApp(&CandidateList,&ucAppIndex);
            Uart_Printf("---0101----ucResult=%02x\n",ucResult);
        }
        if(!ucResult)
        {
            //选择应用来完成交易
            ucResult = APPSELECT_FinalSelect(ucAppIndex,&CandidateList);
            Uart_Printf("---0102----ucResult=%02x\n",ucResult);
            if(ucResult == EMVERROR_RESELECT)
            {
//                MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//                MSG_DisplayMsg(0,2,1,MSG_Retry);
//                Os__xget_key();
                ucResult = EMVERROR_SUCCESS;
                continue;
            }
        }
        if(!ucResult)
        {
//            UTIL_SavEMVTransData();
            //UTIL_IncreaseTraceNumber(0);
            EMVDATA_TransInit(&TransReqInfo);//交易前把交易相关信息发给EMV内核.
        }

        if(!ucResult)
        {
            if(ucResult = EMV_SetEMVConfig()) return ucResult;
        }
        if(!ucResult)
        {
//            MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//            MSG_DisplayMsg(0,2,1,MSG_DATA_PROCESSED);

            //初始化应用.
            ucResult = INTIAPP_GPO();
            Uart_Printf("---0103----ucResult=%02x\n",ucResult);
            if(ucResult == EMVERROR_RESELECT)
            {
                //从内核中获取终端与卡片共同支持的应用信息
                ucResult = APPSELECT_GetAppCandidate(&CandidateList);
                Uart_Printf("---0104----ucResult=%02x\n",ucResult);
//                MSG_DisplyTransType(G_NORMALTRANS_ucTransType ,0,0,true,255);
//                MSG_DisplayMsg(0,2,1,MSG_Retry);
//                Os__xget_key();
                continue;
            }else if(ucResult != EMVERROR_SUCCESS)
            {
//                G_NORMALTRANS_ucFallBackFlag = 2;
                break;
            }else
                break;
        }
    }

    //读应用数据过程.
    if(!ucResult)
        ucResult = READAPP_ReadData();
    if(!ucResult)
    {
        MasApp_SetEMVTransInfo();
    }
//    UTIL_TestDispStr(G_NORMALTRANS_aucSourceAcc, 10, (unsigned char *)"G_NORMALTRANS_aucSourceAcc" , NULL);

    Uart_Printf("---0105----ucResult=%02x\n",ucResult);
    //联机下载汇率
#if 0
    if((!ucResult)&&(G_NORMALTRANS_ucOptOutFlag == 0))
    {
        //检查卡表
        if(ucResult == SUCCESS)
        {
            if(DataSave0.ConstantParamData.ucOpenDCCFlag == 1)
                pucCardFind =  BIN_LookupBin_Process();
        }
        Uart_Printf("---00105----pucCardFind=%02x\n",pucCardFind);
        if( pucCardFind == SUCCESS)
        {
            G_NORMALTRANS_ucOptOutFlag = 1;
        }
        else
        {
            //下载汇率
            ucResult = TRANS_LookUpCurrency();
            if(ucResult != SUCCESS)
            {
                ucErrorType =1;
                if((ucResult != ERR_CANCEL)
                &&(ucResult != ERR_END))
                {
                     MSG_DisplayErrMsg(ucResult);
                     ucResult = ERR_CANCEL;
                }
            }
        }
    }
#else
    TransReqInfo.uiAmount = 100;
    memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
#endif
//    if(!ucResult)
//    {
//        if(G_NORMALTRANS_ucOptOutFlag == 0)
//        {
//            TransReqInfo.uiAmount = G_NORMALTRANS_ulTrueAmount;
//            memcpy(&EMVConfig.aucTransCurrencyCode[0],NormalTransData.aucCurrencyCode,2);
//            UTIL_TestDispStr(EMVConfig.aucTransCurrencyCode, 2, (uchar *)"EMVConfig.aucTransCurrencyCode:", NULL);
//        }
//        else
//        {
//            memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
//            TransReqInfo.uiAmount = G_NORMALTRANS_ulAmount;//liuyg  110825
//        }
//    }
    EMVDATA_ChangeTransAmount(TransReqInfo.uiAmount,0);
    EMVDATA_SetCountryCode(EMVConfig.aucTermCountryCode);
    EMVDATA_SetTransCurrencyCode(EMVConfig.aucTransCurrencyCode);
    if(!ucResult)
    {
        uiLogSumAmount = CalcLogTransAmount();
    }
    Uart_Printf("---0106----uiLogSumAmount=%d\n",uiLogSumAmount);
    //脱机数据认证过程.
    if(!ucResult)
        ucResult = DATAAUTH_Process();
    Uart_Printf("---0107----ucResult=%02x\n",ucResult);
    //处理限制过程.
    if(!ucResult)
        ucResult = PROCESSRESTRICT_Process();
    Uart_Printf("---0108----ucResult=%02x\n",ucResult);

    if(!ucResult)
        MasApp_SetEMVTransInfo();
    //持卡人验证过程
    if(!ucResult)
        ucResult = EMVVERIFY_Process();
    Uart_Printf("---0109----ucResult=%02x\n",ucResult);
    if(ucResult == ERR_END)
        return ucResult;
    Uart_Printf("---0110----ucResult=%02x\n",ucResult);
    if(!ucResult)
    {
        //终端风险管理过程
        ucResult = TERMRISKMANAGE_Process(uiLogSumAmount);
        Uart_Printf("---0111----ucResult=%02x\n",ucResult);
    }
    if(!ucResult)
    {
        //终端行为分析过程
        CryptType = TERMACTANALYSIS_Process(true);
        Uart_Printf("---0112----CryptType=%02x\n",CryptType);

        //把终端分析交易结果发给卡片,卡片做行为分析
        ucResult = APPCRYPT_OfflineProcess(CryptType);
        Uart_Printf("APPCRYPT_OfflineProcess()_______ucResult = %02x\n",ucResult);


        if(ucResult ==EMVERROR_NEEDARQC)
        {
//            Uart_Printf("G_NORMALTRANS_ucTransType = %02x\n",G_NORMALTRANS_ucTransType);
//            G_NORMALTRANS_ucFallBackFlag =0;//Add 08-04-02
            memset(aucRecvBuff,0x00,sizeof(aucRecvBuff));
            uiRecvLen = sizeof(aucRecvBuff);
#if 0
            //ucResult = TRANS_OnlineProcess();
            switch(G_NORMALTRANS_ucTransType)
            {
            case  TRANS_PURCHASE:
                 G_NORMALTRANS_ucTransType =  TRANS_PURCHASE;
                 if(G_NORMALTRANS_ucOptOutFlag)
                 {
                    ucResult = TRANS_Online_OptOut();
                 }
                else
                {
                    ucResult =  TRANS_SaleOnline();
                }
                break;
            case  TRANS_PREAUTH:
                //预授权交易不走直接OPTOUT
                 G_NORMALTRANS_ucTransType =  TRANS_PREAUTH;
                 ucResult = TRANS_Online_OptOut();
                break;
            default:
                    ucResult = ERR_CANCEL;
                break;
            }
#endif
#if 0
            Uart_Printf("TRANS_OnlineProcess() ---- ucResult = %02x\n",ucResult);
            if((ucResult==ERR_DIAL)
                ||ucResult==DIAL_BUSY
                ||ucResult==DIAL_NO_TONE
                ||ucResult==DIAL_NO_CARRIER
                ||ucResult==DIAL_LINE_BUSY
                ||ucResult==ERR_CANCEL
              )
            {
                bConnectSuccess = FALSE;
            }
            else
            {
                bConnectSuccess = TRUE;
                memset(ucRstBuf ,0,sizeof(ucRstBuf));
                if(ucResult == ERR_HOSTCODE )
                {
                    memcpy(ucRstBuf, G_RUNDATA_aucErrorExtCode,2);
                }
                else
                {
                    char_asc(ucRstBuf, 2, &ucResult);
                }
                if(ucResult)
                {
                    ucHostResult =ucResult ;
                }

                EMVTRANSTAG_SetTag(ALLPHASETAG,(uchar *)"\x8A",ucRstBuf,2);
                memset(aucRecvBuff,0,sizeof(aucRecvBuff));
                uiRecvLen= sizeof(aucRecvBuff);
                if(ISO8583_GetBitValue(55,aucRecvBuff,(short*)&uiRecvLen,sizeof(aucRecvBuff)) == SUCCESS)
                {
                    Uart_Printf("uiRecvLen=%d\n",uiRecvLen);
                    for(ucJ=0;ucJ<uiRecvLen;ucJ++)
                        Uart_Printf(" %02x",aucRecvBuff[ucJ]);
                    Uart_Printf("\n");
                    Uart_Printf("\n ucRstBuf: %s",ucRstBuf);
                    //联机请求后,把发卡行IC卡信息发给卡片,完成发卡行认证与脚本处理
                    ucResult = APPCRYPT_OnlineRespDataProcess(ucRstBuf,aucRecvBuff,uiRecvLen);
                }
            }
#endif
            Uart_Printf("---0115----ucResult=%02x\n",ucResult);

            if(ucResult != ERR_CANCEL)
            {
                ucResult = APPCRYPT_OnlineProcess(bConnectSuccess);
            }
            if(EMVTransInfo.ucScriptResultNum && !ucResult)
            {
//                if(G_NORMALTRANS_ucOptOutFlag == 0)
//                {
//                    Uart_Printf("\nucScriptResultNum%d",EMVTransInfo.ucScriptResultNum);
//                    UTIL_TestDispStr((uchar*)EMVTransInfo.ScriptResult, 5, (uchar*)"-1-EMVTransInfo.ScriptResult", NULL);
//                    ucResult = Trans_EMVProcessScript();
//                }
//                else
//                {
//                    memcpy(DataSave0.ChangeParamData.aucScriptResult,EMVTransInfo.ScriptResult,10);
//                    Os__saved_set((unsigned char *)(&DataSave0.ChangeParamData.ucScriptInformValid),
//                                    DATASAVEVALIDFLAG,
//                                    sizeof(unsigned char));
//                    XDATA_Write_Vaild_File(DataSaveChange);
//                }
            }
//            if(EMVTransInfo.bReversal&&bConnectSuccess)
//            {
//                DataSave0.ReversalISO8583Data.ucValid = DATASAVEVALIDFLAG;
//                XDATA_Write_Vaild_File(DataSaveRevsal8583);
//            }
        }
        else
            ucResult = ERR_CANCEL;
    }

    Uart_Printf("\n EMV return %02x",ucResult);
    Uart_Printf("\n bConnectSuccess = %02x\n",bConnectSuccess);
    Uart_Printf("\n EMVTransInfo.bReversal = %02x\n",EMVTransInfo.bReversal);

    if(ucResult == EMVERR_SUCCESS)
    {
//        if(TransCapture(EMVTransInfo.enTransResult))
//        {
//            if(EMVTransInfo.enTransResult ==ONLINEAPPROVED||EMVTransInfo.enTransResult ==UNABLEONLINE_OFFLINEAPPROVED)
//            {
//                    TRANS_SetField55(SECONDGACPHASETAG,Msg_EMV_BatchTrans,G_NORMALTRANS_ucField55Data,(ushort *)&G_NORMALTRANS_uiField55Len);
//                    TRANS_SetField56(SECONDGACPHASETAG,G_NORMALTRANS_ucField56Data,(ushort *)&G_NORMALTRANS_uiField56Len);

//            }
//            else
//            {
//                    TRANS_SetField55(FIRSTGACPHASETAG,Msg_EMV_BatchTrans,G_NORMALTRANS_ucField55Data,(ushort *)&G_NORMALTRANS_uiField55Len);
//                    TRANS_SetField56(FIRSTGACPHASETAG,G_NORMALTRANS_ucField56Data,(ushort *)&G_NORMALTRANS_uiField56Len);

//            }

//        }else
//        {

//            ucAPPResult = ERR_CANCEL;
//        }
//        G_NORMALTRANS_ucFallBackFlag = 0;
//        UTIL_DispTransResult();
    }
    else
    {
        if(ucResult == EMVERROR_NOSUPPORTAPP
            ||ucResult == EMVERROR_FALLBACK)
        {
//            G_NORMALTRANS_ucFallBackFlag=2;
            ucAPPResult=EMVERROR_FALLBACK;
        }
        if(ucErrorType == 0)
        {
            EMVERROR_DisplayMsg(ucResult);
        }
        ucAPPResult = ERR_CANCEL;
    }

    if(ucHostResult)
    {
//        COMMS_FinComm();
//        Os__clr_display(255);
//        MSG_DisplayErrMsg(ucHostResult);
        ucAPPResult = ERR_CANCEL;
    }
    //根据AID判卡别
    MasAPP_CheckAID();
    MasApp_CopytEMVTransInfoToNormalTrans();
//    Uart_Printf("33G_NORMALTRANS_ucFallBackFlag:%d\n",G_NORMALTRANS_ucFallBackFlag);
    //PrintTransTicket();
    return ucAPPResult;
}
