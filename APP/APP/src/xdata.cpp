#include "osmain.h"
#include <QDebug>
#include "xdata.h"
#include "MsgReport.h"
#include "paramData.h"
#include "transData.h"
#include "iso8583.h"
#include "global.h"

xDATA::xDATA(){}
xDATA::~xDATA(){}

const FILEINFO xDATA::FileInfo[DataSaveMaxIndex + 1] =
{
    {"constant"},       //存储固定参数
    {"change"},         //存储动态参数
    {"cashier"},        //存储柜员信息
    {"transinfo"},      //存储统计信息
    {"toldinfo"},       //存储备统计信息
    {"rev8583"},        //存储冲正数据
    {"binstatus"},      //bin状态
    {"savetrans"},      //存储交易信息
    {"bintable"}        //存储卡BIN,issuer
//    {""}
};

void xDATA::CheckFileExist()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char i = 0;

    for(i = 0; i < DataSaveSaveTrans; i++)
        CheckValidFile(i);
    for(i = DataSaveSaveTrans; i < DataSaveMaxIndex; i++)
        CheckSubsectionFile(i);

    ClearGlobalData();
}

void xDATA::ClearGlobalData()
{
    qDebug() << Q_FUNC_INFO;
    CleanError();
    memset((unsigned char *)&NormalTransData,0,sizeof(NormalTransData));
    memset((unsigned char *)&ExtraTransData,0,sizeof(ExtraTransData));
    memset((unsigned char *)&RunData,0,sizeof(RunData));
    memset((unsigned char *)&ISO8583Data,0,sizeof(ISO8583Data));
    ISO8583_Clear();

    for(unsigned char i = 0;i<DataSaveSaveTrans;i++)
    {
        ReadValidFile(i);
    }
}

void *xDATA::GetFile(const unsigned char ucFileIndex, long *puiDataSize)
{
    void *pData = NULL;
    *puiDataSize = 0;

    switch(ucFileIndex)
    {
    case DataSaveConstant:
        *puiDataSize = sizeof(CONSTANT_PARAM);
        return &g_constantParam;
    case DataSaveChange:
        *puiDataSize = sizeof(CHANGE_PARAM);
        return &g_changeParam;
    case DataSaveCashier:
        *puiDataSize = sizeof(SYSCASHIER);
        return &g_cashier;
    case DataSaveTransInfo:
        *puiDataSize = sizeof(TRANSINFO);
        return &g_transInfo;
    case DataSaveReversal:
        *puiDataSize = sizeof(REVERSEDISO8583);
        return &g_reversedTrans;
    case DataSaveBINStatus:
        *puiDataSize = sizeof(BIN_STATUS);
        return &g_binStatus;
    default:
        break;
    }
    return pData;
}

void * xDATA::GetSubsectionFile(const unsigned char ucFileIndex, long *puiDataSize)
{
    void *pData = NULL;
    *puiDataSize = 0;

    switch(ucFileIndex)
    {
    case DataSaveSaveTrans:
        *puiDataSize = sizeof(NORMAL_TRANS);
        return &g_saveTrans;
    case DataSaveBINTable:
        *puiDataSize = sizeof(BIN_DATA);
        return &g_binData;
    default:
        break;
    }
    return pData;
}

unsigned char xDATA::CheckSubsectionFile(const unsigned char ucFileIndex)
{
    int iFileSize;
    int iFileHandel;
    if((iFileSize = OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName)) < 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_CREATE);
        if(iFileHandel >= 0)
        {
            OSAPP_FileClose(iFileHandel);
        }
    }
    return(OSAPP_FileGetLastError());
}

unsigned char xDATA::WriteSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex,int operate)
{
    operate = 0;
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    int iFileErrNo;
    long uiDataSize;
    void *pData = NULL;
    pData = GetSubsectionFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }
    iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_WRITE);
    iFileErrNo = OSAPP_FileSeek(iFileHandel, uiIndex * uiDataSize, SEEK_SET);

    if(!iFileErrNo)
    {
        iFileErrNo = OSAPP_FileWrite(iFileHandel, (unsigned char *)pData, uiDataSize);
    }
    OSAPP_FileClose(iFileHandel);
    ucResult= OSAPP_FileGetLastError();
    if (ucResult)
    {
        SetFileError((FileErrIndex)ucResult);
        return ERR_XFILE_WRITE;
    }
    qDebug("写[%d]号文件返回值:%02x\n",ucFileIndex,ucResult);
    return ucResult;
}

unsigned char xDATA::ReadSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex)
{
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    int iFileErrNo;
    long uiDataSize;
    void *pData = NULL;
    pData = GetSubsectionFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }

    if(OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName) > 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
        iFileErrNo = OSAPP_FileSeek(iFileHandel, uiIndex * uiDataSize, SEEK_SET);
        if(!iFileErrNo)
            iFileErrNo = OSAPP_FileRead(iFileHandel, (unsigned char *)pData, uiDataSize);
        OSAPP_FileClose(iFileHandel);
    }
    ucResult = OSAPP_FileGetLastError();
    if (ucResult)
    {
        SetFileError((FileErrIndex)ucResult);
        return ERR_XFILE_WRITE;
    }
    qDebug("读[%d]号文件返回值:%02x\n",ucFileIndex,ucResult);
    return ucResult;
}

unsigned char xDATA::ClearSubsectionFile(const unsigned char ucFileIndex)
{
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    long uiDataSize;
    void *pData = NULL;
    pData = GetSubsectionFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }
    if(OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName) > 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
        OSAPP_FileTrunc(iFileHandel, 0);
        OSAPP_FileClose(iFileHandel);
    }
    ucResult= OSAPP_FileGetLastError();
    if (ucResult)
    {
        SetFileError((FileErrIndex)ucResult);
        return ERR_XFILE_WRITE;
    }
    qDebug("清[%d]号文件返回值:%02x\n",ucFileIndex,ucResult);
    return ucResult;
}

unsigned char xDATA::CheckValidFile(const unsigned char ucFileIndex)
{
    long iFileSize;
    int iFileHandel;
    int iFileErrNo;
    long uiDataSize;
    void *pData = NULL;
    pData = GetFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }

    if((iFileSize = OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName)) < 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_CREATE);
        if(iFileHandel >= 0)
        {
            memset(pData, 0, uiDataSize);
            OSAPP_FileClose(iFileHandel);
        }
        SetDefaultValue(ucFileIndex);
    }
    else
    {
        if(iFileSize == uiDataSize)
        {
            iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
            iFileErrNo = OSAPP_FileSeek(iFileHandel, 0, SEEK_SET);
            if(!iFileErrNo)
            {
                iFileErrNo = OSAPP_FileRead(iFileHandel, (unsigned char*)pData, uiDataSize);
            }
            OSAPP_FileClose(iFileHandel);
        }
        else if(iFileSize)
        {
            iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
            OSAPP_FileTrunc(iFileHandel, 0);
            iFileSize = 0;
            OSAPP_FileClose(iFileHandel);
        }
        if(iFileSize == 0)
        {
            SetDefaultValue(ucFileIndex);
        }
    }
    return(OSAPP_FileGetLastError());
}

unsigned char xDATA::ReadValidFile(const unsigned char ucFileIndex)
{
    int iFileHandel;
    int iFileErrNo;
    long uiDataSize;
    void *pData = NULL;
    pData = GetFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }

    if(OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName) > 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_READ);
        iFileErrNo = OSAPP_FileSeek(iFileHandel,0,SEEK_SET);
        if(!iFileErrNo)
        {
            iFileErrNo = OSAPP_FileRead(iFileHandel, (unsigned char*)pData,uiDataSize);
        }
        OSAPP_FileClose(iFileHandel);
    }
    return(OSAPP_FileGetLastError());
}

unsigned char xDATA::ClearValidFile(const unsigned char ucFileIndex)
{
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    long uiDataSize;
    void *pData = NULL;
    pData = GetFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }
    if(OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName) > 0)
    {
        iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
        OSAPP_FileTrunc(iFileHandel, 0);
        OSAPP_FileClose(iFileHandel);
    }
    ucResult= OSAPP_FileGetLastError();
    if (ucResult)
    {
        SetFileError((FileErrIndex)ucResult);
        return ERR_XFILE_WRITE;
    }
    qDebug("清[%d]号文件返回值:%02x\n",ucFileIndex,ucResult);
    return ucResult;
}

unsigned char xDATA::WriteValidFile(const unsigned char ucFileIndex)
{
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    int iFileErrNo;
    long uiDataSize;
    void *pData = NULL;
    pData = GetFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        return 0;//Nothing to do.
    }
    iFileHandel = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_WRITE);
    iFileErrNo = OSAPP_FileSeek(iFileHandel, 0, SEEK_SET);
    if(!iFileErrNo)
    {
        //#if DATASAVE_TEST > 0
        //		qDebug("Writing File, ucFileIndex = %d\n", ucFileIndex);
        //#endif
        iFileErrNo = OSAPP_FileWrite(iFileHandel, (unsigned char*)pData, uiDataSize);
    }
    OSAPP_FileClose(iFileHandel);
    ucResult= OSAPP_FileGetLastError();
    if (ucResult)
    {
        SetFileError((FileErrIndex)ucResult);
        return ERR_XFILE_WRITE;
    }
    qDebug("写[%d]号文件返回值:%02x\n",ucFileIndex,ucResult);

    return ucResult;
}

void xDATA::LoadConstantDefaultData(void)
{
    memset(&g_constantParam, 0, sizeof(CONSTANT_PARAM));

    // NII
    memcpy(g_constantParam.aucNii, "0001", PARAM_NII_LEN);

    g_constantParam.commMode = PARAM_COMMMODE_GPRS;
    g_constantParam.hostType=HOST_IP;
    memcpy(g_constantParam.GSM.aucAPN, "CMNET", 5);
//    memcpy(g_constantParam.GSM.aucUserName, "", 40);
//    memcpy(g_constantParam.GSM.aucPassword, "", 20);

    // ip
    g_constantParam.ulHostIP[0] = 0x74D46CE5;
    g_constantParam.ulHostIP[1] = 0x1B938067;
    g_constantParam.uiHostPort[0] = 5999;
    g_constantParam.uiHostPort[1] = 5999;

    // domain name
    memcpy(g_constantParam.aucDomainName, "mjl1.xtf.im", PARAM_DOMAIN_LEN);

    g_constantParam.ucHostIndex = 0;

    //终端号
    memcpy(g_constantParam.aucTerminalID, "MBBR0101", PARAM_TERMINALID_LEN);

    // 商户号
    memcpy(g_constantParam.aucMerchantID, "000000000000000", PARAM_MERCHANTID_LEN);

    //货币代码
    g_constantParam.usCurrencyId = 50;
    memcpy(g_constantParam.aucCurrencySign, (unsigned char *)"BDT", 3);

    // 最大保存交易笔数
    g_constantParam.uiMaxTotalNb = 300;

    // 最大最小金额
    g_constantParam.ulMaxAmount = 999999999;
    g_constantParam.ulMinAmount = 0;

    g_constantParam.advance.TRANS_ENABLE=true;
    g_constantParam.advance.PIN_ENABLE=true;
    g_constantParam.advance.MANUAL_ENABLE=true;

    g_constantParam.deposit.TRANS_ENABLE=true;
    g_constantParam.deposit.PIN_ENABLE=true;
    g_constantParam.deposit.MANUAL_ENABLE=true;

    g_constantParam.balance.TRANS_ENABLE=true;
    g_constantParam.balance.PIN_ENABLE=true;
    g_constantParam.balance.MANUAL_ENABLE=true;

    g_constantParam.p2p.TRANS_ENABLE=true;
    g_constantParam.p2p.PIN_ENABLE=true;
    g_constantParam.p2p.MANUAL_ENABLE=true;

    g_constantParam.transvoid.TRANS_ENABLE=true;
    g_constantParam.transvoid.PIN_ENABLE=true;
    g_constantParam.transvoid.MANUAL_ENABLE=true;

    g_constantParam.adjust.TRANS_ENABLE=true;
    g_constantParam.adjust.PIN_ENABLE=true;
    g_constantParam.adjust.MANUAL_ENABLE=true;

    g_constantParam.pinchange.TRANS_ENABLE=true;
    g_constantParam.pinchange.PIN_ENABLE=true;
    g_constantParam.pinchange.MANUAL_ENABLE=true;

    // timeout
    g_constantParam.TIMEOUT_PAPERTEAR=5*1000;   // 打印撕纸等待时间
    g_constantParam.TIMEOUT_ERRMSG=10*1000;     // 报错窗口自动关闭时间
    g_constantParam.TIMEOUT_HOST=30*1000;       // 主机超时
    g_constantParam.TIMEOUT_UI=600*1000;         // 界面超时

    g_constantParam.connectionRetry=0;          // 连接重试次数

    // flag
    g_constantParam.flagSelectAccount=true;
    g_constantParam.flagSettleReceipt=true;

    // print
    memcpy(g_constantParam.aucReceiptLine1, "ITCL", PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine2, "PRODUCT QUALITY SERVICE", PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine3, "DHAKA", PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucReceiptLine4, " ", PARAM_PRINT_BIG_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine1, "I AGREE TO PAY ABOVE TOTAL AMOUNT", PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine2, "ACCORDING TO CARD ISSUER AGREEMENT", PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine3, " ", PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucAgreeLine4, "THANK YOU FOR BANKING", PARAM_PRINT_SMALL_FONT_LEN);
    memcpy(g_constantParam.aucCompanyCopyLabel, " --- BANK COPY --- ", PARAM_PRINT_BIG_FONT_LEN);
}

void xDATA::LoadChangeDefaultData(void)
{
    memset(&g_changeParam, 0, sizeof(CHANGE_PARAM));

    g_changeParam.simCard=true;
    g_changeParam.simTrans=false;

    // mac key flag
    g_changeParam.boolMacKeyFlag=false;
    //STAN
    g_changeParam.ulTransNum = 0;

    // invoice
    g_changeParam.ulInvoiceNum = 1;

    // batch number
    g_changeParam.ulBatchNumber = 1;

    g_changeParam.boolCashierLogonFlag = true;
}

void xDATA::LoadCashierDefaultData(void)
{
    unsigned char ucI = 1;
    unsigned char ucJ = 0;
    //保存5个默认柜员
    for(;ucI<=5;ucI++)
    {
        ucJ = ucI -1 ;
        memcpy(&g_cashier.aucCashierPass[ucJ] ,"0000" ,CASH_CASHIERPASSLEN);
        char_asc((unsigned char*)&g_cashier.aucCashierNo[ucJ], CASH_CASHIERNOLEN,&ucI);
    }
    //保存默认主管号和密码
    memcpy(g_cashier.aucSYSCashierNo,"00",CASH_SYSCASHIERNOLEN);
    memcpy(g_cashier.aucSYSCashierPass,"123456",CASH_SYSCASHIERPASSLEN);
    g_cashier.ucSYSCashierExitFlag = 1;
    //保存默认系统管理员号和密码
    memcpy(g_cashier.aucSuperNo, "99", CASH_MAXSUPERNO);
    memcpy(g_cashier.aucSuperPSW,"12345678",8);

    // unused
    memcpy(g_cashier.aucSafePSW,"123456",6);
    memcpy(g_cashier.aucSuperPassWord,"838055",6);
}

void xDATA::LoadCardBINDefaultData()
{
    memset(&g_binData, 0, sizeof(BIN_DATA));
    memcpy(g_binData.aucIssuerText,"Default Issuer",PARAM_ISSUER_TEXT_LEN);
    memcpy(g_binData.aucCardBINText,"Default Card BIN",PARAM_CARDBIN_TEXT_LEN);
    g_binData.uiIssuerID=0;
    g_binData.uiAcquirerID=0;
    g_binData.ulCardBINLow=000000;
    g_binData.ulCardBINHigh=999999;
    xDATA::WriteSubsectionFile(xDATA::DataSaveBINTable,0);

    memset(&g_binStatus, 0, sizeof(BIN_STATUS));
    g_binStatus.status[0]=true;
    g_binStatus.uiQuantity=1;
}

unsigned char xDATA::SetDefaultValue(const unsigned char ucFileIndex)
{
    qDebug() << Q_FUNC_INFO;
    switch(ucFileIndex)
    {
    case DataSaveConstant:
        LoadConstantDefaultData();
        break;
    case DataSaveChange:
        LoadChangeDefaultData();
        break;
    case DataSaveCashier:
        LoadCashierDefaultData();
        break;
    case DataSaveBINTable:
        LoadCardBINDefaultData();
        break;
    default:
        break;
    }
    return WriteValidFile(ucFileIndex);
}

unsigned char xDATA::CheckCashierNo(unsigned char *pucIndex,unsigned char *pucCashierNo)
{
    unsigned char ucI;
    bool bFirstFlag = true;

    for(ucI=0;ucI<CASH_MAXCASHIER;ucI++)
    {
        if( bFirstFlag == true)
        {
            if( !g_cashier.aucCashierNo[ucI][0])
            {
                bFirstFlag = false;
                *pucIndex = ucI;
            }
        }
        if( !memcmp(g_cashier.aucCashierNo[ucI],pucCashierNo,CASH_CASHIERNOLEN))
        {
            break;
        }
    }
    if( ucI == CASH_MAXCASHIER )
    {
        return(ERR_CASH_NOTEXIST);
    }else
    {
        *pucIndex = ucI;
        return(ERR_CASH_EXIST);
    }
}

int	xDATA::FILE_ReadRecordInternal(int iHandle,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen)
{
    UCHAR	ucUINTLen = sizeof(UINT);
    int	iResult = SALEERR_SUCCESS;
    if(iHandle>=0)
    {
        int iFileResult=OSAPP_FileSeek(iHandle,ucUINTLen+uiRecordLen*(uiRecordNum-1),SEEK_SET);
        if(!iFileResult)
        {
            iFileResult=OSAPP_FileRead(iHandle,(UCHAR*)pRecordBuff,uiRecordLen);
            if(iFileResult!= uiRecordLen)
                iResult = SALEERR_READFILE;
        }
        else
            iResult = SALEERR_SEEKFILE;
    }
    else iResult=SALEERR_OPENFILE;
    return iResult;
}

int	xDATA::FILE_ReadRecordExternal(const unsigned char ucFileIndex,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen)
{
    int	iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
    if(iHandle>=0)
        iResult=FILE_ReadRecordInternal(iHandle,uiRecordNum,pRecordBuff,uiRecordLen);
    else
        iResult = SALEERR_OPENFILE;
    OSAPP_FileClose(iHandle);
    return iResult;
}

int xDATA::FILE_ReadRecordNumInternal(int iHandle,unsigned int * puiRecordNum)
{
    unsigned int uiOutLen = 0;
    int	iResult = SALEERR_SUCCESS;
    if(iHandle>=0)
    {
        int	iFileResult=OSAPP_FileSeek(iHandle,0,SEEK_SET);
        if(!iFileResult)
        {
            uiOutLen=sizeof(UINT);
            iFileResult=OSAPP_FileRead(iHandle,(UCHAR*)puiRecordNum,uiOutLen);
            if(iFileResult != (int)uiOutLen)
                iResult = SALEERR_READFILE;
        }
        else
            iResult = SALEERR_SEEKFILE;
    }
    else
        iResult = SALEERR_OPENFILE;
    return iResult;
}

int xDATA::FILE_ReadRecordNumExternal(const unsigned char ucFileIndex,unsigned int *puiRecordNum)
{
    int	iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
    if(iHandle>=0)
        iResult = FILE_ReadRecordNumInternal(iHandle,puiRecordNum);
    else
        iResult = SALEERR_OPENFILE;
    OSAPP_FileClose(iHandle);
    return iResult;
}

unsigned int xDATA::FILE_SearchRecord(const unsigned char ucFileIndex,unsigned short uiRecordSize,unsigned short uiRecordItemAddrOffset,
                                      unsigned char *pucItemValue,unsigned short uiItemValueLen,unsigned char *pRecordBuff)
{
    UINT uiLow,uiHigh,uiMid = 0;
    UINT uiRecordNum ,uiI = 0;
    int	iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ);
    if(iHandle>=0)
    {
        iResult  =FILE_ReadRecordNumInternal(iHandle,&uiRecordNum);
        if(!iResult && uiRecordNum)
        {
            uiLow = 1;uiHigh = uiRecordNum;
            while(uiLow<=uiHigh)
            {
                uiMid=(uiLow+uiHigh)/2;
                int uiFileResult=FILE_ReadRecordInternal(iHandle,uiMid,pRecordBuff,uiRecordSize);
                if(uiFileResult==uiRecordSize)
                {
                    int iCmpResult = memcmp(pRecordBuff+uiRecordItemAddrOffset,pucItemValue,uiItemValueLen);
                    if(!iCmpResult)
                        break;
                    else if(iCmpResult>0)
                        uiHigh=uiMid-1;
                    else uiLow=uiMid+1;
                }
                else
                    iResult = SALEERR_READFILE;
            }
            if(uiLow>uiHigh||iResult) 	uiI=0;
            else uiI = uiMid;
        }
    }
    OSAPP_FileClose(iHandle);
    return uiI;
}

int	xDATA::FILE_WriteRecordNumInternal(int iHandle,unsigned int uiRecordNum)
{
    unsigned int uiOutLen = 0;
    int	iResult = SALEERR_SUCCESS;
    if(iHandle>=0)
    {
        int iFileResult=OSAPP_FileSeek(iHandle,0,SEEK_SET);
        if(!iFileResult)
        {
            uiOutLen = sizeof(UINT);
            iFileResult = OSAPP_FileWrite(iHandle,(UCHAR*)&uiRecordNum,uiOutLen);
            if(iFileResult!=(int)uiOutLen) iResult = SALEERR_WRITEFILE;
        }
        else iResult = SALEERR_SEEKFILE;
    }
    else iResult = SALEERR_OPENFILE;
    return iResult;
}

int	xDATA::FILE_WriteRecord(int iHandle,unsigned int uiRecordNum,unsigned char* pRecordBuff,unsigned short uiRecordLen)
{
    UCHAR ucUINTLen = sizeof(UINT);
    int	iResult = SALEERR_SUCCESS;
    if(iHandle>=0)
    {
        int iFileResult=OSAPP_FileSeek(iHandle,ucUINTLen+uiRecordLen*(uiRecordNum-1),SEEK_SET);
        if(!iFileResult)
        {
            iFileResult=OSAPP_FileWrite(iHandle,(UCHAR*)pRecordBuff,uiRecordLen);
            if(iFileResult!=uiRecordLen) iResult = SALEERR_WRITEFILE;
        }
        else iResult = SALEERR_SEEKFILE;
    }
    else iResult = SALEERR_OPENFILE;
    return iResult;
}

unsigned int xDATA::FILE_InsertRecord(const unsigned char ucFileIndex,unsigned char *pucRecord,unsigned short uiRecordSize)
{
    UINT uiRecordNum = 0;long lFileLong = 0;
    int	iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName, O_READ|O_WRITE);
    if(iHandle>=0)
    {
        lFileLong = OSAPP_FileSize((char *)FileInfo[ucFileIndex].aucFileName);
        if(lFileLong==0)
        {
            uiRecordNum=0;
            iResult = FILE_WriteRecordNumInternal(iHandle,uiRecordNum);
        }
        if(!iResult)
            iResult = FILE_ReadRecordNumInternal(iHandle,&uiRecordNum);
        if(!iResult)
        {
            uiRecordNum++;
            iResult = FILE_WriteRecord(iHandle,uiRecordNum,pucRecord,uiRecordSize);
        }
        if(!iResult)
            iResult = FILE_WriteRecordNumInternal(iHandle,uiRecordNum);
    }
    if(iResult) uiRecordNum=0;
    OSAPP_FileClose(iHandle);
    return uiRecordNum;
}

int xDATA::FILE_DeleteRecord(const unsigned char ucFileIndex,unsigned short uiRecordLen,unsigned int uiStartNum,unsigned int uiDeleteNums)
{
    UINT	uiRecordNums,uiI = 0;
    UINT ucUINTLen = sizeof(UINT);
    UCHAR * aucBuff = NULL;
    aucBuff = (unsigned char *)malloc(sizeof(unsigned char)*255);memset(aucBuff,0,255);
    int iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_READ|O_WRITE);
    if(iHandle<0) return SALEERR_OPENFILE;
    iResult = FILE_ReadRecordNumInternal(iHandle,&uiRecordNums);
    if(iResult) return iResult;
    if(uiStartNum>uiRecordNums) return SALEERR_DELETERECORD;
    if(uiStartNum+uiDeleteNums-1>uiRecordNums)
        uiDeleteNums = uiRecordNums - uiStartNum;
    iResult = SALEERR_SUCCESS;
    for(uiI=uiStartNum+uiDeleteNums;!iResult&&uiI<=uiRecordNums;uiI++)
    {
        iResult = FILE_ReadRecordInternal(iHandle,uiI,aucBuff,uiRecordLen);
        if(!iResult) iResult = FILE_WriteRecord(iHandle,uiI-uiDeleteNums,aucBuff,uiRecordLen);
    }
    uiRecordNums-=uiDeleteNums;
    if(!iResult) iResult = FILE_WriteRecordNumInternal(iHandle,uiRecordNums);
    if(!iResult) iResult = OSAPP_FileTrunc(iHandle,ucUINTLen+uiRecordNums*uiRecordLen);
    if(iHandle>=0) OSAPP_FileClose(iHandle);
    free(aucBuff);
    return iResult;
}

int xDATA::FILE_UpdateRecord(const unsigned char ucFileIndex,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen)
{
    int iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_READ|O_WRITE);
    if(iHandle>=0)
        iResult = FILE_WriteRecord(iHandle,uiRecordNum,pRecordBuff,uiRecordLen);
    else iResult = SALEERR_OPENFILE;
    OSAPP_FileClose(iHandle);
    return iResult;
}

void xDATA::UpdateCashierData(unsigned char ucNo,unsigned char *pucCashierNo,unsigned char *pucCashierPass)
{
    if( pucCashierNo )
    {
        Os__saved_copy(pucCashierNo,g_cashier.aucCashierNo[ucNo],CASH_CASHIERNOLEN);
    }else
    {
        memset(g_cashier.aucCashierNo[ucNo],0,CASH_CASHIERNOLEN);
    }
    if( pucCashierPass )
    {
        Os__saved_copy(pucCashierPass,g_cashier.aucCashierPass[ucNo],CASH_CASHIERPASSLEN);
    }else
    {
        memset(g_cashier.aucCashierPass[ucNo],0,CASH_CASHIERPASSLEN);
    }
    WriteValidFile(DataSaveCashier);
}
