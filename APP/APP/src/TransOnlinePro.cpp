#include "TransOnlinePro.h"
#include "transonline.h"
#include "global.h"
#include "msgreport.h"
#include "xdata.h"
#include "sav.h"

extern "C"{
    void Uart_Printf(char *fmt,...);
}
void getTransNum(void)
{
    //更新终端批次
    if(g_changeParam.ulBatchNumber < 1)
        g_changeParam.ulBatchNumber = 1;

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
    case TransMode_Settle:              //结算 Settlement
        ucResult = SettlementPro();
        break;
    case TransMode_CashDeposit:         //存款 Deposit
    case TransMode_DepositVoid:         //存款撤销 Deposit Void
    case TransMode_DepositAdjust:
        ucResult = DepositPro();
        break;
    case TransMode_CashAdvance:         //取款 Advance
    case TransMode_AdvanceVoid:         //取款撤销 Advance Void
    case TransMode_AdvanceAdjust:
        ucResult = AdvancePro();
        break;
    case TransMode_BalanceInquiry:      //查余 Balance Inquiry
        ucResult = BalanceInquiryPro();
        break;
    case TransMode_PINChange:
        ucResult = PINChangePro();      //改密 PIN Change
        break;
    case TransMode_CardTransfer:        //转账 P2P Transfer
        ucResult = TransferPro();
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

    if(NormalTransData.transType != TransMode_CashDeposit
    && NormalTransData.transType != TransMode_DepositVoid
    && NormalTransData.transType != TransMode_DepositAdjust)
        return ERR_UNKNOWTRANSTYPE;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_Deposit_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
    {
        if(NormalTransData.transType != TransMode_DepositVoid)
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

    if(NormalTransData.transType != TransMode_CashAdvance
    && NormalTransData.transType != TransMode_AdvanceVoid
    && NormalTransData.transType != TransMode_AdvanceAdjust)
        return ERR_UNKNOWTRANSTYPE;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_Advance_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
    {
        if(NormalTransData.transType != TransMode_AdvanceVoid)
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

//改密 PIN Change
unsigned char TransOnlinePro::PINChangePro(void)
{
    unsigned char ucResult;

    ucResult = ReversalPro();
    if(!ucResult)
    {
        NormalTransData.ulTraceNumber = g_changeParam.ulTransNum;
        ucResult = TRANS_ONLINE_PINChange_pack(&NormalTransData, &ExtraTransData);
    }
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_PINChange_unpack(&NormalTransData);
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
    unsigned char ucResult = SUCCESS_TRACKDATA;

    emit sigStatus("Initial Settlement");

    if((ucResult = ReversalPro()))
        return ucResult;

    xDATA::ReadValidFile(xDATA::DataSaveTransInfo);
    if(!g_transInfo.auiTransIndex[0])
        return ERR_TRANSEMPTY;

    if(!ucResult)
        ucResult = TRANS_ONLINE_Settlement_pack(TransMode_Settle, &g_transInfo.TransTotal);
    if(!ucResult)
        ucResult = GenSendReceive();
    if(!ucResult)
        ucResult = TRANS_ONLINE_Settlement_unpack();

    if(ucResult == ERR_SETTLE_UNBALANCE)
    {
        emit sigStatus("Settlement Unbalance");

        for(unsigned int uiIndex = 0; uiIndex < g_constantParam.uiMaxTotalNb; uiIndex++)
        {
            if(!g_transInfo.auiTransIndex[uiIndex])
                break;

            QString str;
            str.append("Batch Uploading :: ");
            str.append(QString::number(uiIndex));
            emit sigStatus(str);

            memset(&g_saveTrans, 0, sizeof(NORMAL_TRANS));
            ucResult = xDATA::ReadSubsectionFile(xDATA::DataSaveSaveTrans, uiIndex);
            if(!ucResult)
                ucResult = TRANS_ONLINE_BatchUpload_pack(&g_saveTrans);
            if(!ucResult)
                ucResult = GenSendReceive();
            if(!ucResult)
                ucResult = TRANS_ONLINE_BatchUpload_unpack();
        }
        if(!ucResult)
        {
            emit sigStatus("Finishing Settlement");

            ucResult = TRANS_ONLINE_Settlement_pack(TransMode_Settle2, &g_transInfo.TransTotal);
            if(!ucResult)
                ucResult = GenSendReceive();
            if(!ucResult)
                ucResult = TRANS_ONLINE_Settlement_unpack();
        }
    }
#if 0
    //参数更新
    if(!ucResult)
    {
        //清数据
        SAV_CleanCurrentBatch();
        //更新批次
        if(g_changeParam.ulBatchNumber >= 1 && g_changeParam.ulBatchNumber < 999999)
            ++g_changeParam.ulBatchNumber;
        else
            g_changeParam.ulBatchNumber = 1;

        xDATA::WriteValidFile(xDATA::DataSaveChange);
    }
#endif
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
