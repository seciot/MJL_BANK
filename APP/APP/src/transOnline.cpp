#include "transonline.h"
#include "iso8583.h"

#include "global.h"
#include "xdata.h"

extern "C"{
#include "ostools.h"
#include "sand_time.h"
#include "key.h"
}

static unsigned int TRANS_Tool_TotalPack(unsigned char *pOutData, unsigned int uiTotalNum, unsigned long ulTotalAmount)
{
    unsigned int uiOffSet;

    uiOffSet = 0;
    int_asc(&pOutData[uiOffSet], 3, &uiTotalNum);
    uiOffSet += 3;
    long_asc(&pOutData[uiOffSet], 12, &ulTotalAmount);
    uiOffSet += 12;

    return uiOffSet;
}

static unsigned int TRANS_Tool_TagPack(unsigned short usTagID, unsigned char *pTag, unsigned int uiTagLen, unsigned char *pOutData)
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

unsigned char TRANS_ONLINE_GetResponseCode(void)
{
    unsigned char   ucResult = SUCCESS_TRACKDATA;
    unsigned char   aucBuf[3];
    short    iLen = 0;

    memset(aucBuf, 0, sizeof(aucBuf));
    ucResult = ISO8583_GetBitValue(39, aucBuf, &iLen, sizeof(aucBuf));
    if(!ucResult)
    {
        if(memcmp(aucBuf, "00", 2))
        {
            SetHostError((HostErrIndex)asc_long(aucBuf, 2));
            ucResult = ERR_HOSTCODE;
        }

        if(ucResult)
        {
            switch(GetHostError())
            {
            case HOSTERR_94:        //重复流水
                g_changeParam.ulTransNum += 50;
                xDATA::WriteValidFile(xDATA::DataSaveChange);
                break;
            case HOSTERR_95:        //结算不平
                ucResult = ERR_SETTLE_UNBALANCE;
                break;
            default:
                break;
            }
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
    Os__read_date(aucBuf);
    memcpy(aucBuf, &aucBuf[4], 2);
    if(!ISO8583_GetBitValue(13, &aucBuf[2], &sBufLen, sizeof(aucBuf)))
    {
//        pData[0] = 0x20;
//        pData[1] = 0x13;
//        asc_bcd(&pData[2], sBufLen/2, aucBuf, sBufLen);
        asc_bcd(pData, (sBufLen+2)/2, aucBuf, (sBufLen+2));
    }
}

static void TRANS_ONLINE_SetProCode(TransMode m_transMode, unsigned char *pProCode)
{
    typedef struct
    {
        unsigned short  msgID;
        unsigned char   aucCode[7];
    }TransCode;

    TransCode transCodeTab[TramsMode_MaxIndex] =
    {
        {800, "920000"},    //签到(工作密钥)
        {800, "950000"},    //签到(传输密钥)
        {500, "920000"},    //结算
        {100, "310000"},    //查余
        {200, "210000"},    //存钱
        {200, "220000"},    //存钱撤销
        {200, "010000"},    //取钱
        {200, "020000"},    //取钱撤销
        {200, "610000"},    //转账
        {200, "700000"},    //改密
        {000, "000000"},    //调整
    };

    // Set Msg ID
    ISO8583_SetMsgID(transCodeTab[m_transMode].msgID);
    memcpy(pProCode, transCodeTab[m_transMode].aucCode, 6);
}

static void TRANS_ONLINE_SetEntryMode(InputModeIndex m_InputModeIndex, bool isInputPin)
{
    unsigned char aucBuf[5];

    typedef struct
    {
        unsigned char inputCode[3];
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
    if(isInputPin)
        aucBuf[3] = '1';
    else
        aucBuf[3] = '2';
    ISO8583_SetBitValue(22, aucBuf, 4);
}

static void TRANS_ONLINE_CommonInfo_pack(void)
{
    unsigned char aucBuf[20];

    //11,12,13,24,41,42

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

    // Set bit 42
    if(strlen((char *)g_constantParam.aucMerchantID))
        ISO8583_SetBitValue(42, g_constantParam.aucMerchantID, PARAM_MERCHANTID_LEN);
}

static void TRANS_ONLINE_CommonTransInfo_pack(NormalTrans *pNormalTrans)
{
    unsigned char aucBuf[10];

    //(11,12,13,24,41,42)14,22,25,49,62,64

    //11,12,13,24,41,42
    TRANS_ONLINE_CommonInfo_pack();

    // Set bit 14
    if(memcmp(pNormalTrans->ucExpiredDate, "0000", 4))
        ISO8583_SetBitValue(14, pNormalTrans->ucExpiredDate, TRANS_DATE_LEN);

    // Set bit 22
    TRANS_ONLINE_SetEntryMode(pNormalTrans->inputMode, pNormalTrans->isInputPin);

    // Set bit 25
    ISO8583_SetBitValue(25, (unsigned char *)"00", 2);

    // Set bit 49
    memset(aucBuf, 0, sizeof(aucBuf));
    short_asc(aucBuf, PARAM_CURRENCY_CODE_LEN, &g_constantParam.usCurrencyId);
    ISO8583_SetBitValue(49, aucBuf, PARAM_CURRENCY_CODE_LEN);

    // Set bit 62
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulInvoiceNum);
    ISO8583_SetBitValue(62, aucBuf, 6);

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag)
        ISO8583_SetBitValue(64, (unsigned char*)"0000000000000000", 16);
}

unsigned char TRANS_ONLINE_CheckResponseValid(TransMode m_transMode)
{
    unsigned char ucResult;

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
    case TransMode_AdvanceVoid:      //撤销
    case TransMode_DepositVoid:      //撤销
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

    // Set Msg ID and Set bit 03 Processing Code
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(TransMode_DownWK, aucBuf);
    ISO8583_SetBitValue(3, aucBuf, 6);

    //11,12,13,24,41,42
    TRANS_ONLINE_CommonInfo_pack();
    ISO8583_RemoveBit(42);

    return ucResult;
}

unsigned char TRANS_ONLINE_DownWK_unpack(void)
{
    unsigned char ucResult;
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
//                xDATA::WriteValidFile(xDATA::DataSaveChange);
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

    //03,11,12,13,24,41
    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(TransMode_DownEWK, aucBuf);
    ISO8583_SetBitValue(3, aucBuf, 6);

    //11,12,13,24,41,42
    TRANS_ONLINE_CommonInfo_pack();
    ISO8583_RemoveBit(42);

    return ucResult;
}

unsigned char TRANS_ONLINE_DownEWK_unpack(void)
{
    unsigned char ucResult;
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

//存款/撤销
unsigned char TRANS_ONLINE_Deposit_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];

    if(pNormalTrans->transType != TransMode_CashDeposit
    && pNormalTrans->transType != TransMode_DepositVoid)
        return ERR_UNKNOWTRANSTYPE;

    //03,04,11,14,22,24,25,35,41,42,49,62,64(02,35,52)
    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(pNormalTrans->transType, aucBuf);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    //(11,12,13,24,41,42)14,22,25,49,62,64
    TRANS_ONLINE_CommonTransInfo_pack(pNormalTrans);

    // Set bit 02
    if(INPUTMODE_Manual == pNormalTrans->inputMode
    || TransMode_DepositVoid == pNormalTrans->transType)
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 35
    if(TransMode_CashDeposit == pNormalTrans->transType
    && INPUTMODE_MagneticStripe2 == pNormalTrans->inputMode)
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);

    if(TransMode_DepositVoid == pNormalTrans->transType)
    {
        // Set bit 37
        ISO8583_SetBitValue(37, pNormalTrans->aucRefNum, TRANS_REFNUM_LEN);
        // Set bit 38
        ISO8583_SetBitValue(38, pNormalTrans->aucAuthCode, TRANS_AUTHCODE_LEN);
        // Set bit 49
        ISO8583_RemoveBit(49);
    }

    // Set bit 52
    if(TransMode_CashDeposit == pNormalTrans->transType
    && pNormalTrans->isInputPin)
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);

    return ucResult;
}

unsigned char TRANS_ONLINE_Deposit_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
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

//取款/撤销
unsigned char TRANS_ONLINE_Advance_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[20];

    if(pNormalTrans->transType != TransMode_CashAdvance
    && pNormalTrans->transType != TransMode_AdvanceVoid)
        return ERR_UNKNOWTRANSTYPE;

    //正常取款 (11,12,13,24,25,41,42)02,03,04,14,22,35,49,52,62,64
    //撤销取款 (11,12,13,24,25,41,42)02,03,04,14,22,37,38,62,64

    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(pNormalTrans->transType, aucBuf);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    //(11,12,13,24,41,42)14,22,25,49,62,64
    TRANS_ONLINE_CommonTransInfo_pack(pNormalTrans);

    // Set bit 02
    if(INPUTMODE_Manual == pNormalTrans->inputMode
    || TransMode_AdvanceVoid == pNormalTrans->transType)
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 35
    if(TransMode_CashAdvance == pNormalTrans->transType
    && INPUTMODE_MagneticStripe2 == pNormalTrans->inputMode)
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);

    if(TransMode_AdvanceVoid == pNormalTrans->transType)
    {
        // Set bit 37
        ISO8583_SetBitValue(37, pNormalTrans->aucRefNum, TRANS_REFNUM_LEN);
        // Set bit 38
        ISO8583_SetBitValue(38, pNormalTrans->aucAuthCode, TRANS_AUTHCODE_LEN);
        // Set bit 49
        ISO8583_RemoveBit(49);
    }

    // Set bit 52
    if(TransMode_CashAdvance == pNormalTrans->transType
    && pNormalTrans->isInputPin)
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);

    return ucResult;
}

unsigned char TRANS_ONLINE_Advance_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
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

    if(pNormalTrans->transType != TransMode_BalanceInquiry)
        return ERR_UNKNOWTRANSTYPE;

    //03,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(pNormalTrans->transType, aucBuf);
    aucBuf[2] = pNormalTrans->accType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    //(11,12,13,24,41,42)14,22,25,49,62,64
    TRANS_ONLINE_CommonTransInfo_pack(pNormalTrans);

    // Set bit 02
    if(INPUTMODE_Manual == pNormalTrans->inputMode)
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 35
    if(INPUTMODE_MagneticStripe2 == pNormalTrans->inputMode)
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);

    // Set bit 52
    if(pNormalTrans->isInputPin)
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);

    return ucResult;
}

unsigned char TRANS_ONLINE_BalanceInquiry_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
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

unsigned char TRANS_ONLINE_PINChange_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[50];
    unsigned int uiOffSet = 0;

    //03,11,14,24,25,35,41,42,49,62,64(02,22,35,52)
    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(pNormalTrans->transType, aucBuf);
    ISO8583_SetBitValue(3, aucBuf, 6);

    //(11,12,13,24,41,42)14,22,25,49,62,64
    TRANS_ONLINE_CommonTransInfo_pack(pNormalTrans);

    // Set bit 02
    if(INPUTMODE_Manual == pNormalTrans->inputMode)
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 35
    if(INPUTMODE_MagneticStripe2 == pNormalTrans->inputMode)
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);

    // Set bit 52
    if(pNormalTrans->isInputPin)
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);

    // Set bit 57
    memset(aucBuf, 0, sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_Tool_TagPack(70, pExtraTrans->aucChangePINData, TRANS_PINDATALEN, &aucBuf[uiOffSet]);
    ISO8583_SetBitValue(57, aucBuf, uiOffSet);

    return ucResult;
}

unsigned char TRANS_ONLINE_PINChange_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
    unsigned char aucBuf[20];
    short sBufLen = 0;

    ucResult = TRANS_ONLINE_CheckResponseValid(pNormalTrans->transType);
    if(!ucResult)
    {
        //get bit12 trans time and get bit13 trans data
//        TRANS_ONLINE_GetTransDataTime(pNormalTrans->aucDate, pNormalTrans->aucTime);
        //get bit37
        memset(aucBuf, 0, sizeof(aucBuf));
        if(!ISO8583_GetBitValue(37, aucBuf, &sBufLen, sizeof(aucBuf)))
            memcpy(pNormalTrans->aucRefNum, aucBuf, TRANS_REFNUM_LEN);
        //get bit38
//        memset(aucBuf, 0, sizeof(aucBuf));
//        if(!ISO8583_GetBitValue(38, aucBuf, &sBufLen, sizeof(aucBuf)))
//            memcpy(pNormalTrans->aucAuthCode, aucBuf, TRANS_AUTHCODE_LEN);
    }
    return ucResult;
}

//转账
unsigned char TRANS_ONLINE_Transfer_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucBuf[100];
    unsigned int uiOffSet = 0;

    if(pNormalTrans->transType != TransMode_CardTransfer)
        return ERR_UNKNOWTRANSTYPE;

    //03,04,11,14,24,25,35,41,42,49,62,64(02,22,35, 52)
    ISO8583_Clear();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(pNormalTrans->transType, aucBuf);
    aucBuf[2] = pNormalTrans->accType;
    aucBuf[4] = pNormalTrans->toAccType;
    ISO8583_SetBitValue(3, aucBuf, 6);

    //(11,12,13,24,41,42)14,22,25,49,62,64
    TRANS_ONLINE_CommonTransInfo_pack(pNormalTrans);

    // Set bit 02
    if(INPUTMODE_Manual == pNormalTrans->inputMode)
        ISO8583_SetBitValue(2, pNormalTrans->aucSourceAcc, pNormalTrans->ucSourceAccLen);

    // Set bit 04
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 12, &pNormalTrans->ulAmount);
    ISO8583_SetBitValue(4, aucBuf, TRANS_AMOUNT_LEN);

    // Set bit 35
    if(INPUTMODE_MagneticStripe2 == pNormalTrans->inputMode)
        ISO8583_SetBitValue(35, pExtraTrans->magData.aucISO2, pExtraTrans->magData.uiISO2Len);

    // Set bit 52
    if(pNormalTrans->isInputPin)
        ISO8583_SetBitHexValue(52, pExtraTrans->aucPINData, TRANS_PINDATALEN);

    // Set bit 57
    memset(aucBuf, 0, sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_Tool_TagPack(61, (unsigned char *)"1", 1, &aucBuf[uiOffSet]);
    uiOffSet += TRANS_Tool_TagPack(62, pNormalTrans->aucToAcc, pNormalTrans->ucToAccLen, &aucBuf[uiOffSet]);
    ISO8583_SetBitValue(57, aucBuf, uiOffSet);

    return ucResult;
}

unsigned char TRANS_ONLINE_Transfer_unpack(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
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
    unsigned long ulAmount = 0;
    unsigned int uiNb = 0;
    unsigned int uiOffSet = 0;

    ISO8583_Clear();

    //11,12,13,24,41,42
    TRANS_ONLINE_CommonInfo_pack();

    // Set Msg ID and Set bit 03 Processing Code
    //1-2位事务处理码, 3-4账户类型, 5转移账户类型, 6初始化交易值为4
    memset(aucBuf, 0, sizeof(aucBuf));
    TRANS_ONLINE_SetProCode(TransMode_Settle, aucBuf);
    ISO8583_SetBitValue(3, aucBuf, 6);

    // Set bit 60
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulBatchNumber);
    ISO8583_SetBitValue(60, aucBuf, 6);

    // Set bit 63
    memset(aucBuf, '0', sizeof(aucBuf));
    uiOffSet = 0;
    //内卡
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);

    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);

    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);

    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], 0, 0);
    ISO8583_SetBitValue(63, aucBuf, uiOffSet);

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (const unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_Settlement_unpack(void)
{
    unsigned char ucResult;

    ucResult = TRANS_ONLINE_GetResponseCode();

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
    TRANS_ONLINE_SetEntryMode(pNormalTrans->inputMode, pNormalTrans->isInputPin);

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

    //11,12,13,24,41,42
    TRANS_ONLINE_CommonInfo_pack();

    // Set bit 03 Processing Code
    ISO8583_SetBitValue(3, (unsigned char *)"960000", 6);

    // Set bit 60
    memset(aucBuf, 0, sizeof(aucBuf));
    long_asc(aucBuf, 6, &g_changeParam.ulBatchNumber);
    ISO8583_SetBitValue(60, aucBuf, 6);

    // Set bit 63
    memset(aucBuf, '0', sizeof(aucBuf));
    uiOffSet = 0;
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], pTransTotal->uiCDebitNb, pTransTotal->ulCDebitAmount);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], pTransTotal->uiCCreditNb, pTransTotal->ulCCreditAmount);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], pTransTotal->uiDDebitNb, pTransTotal->ulDDebitAmount);
    uiOffSet += TRANS_Tool_TotalPack(&aucBuf[uiOffSet], pTransTotal->uiDCreditNb, pTransTotal->ulDCreditAmount);
    ISO8583_SetBitValue(63, aucBuf, uiOffSet);

    // Set bit 64 MAC
    if(g_changeParam.boolMacKeyFlag == true)
        ISO8583_SetBitValue(64, (const unsigned char*)"0000000000000000", 16);

    return ucResult;
}

unsigned char TRANS_ONLINE_BatchUploadEnd_unpack(void)
{
    unsigned char ucResult;

    ucResult = TRANS_ONLINE_GetResponseCode();

    return ucResult;
}
