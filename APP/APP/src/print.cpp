#include "print.h"
#include "xdata.h"
#include "commontools.h"
#include <stdio.h>
#include <string.h>
#include "global.h"



extern "C"
{
    #include "ostools.h"
    #include "printUtil.h"
    #include "sand_time.h"
    void Uart_Printf(char *fmt,...);
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
    strcat((char*) pucOut, (const char*) pCurrencySign);
    if (ucNegative == 1)
    {
        strcat((char*) pucOut, "-");
    }
    strcat((char*) pucOut, pcFind);
    return pucOut;
}

void getTransType(unsigned char *pOutData, TransMode m_transType)
{
    switch(m_transType)
    {
    case TransMode_CashDeposit:      //存钱
        sprintf((char*)pOutData, "%s", (char *)"Deposit");
        break;
    case TransMode_CashAdvance:      //取钱
        sprintf((char*)pOutData, "%s", (char *)"Advance");
        break;
    case TransMode_AdvanceVoid:         //撤销
        sprintf((char*)pOutData, "%s", (char *)"Void");
        break;
    case TransMode_CardTransfer:     //转账
        sprintf((char*)pOutData, "%s", (char *)"Transfer");
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
    unsigned char aucBufLeft[25];
    unsigned char aucBufRight[25];

    //PRINT_BMP((const char*)"bmp/logo.bmp");
    //PRINT_xlinefeed(LINENUM*1);

    //标题
    PRINT_UTIL_InfoChar("PRIME BANK LTD", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    //门店信息
    PRINT_UTIL_InfoChar("29 RAJUK AVENUE,MOTIJHEEL,DHAKA", PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //终端号    商户号
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    getInfoBuf(aucBufLeft, "TERM# ", 6, g_constantParam.aucTerminalID, 8);
    getInfoBuf(aucBufRight, "MER# ", 5, g_constantParam.aucMerchantID, 15);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
}

//打印交易小票单
void PrintTransTicket(NormalTrans *pNormalTrans, TicketType ucTicketType, bool isReprint)
{
    unsigned char aucBuf[40];
    unsigned char aucBufLeft[40];
    unsigned char aucBufRight[40];
    unsigned long ulAmount = 0;

//    PRINT_CheckState();
//    return;

    //商户信息
    PrintTitle();

    //卡信息
    PRINT_UTIL_InfoChar("CARD TYPER MASTER", PRINT_LEFT, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_xlinefeed(LINENUM*1);
    //卡号
    PRINT_UTIL_InfoChar((const char *)pNormalTrans->aucSourceAcc, PRINT_LEFT, PF_DOUBLE_SIZE, THERMAL);
    //卡个人信息

    PRINT_xlinefeed(LINENUM*2);

    //交易类型    卡有效期
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memcpy(&aucBufRight[0], &pNormalTrans->ucExpiredDate[0], 2);
    aucBufRight[2] = '/';
    memcpy(&aucBufRight[3], &pNormalTrans->ucExpiredDate[2], 2);

    getTransType(aucBufLeft, pNormalTrans->transType);
    PRINT_UTIL_ChangeFont_GroupInfo(PF_DOUBLE_HEIGHT,
                                    NULL, PF_NORMAL,
                                    aucBufLeft, PF_DOUBLE_SIZE,
                                    (unsigned char *)"EXPY DATE: ", PF_DOUBLE_HEIGHT,
                                    aucBufRight, PF_DOUBLE_HEIGHT);

    //批次号    流水号
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &pNormalTrans->ulBatchNumber);
    getInfoBuf(aucBufLeft, "BATCH NO.: ", 11, aucBuf, 6);

    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &pNormalTrans->ulTraceNumber);
    getInfoBuf(aucBufRight, "TRACE NO.: ", 11, aucBuf, 6);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //交易日期  MMDD,YY  HH:MM
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memset(aucBuf, 0, sizeof(aucBuf));
    getFormDate(pNormalTrans->aucDate, aucBuf);
    getInfoBuf(aucBufLeft, "DATE: ", 6, aucBuf, strlen((char *)aucBuf));

    memset(aucBuf, 0, sizeof(aucBuf));
    getFormTime(pNormalTrans->aucTime, aucBuf);
    getInfoBuf(aucBufRight, "TIME: ", 6, aucBuf, strlen((char *)aucBuf));

    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //参考号    授权号
    PRINT_UTIL_ChangeFont_GroupInfo(PF_DOUBLE_HEIGHT,
                                    (unsigned char *)"REF.NO.: ", PF_DOUBLE_HEIGHT,
                                    pNormalTrans->aucRefNum, PF_DOUBLE_HEIGHT,
                                    (unsigned char *)"APPR.CODE: ", PF_DOUBLE_HEIGHT,
                                    pNormalTrans->aucAuthCode, PF_DOUBLE_SIZE);

    //金额统计
    //消费金额
    memset(aucBufRight, 0, sizeof(aucBufRight));
    PRINT_FormAmount(aucBufRight, pNormalTrans->ulAmount, 2, 0, g_constantParam.aucCurrencySign);
    ulAmount += pNormalTrans->ulAmount;
    PRINT_UTIL_TransInfo((const unsigned char *)"BASE", aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_SIZE, THERMAL);
//    //小费
//    memset(aucBufRight, 0, sizeof(aucBufRight));
//    sprintf((char*)aucBufRight, "%s",(char *)"TK          0.00");
//    PRINT_UTIL_TransInfo((const unsigned char *)"TIP ", aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_TransInfo((const unsigned char *)"", (const unsigned char *)"------------", 12, PRINT_RIGHT, PF_NORMAL, THERMAL);
    //总计
    memset(aucBufRight, 0, sizeof(aucBufRight));
    PRINT_FormAmount(aucBufRight, ulAmount, 2, 0, g_constantParam.aucCurrencySign);
    PRINT_UTIL_TransInfo((const unsigned char *)"TOTAL", aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_SIZE, THERMAL);

    //签名提示
    if(MerchantTicket == ucTicketType)
    {
        PRINT_xlinefeed(LINENUM*5);
        PRINT_UTIL_PrintInfoLine((unsigned char *)"SIGN X", THERMAL);
        PRINT_UTIL_InfoChar("I AGREE TO PAY ABOVE TOTAL AMOUNT", PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
        PRINT_UTIL_InfoChar("ACCORDING TO CARD ISSVER AGREEMENT", PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    }

    PRINT_xlinefeed(LINENUM*2);
    //重印
    if(isReprint)
        PRINT_UTIL_InfoChar("*** DUPLICATE ***", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    //票据类型
    switch(ucTicketType)
    {
    case MerchantTicket:
        PRINT_UTIL_InfoChar("--- Merchant Copy ---", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
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

//打印结算单
void PrintSettleTicket(void)
{
    unsigned char aucBuf[50];
    unsigned char aucBuf2[50];
    unsigned char aucBufMid[50];
    unsigned char aucBufLeft[50];
    unsigned char aucBufRight[50];

    unsigned char   aucDate[7];    //  记录系统当前日期
    unsigned char   aucTime[7];    //  记录系统当前时间

    //商户信息
    PrintTitle();

    //批次号 打印要在显示在中间
    memset(aucBufMid, 0, sizeof(aucBufMid));

    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &NormalTransData.ulBatchNumber);
    getInfoBuf(aucBufMid, "BATCH #:", 8, aucBuf, 6);

    PRINT_UTIL_InfoChar((const char*)aucBufMid, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //打印系统当前日期时间
    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBuf2, 0, sizeof(aucBuf2));
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    memset(aucDate, 0, sizeof(aucDate));
    memset(aucTime, 0, sizeof(aucTime));

    Os__read_date(aucDate);     //aucData: 230713
    Uart_Printf("\naucData = %s\n", aucDate);
    memcpy(&aucBuf[0],&aucDate[4],2);
    memcpy(&aucBuf[2],&aucDate[2],2);
    memcpy(&aucBuf[4],&aucDate[0],2);
    aucBuf2[0]=0x20;
    asc_bcd(&aucBuf2[1],3,aucBuf,6);
    getFormDate(aucBuf2, aucBufLeft);

    Os__read_time_sec(aucTime);
    Uart_Printf("\naucTime = %s\n", aucTime);
    memset(aucBuf,0,sizeof(aucBuf));
    asc_bcd(aucBuf,3,aucTime,6);
    getFormTime(aucBuf, aucBufRight);

    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);

    //

    PRINT_xlinefeed(LINENUM*2);
    PRINT_UTIL_InfoChar("BANKED TOTALS REPORT", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    //结算批次发起的起始日期 时间
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memset(aucBuf, 0, sizeof(aucBuf));
    getFormDate(NormalTransData.aucDate, aucBuf);   //日期
    getInfoBuf(aucBufLeft, "START: ", 7, aucBuf, strlen((char *)aucBuf));
    getFormTime(NormalTransData.aucTime, aucBufRight);

    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);


    //********************** 分类统计 **********************
    PRINT_xlinefeed(LINENUM*1);
    //交易类型
    PRINT_UTIL_InfoChar("ADVANCE", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);

    PRINT_xlinefeed(LINENUM*1);
    //交易类型      笔数      货币符号        金额
    PRINT_UTIL_TransInfo((const unsigned char *)"TOTAL", (const unsigned char *)"AMOUNT", 6, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_UTIL_TransInfo((const unsigned char *)"--------", (const unsigned char *)"--------", 8, PRINT_MIDDLE, PF_NORMAL, THERMAL);

    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    memset(aucBuf, 0, sizeof(aucBuf));
    sprintf((char*)aucBufLeft, "%d", g_transInfo.TransTotal.uiAdvanceNb);
    PRINT_FormAmount(aucBufRight, g_transInfo.TransTotal.ulAdvanceAmount, 2, 0, g_constantParam.aucCurrencySign);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    //笔数      货币符号        金额
    //PRINT_UTIL_TransInfo((const unsigned char *)"--------", (const unsigned char *)"    --------", 12, PRINT_RIGHT, PF_NORMAL, THERMAL);

    //交易类型
    PRINT_UTIL_InfoChar("DEPOSIT", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    PRINT_UTIL_TransInfo((const unsigned char *)"TOTAL", (const unsigned char *)"AMOUNT", 6, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_UTIL_TransInfo((const unsigned char *)"--------", (const unsigned char *)"--------", 8, PRINT_MIDDLE, PF_NORMAL, THERMAL);

    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    memset(aucBuf, 0, sizeof(aucBuf));
    sprintf((char*)aucBufLeft, "%d", g_transInfo.TransTotal.uiDepositNb);
    PRINT_FormAmount(aucBufRight, g_transInfo.TransTotal.ulDepositAmount, 2, 0, g_constantParam.aucCurrencySign);
    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    //********************** 统计 **********************
    //    PRINT_xlinefeed(LINENUM*1);
    //    PRINT_UTIL_InfoChar("GRAND TOTAL", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    //    PRINT_xlinefeed(LINENUM*1);
    //    //交易类型      笔数      货币符号        金额
    //    PRINT_UTIL_TransInfo((const unsigned char *)"--------", (const unsigned char *)"    --------", 12, PRINT_RIGHT, PF_NORMAL, THERMAL);
    //    //笔数      货币符号        金额
    //    PRINT_UTIL_TransInfo((const unsigned char *)"--------", (const unsigned char *)"    --------", 12, PRINT_RIGHT, PF_NORMAL, THERMAL);

    //*************************************************

    //结束  日期 时间
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));

    memset(aucBuf, 0, sizeof(aucBuf));
    getFormDate(NormalTransData.aucDate, aucBuf);   //日期
    getInfoBuf(aucBufLeft, "ENDED: ", 7, aucBuf, strlen((char *)aucBuf));
    getFormTime(NormalTransData.aucTime, aucBufRight);

    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    //
    PRINT_UTIL_InfoChar("SETTLEMENT OK", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_InfoChar("THANK YOU", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_InfoChar("PRIME BANK LTD.", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    PRINT_UTIL_PrintLine(THERMAL);
    PRINT_ProjAndVer();
    PRINT_xlinefeed(LINENUM*6);
    PRINT_CheckState();
}

//打印交易明细
void PrintTransDetailTicket(void)
{
    unsigned char aucBuf[50];
    unsigned char aucBuf2[50];
    unsigned char aucBufMid[50];
    unsigned char aucBufLeft[50];
    unsigned char aucBufRight[50];

    unsigned char   aucDate[7];    //  记录系统当前日期
    unsigned char   aucTime[7];    //  记录系统当前时间

    //商户信息
    PrintTitle();

    //批次号 打印要在显示在中间
    memset(aucBufMid, 0, sizeof(aucBufMid));

    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &NormalTransData.ulBatchNumber);
    getInfoBuf(aucBufMid, "BATCH #:", 8, aucBuf, 6);

    PRINT_UTIL_InfoChar((const char*)aucBufMid, PRINT_MIDDLE, PF_DOUBLE_HEIGHT, THERMAL);

    //打印系统当前时间
    memset(aucBuf, 0, sizeof(aucBuf));
    memset(aucBuf2, 0, sizeof(aucBuf2));
    memset(aucBufLeft, 0, sizeof(aucBufLeft));
    memset(aucBufRight, 0, sizeof(aucBufRight));
    memset(aucDate, 0, sizeof(aucDate));
    memset(aucTime, 0, sizeof(aucTime));

    Os__read_date(aucDate);     //aucData: 230713
    Uart_Printf("\naucData = %s\n", aucDate);
    memcpy(&aucBuf[0],&aucDate[4],2);
    memcpy(&aucBuf[2],&aucDate[2],2);
    memcpy(&aucBuf[4],&aucDate[0],2);
    aucBuf2[0]=0x20;
    asc_bcd(&aucBuf2[1],3,aucBuf,6);
    getFormDate(aucBuf2, aucBufLeft);

    Os__read_time_sec(aucTime);
    Uart_Printf("\naucTime = %s\n", aucTime);
    memset(aucBuf,0,sizeof(aucBuf));
    asc_bcd(aucBuf,3,aucTime,6);
    getFormTime(aucBuf, aucBufRight);

    PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);

    //
    PRINT_xlinefeed(LINENUM*2);
    PRINT_UTIL_InfoChar("BATCH ANALYSIS REPORT", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    //交易详细清单
    for(int index = 0; index < ((int)g_constantParam.uiMaxTotalNb); index++)
    {
        if(g_transInfo.auiTransIndex[index])
        {
            //:- 读取数据保存到NormalTransData
            memset(&NormalTransData,0,sizeof(NORMAL_TRANS));
            xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, index);
            memcpy(&NormalTransData,&g_saveTrans,sizeof(NORMAL_TRANS));

            //清单第一行
            memset(aucBuf, 0, sizeof(aucBuf));
            memset(aucBufLeft, 0, sizeof(aucBufLeft));
            memset(aucBufRight, 0, sizeof(aucBufRight));

            PRINT_FormAmount(aucBuf, NormalTransData.ulAmount, 2, 0, g_constantParam.aucCurrencySign);
            getInfoBuf(aucBufLeft, (const char*)NormalTransData.aucSourceAcc, TRANS_ACCLEN, aucBuf,  strlen((char *)aucBuf));
            getInfoBuf(aucBufRight, "Appr. ", 6, NormalTransData.aucAuthCode, TRANS_AUTHCODE_LEN);

            PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);

            //清单第二行
            memset(aucBuf, 0, sizeof(aucBuf));
            memset(aucBuf2, 0, sizeof(aucBuf2));
            memset(aucBufLeft, 0, sizeof(aucBufLeft));
            memset(aucBufRight, 0, sizeof(aucBufRight));

            getFormDate(NormalTransData.aucDate, aucBuf);   //日期
            memcpy(&aucBuf[10], "  ", 2);
            getFormTime(NormalTransData.aucTime, aucBuf2);  //时间
            getInfoBuf(aucBufLeft, (const char*)aucBuf, strlen((char*)aucBuf), aucBuf2, strlen((char*)aucBuf2));

            memset(aucBuf, 0, sizeof(aucBuf));

            memcpy(&aucBuf[0], &NormalTransData.ucExpiredDate[0], 2);
            aucBuf[2] = '/';
            memcpy(&aucBuf[3], &NormalTransData.ucExpiredDate[2], 2);

            getInfoBuf(aucBufRight, "Exp. ", 5, aucBuf, strlen((char*)aucBuf));

            PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);

            //清单第三行
            memset(aucBuf, 0, sizeof(aucBuf));
            memset(aucBufLeft, 0, sizeof(aucBufLeft));
            memset(aucBufRight, 0, sizeof(aucBufRight));

            switch(NormalTransData.transType)
            {
            case TransMode_CashDeposit: //存钱
                memcpy(aucBufLeft, "CashDeposit", 11);
                break;

            case TransMode_CashAdvance: //取钱
                memcpy(aucBufLeft, "CashAdvance", 11);
                break;

            case TransMode_AdvanceVoid:    //撤销
                memcpy(aucBufLeft, "CashVoid", 8);
                break;

            case TransMode_CardTransfer: //转账
                memcpy(aucBufLeft, "CashTransfer", 12);
                break;

            case TransMode_BalanceInquiry: //查余
                memcpy(aucBufLeft, "BalanceInquiry", 14);
                break;

             default:
                memcpy(aucBufLeft, "Unknown Trans", 13);
                break;
            }

            long_asc(aucBuf, 6, &NormalTransData.ulTraceNumber);
            getInfoBuf(aucBufRight, "Trace: ", 7, aucBuf, 6);

            PRINT_UTIL_TransInfo(aucBufLeft, aucBufRight, strlen((char *)aucBufRight), PRINT_RIGHT, PF_DOUBLE_HEIGHT, THERMAL);
            PRINT_xlinefeed(LINENUM*1);
        }
    }


    //
    PRINT_UTIL_InfoChar("END OF REPORT", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_UTIL_InfoChar("PRIME BANK LTD.", PRINT_MIDDLE, PF_DOUBLE_SIZE, THERMAL);
    PRINT_xlinefeed(LINENUM*1);

    PRINT_UTIL_PrintLine(THERMAL);
    PRINT_ProjAndVer();
    PRINT_xlinefeed(LINENUM*6);
    PRINT_CheckState();
}

