#include "print.h"
#include <stdio.h>
#include <string.h>
#include "commontools.h"
#include "xdata.h"
#include "global.h"

extern "C"{
    #include "ostools.h"
    #include "sand_time.h"
    #include "printUtil.h"
}

#define LINENUM 0x16
#define min(a, b)       (((a) < (b)) ? (a) : (b))

void getInfoBuf(unsigned char *pOutData, const char *pCaption, int iCaptionLen, unsigned char *pValue, int iValueLen)
{
    memcpy(&pOutData[0], pCaption, iCaptionLen);
    memcpy(&pOutData[iCaptionLen], pValue, iValueLen);
}

unsigned char *PRINT_FormAmount(unsigned char *pucOut, unsigned long ulAmount,
        unsigned char ucDecimalPos, unsigned char ucNegative, unsigned char *pCurrencySign)
{
    unsigned char aucBuf[12];
    char *pcFind;
    pucOut[0] = 0;

    getAmount(aucBuf, ulAmount, ucDecimalPos);
#if 0
    pcFind = strrchr((char *) aucBuf, ' ');
    if (pcFind == NULL)
    {
        pcFind = (char *) aucBuf;
    }
    else
    {
        ++pcFind;
    }
#else
    pcFind = (char *) aucBuf;
#endif

    if(ucNegative == 1)
        strcat((char*) pucOut, "-");
    strcat((char*) pucOut, pcFind);
    strcat((char*) pucOut, (const char*) pCurrencySign);
    return pucOut;
}

void getTransType(unsigned char *pOutData, TransMode m_transType)
{
    switch(m_transType)
    {
    case TransMode_CashDeposit:     //存款
    case TransMode_DepositAdjust:
        sprintf((char*)pOutData, "%s", "CASH DEPOSIT");
        break;
    case TransMode_DepositVoid:     //存款撤销
        sprintf((char*)pOutData, "%s", "CASH DEPOSIT VOID");
        break;
    case TransMode_CashAdvance:     //取款
    case TransMode_AdvanceAdjust:
        sprintf((char*)pOutData, "%s", "CASH ADVANCE");
        break;
    case TransMode_AdvanceVoid:     //取款撤销
        sprintf((char*)pOutData, "%s", "CASH ADVANCE VOID");
        break;
    case TransMode_CardTransfer:    //转账
        sprintf((char*)pOutData, "%s", "P2P TRANSFER");
        break;
    default:
        break;
    }
}

void PRINT_ProjAndVer(void)
{
    unsigned char aucPrintBuf[] = CURRENT_PRJ" "CURRENT_VER" "CURRENT_DATE;
    unsigned short usOffset, usLen, usCharNum;

    usCharNum = PRINT_UTIL_GetMaxLength(PF_SMALL, THERMAL);

    usLen = strlen((char *)aucPrintBuf);
    for(usOffset = 0; usOffset < usLen; usOffset += min(usCharNum, usLen - usOffset))
    {
        PRINT_UTIL_Info(&aucPrintBuf[usOffset], PRINT_LEFT, PF_SMALL, THERMAL);
    }
}

void PrintTitle(void)
{
    PRINT_BMP((const char*)"bmp/logo.bmp");

    PRINT_UTIL_InfoChar("ARAB BANGLADESH LTD", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);
    PRINT_UTIL_Info(g_constantParam.aucReceiptLine1, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_Info(g_constantParam.aucReceiptLine2, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_Info(g_constantParam.aucReceiptLine3, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_Info(g_constantParam.aucReceiptLine4, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
//    PRINT_xlinefeed(LINENUM*1);
}

//打印交易小票单
void PrintTransTicket(NormalTrans *pNormalTrans, TicketType ucTicketType, bool isReprint)
{
    unsigned char aucBuf[40];
    unsigned char aucBufLeft[40];
    unsigned char aucBufRight[40];
    unsigned long ulAmount = 0;

    //商户信息
    PrintTitle();

    //交易日期  MMDD,YY  HH:MM
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memcpy(aucBufLeft, "DATE/TIME: ", 11);
    getFormDate(pNormalTrans->aucDate, &aucBufLeft[11]);
    getFormTime(pNormalTrans->aucTime, aucBufRight);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //终端号
    memset(aucBuf, 0, sizeof(aucBuf));
    getInfoBuf(aucBuf, "TERM# ", 6, g_constantParam.aucTerminalID, 8);
    PRINT_UTIL_Info(aucBuf, PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);

    //商户号
    memset(aucBuf, 0, sizeof(aucBuf));
    getInfoBuf(aucBuf, "MER# ", 5, g_constantParam.aucMerchantID, 15);
    PRINT_UTIL_Info(aucBuf, PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);

    //批次号    流水号
    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    long_asc(aucBuf, 6, &pNormalTrans->ulBatchNumber);
    getInfoBuf(aucBufLeft, "BATCH# ", 7, aucBuf, 6);

    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    long_asc(aucBuf, 6, &pNormalTrans->ulTraceNumber);
    getInfoBuf(aucBufRight, "TRACE# ", 7, aucBuf, 6);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //交易类型
    memset(aucBuf, 0, sizeof(aucBuf));
    getTransType(aucBuf, pNormalTrans->transType);
    PRINT_UTIL_Info(aucBuf, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    /*************************************************/
    //卡号
    PRINT_UTIL_InfoChar("CARD:", PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_UTIL_InfoChar((const char *)pNormalTrans->aucSourceAcc, PRINT_LEFT, PF_DOUBLE_SIZE, THERMAL);

    //卡有效期    类型
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(&aucBuf[0], &pNormalTrans->ucExpiredDate[0], 2);
    aucBuf[2] = '/';
    memcpy(&aucBuf[3], &pNormalTrans->ucExpiredDate[2], 2);
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    getInfoBuf(aucBufLeft, "EXPY: ", 6, aucBuf, 5);

    memset(aucBufRight, 0, sizeof(aucBufRight));
    getInfoBuf(aucBufRight, "TYPE: ", 6, (unsigned char *)"QCASH", 5);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //参考号    授权号
#if 0
    PRINT_UTIL_ChangeFont_GroupInfo(PF_DOUBLE_HEIGHT,
                                    (unsigned char *)"REF# ", PF_DOUBLE_HEIGHT,
                                    pNormalTrans->aucRefNum, PF_DOUBLE_HEIGHT,
                                    (unsigned char *)"APPR# ", PF_DOUBLE_HEIGHT,
                                    pNormalTrans->aucAuthCode, PF_DOUBLE_SIZE);
#else
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    getInfoBuf(aucBufLeft, "REF# ", 5, pNormalTrans->aucRefNum, 12);
    getInfoBuf(aucBufRight, "APPR# ", 6, pNormalTrans->aucAuthCode, 6);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
#endif

    //金额统计
    //消费金额
    memset(aucBufRight, 0, sizeof(aucBufRight));
    if(pNormalTrans->transType == TransMode_AdvanceAdjust
    || pNormalTrans->transType == TransMode_DepositAdjust)
        ulAmount += pNormalTrans->ulAdjustAmount;
    else
        ulAmount += pNormalTrans->ulAmount;

    memset(aucBufRight, 0, sizeof(aucBufRight));
    PRINT_FormAmount(aucBufRight, ulAmount, 2, 0, g_constantParam.aucCurrencySign);
    PRINT_UTIL_TransInfo((const unsigned char *)"TOTAL", aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_SIZE, THERMAL);

    //签名提示
    if(MerchantTicket == ucTicketType)
    {
        PRINT_xlinefeed(LINENUM*4);
        PRINT_UTIL_PrintInfoLine((unsigned char *)"SIGN X", THERMAL);
        PRINT_UTIL_Info(g_constantParam.aucAgreeLine1, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
        PRINT_UTIL_Info(g_constantParam.aucAgreeLine2, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
        PRINT_UTIL_Info(g_constantParam.aucAgreeLine3, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
        PRINT_UTIL_Info(g_constantParam.aucAgreeLine4, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    }

    //重印
    if(isReprint)
        PRINT_UTIL_InfoChar("*** DUPLICATE ***", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    //票据类型
    switch(ucTicketType)
    {
    case MerchantTicket:
        PRINT_UTIL_Info(g_constantParam.aucCompanyCopyLabel, PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
        break;
    case CardHolderTicket:
        PRINT_UTIL_InfoChar("--- Card Holder Copy ---", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
        break;
    }
    PRINT_UTIL_PrintLine(THERMAL);
    PRINT_ProjAndVer();
    PRINT_xlinefeed(LINENUM*6);
    PRINT_CheckState();
}

static void PrintTransTicket(NormalTrans *pNormalTrans, unsigned int uiNo)
{
    unsigned char aucBufLeft[40];
    unsigned char aucBufRight[40];
    unsigned int uiOffSet = 0;

    /******************* 第一行 *******************/
    uiOffSet = 0;
    memset(aucBufLeft, ' ', sizeof(aucBufLeft));
    //序号
    int_asc(aucBufLeft, 3, &uiNo);
    aucBufLeft[3] = '.';
    uiOffSet += 6;
    //日期 时间
    getFormDate(pNormalTrans->aucDate, &aucBufLeft[uiOffSet]);
    uiOffSet += 12;
    getFormTime(pNormalTrans->aucTime, &aucBufLeft[uiOffSet]);
    uiOffSet += 5;
    aucBufLeft[uiOffSet] = 0;
    //交易类型
    memset(aucBufRight, 0, sizeof(aucBufRight));
    getTransType(aucBufRight, pNormalTrans->transType);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);
    /******************* 第二行 *******************/
    //卡号
    memset(aucBufLeft, ' ', sizeof(aucBufLeft));
    uiOffSet = 6;
    memcpy(&aucBufLeft[uiOffSet], pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);
    uiOffSet += pNormalTrans->ucSourceAccLen;
    aucBufLeft[uiOffSet] = 0;
    PRINT_UTIL_Info(aucBufLeft, PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);
    /******************* 第三行 *******************/
    memset(aucBufRight, 0, sizeof(aucBufRight));
    if(pNormalTrans->transType == TransMode_AdvanceAdjust
    || pNormalTrans->transType == TransMode_DepositAdjust)
        PRINT_FormAmount(aucBufRight, pNormalTrans->ulAdjustAmount, 2, 0, g_constantParam.aucCurrencySign);
    else
        PRINT_FormAmount(aucBufRight, pNormalTrans->ulAmount, 2, 0, g_constantParam.aucCurrencySign);

    PRINT_UTIL_Info(aucBufRight, PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);
}

static void PrintTransDetailTicket_ex(unsigned long ulTotalNb)
{
    PRINT_UTIL_InfoChar("***TRANSACTION DETAILS***", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    for(unsigned int uiIndex = 0; uiIndex < ulTotalNb; uiIndex++)
    {
        memset(&g_saveTrans, 0, sizeof(NORMAL_TRANS));
        xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, uiIndex);
        PrintTransTicket(&g_saveTrans, uiIndex);
        PRINT_xlinefeed(LINENUM*1);
    }
}

//打印结算单
void PrintSettleTicket(void)
{
    unsigned char aucBuf[20];
    unsigned char aucDateTime[8];
    unsigned char aucBufLeft[40];
    unsigned char aucBufRight[40];

    //商户信息
    PrintTitle();
    PRINT_UTIL_InfoChar("***SETTLEMENT RECEIPT***", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    //交易日期  MMDD,YY  HH:MM
    memset(aucBufLeft, ' ', sizeof(aucBufLeft));
    memcpy(aucBufLeft, "DATE/TIME:", 10);

    memset(aucBuf, 0, sizeof(aucBuf));
    Os__read_date(aucBuf);
    memcpy(&aucBuf[0], "20", 2);
    memcpy(&aucBuf[2], &aucBuf[4], 2);
    memset(aucDateTime, 0, sizeof(aucDateTime));
    asc_bcd(aucDateTime, 8/2, aucBuf, 8);
    getFormDate(aucDateTime, &aucBufLeft[11]);

    memset(aucBuf, 0, sizeof(aucBuf));
    Os__read_time_sec(aucBuf);
    memset(aucDateTime, 0, sizeof(aucDateTime));
    asc_bcd(aucDateTime, 4/2, aucBuf, 4);
    getFormTime(aucDateTime, &aucBufLeft[20]);
    PRINT_UTIL_Info(aucBufLeft, PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);

    //批次号    HOST
    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    long_asc(aucBuf, 6, &g_changeParam.ulBatchNumber);
    getInfoBuf(aucBufLeft, "BATCH# ", 7, aucBuf, 6);

//    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBufRight, 0, sizeof(aucBufRight));
//    long_asc(aucBuf, 6, &pNormalTrans->ulTraceNumber);
//    getInfoBuf(aucBufRight, "TRACE# ", 7, aucBuf, 6);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    PRINT_xlinefeed(LINENUM*1);

    xDATA::ReadValidFile(xDATA::DataSaveTransInfo);
    PrintTransDetailTicket_ex(g_transInfo.ulTransNumber);
    PRINT_xlinefeed(LINENUM*6);
    PRINT_CheckState();
}

//打印交易明细
void PrintTransDetailTicket(void)
{
    //商户信息
    PrintTitle();

    xDATA::ReadValidFile(xDATA::DataSaveTransInfo);
    PrintTransDetailTicket_ex(g_transInfo.ulTransNumber);
    PRINT_xlinefeed(LINENUM*6);
    PRINT_CheckState();
}
