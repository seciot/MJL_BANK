#define EMVINTERFACEDATA
#include "EMVInterface.h"
#include "EMVGlobal.h"
#include "msg.h"
#undef EMVINTERFACEDATA

#include "ostools.h"

const EMVTAGATTR EMVParamTagAttrInfo[] =
{
    {"\x9F\x06"},//AID
    {"\x9F\x09"},
    {"\x9F\x22"},//
    {"\x9F\x1B"},//FLOOR LIMIT
    {"\x9F\x35"},
    {"\x9F\x15"},
    {"\xDF\x01"},//ASI
    {"\xDF\x02"},//
    {"\xDF\x03"},//
    {"\xDF\x04"},//
    {"\xDF\x05"},//
    {"\xDF\x06"},//RID
    {"\xDF\x07"},//
    {"\xDF\x11"},//TAC-DEFAULT
    {"\xDF\x12"},//TAC-ONLINE
    {"\xDF\x13"},//TAC-DENY
    {"\xDF\x14"},//DDOL DEFAULT
    {"\xDF\x15"},
    {"\xDF\x16"},
    {"\xDF\x17"},
    {"\xDF\x18"},
    {"\xDF\x25"}
};

unsigned char CAPK_TermCAPKNum(void)
{
    if(ucTermCAPKNum)
        Uart_Printf((char *)"ucTermCAPKNum = %d\n", ucTermCAPKNum);
    return ucTermCAPKNum;
}

unsigned char TERMAPP_TermAppNum(void)
{
    if(ucTermAIDNum)
        Uart_Printf((char *)"ucTermAIDNum = %d\n", ucTermAIDNum);
    return ucTermAIDNum;
}

unsigned char TERMAPP_DeleteAll(void)
{
    memset(AppListTerm, 0x00, sizeof(TERMAPP)*16);
    ucTermAIDNum = 0;
    return SUCCESS_TRACKDATA;
}

EMVTAGINFO * EMVINTERFACE_SearchTag(ICTRANSINFO *pICTransInfo,unsigned char *paucTag)
{
    unsigned char ucI,ucScrLen,ucTagLen;
    EMVTAGINFO	*pEMVTagInfo;
    const EMVTAGATTR *pEMVTagAttr;

    ucScrLen=strlen((char *)paucTag);
    pEMVTagInfo=pICTransInfo->EMVTagInfo;
    pEMVTagAttr=pICTransInfo->pEMVTagAttr;

    for(ucI=0;ucI<ICTransInfo.ucTagNum;ucI++,pEMVTagAttr++,pEMVTagInfo++)
    {
        ucTagLen=strlen((char *)pEMVTagAttr->aucTag);
        if(ucScrLen==ucTagLen&&!memcmp(pEMVTagAttr->aucTag,paucTag,ucTagLen))
        {
            //if(pEMVTagInfo->uiLen&&pEMVTagInfo->pTagValue)
                return pEMVTagInfo;
        //	else break;

        }

    }
    return NULL;
}

unsigned char EMVINTERFACE_GetTagValue(ICTRANSINFO *pICTransInfo,unsigned char *paucTag,
                                unsigned char *paucTagValue,unsigned short* uiTagValueLen)
{
    EMVTAGINFO *pEMVTagInfo;
    unsigned short uiBufLen;

    pEMVTagInfo = EMVINTERFACE_SearchTag(pICTransInfo, paucTag);
    if(pEMVTagInfo == NULL)
        return EMVTAGERR_TAGNOTEXIST;

//    uiBufLen = pEMVTagInfo->uiLen > *uiTagValueLen?*uiTagValueLen:pEMVTagInfo->uiLen;

    if(pEMVTagInfo->uiLen > *uiTagValueLen)
        uiBufLen = *uiTagValueLen;
    else
        uiBufLen = pEMVTagInfo->uiLen;

    memcpy(paucTagValue, pEMVTagInfo->pTagValue, uiBufLen);
    *uiTagValueLen = uiBufLen;

#ifdef EMVTAG_DEBUG
    Uart_Printf((char *)"\nTag %02X ", *paucTag);
    if(*paucTag&0x1f)
        Uart_Printf("%02X ", *(paucTag+1));
    Uart_Printf("\nTag Value");
    {
        unsigned char ucI;

        for(ucI=0;ucI<*uiTagValueLen;ucI++)
        {
            if(!(ucI%16)) Uart_Printf("\n");
            Uart_Printf("%02X ",*(paucTagValue+ucI));
        }
    }
#endif
    return EMVTAGERR_SUCCESS;
}


unsigned char EMVINTERFACE_Unpack(ICTRANSINFO *pICTransInfo)
{
    unsigned char ucI, ucLenAttr;
    unsigned char *pucBuff;
    EMVTAGINFO *pEMVTagInfo;
    const EMVTAGATTR *pEMVTagAttr;

    Uart_Printf((char *)"\n****EMVTAG_Unpack*****");
    for(ucI = 0, pEMVTagInfo = pICTransInfo->EMVTagInfo; ucI < EMVTAG_NUMMAX; ucI++, pEMVTagInfo++)
    {
        pEMVTagInfo->uiLen = 0;
        pEMVTagInfo->pTagValue = NULL;
    }

    memset(pICTransInfo->aucEMVTagVaule, 0x00, sizeof(pICTransInfo->aucEMVTagVaule));
    pICTransInfo->uiTagBuffLen = 0;

    if(pICTransInfo->uiTransInfoLen<2)
        return EMVTAGERR_DATA;

    pucBuff = pICTransInfo->aucTransInfo;
    while(pucBuff < (pICTransInfo->aucTransInfo + pICTransInfo->uiTransInfoLen))
    {
        for(ucI = 0; ucI < pICTransInfo->ucTagNum; ucI++)
        {
            pEMVTagAttr = &pICTransInfo->pEMVTagAttr[ucI];
            if(!memcmp(pucBuff, pEMVTagAttr->aucTag, strlen((char *)pEMVTagAttr->aucTag)))
                break;
        }

        if(ucI < pICTransInfo->ucTagNum)
        {
            pucBuff += strlen((char *)pEMVTagAttr->aucTag);
            pEMVTagInfo = &pICTransInfo->EMVTagInfo[ucI];
            if(!pEMVTagInfo->uiLen)
            {
                pEMVTagInfo->pTagValue = pICTransInfo->aucEMVTagVaule + pICTransInfo->uiTagBuffLen;
                ucLenAttr = EMVTAG_GetTagLenAttr(pucBuff, &pEMVTagInfo->uiLen);
                pucBuff += ucLenAttr;
                if((pICTransInfo->uiTagBuffLen + pEMVTagInfo->uiLen) > EMVTAB_BUFFMAX)
                    return EMVTAGERR_OVERFLOW;

                memcpy(pEMVTagInfo->pTagValue, pucBuff, pEMVTagInfo->uiLen);
                pucBuff += pEMVTagInfo->uiLen;
                pICTransInfo->uiTagBuffLen += pEMVTagInfo->uiLen;
            }
            else
                return EMVTAGERR_DATA;
        }
        else pucBuff++;
    }
#ifdef EMVTAG_DEBUG
    Uart_Printf((char *)"\nbefore EMVTAG_DataDump");
    EMVTAG_DataDump(*pICTransInfo);
#endif
    return EMVTAGERR_SUCCESS;
}

unsigned char EMVINTERFACE_INIT(ICTRANSINFO *pICTransInfo, const EMVTAGATTR EMVTagAttrInfo[], unsigned char ucArrayNum)
{
    const EMVTAGATTR *pEMVTagAttr;
    EMVTAGINFO *pEMVTagInfo;

    pEMVTagAttr = EMVTagAttrInfo;
    pEMVTagInfo = pICTransInfo->EMVTagInfo;

    memset(pICTransInfo, 0x00, sizeof(ICTRANSINFO));
    if(ucArrayNum > EMVTAG_NUMMAX)
        return EMVTAGERR_OVERFLOW;

    pICTransInfo->pEMVTagAttr = EMVTagAttrInfo;
    pICTransInfo->ucTagNum = ucArrayNum;
    pICTransInfo->uiTransInfoLen = EMVTAB_BUFFMAX;
    return EMVTAGERR_SUCCESS;
}

#if 0
unsigned char EMV_LoadCAPK(void)
{
    unsigned char ucResult,ucI,ucJ,ucK,I,J;

    if(G_RUNDATA_ucDialFlag != 1)
    {
        COMMS_PreComm();
        G_RUNDATA_ucDialFlag = 1;
    }

    G_NORMALTRANS_ucTransType=TRANS_SENDSTATS;
    DataSave0.ChangeParamData.euLoadParamType=LOADPARAMTYPE_PKQUERY;
    XDATA_Write_Vaild_File(DataSaveChange);
    ucCAPKNum=0;
    ucTermCAPKNum=0;
    ucResult=LOGON_Online();

    //if(G_RUNDATA_ucDialFlag != 1)
        COMMS_FinComm();
    DataSave0.ChangeParamData.ucPackgeHeadEve=0x00;

    if(!ucResult)
        ucResult = UTIL_SaveCAPKFile();

    if(( ucResult != SUCCESS)&&(ucResult != ERR_CANCEL)&&(ucResult != ERR_END))
    {
        MSG_DisplayErrMsg(ucResult);
        ucResult = ERR_CANCEL;
    }

    return ucResult;

}
#endif
unsigned char EMV_SetEMVConfig(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char ucBuf[30],ucBuf2[30];
    unsigned short ucLen;
    TACCODE tac;
    unsigned char aucTermCapab[TERMCAPABLEN];
    unsigned char aucTermAddCapab[TERMADDCAPABLEN];

    ucLen = sizeof(ucBuf);
    memset( ucBuf,  0, sizeof(ucBuf));
    memset( ucBuf2,  0, sizeof(ucBuf2));
    EMVTRANSTAG_GetTagValue(ALLPHASETAG,(PUCHAR)"\x4F", ucBuf, &ucLen);
    hex_asc(ucBuf2, ucBuf, ucLen*2);
    Uart_Printf((char *)"\nEMV_SetEMVConfig()   AID: %s", ucBuf2);
    Uart_Printf((char *)"\nucBuf: %02x %02x %02x %02x %02x", ucBuf[0], ucBuf[1], ucBuf[2], ucBuf[3], ucBuf[4]);
    EMVConfig.uiFloorLimit = 1;

    if(!memcmp("\xA0\x00\x00\x00\x03", ucBuf, 5))//VISA
    {
        memcpy(&EMVConfig.TAC.aucDenial , "\x00\x10\x00\x00\x00",5);
        memcpy(&EMVConfig.TAC.aucOnline , "\xD8\x40\x04\xF8\x00" ,5);
        memcpy(&EMVConfig.TAC.aucDefault , "\xD8\x40\x00\xA8\x00",5);
        memcpy(&EMVConfig.aucTermCountryCode, "\x01\x56",2);
//        memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
        memcpy(&EMVConfig.aucAppVerNumber, "\x00\x8C",2);
        EMVConfig.uiFloorLimit = 1;
        Uart_Printf((char *)"\n-------VISA---------\n");
    }
    else if(!memcmp("\xA0\x00\x00\x00\x04", ucBuf, 5))//MASTER
    {
        memcpy(&EMVConfig.TAC.aucDenial , "\x04\x00\x00\x00\x00",5);
        memcpy(&EMVConfig.TAC.aucOnline , "\xF8\x50\xA8\xF8\x00" ,5);
        memcpy(&EMVConfig.TAC.aucDefault , "\xFC\x50\xA8\xA0\x00",5);
        memcpy(&EMVConfig.aucTermCountryCode, "\x01\x56",2);
        //memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
        memcpy(&EMVConfig.aucAppVerNumber, "\x00\x02",2);
        EMVConfig.uiFloorLimit = 1;
        Uart_Printf((char *)"\n-------MASTER---------\n");
    }
    else if(!memcmp("\xA0\x00\x00\x00\x65", ucBuf, 5))//JCB
    {
        memcpy(&EMVConfig.TAC.aucDenial , "\x00\x10\x00\x00\x00",5);
        memcpy(&EMVConfig.TAC.aucOnline , "\xFC\x60\xAC\xF8\x00" ,5);
        memcpy(&EMVConfig.TAC.aucDefault , "\xFC\x60\x24\xA8\x00",5);
        memcpy(&EMVConfig.aucTermCountryCode, "\x01\x56",2);
        //memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
        memcpy(&EMVConfig.aucAppVerNumber, "\x02\x00",2);
        EMVConfig.uiFloorLimit = 1;
#if 0
        G_NORMALTRANS_ucOptOutFlag = 1;//liuyg  110825
#endif
        Uart_Printf((char *)"\n-------JCB---------\n");
    }
    else
        return ERR_NOTPROC;

#if 0
    if(G_NORMALTRANS_ucOptOutFlag == 0)
    {
        UTIL_TestDispStr(NormalTransData.aucCurrencyCode, 2, "||NormalTransData.aucCurrencyCode:", NULL);
        memset(&EMVConfig.aucTransCurrencyCode,0,sizeof(EMVConfig.aucTransCurrencyCode));
        memcpy(&EMVConfig.aucTransCurrencyCode[0],NormalTransData.aucCurrencyCode,2);
        //memcpy(&EMVConfig.aucTransCurrencyCode, "\x09\x01",2);
    }
    else
        memcpy(&EMVConfig.aucTransCurrencyCode, "\x01\x56",2);
#endif
//    ucResult = UTIL_WriteEMVConfigFile(&EMVConfig);
    EMVDATA_SetAppversion(EMVConfig.aucAppVerNumber);
    EMVDATA_SetDDOL(EMVConfig.aucDefaultDOL,EMVConfig.ucDefaultDOLLen);
    Uart_Printf((char *)"\nDDOL:%02x,%02x,%02x\n",EMVConfig.aucDefaultDOL[0],EMVConfig.aucDefaultDOL[1],EMVConfig.aucDefaultDOL[2]);
    EMVDATA_SetFloorLimit(EMVConfig.uiFloorLimit);
    //modify date:080318
    tac.bDenialExist = TRUE;
    tac.bOnlineExist = TRUE;
    tac.bDefaultExist = TRUE;
    //----end ------
    memcpy(&tac.aucDefault,&EMVConfig.TAC.aucDefault,5);
    memcpy(&tac.aucDenial,&EMVConfig.TAC.aucDenial,5);
    memcpy(&tac.aucOnline,&EMVConfig.TAC.aucOnline,5);
    EMVDATA_SetTAC(tac);
    EMVDATA_SetThreshold(EMVConfig.uiThreshold);
    EMVDATA_SetTragetPercent(EMVConfig.ucTragetPercent,EMVConfig.ucMaxTragetPercent);

    memcpy(aucTermCapab ,EMVConfig.aucTermCapab ,TERMCAPABLEN);
    memcpy(aucTermAddCapab ,EMVConfig.aucTermAddCapab ,TERMADDCAPABLEN);
    EMVDATA_SetTermCapab(aucTermCapab);
    EMVDATA_SetTermAddCapab(aucTermAddCapab);
    return ucResult;
}
