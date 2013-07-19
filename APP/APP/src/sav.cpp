#include "sav.h"
#include "msg.h"
#include "xdata.h"
#include "global.h"

extern "C"{
    void Uart_Printf(char *fmt,...);
}

static unsigned int sg_uiTransIndex;
static unsigned int sg_uiOldTransIndex;

enum SAV_TRANS_TYPE
{
    SAV_TRANS_NORMAL        = 1,
    SAV_TRANS_NIIVOID       = 2
}SavTransType;

unsigned char SAV_CheckNormalTransIndex(void)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;

    //find storage space
    if(g_transInfo.ulTransNumber >= g_constantParam.uiMaxTotalNb)
        ucResult = ERR_TRANSFILEFULL;

    if(!ucResult)
    {
        //transaction index
        sg_uiTransIndex = g_transInfo.ulTransNumber;
        ucResult = xDATA::WriteSubsectionFile(xDATA::DataSaveSaveTrans, sg_uiTransIndex);
    }
    return ucResult;
}

unsigned char SAV_TransSave(void)
{
    unsigned char ucResult;
    TRANSTOTAL *pTransTotal;

    if(!g_transInfo.auiTransIndex[sg_uiTransIndex])
        ++g_transInfo.ulTransNumber;

    //置交易保存类型
    switch(NormalTransData.transType)
    {
    case TransMode_CashVoid:
        g_transInfo.auiTransIndex[sg_uiTransIndex] = SAV_TRANS_NORMAL;
        g_transInfo.auiTransIndex[sg_uiOldTransIndex] = SAV_TRANS_NIIVOID;
        break;
    default:
        g_transInfo.auiTransIndex[sg_uiTransIndex] = SAV_TRANS_NORMAL;
        break;
    }

    //保存交易
    memset(&g_saveTrans, 0, sizeof(g_saveTrans));
    memcpy(&g_saveTrans, &NormalTransData, sizeof(NormalTrans));
    ucResult = xDATA::WriteSubsectionFile(xDATA::DataSaveSaveTrans, sg_uiTransIndex);

    //统计信息
    if(!ucResult)
    {
        pTransTotal = &g_transInfo.TransTotal;

        ++pTransTotal->uiTotalNb;       //总笔数累加

        switch(NormalTransData.transType)
        {
        case TransMode_CashDeposit:     //存款
            ++pTransTotal->uiDepositNb;
            pTransTotal->ulDepositAmount += NormalTransData.ulAmount;
            break;
        case TransMode_CashAdvance:     //取款
            ++pTransTotal->uiAdvanceNb;
            pTransTotal->ulAdvanceAmount += NormalTransData.ulAmount;
            break;
        case TransMode_CardTransfer:    //转账
            ++pTransTotal->uiTransferNb;
            pTransTotal->ulTransferAmount += NormalTransData.ulAmount;
            break;
        case TransMode_CashVoid:        //撤销
            ++pTransTotal->uiVoidNb;
            pTransTotal->ulVoidAmount += NormalTransData.ulAmount;
            break;
        default:
            break;
        }

        g_transInfo.ulLastTransNumber = sg_uiTransIndex;
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
