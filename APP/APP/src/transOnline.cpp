#include "transonline.h"
#include "iso8583.h"

#include "global.h"
#include "xdata.h"

extern "C"{
#include "ostools.h"
#include "sand_time.h"
#include "key.h"
}

void TRANS_ONLINE_GetEntryMode(InputModeIndex m_InputModeIndex, int isPIN)
{
    unsigned char aucBuf[5];

    typedef struct
    {
        unsigned char   inputCode[3];
    }InputMode;

    const InputMode inputModeTab[INPUTMODE_MaxIndex] =
    {
        {"01"},
        {"02"},
        {"05"},
        {"07"},
        {"09"},
        {"80"},
        {"90"},
        {"91"}
    };

    // Set bit 22
    memset(aucBuf, 0, sizeof(aucBuf));
    aucBuf[0] = '0';
    memcpy(&aucBuf[1], inputModeTab[m_InputModeIndex].inputCode, 2);
    if(isPIN == 1)
        aucBuf[3] = '1';
    else
        aucBuf[3] = '2';
    ISO8583_SetBitValue(22, aucBuf, 4);
}

unsigned char TRANS_ONLINE_GetResponseCode(void)
{
    unsigned char   ucResult = SUCCESS_TRACKDATA;
    unsigned char   aucBuf[3];
    short    iLen = 0;

    memset(aucBuf, 0, sizeof(aucBuf));
    ucResult = ISO8583_GetBitValue(39, aucBuf, &iLen, sizeof(aucBuf));
    if(!ucResult)
    {
        if(!memcmp(aucBuf, "94", 2))
        {
            g_changeParam.ulTransNum += 50;
            xDATA::WriteValidFile(xDATA::DataSaveChange);
        }

        if(memcmp(aucBuf, "00", 2))
        {
            SetHostError((HostErrIndex)asc_long(aucBuf, 2));
            ucResult = ERR_HOSTCODE;
        }
    }
    return ucResult;
}

void TRANS_ONLINE_GetTransDataTime(unsigned char *pData, unsigned char *pTime)
{
    unsigned char aucBuf[8];
    short sBufLen = 0;

    //get bit12(M)
    memset(aucBuf, 0, sizeof(aucBuf));
    if(!ISO8583_GetBitValue(12, aucBuf, &sBufLen, sizeof(aucBuf)))
        asc_bcd(pTime, sBufLen/2, aucBuf, sBufLen);

    //get bit13(M)
    memset(aucBuf, 0, sizeof(aucBuf));
    if(!ISO8583_GetBitValue(13, aucBuf, &sBufLen, sizeof(aucBuf)))
    {
        pData[0] = 0x20;
        pData[1] = 0x13;
        asc_bcd(&pData[2], sBufLen/2, aucBuf, sBufLen);
    }
}

unsigned int TRANS_ONLINE_SetTransInfo(unsigned char *pOutData, unsigned int uiTotalNum, unsigned long ulTotalAmount)
{
    unsigned int uiOffSet;

    uiOffSet = 0;
    int_asc(&pOutData[uiOffSet], 3, &uiTotalNum);
    uiOffSet += 3;
    long_asc(&pOutData[uiOffSet], 12, &ulTotalAmount);
    uiOffSet += 12;

    return uiOffSet;
}

unsigned int TRANS_ONLINE_getTAG_pack(unsigned short usTagID, unsigned char *pTag, unsigned int uiTagLen, unsigned char *pOutData)
{
    unsigned int iLen = 0;

    //总长度
    iLen = uiTagLen + 2;
    long_asc(&pOutData[0], 3, (unsigned long *)&iLen);
    iLen += 3;

    //ID
    short_asc(&pOutData[3], 2, &usTagID);

    //内容
    memcpy(&pOutData[5], pTag, uiTagLen);

    return iLen;
}

unsigned char TRANS_ONLINE_CheckResponseValid(TransMode m_transMode)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;

    ucResult = ISO8583_CompareSentBit(3);
    if(!ucResult)
        ucResult = TRANS_ONLINE_GetResponseCode();
//    if(!ucResult)
//        ucResult = ISO8583_CompareSentBit(11);
    if(!ucResult)
        ucResult = ISO8583_CompareSentBit(24);
    if(!ucResult)
        ucResult = ISO8583_CompareSentBit(41);

    switch(m_transMode)
    {
    case TransMode_CashDeposit:      //存钱
    case TransMode_CashAdvance:      //取钱
        if(!ucResult)
            ucResult = ISO8583_CompareSentBit(4);
    case TransMode_CashVoid:         //撤销
    case TransMode_BalanceInquiry:   //查余
    case TransMode_CardTransfer:     //转账
        if(!ucResult)
            ucResult = ISO8583_CompareSentBit(49);
        break;
    default:
        break;
    }
    return ucResult;
}

void TRANS_ONLINE_ReversalPack(void)
{
#if 0
    /* save to backup first */
    memcpy(&Trans_8583Data.SendISO8583Data, &ISO8583Data, sizeof(BACKUPISO8583));
    ISO8583_SetMsgID(400);
    ISO8583_RemoveBit(2);
    ISO8583_RemoveBit(12);
    ISO8583_RemoveBit(13);
    ISO8583_RemoveBit(48);
    ISO8583_RemoveBit(52);
    /* Save new reversal */
    ISO8583_SaveReversalISO8583Data((const unsigned char*)"00");
    /* restore from backup at last */
    memcpy(&ISO8583Data, &Trans_8583Data.SendISO8583Data, sizeof(BACKUPISO8583));
#endif
}

unsigned char TRANS_ONLINE_DownWK_pack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[10];

    //03,11,12,13,24,41
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(800);

    // Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    ISO8583_SetBitValue(3, (unsigned char *)"920000", 6);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 12
    memset(aucBuf, 0x00, sizeof(aucBuf));
    Os__read_time_sec(aucBuf);
    ISO8583_SetBitValue(12, aucBuf, 6);

    // Set bit 13
    memset(aucBuf, 0x00, sizeof(aucBuf));
    Os__read_date(&aucBuf[2]);
    memcpy(&aucBuf[0], &aucBuf[4], 2);
    ISO8583_SetBitValue(13, aucBuf, 4);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    return ucResult;
}

unsigned char TRANS_ONLINE_DownWK_unpack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[35];
    short iLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(TransMode_DownWK);
    //get bit62 工作密钥
    if(!ucResult)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        ucResult = ISO8583_GetBitValue(62, aucBuf, &iLen, sizeof(aucBuf));
        if(!ucResult)
        {
            if(memcmp(&aucBuf[8], "FFFFFFFF", 8))
            {
//                g_changeParam.boolMacKeyFlag = true;
                xDATA::WriteValidFile(xDATA::DataSaveChange);
            }
            ucResult = KEY_StoreSingleNewKey(&aucBuf[2], (iLen - 2));
        }
    }
    return ucResult;
}

unsigned char TRANS_ONLINE_DownEWK_pack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[10];

    //03,11,12,13,41
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(800);

    // Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    ISO8583_SetBitValue(3, (unsigned char *)"950000", 6);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 12
    memset(aucBuf, 0x00, sizeof(aucBuf));
    Os__read_time_sec(aucBuf);
    ISO8583_SetBitValue(12, aucBuf, 6);

    // Set bit 13
    memset(aucBuf, 0x00, sizeof(aucBuf));
    Os__read_date(&aucBuf[2]);
    memcpy(&aucBuf[0], &aucBuf[4], 2);
    ISO8583_SetBitValue(13, aucBuf, 4);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    return ucResult;
}

unsigned char TRANS_ONLINE_DownEWK_unpack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[35];
    short iLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(TransMode_DownEWK);
    //get bit62 工作密钥
    if(!ucResult)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        ucResult = ISO8583_GetBitValue(62, aucBuf, &iLen, sizeof(aucBuf));
        if(!ucResult)
            ucResult = KEY_StoreSingleNewKey(&aucBuf[2], (iLen - 2));
    }
    return ucResult;
}

//存款
unsigned char TRANS_ONLINE_Deposit_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];

    //03,04,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(200);

    // Set bit 03 Processing Code
    // 1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, (unsigned char *)"210000", 6);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 49
    memset(aucBuf, 0, sizeof(aucBuf));
    short_asc(aucBuf, PARAM_CURRENCY_CODE_LEN, &g_constantParam.usCurrencyId);
    ISO8583_SetBitValue(49, aucBuf, PARAM_CURRENCY_CODE_LEN);

    switch(pNormalTrans->inputMode)
    {
    case INPUTMODE_Manual:
        // Set bit 02
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);
        break;
    case INPUTMODE_MagneticStripe2:
        // Set bit 35
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);
        break;
    default:
        break;
    }

    if(pExtraTrans->ucInputPINLen)
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 1);
        // Set bit 52
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);
    }
    else
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 0);
    }

    // Set bit 62
    if(g_changeParam.ulInvoiceNum)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
        ISO8583_SetBitValue(62, aucBuf, 6);
    }

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_Deposit_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short sBufLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(pNormalTrans->transType);
    if(!ucResult)
    {
        //get bit12 trans time and get bit13 trans data
        TRANS_ONLINE_GetTransDataTime(pNormalTrans->aucDate, pNormalTrans->aucTime);
        //get bit37
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(37, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucRefNum, aucBuf, TRANS_REFNUM_LEN);
        //get bit38
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(38, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucAuthCode, aucBuf, TRANS_AUTHCODE_LEN);
    }
    return ucResult;
}

//取款
unsigned char TRANS_ONLINE_Advance_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];

    //03,04,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(200);

    // Set bit 03 Processing Code
    // 1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, (unsigned char *)"010000", 6);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 49
    memset(aucBuf, 0, sizeof(aucBuf));
    short_asc(aucBuf, PARAM_CURRENCY_CODE_LEN, &g_constantParam.usCurrencyId);
    ISO8583_SetBitValue(49, aucBuf, PARAM_CURRENCY_CODE_LEN);

    switch(pNormalTrans->inputMode)
    {
    case INPUTMODE_Manual:
        // Set bit 02
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);
        break;
    case INPUTMODE_MagneticStripe2:
        // Set bit 35
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);
        break;
    default:
        break;
    }

    if(pExtraTrans->ucInputPINLen)
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 1);
        // Set bit 52
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);
    }
    else
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 0);
    }

    // Set bit 62
    if(g_changeParam.ulInvoiceNum)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
        ISO8583_SetBitValue(62, aucBuf, 6);
    }

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_Advance_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short sBufLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(pNormalTrans->transType);
    if(!ucResult)
    {
        //get bit12 trans time and get bit13 trans data
        TRANS_ONLINE_GetTransDataTime(pNormalTrans->aucDate, pNormalTrans->aucTime);
        //get bit37
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(37, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucRefNum, aucBuf, TRANS_REFNUM_LEN);
        //get bit38
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(38, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucAuthCode, aucBuf, TRANS_AUTHCODE_LEN);
    }
    return ucResult;
}

//查余
unsigned char TRANS_ONLINE_BalanceInquiry_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[6];

    //03,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(100);

    // Set bit 03 Processing Code
    // 1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, (unsigned char *)"310000", 6);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 49
    memset(aucBuf, 0, sizeof(aucBuf));
    short_asc(aucBuf, PARAM_CURRENCY_CODE_LEN, &g_constantParam.usCurrencyId);
    ISO8583_SetBitValue(49, aucBuf, PARAM_CURRENCY_CODE_LEN);

    switch(pNormalTrans->inputMode)
    {
    case INPUTMODE_Manual:
        // Set bit 02
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);
        break;
    case INPUTMODE_MagneticStripe2:
        // Set bit 35
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);
        break;
    default:
        break;
    }

    if(pExtraTrans->ucInputPINLen)
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 1);
        // Set bit 52
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);
    }
    else
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 0);
    }

    // Set bit 62
    if(g_changeParam.ulInvoiceNum)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
        ISO8583_SetBitValue(62, aucBuf, 6);
    }

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_BalanceInquiry_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short sBufLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(pNormalTrans->transType);
    if(!ucResult)
    {
        //04
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(4, aucBuf, &sBufLen, sizeof(aucBuf)))
        {
            pNormalTrans->ulBalance = asc_long(aucBuf, 12);
        }
        //get bit12 trans time and get bit13 trans data
        TRANS_ONLINE_GetTransDataTime(pNormalTrans->aucDate, pNormalTrans->aucTime);
        //get bit37
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(37, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucRefNum, aucBuf, TRANS_REFNUM_LEN);
        //get bit38
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(38, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucAuthCode, aucBuf, TRANS_AUTHCODE_LEN);

        //63
    }
    return ucResult;
}

//转账
unsigned char TRANS_ONLINE_Transfer_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[100];
    unsigned int uiOffSet = 0;

    //03,04,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(200);

    // Set bit 03 Processing Code
    // 1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    memcpy(aucBuf, (unsigned char *)"610000", 6);
    aucBuf[2] = pNormalTrans->accType;
    aucBuf[4] = pNormalTrans->toAccType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 49
    memset(aucBuf, 0, sizeof(aucBuf));
    short_asc(aucBuf, PARAM_CURRENCY_CODE_LEN, &g_constantParam.usCurrencyId);
    ISO8583_SetBitValue(49, aucBuf, PARAM_CURRENCY_CODE_LEN);

    switch(pNormalTrans->inputMode)
    {
    case INPUTMODE_Manual:
        // Set bit 02
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);
        break;
    case INPUTMODE_MagneticStripe2:
        // Set bit 35
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);
        break;
    default:
        break;
    }

    if(pExtraTrans->ucInputPINLen)
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 1);
        // Set bit 52
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);
    }
    else
    {
        // Set bit 22
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 0);
    }

    // Set bit 57
    memset(aucBuf, 0, sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_ONLINE_getTAG_pack(61, (unsigned char *)"1", 1, &aucBuf[uiOffSet]);
    uiOffSet += TRANS_ONLINE_getTAG_pack(62, pNormalTrans->aucToAcc, pNormalTrans->ucToAccLen, &aucBuf[uiOffSet]);
    ISO8583_SetBitValue(57, aucBuf, uiOffSet);

    // Set bit 62
    if(g_changeParam.ulInvoiceNum)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
        ISO8583_SetBitValue(62, aucBuf, 6);
    }

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (const unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_Transfer_unpack(NormalTrans *pNormalTrans)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short sBufLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(pNormalTrans->transType);
    if(!ucResult)
    {
        //get bit12 trans time and get bit13 trans data
        TRANS_ONLINE_GetTransDataTime(pNormalTrans->aucDate, pNormalTrans->aucTime);
        //get bit37
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(37, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucRefNum, aucBuf, TRANS_REFNUM_LEN);
        //get bit38
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(38, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucAuthCode, aucBuf, TRANS_AUTHCODE_LEN);
    }
    return ucResult;
}

//结算
unsigned char TRANS_ONLINE_Settlement_pack(TRANSTOTAL *pTransTotal)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[100];
    unsigned int uiOffSet = 0;

    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(500);

    // Set bit 03 Processing Code
    ISO8583_SetBitValue(3, (unsigned char *)"920000", 6);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    memcpy(g_constantParam.aucMerchantID, (unsigned char*)"000120100023348", 15);  //:- 死
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 60
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulBatchNumber);
    ISO8583_SetBitValue(60, aucBuf, 6);

    // Set bit 63
    memset(aucBuf, '0', sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiCDebitNb, pTransTotal->ulCDebitAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiCCreditNb, pTransTotal->ulCCreditAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiDDebitNb, pTransTotal->ulDDebitAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiDCreditNb, pTransTotal->ulDCreditAmount);
    ISO8583_SetBitValue(63, aucBuf, uiOffSet);

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (const unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_Settlement_unpack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short    iLen = 0;

    memset(aucBuf, 0, sizeof(aucBuf));
    ucResult = ISO8583_GetBitValue(39, aucBuf, &iLen, sizeof(aucBuf));
    if(!ucResult)
    {
        if(memcmp(aucBuf, "95", 2))     //结算不平
        {
            ucResult = ERR_SETTLE_UNBALANCE;
        }
        else if(memcmp(aucBuf, "00", 2))
        {
            SetHostError((HostErrIndex)asc_long(aucBuf, 2));
            ucResult = ERR_HOSTCODE;
        }
    }
    return ucResult;
}

//批上送
unsigned char TRANS_ONLINE_BatchUpload_pack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];

    //02,03,04,11,12,13,14,22,24,25,37,38,39,41,42,60,62,64
    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(320);

    // Set bit 03 Processing Code
    ISO8583_SetBitValue(3, (unsigned char *)"000000", 6);

    // Set bit 02
    ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 12

    // Set bit 13

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 22
//    if(pExtraTrans->ucInputPINLen)
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 1);
//    else
        TRANS_ONLINE_GetEntryMode(pNormalTrans->inputMode, 0);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 60

    // Set bit 62
    if(g_changeParam.ulInvoiceNum)
    {
        memset(aucBuf, 0, sizeof(aucBuf));
        long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
        ISO8583_SetBitValue(62, aucBuf, 6);
    }

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_BatchUpload_unpack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;

    return ucResult;
}

//批上送结束
unsigned char TRANS_ONLINE_BatchUploadEnd_pack(TRANSTOTAL *pTransTotal)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[100];
    unsigned int uiOffSet = 0;

    ISO8583_Clear();

    // Set Msg ID
    ISO8583_SetMsgID(500);

    // Set bit 03 Processing Code
    ISO8583_SetBitValue(3, (unsigned char *)"960000", 6);

    // Set bit 11 trans number
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulTransNum);
    ISO8583_SetBitValue(11, aucBuf, 6);

    // Set bit 24
    ISO8583_SetBitValue(24, g_constantParam.aucNii, PARAM_NII_LEN);

    // Set bit 41
    ISO8583_SetBitValue(41, g_constantParam.aucTerminalID, PARAM_TERMINALID_LEN);

    // Set bit 42
    ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);

    // Set bit 60
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulBatchNumber);
    ISO8583_SetBitValue(60, aucBuf, 6);

    // Set bit 63
    memset(aucBuf, '0', sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiCDebitNb, pTransTotal->ulCDebitAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiCCreditNb, pTransTotal->ulCCreditAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiDDebitNb, pTransTotal->ulDDebitAmount);
    uiOffSet += TRANS_ONLINE_SetTransInfo(&aucBuf[uiOffSet], pTransTotal->uiDCreditNb, pTransTotal->ulDCreditAmount);
    ISO8583_SetBitValue(63, aucBuf, uiOffSet);

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (const unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_BatchUploadEnd_unpack(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];
    short    iLen = 0;

    memset(aucBuf, 0, sizeof(aucBuf));
    ucResult = ISO8583_GetBitValue(39, aucBuf, &iLen, sizeof(aucBuf));
    if(!ucResult)
    {
        if(memcmp(aucBuf, "95", 2))     //结算不平
        {
            ucResult = ERR_SETTLE_UNBALANCE;
        }
        else if(memcmp(aucBuf, "00", 2))
        {
            SetHostError((HostErrIndex)asc_long(aucBuf, 2));
            ucResult = ERR_HOSTCODE;
        }
    }
    return ucResult;
}
