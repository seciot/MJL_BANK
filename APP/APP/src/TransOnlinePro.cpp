#include "TransOnlinePro.h"
#include "transonline.h"
#include "msg.h"
#include "global.h"
#include "xdata.h"
#include "sav.h"

extern "C"{
    void Uart_Printf(char *fmt,...);
}
void getTransNum(void)
{
    //更新终端批次
    if(g_changeParam.ulBatchNumber < 1)
        g_changeParam.ulTransNum = 1;

    //更新终端流水
    if(g_changeParam.ulTransNum >= 1 && g_changeParam.ulTransNum < 999999)
        ++g_changeParam.ulTransNum;
    else
        g_changeParam.ulTransNum = 1;

    xDATA::WriteValidFile(xDATA::DataSaveChange);
}

TransOnlinePro::TransOnlinePro(int onlineType, bool RetryFlag, QObject *parent):
    CommsSocket(parent), iOnlineType(onlineType), offRetry(RetryFlag)
{
    qDebug() << Q_FUNC_INFO;
}

TransOnlinePro::~TransOnlinePro()
{
   qDebug() << Q_FUNC_INFO;  
}

void TransOnlinePro::run()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult;

    CleanError();
    emit EableNotify(false);
    PreComm();

    getTransNum();
    switch(iOnlineType)
    {
    case TransMode_DownWK:              //签到(工作密钥)
        ucResult = DownWK();
        break;
    case TransMode_DownEWK:             //签到(传输密钥)
        ucResult = DownEWK();
        break;
    case TransMode_CashDeposit:         //存钱
        ucResult = DepositPro();
        break;
    case TransMode_CashAdvance:         //取钱
        ucResult = AdvancePro();
        break;
    case TransMode_BalanceInquiry:      //查余
        ucResult = BalanceInquiryPro();
        break;
    case TransMode_CardTransfer:        //转账
        ucResult = TransferPro();
        break;
    case TransMode_Settle:              //结算
        ucResult = SettlementPro();
        break;
    default:
        ucResult = ERR_UNKNOWTRANSTYPE;
        break;
    }

    Os__gif_stop();
    if(continueFlag == true)
    {
        FinComm();
        emit sendResult(ucResult);
    }
}

unsigned char TransOnlinePro::DownWK(void)
{
    unsigned char ucResult;

    ucResult = TRANS_ONLINE_DownWK_pack();
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_DownWK_unpack();
    return ucResult;
}

unsigned char TransOnlinePro::DownEWK(void)
{
    unsigned char ucResult;

    ucResult = TRANS_ONLINE_DownEWK_pack();
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_DownEWK_unpack();
    return ucResult;
}

//存款 Deposit
unsigned char TransOnlinePro::DepositPro(void)
{
    unsigned char ucResult;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_Deposit_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
    {
        TRANS_ONLINE_ReversalPack();
        ucResult = GenSendReceive();
    }
    if(!ucResult)
        ucResult = TRANS_ONLINE_Deposit_unpack(&NormalTransData);
    if(!ucResult)
        ucResult = SAV_TransSave();
    return ucResult;
}

//取款 Advance
unsigned char TransOnlinePro::AdvancePro(void)
{
    unsigned char ucResult;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_Advance_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
    {
        TRANS_ONLINE_ReversalPack();
        ucResult = GenSendReceive();
    }
    if(!ucResult)
        ucResult = TRANS_ONLINE_Advance_unpack(&NormalTransData);
    if(!ucResult)
        ucResult = SAV_TransSave();
    return ucResult;
}

//余额查询 Balance Inquiry
unsigned char TransOnlinePro::BalanceInquiryPro(void)
{
    unsigned char ucResult;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_BalanceInquiry_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_BalanceInquiry_unpack(&NormalTransData);
    return ucResult;
}

//卡卡转帐 P2P Transfer
unsigned char TransOnlinePro::TransferPro(void)
{
    unsigned char ucResult;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_Transfer_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
    {
        TRANS_ONLINE_ReversalPack();
        ucResult = GenSendReceive();
    }
    if(!ucResult)
        ucResult = TRANS_ONLINE_Transfer_unpack(&NormalTransData);
    if(!ucResult)
        ucResult = SAV_TransSave();
    return ucResult;
}

//结算 Settlement
unsigned char TransOnlinePro::SettlementPro(void)
{
    unsigned char ucResult;
    unsigned int uiTransIndex = 0;

    ucResult = ReversalPro();
    if(ucResult)
        return ucResult;

    xDATA::ReadValidFile(xDATA::DataSaveTransInfo);

    //一次结算
    ucResult = TRANS_ONLINE_Settlement_pack(&g_transInfo.TransTotal);
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_Settlement_unpack();

    //**************** 结算不平(二次结算) ****************
    //批上送
    if(ucResult == ERR_SETTLE_UNBALANCE)
    {
        if((ucResult = xDATA::ReadValidFile(xDATA::DataSaveTransInfo)))
            return ucResult;

        for(uiTransIndex = 0; uiTransIndex < g_constantParam.uiMaxTotalNb; uiTransIndex++)
        {
            if(ucResult)
                break;

            if(!g_transInfo.auiTransIndex[uiTransIndex])
                break;

            memset(&g_saveTrans, 0, sizeof(NORMAL_TRANS));
            ucResult = xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, uiTransIndex);
            if(!ucResult)
                ucResult = TRANS_ONLINE_BatchUpload_pack(&g_saveTrans);
            if(!ucResult)
                ucResult = GenSendReceive();
            if(!ucResult)
                ucResult = TRANS_ONLINE_BatchUpload_unpack();
        }

        //批上送结束
        if(!ucResult)
            ucResult = TRANS_ONLINE_BatchUploadEnd_pack(&g_transInfo.TransTotal);
        if(!ucResult)
            ucResult = GenSendReceive();
        if(!ucResult)
            ucResult = TRANS_ONLINE_BatchUploadEnd_unpack();
    }

    //参数更新
    if(!ucResult)
    {
        //更新批次
        if(g_changeParam.ulBatchNumber >= 1 && g_changeParam.ulBatchNumber < 999999)
            ++g_changeParam.ulBatchNumber;
        else
            g_changeParam.ulBatchNumber = 1;

        xDATA::WriteValidFile(xDATA::DataSaveChange);
    }
    return ucResult;
}

unsigned char TransOnlinePro::ReversalPro(void)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = SUCCESS_TRACKDATA;

    xDATA::ReadValidFile(xDATA::DataSaveReversal);
    if(g_reversedTrans.ucValid)
    {
        Uart_Printf((char *)"\n=========== Reversal Begin ===========\n");
        memcpy(&ISO8583Data, &g_reversedTrans.BackupISO8583Data, sizeof(BACKUPISO8583));
        ucResult = GenSendReceive();
        //更新参数
        if(!ucResult)
        {
            g_reversedTrans.ucValid = 0;
            ucResult = xDATA::WriteValidFile(xDATA::DataSaveReversal);
        }
        Uart_Printf((char *)"\nReversal result = %02x", ucResult);
        Uart_Printf((char *)"\n=========== Reversal End ===========\n");
    }
    return ucResult;
}
