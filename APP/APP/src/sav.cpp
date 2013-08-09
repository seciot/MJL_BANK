#include "sav.h"
#include "msg.h"
#include "xdata.h"
#include "global.h"

extern "C"{
    void Uart_Printf(char *fmt,...);
}

static unsigned int sg_uiTransIndex;
static unsigned int sg_uiOldTransIndex;

#define TransInfo           g_transInfo
#define OldTransInfo        g_oldtransInfo
#define TotalInfo           g_transInfo.TransTotal

static TotalType SAV_GetTransTotalType(TransMode transType)
{
    switch(transType)
    {
    case TransMode_CashDeposit:     //存款
        return TotalDeposit;
    case TransMode_CashAdvance:     //取款
        return TotalAdvance;
    case TransMode_CardTransfer:    //转账
        return TotalTransfer;
    case TransMode_AdvanceVoid:     //撤销
        return TotalVoid;
    default:
        return TotalMaxIndex;
    }
}

static TotalBusinessType SAV_GetBusinessTotalType(TotalType totalType)
{
    //TotalCreditBusiness   贷记业务(存款)
    //TotalDebitBusiness    借记业务(取款)

    switch(totalType)
    {
    case TotalDeposit:          //存款
    case TotalDepositVoid:
        return TotalCreditBusiness;
    case TotalAdvance:          //取款
    case TotalAdvanceVoid:
        return TotalDebitBusiness;
    default:
        return TotalBusinessMaxIndex;
    }
}

static void SAV_CalcTransTotal(TotalType totalType, unsigned long ulAmount)
{
    if(totalType >= TotalMaxIndex)
        return;

    ++TotalInfo.total[totalType].uiNb;
    TotalInfo.total[totalType].ulAmount += ulAmount;
}

static void SAV_CalcBusinessTotal(TotalType totalType, bool isDesc,
                                  unsigned char ucCardType, unsigned long ulAmount)
{
    TotalBusinessType businessType;

    if(totalType >= TotalMaxIndex)
        return;

    businessType = SAV_GetBusinessTotalType(totalType);
    if(businessType >= TotalBusinessMaxIndex)
        return;

    if(isDesc)
    {
        --TotalInfo.totalBusiness[ucCardType][businessType].uiNb;
        TotalInfo.totalBusiness[ucCardType][businessType].ulAmount -= ulAmount;
    }
    else
    {
        ++TotalInfo.totalBusiness[ucCardType][businessType].uiNb;
        TotalInfo.totalBusiness[ucCardType][businessType].ulAmount += ulAmount;
    }
}

static unsigned int SAV_FindSavedTransByTrace(unsigned long ulTrace)
{
    unsigned int uiIndex = 0;

    for(uiIndex = 0; uiIndex < TransInfo.ulTransNumber; uiIndex++)
    {
        //compare ticket number, if confirms get out trans
        xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, uiIndex);
        if(ulTrace == g_saveTrans.ulTraceNumber)
            break;
    }
    return uiIndex;
}

unsigned char SAV_CleanTransaction(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    //clean
    memset(TransInfo.auiTransIndex, 0, sizeof(TransInfo.auiTransIndex));
    TransInfo.ulTransNumber = 0x00;
    memset(&TransInfo.TransTotal, 0, sizeof(TransInfo.TransTotal));

    ucResult = xDATA::ClearSubsectionFile(xDATA::DataSaveSaveTrans);
    if(!ucResult)
        ucResult = xDATA::WriteValidFile(xDATA::DataSaveTransInfo);
    return ucResult;
}

unsigned char SAV_CleanCurrentBatch(void)
{
    //backup
    memcpy(&OldTransInfo, &TransInfo, sizeof(TRANSINFO));
    xDATA::WriteValidFile(xDATA::DataSaveOldTransInfo);

    //clean
    return SAV_CleanTransaction();
}

unsigned char SAV_CheckNormalTransIndex(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;

    //find storage space
    if(TransInfo.ulTransNumber >= g_constantParam.uiMaxTotalNb)
        ucResult = ERR_TRANSFILEFULL;

    if(!ucResult)
    {
        //transaction index
        sg_uiTransIndex = TransInfo.ulTransNumber;
        ucResult = xDATA::WriteSubsectionFile(xDATA::DataSaveSaveTrans, sg_uiTransIndex);
    }
    return ucResult;
}

unsigned char SAV_CheckVoidTransIndex(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
    unsigned int uiIndex = 0;

    //判断一下交易是否已满
    ucResult = SAV_CheckNormalTransIndex();
    if(ucResult)
        return ucResult;

    //按流水寻找交易
    uiIndex = SAV_FindSavedTransByTrace(pNormalTrans->ulOrgTraceNumber);
    if(uiIndex >= TransInfo.ulTransNumber)
        return ERR_NOTRANS;

    //判断一下该交易是否已撤销,或者已调整
    switch(TransInfo.auiTransIndex[uiIndex])
    {
    case SAV_TRANS_NORMAL:
        break;
    case SAV_TRANS_NIIVOID:
        return ERR_ALREADYVOID;
    default:
        return ERR_NOTPROC;
    }

    //判断一下该交易是否可撤销
    switch(g_saveTrans.transType)
    {
    case TransMode_CashDeposit:
        pNormalTrans->transType = TransMode_DepositVoid;
        break;
    case TransMode_CashAdvance:
        pNormalTrans->transType = TransMode_AdvanceVoid;
        break;
    default:
        return ERR_NOTPROC;
    }

    sg_uiOldTransIndex = uiIndex;
    //赋值
    pNormalTrans->inputMode     = g_saveTrans.inputMode;        //输入方式
    pNormalTrans->isInputPin    = g_saveTrans.isInputPin;
    pNormalTrans->cardType      = g_saveTrans.cardType;         //卡类型
    pNormalTrans->accType       = g_saveTrans.accType;          //账户类型
    pNormalTrans->ulAmount      = g_saveTrans.ulAmount;         //金额
    pNormalTrans->ucSourceAccLen = g_saveTrans.ucSourceAccLen;  //卡号长度
    //卡号
    memcpy(pNormalTrans->aucSourceAcc, g_saveTrans.aucSourceAcc, TRANS_ACCLEN);
    //卡有效期
    memcpy(pNormalTrans->ucExpiredDate, g_saveTrans.ucExpiredDate, TRANS_DATE_LEN);
    //授权号
    memcpy(pNormalTrans->aucAuthCode, g_saveTrans.aucAuthCode, TRANS_REFNUM_LEN);
    //参考号
    memcpy(pNormalTrans->aucRefNum, g_saveTrans.aucRefNum, TRANS_REFNUM_LEN);

    //原参考号
    memcpy(pNormalTrans->aucOrgRefNum, g_saveTrans.aucRefNum, TRANS_REFNUM_LEN);
    //原交易流水
    pNormalTrans->ulOrgTraceNumber = g_saveTrans.ulTraceNumber;

    return ucResult;
}

unsigned char SAV_CheckAdjustTransIndex(NormalTrans *pNormalTrans)
{
    unsigned char ucResult;
    unsigned int uiIndex = 0;

    //判断一下交易是否已满
    ucResult = SAV_CheckNormalTransIndex();
    if(ucResult)
        return ucResult;

    //按流水寻找交易
    uiIndex = SAV_FindSavedTransByTrace(pNormalTrans->ulOrgTraceNumber);
    if(uiIndex >= TransInfo.ulTransNumber)
        return ERR_NOTRANS;

    //判断一下该交易是否已撤销
    switch(TransInfo.auiTransIndex[uiIndex])
    {
    case SAV_TRANS_NORMAL:
        break;
    case SAV_TRANS_ADJUST:
        return ERR_ALREADYVOID;
    default:
        return ERR_NOTPROC;
    }

    //判断一下该交易是否可撤销
    switch(g_saveTrans.transType)
    {
    case TransMode_CashDeposit:
    case TransMode_DepositAdjust:
        pNormalTrans->transType = TransMode_DepositAdjust;
        break;
    case TransMode_CashAdvance:
    case TransMode_AdvanceAdjust:
        pNormalTrans->transType = TransMode_AdvanceAdjust;
        break;
    default:
        return ERR_NOTPROC;
    }

    sg_uiOldTransIndex = uiIndex;
    //赋值
    pNormalTrans->inputMode     = g_saveTrans.inputMode;        //输入方式
    pNormalTrans->isInputPin    = g_saveTrans.isInputPin;
    pNormalTrans->cardType      = g_saveTrans.cardType;         //卡类型
    pNormalTrans->accType       = g_saveTrans.accType;          //账户类型
    pNormalTrans->ulAmount      = g_saveTrans.ulAmount;         //金额
    pNormalTrans->ucSourceAccLen = g_saveTrans.ucSourceAccLen;  //卡号长度
    //卡号
    memcpy(pNormalTrans->aucSourceAcc, g_saveTrans.aucSourceAcc, TRANS_ACCLEN);
    //卡有效期
    memcpy(pNormalTrans->ucExpiredDate, g_saveTrans.ucExpiredDate, TRANS_DATE_LEN);
    //授权号
    memcpy(pNormalTrans->aucAuthCode, g_saveTrans.aucAuthCode, TRANS_REFNUM_LEN);
    //参考号
    memcpy(pNormalTrans->aucRefNum, g_saveTrans.aucRefNum, TRANS_REFNUM_LEN);
    //原参考号
    memcpy(pNormalTrans->aucOrgRefNum, g_saveTrans.aucRefNum, TRANS_REFNUM_LEN);
    //原交易流水
    pNormalTrans->ulOrgTraceNumber = g_saveTrans.ulTraceNumber;

    return ucResult;
}

unsigned char SAV_TransSave(void)
{
    unsigned char ucResult;
    TotalType totalType;

    if(!TransInfo.auiTransIndex[sg_uiTransIndex])
        ++TransInfo.ulTransNumber;

    //置交易保存类型
    switch(NormalTransData.transType)
    {
    case TransMode_AdvanceVoid:
    case TransMode_DepositVoid:
        TransInfo.auiTransIndex[sg_uiTransIndex] = SAV_TRANS_NORMAL;
        TransInfo.auiTransIndex[sg_uiOldTransIndex] = SAV_TRANS_NIIVOID;
        break;
    case TransMode_AdvanceAdjust:
    case TransMode_DepositAdjust:
        TransInfo.auiTransIndex[sg_uiTransIndex] = SAV_TRANS_NORMAL;
        TransInfo.auiTransIndex[sg_uiOldTransIndex] = SAV_TRANS_ADJUST;
        break;
    default:
        TransInfo.auiTransIndex[sg_uiTransIndex] = SAV_TRANS_NORMAL;
        break;
    }

    //保存交易
    memset(&g_saveTrans, 0, sizeof(g_saveTrans));
    memcpy(&g_saveTrans, &NormalTransData, sizeof(NormalTrans));
    ucResult = xDATA::WriteSubsectionFile(xDATA::DataSaveSaveTrans, sg_uiTransIndex);

    //统计信息
    if(!ucResult)
    {
        totalType = SAV_GetTransTotalType(NormalTransData.transType);

        ++TotalInfo.uiTotalNb;
        SAV_CalcTransTotal(totalType, NormalTransData.ulAmount);
        switch(totalType)
        {
        //撤销交易
        case TotalDepositVoid:
        case TotalAdvanceVoid:
            SAV_CalcBusinessTotal(totalType, true, 0, NormalTransData.ulAmount);
            break;
        //调整交易
        case TotalDepositAdjust:
        case TotalAdvanceAdjust:
            SAV_CalcBusinessTotal(totalType, true, 0, NormalTransData.ulAmount);
            SAV_CalcBusinessTotal(totalType, false, 0, NormalTransData.ulAdjustAmount);
            break;
        default:
            SAV_CalcBusinessTotal(totalType, false, 0, NormalTransData.ulAmount);
            break;
        }

        TransInfo.ulLastTransNumber = sg_uiTransIndex;
        ucResult = xDATA::WriteValidFile(xDATA::DataSaveTransInfo);
    }

    //清冲正标志
    if(!ucResult)
    {
        memset(&g_reversedTrans, 0, sizeof(REVERSEDISO8583));
        ucResult = xDATA::WriteValidFile(xDATA::DataSaveReversal);
    }
    //更新参数
    if(!ucResult)
    {
        if(g_changeParam.ulInvoiceNum >= 1 && g_changeParam.ulInvoiceNum < 999999)
            ++g_changeParam.ulInvoiceNum;
        else
            g_changeParam.ulInvoiceNum = 1;

        ucResult = xDATA::WriteValidFile(xDATA::DataSaveChange);
    }
    return ucResult;
}

unsigned char SAV_SettleSave(void)
{
    unsigned char ucResult;

    //清数据
    ucResult = SAV_CleanCurrentBatch();
    if(!ucResult)
    {
        //更新批次
        if(g_changeParam.ulBatchNumber >= 1 && g_changeParam.ulBatchNumber < 999999)
            ++g_changeParam.ulBatchNumber;
        else
            g_changeParam.ulBatchNumber = 1;

            ucResult = xDATA::WriteValidFile(xDATA::DataSaveChange);
    }
    return ucResult;
}
