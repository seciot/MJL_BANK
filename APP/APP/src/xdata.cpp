#include "paramData.h"
#include "transData.h"
#include "xdata.h"
#include "iso8583.h"
#include "global.h"

xDATA::xDATA(){}
xDATA::~xDATA(){}

const FILEINFO xDATA::FileInfo[DataSaveMaxIndex + 1] =
{
    {"constant"},//存储固定参数
    {"change"},//存储动态参数 Eg:流水号..
    {"cashier"},
    {"transinfo"},
    {"rev8583"},		//存放冲正数据
    {"back8583"},
    {"scitdt"},			//脚本信息
    {"maxtemp"},

    {"ConstPara"},
    {"EMVConfig"},
    {"savetrans"},
    {"colldata"},			//采集数据文件
    {"black"},			//存放黑名单数据

    {"CAPK"},
    {"TermApps"},
    {"TransLog"},
    {"Except"},
    {"BatchRec"},
    {"IPKRevoke"},
    {"AIDParam"},
    {"ErrICC"},

    {""}
};
void xDATA::CheckFileExist()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char i = 0;
    //#if INIFILE_ENABLE
    //	if(INIFILE_CheckParamFileExist())
    //		INIFILE_LoadParamFileToSystem();
    //	else
    //	{
    //#if MAN_TEST > 0
    //		INIFILE_LoadTMKEY();
    //#endif
    //		LoadIniFileDefaultData();
    //		INIFILE_UpdateParamFile();
    //	}
    //#endif
    for(i=0;i<DataSaveSaveTrans;i++)
        CheckValidFile(i);
    for(i=DataSaveSaveTrans;i<DataSaveMaxIndex;i++)
        CheckSubsectionFile(i);
    ClearGlobalData();
}
void *xDATA::GetCashierData(long *puiDataSize)
{
    *puiDataSize = sizeof(SYSCASHIER);
    return &g_cashier;
}
void *xDATA::GetConstantData(long *puiDataSize)
{
    *puiDataSize = sizeof(CONSTANT_PARAM);
    return &g_constantParam;
}
void * xDATA::GetChangeData(long *puiDataSize)
{
    *puiDataSize = sizeof(CHANGE_PARAM);
    return &g_changeParam;
}
void * xDATA::GetTransInfoData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    *puiDataSize = sizeof(TRANSINFO);
    return &g_transInfo;
}
void * xDATA::GetReversalData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(REVERSED_TRANS);
    //return &g_reversedTrans;
}
void * xDATA::Get8583Backup(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //   *puiDataSize = sizeof(TRANS_8583_DATA);
    //return &Trans_8583Data;
}
void * xDATA::GetScriptData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(Script_Data);
    //return &g_scriptData;
}
void * xDATA::GetTempData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(TEMPFILE_DATA);
    //return &g_tempFileData;
}
void * xDATA::GetSaveCollData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(NORMALTRANS);
    //return &xDATA_CollectTrans;
}
void * xDATA::GetTransactionData(long *puiDataSize)
{
    *puiDataSize = sizeof(NORMAL_TRANS);
    return &g_saveTrans;
}
void * xDATA::GetBlackListData(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(BLACK_DATA);
    //return  &BlackListData;
}
void * xDATA::GetConstParamFile(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(EMV_CONSTPARAM);
    //return &ConstParam;
}
void * xDATA::GetEMVConfigFile(long *puiDataSize)
{
    long data;
    data = *puiDataSize;
    return NULL;
    //*puiDataSize = sizeof(TERMCONFIG);
    //return &EMVConfig;
}
void * xDATA::GetSubsectionFile(const unsigned char ucFileIndex, long *puiDataSize)
{
    void *pData = NULL;
    *puiDataSize = 0;

    switch(ucFileIndex)
    {
    case DataSaveSaveTrans:
        pData = GetTransactionData(puiDataSize);
        break;
    case SaveCollectData:
        pData = GetSaveCollData(puiDataSize);
        break;
    case BlackFile:
        pData = GetBlackListData(puiDataSize);
        break;
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
        //#if SQLITE3_ENABLE > 0
        //		if(ucFileIndex == DataSaveSaveTrans)
        //		{
        //			unsigned char FileName[15] = {0};
        //			sprintf((char *)FileName,"%s.db",FileInfo[ucFileIndex].aucFileName);
        //			SQLITE3_TRANS_CREATE_DataBase((char *)FileName);
        //			/*return 0;*/
        //		}
        //#endif
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
    //#if SQLITE3_ENABLE > 0
    //		if(ucFileIndex == DataSaveSaveTrans)
    //		{
    //			memcpy(&NormalTransData, &g_saveTrans, sizeof(NORMALTRANS));
    //			switch(operate)
    //			{
    //			case DB_UPDATE:
    //				SQLITE3_TRANS_UPDATE_IndexRequest((int)uiIndex);
    //				break;
    //			case DB_INSERT:
    //				SQLITE3_TRANS_INSERT_IndexRequest((int)uiIndex);
    //				break;
    //			default:
    //				break;
    //			}
    //			return 0;
    //		}
    //#endif
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
    //#if DATASAVE_TEST > 0
    //    qDebug("WriteSubsection ucFileIndex = %u, uiIndex = %d, iDataSize = %ld\n", ucFileIndex, uiIndex, uiDataSize);
    //#endif
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
    //#if SQLITE3_ENABLE > 0
    //		if(ucFileIndex == DataSaveSaveTrans)
    //		{
    //			int iret = SQLITE3_TRANS_SELECT_IndexRequest((int)uiIndex);
    //			if(!iret) memcpy(&g_saveTrans, &NormalTransData, sizeof(NORMALTRANS));
    //			else	return ERR_SQLITE3;
    //			return 0;
    //		}
    //#endif
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
    //#if DATASAVE_TEST > 0
    //    qDebug("ReadSubsection ucFileIndex = %d, uiIndex = %d, iDataSize = %ld\n", ucFileIndex, uiIndex, uiDataSize);
    //#endif
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
    //#if SQLITE3_ENABLE > 0
    //		if(ucFileIndex == DataSaveSaveTrans)
    //		{
    //			SQLITE3_TRANS_DELETE_Request(TABLEDEL_ALL);
    //			return 0;
    //		}
    //#endif
    unsigned char ucResult = SUCCESS;
    int iFileHandel;
    long uiDataSize;
    void *pData = NULL;
    pData = GetSubsectionFile(ucFileIndex, &uiDataSize);
    if(pData == NULL)
    {
        //return 0;//Nothing to do.
    }
#if DATASAVE_TEST > 0
    qDebug("ClearSubsection ucFileIndex = %d\n", ucFileIndex);
#endif
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


void * xDATA::GetFile(const unsigned char ucFileIndex, long *puiDataSize)
{
    void *pData = NULL;
    *puiDataSize = 0;

    switch(ucFileIndex)
    {
    case DataSaveConstant:
        pData = GetConstantData(puiDataSize);
        break;
    case DataSaveChange:
        pData = GetChangeData(puiDataSize);
        break;
    case DataSaveCashier:
        pData = GetCashierData(puiDataSize);
        break;
    case DataSaveTransInfo:
        pData = GetTransInfoData(puiDataSize);
        break;
    case DataSaveReversal:
        pData = GetReversalData(puiDataSize);
        break;
    case DataSaveTrans8583:
        pData = Get8583Backup(puiDataSize);
        break;
    case DataSaveScript:
        pData = GetScriptData(puiDataSize);
        break;
    case DataSaveTempMax:
        pData = GetTempData(puiDataSize);
        break;
    case CONSTPARAMFILE:
        pData = GetConstParamFile(puiDataSize);
        break;
    case EMVCONFIGFILE:
        pData = GetEMVConfigFile(puiDataSize);
        break;
    default:
        break;
    }
    return pData;
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
    //getEmvFiles();
    //   G_NORMALTRANS_ulTraceNumber = g_changeParam.ulTraceNumber;
    //memset(G_RUNDATA_aucTerminalID,0,sizeof(G_RUNDATA_aucTerminalID));
    //   memcpy(G_RUNDATA_aucTerminalID,g_constIninfo.aucTerminalID,
    //	PARAM_TERMINALIDLEN);
    //   memcpy(G_RUNDATA_aucMerchantID,g_constIninfo.aucMerchantID,
    //	PARAM_MERCHANTIDLEN);
    //   memcpy(ConstParam.aucTerminalID,g_constIninfo.aucTerminalID,TERMINALIDLEN);
    //   memcpy(ConstParam.aucMerchantID,g_constIninfo.aucMerchantID,MERCHANTIDLEN);
    //   memcpy(ConstParam.aucMerchantName,g_constIninfo.aucMerchantName,MERCHANTNAMELEN);
    WriteValidFile(CONSTPARAMFILE);
    Read_LoadTransType();
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
        //#if DATASAVE_TEST > 0
        //        qDebug("Create fixed size ifilesize = %ld, iDataSize = %ld\n", iFileSize, uiDataSize);
        //#endif
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
            //#if DATASAVE_TEST > 0
            //            qDebug("Fixed size file ok. ifilesize = %ld, iDataSize = %ld\n", iFileSize, uiDataSize);
            //#endif
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
            //#if DATASAVE_TEST > 0
            //            qDebug("Fixed size file size to 0. ifilesize = %ld\n", iFileSize);
            //#endif
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

void xDATA::LoadIniFileDefaultData(void)
{
    //    g_constIninfo.ucEmptySettle='0';
    //    g_constIninfo.ucDisplay='0';
    //    g_constIninfo.ucRePAuthority='0';
    //    g_constIninfo.ucSettlePSW='0';
    //    g_constIninfo.ucCommMode = PARAM_COMMMODE_GPRS;
    //    g_constIninfo.prnflag = 1;
    //    g_constIninfo.linenum = 0x16;
    //    g_constIninfo.font = 0;
    //    g_constIninfo.printtimes = 2;
    //    g_constIninfo.ucHeatPrintFormat = 2;
    //    g_constIninfo.ucSetFlag='0';
    //    g_constIninfo.ucPrint =0x31;//是否打印扣率
    //    g_constIninfo.ucCardDetail = '0';
    //    g_constIninfo.ucUnSaleSwipe='1';
    //    g_constIninfo.ucUnAuthFinSwipe = '1';
    //    g_constIninfo.ucVoidInputPin = '1';
    //    g_constIninfo.ucPREAUTHFINISHInputPin = '1';
    //    g_constIninfo.ucDefaultTransParam=TRANS_PURCHASE;
    //    g_constIninfo.ENCRYPTTYPEParam=ENCRYPTTYPE_DBTDK;
    //    g_constIninfo.ucPreAnthKey='1';
    //    g_constIninfo.uiReceiveTimeout = 60;
    //    g_constIninfo.ucDialRetry = 3 ;
    //    g_constIninfo.ucSelfLogoff = '0';
    //    g_constIninfo.ucOFFLineSendType = '0';
    //    g_constIninfo.uiAntoSendSum=10;
    //    g_constIninfo.ucTransRetry=3;
    //    g_constIninfo.uICCInfo = '0';
    //    g_constIninfo.ucUnManagePwd='0';
    //#if MAN_TEST > 0
    //	g_constIninfo.ucEncryptDataFlag = '1';	//主IP是否加密
    //    g_constIninfo.ucBackIPEnc = '1';          //备份IP是否加密
    //#else
    //    g_constIninfo.ucEncryptDataFlag = '0';	//主IP是否加密
    //    g_constIninfo.ucBackIPEnc = '0';          //备份IP是否加密
    //#endif
    //    g_constIninfo.ucInputCardNo = 0x31; /*0x32-->0x30*/
    //    g_constIninfo.ucCheckPaper = '1';
    //    g_constIninfo.ucDownEMVFlag='1';
    //    g_constIninfo.ucXorTipFee = 0;
    //    g_constIninfo.ucInforFlag='0';
    //    g_constIninfo.ulRefundMaxAmount=1000000;
    //    g_constIninfo.ucDetails='0';
    //    g_constIninfo.ucEngList='1';
    //    g_constIninfo.ucGPRSGUST='0';
    //
    //    g_constIninfo.ucSuperLogonFlag= 0;/*状态设为正常交易*/
    //    g_constIninfo.uiMaxTotalNb = 300;
    //    //默认
    //    memcpy(g_constIninfo.aucGlobalTransEnable,"\xFF\xFB\xFF\x80",4);
    //    /*第一次装机系统默认每天定时结算时间为凌晨0点*/
    //    memcpy(g_constIninfo.aucSettleCycle,"\x30\x30\x30\x30",4);
    //    //电话号码
    //    Os__saved_copy((unsigned char *)"4008200358.",g_constIninfo.aucHostTelNumber1,10);
    //    Os__saved_copy((unsigned char *)"66694500.",g_constIninfo.aucHostTelNumber2,9);
    //    Os__saved_copy((unsigned char *)"66674500.",g_constIninfo.aucHostTelNumber3,9);
    //    Os__saved_copy((unsigned char *)"123456.",g_constIninfo.aucHostTelNumber4,7);
    //    Os__saved_copy((unsigned char *)"\x60\x04\x01\x00\x00",g_constIninfo.aucTpdu,5);
    //#if MAN_TEST > 0
    //	g_constIninfo.ucTerminalType = TYPE_SHOP;
    //	Os__saved_copy((unsigned char *)"37677001",g_constIninfo.aucTerminalID,PARAM_TERMINALIDLEN);
    //    Os__saved_copy((unsigned char *)"301310070118940",g_constIninfo.aucMerchantID,PARAM_MERCHANTIDLEN);
    //	memcpy(&g_constIninfo.ulHostIP, (unsigned char *)"\xDE\x42\x01\x86", 4);
    //	memcpy(&g_constIninfo.ulBackIP, (unsigned char *)"\xDE\x42\x01\x86", 4);
    //#endif
    //    //strcpy((char *)g_constIninfo.aucMerchantName,"杉德IPS420物流POS,欢迎使用杉德PAY卡");
    //#if MAN_TEST > 0
    //    g_constIninfo.uiHostPort=asc_long((unsigned char *)"006001",6);
    //    g_constIninfo.uiHostBackPort=asc_long((unsigned char *)"006001",6);
    //#else
    //	g_constIninfo.uiHostPort=asc_long((unsigned char *)"006000",6);
    //    g_constIninfo.uiHostBackPort=asc_long((unsigned char *)"006000",6);
    //#endif
    //    memcpy(g_constIninfo.aucAPN,(unsigned char*)"MPOS-CUPSH.SH",13);//MPOS-CUPSH.SH
}

void xDATA::LoadConstantDefaultData(void)
{
    qDebug() << Q_FUNC_INFO;

    memset(&g_constantParam, 0, sizeof(CONSTANT_PARAM));
    memcpy(g_constantParam.aucNii, "0001", PARAM_NII_LEN);

    g_constantParam.commMode = PARAM_COMMMODE_GPRS;
    g_constantParam.hostType=HOST_IP;
    memcpy(g_constantParam.GSM.aucAPN, "CMNET", 5);
//    memcpy(g_constantParam.GSM.aucUserName, "", 40);
//    memcpy(g_constantParam.GSM.aucPassword, "", 20);

    g_constantParam.ulHostIP[0] = 0x74D46CE5;
    g_constantParam.ulHostIP[1] = 0x1B938067;
    g_constantParam.uiHostPort[0] = 5999;
    g_constantParam.uiHostPort[1] = 5999;

    g_constantParam.ucHostIndex = 0;

    //终端号
    memcpy(g_constantParam.aucTerminalID, "MBBR0101", PARAM_TERMINALID_LEN);
//    memcpy(g_constantParam.aucTerminalID, "GPODHK01", PARAM_TERMINALID_LEN);

    //货币代码
    g_constantParam.usCurrencyId = 50;
    memcpy(g_constantParam.aucCurrencySign, (unsigned char *)"TK", 2);

    // 最大保存交易笔数
    g_constantParam.uiMaxTotalNb = 300;

    // 最大最小金额
    g_constantParam.ulMaxAmount=999999999;
    g_constantParam.ulMinAmount=0;
}

void xDATA::LoadChangeDefaultData(void)
{
    g_changeParam.simCard=true;
    g_changeParam.simTrans=true;

    g_changeParam.ulTransNum = 0;
    g_changeParam.ulBatchNumber = 1;

    g_changeParam.advance.TRANS_ENABLE=true;
    g_changeParam.advance.PIN_ENABLE=true;
    g_changeParam.advance.MANUAL_ENABLE=true;

    g_changeParam.deposit.TRANS_ENABLE=true;
    g_changeParam.deposit.PIN_ENABLE=true;
    g_changeParam.deposit.MANUAL_ENABLE=true;

    g_changeParam.balance.TRANS_ENABLE=true;
    g_changeParam.balance.PIN_ENABLE=true;
    g_changeParam.balance.MANUAL_ENABLE=true;

    g_changeParam.p2p.TRANS_ENABLE=true;
    g_changeParam.p2p.PIN_ENABLE=true;
    g_changeParam.p2p.MANUAL_ENABLE=true;

    g_changeParam.transvoid.TRANS_ENABLE=true;
    g_changeParam.transvoid.PIN_ENABLE=true;
    g_changeParam.transvoid.MANUAL_ENABLE=true;

    g_changeParam.adjust.TRANS_ENABLE=true;
    g_changeParam.adjust.PIN_ENABLE=true;
    g_changeParam.adjust.MANUAL_ENABLE=true;

    g_changeParam.pinchange.TRANS_ENABLE=true;
    g_changeParam.pinchange.PIN_ENABLE=true;
    g_changeParam.pinchange.MANUAL_ENABLE=true;

    g_changeParam.boolCashierLogonFlag = true;

    // timeout
   g_changeParam.TIMEOUT_PAPERTEAR=5*1000;   // 打印撕纸等待时间
   g_changeParam.TIMEOUT_ERRMSG=10*1000;         // 报错窗口自动关闭时间
   g_changeParam.TIMEOUT_HOST=30*1000;    // 主机超时
   g_changeParam.TIMEOUT_UI=60*1000;    // 界面超时

   g_changeParam.connectionRetry=0;     // 连接重试次数
}

void xDATA::LoadCashierDefaultData(void)
{
    qDebug()<<Q_FUNC_INFO;
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
    memcpy(g_cashier.aucSuperPSW,"\x31\x32\x33\x34\x35\x36\x37\x38",8);
    memcpy(g_cashier.aucSafePSW,"\x31\x32\x33\x34\x35\x36",6);
    memcpy(g_cashier.aucSuperPassWord,"838055",6);
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

    default:
        break;
    }
    return WriteValidFile(ucFileIndex);
}

unsigned char xDATA::DefaultParam(unsigned char ucTermType)
{
    ucTermType = 0;
    //   if(ucTermType  == TYPE_SHOP )//百货
    //{
    //       g_constIninfo.uiReceiveTimeout = 60;
    //       g_constIninfo.ucXorTipFee = 0;
    //       g_constIninfo.usTipFeePre = 0;
    //       g_constIninfo.ucDialRetry=3;
    //       g_constIninfo.ucTransRetry = 3;
    //       g_constIninfo.ucOFFLineSendType = '0';
    //       g_constIninfo.ucUnSaleSwipe = '1';
    //       g_constIninfo.ucUnManagePwd = '0';
    //       g_constIninfo.ucUnAuthFinSwipe = '1';
    //       g_constIninfo.ucVoidInputPin = '1';
    //       g_constIninfo.ucPREAUTHFINISHInputPin = '1';
    //       g_constIninfo.ucDefaultTransParam=TRANS_PURCHASE;
    //	g_constantParam.ucMagPreauthFinish='1';
    //       memcpy(g_constIninfo.aucGlobalTransEnable,
    //				"\x86\x19\xFF\x80" , PARAM_GLOBALTRANSENABLELEN);
    //}
    //   else if(ucTermType == TYPE_HOTEL)//宾馆
    //{
    //       g_constIninfo.uiReceiveTimeout = 60;
    //       g_constIninfo.ucXorTipFee = 0;
    //       g_constIninfo.usTipFeePre = 0;
    //       g_constIninfo.ucDialRetry=3;
    //       g_constIninfo.ucTransRetry = 3;
    //       g_constIninfo.ucUnSaleSwipe = '1';
    //       g_constIninfo.ucUnManagePwd = '0';
    //       g_constIninfo.ucUnAuthFinSwipe = '1';
    //       g_constIninfo.ucVoidInputPin = '1';
    //       g_constIninfo.ucPREAUTHFINISHInputPin = '1';
    //       g_constIninfo.ucDefaultTransParam=TRANS_PURCHASE;
    //		g_constantParam.ucMagPreauthFinish='1';
    //       memcpy(g_constIninfo.aucGlobalTransEnable,
    //				"\xFE\xB0\x00\x00" , PARAM_GLOBALTRANSENABLELEN);
    //}
    //   else if(ucTermType == RESTAURANT)//餐饮
    //{
    //       g_constIninfo.uiReceiveTimeout = 60;
    //       g_constIninfo.ucXorTipFee = 0;
    //       g_constIninfo.usTipFeePre = 0;
    //       g_constIninfo.ucDialRetry=3;
    //       g_constIninfo.ucTransRetry = 3;
    //       g_constIninfo.ucUnSaleSwipe = '1';
    //       g_constIninfo.ucUnManagePwd = '0';
    //       g_constIninfo.ucUnAuthFinSwipe = '1';
    //       g_constIninfo.ucVoidInputPin = '1';
    //       g_constIninfo.ucPREAUTHFINISHInputPin = '1';
    //       g_constIninfo.ucDefaultTransParam=TRANS_PURCHASE;
    //	g_constantParam.ucMagPreauthFinish='1';
    //       memcpy(g_constIninfo.aucGlobalTransEnable,
    //				"\x86\x18\x00\x00" , PARAM_GLOBALTRANSENABLELEN);
    //}
    //INIFILE_UpdateParamFile();
    return WriteValidFile(DataSaveConstant);
}

unsigned char xDATA::CheckCashierNo(unsigned char *pucIndex,unsigned char *pucCashierNo)
{
    unsigned char ucI;
    bool	bFirstFlag = true;

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

void xDATA::WriteSuperLogonFlag(unsigned char ucFlag)
{
    ucFlag= 0;
    //g_constIninfo.ucSuperLogonFlag = ucFlag;
    //INIFILE_UpdateParamViaID("SUPLOGON");
    /*WriteValidFile(DataSaveChange);*/
}

void xDATA::WriteCashierLogonIndex(unsigned char ucIndex)
{
    g_changeParam.ucCashierLogonIndex = ucIndex;
    WriteValidFile(DataSaveChange);
}

void xDATA::getEmvFiles()
{
    qDebug() << Q_FUNC_INFO;
    //unsigned int uiI = 0;
    //   int iResult = FILE_ReadRecordNumExternal(CAPKFILE, &ucTermCAPKNum);
    //if(!iResult)
    //	for(uiI =0;(!iResult)&&(uiI<ucTermCAPKNum)&&(uiI<MAXTERMCAPKNUMS);uiI++)
    //           iResult = FILE_ReadRecordExternal(CAPKFILE, uiI+1, (unsigned char *)&TermCAPK[uiI], sizeof(CAPK));
    //if(!iResult)
    //{
    //	EMVConfig.ucCAPKNums = ucTermCAPKNum;
    //	EMVConfig.pTermSupportCAPK = TermCAPK;
    //	qDebug("-----EMV_Check_file()-------ucTermCAPKNum = %d\n",ucTermCAPKNum);
    //	iResult =FILE_ReadRecordNumExternal(TERMSUPPORTAPPFILE, &ucTermAIDNum);
    //}
    //if(!iResult)
    //{
    //	for(uiI =0;(!iResult)&&(uiI<ucTermAIDNum)&&(uiI<MAXTERMAIDNUMS);uiI++)
    //		iResult = FILE_ReadRecordExternal(TERMSUPPORTAPPFILE, uiI+1,(unsigned char *)&TermAID[uiI], sizeof(TERMSUPPORTAPP));
    //	for(uiI =0;(!iResult)&&(uiI<ucTermAIDNum)&&(uiI<MAXTERMAIDNUMS);uiI++)
    //		iResult = FILE_ReadRecordExternal(EMVAIDPARAMFILE, uiI+1, (unsigned char *)&AIDParam[uiI], sizeof(EMVAIDPARAM));
    //}
    //if(!iResult)
    //{
    //	EMVConfig.pTermSupportApp = TermAID;
    //	SetSandReaderAIDInfo();
    //	iResult = FILE_ReadRecordNumExternal(EXCEPTFILE, &ucExceptFileNum);
    //}
    //if(!iResult)
    //	for(uiI =0;(!iResult)&&(uiI<ucExceptFileNum)&&(uiI<MAXEXCEPTFILENUMS);uiI++)
    //		iResult = FILE_ReadRecordExternal(EXCEPTFILE, uiI+1, (unsigned char *)&ExceptFile[uiI], sizeof(EXCEPTPAN));
    //if(!iResult)
    //{
    //	EMVConfig.ucExceptFileNum = ucExceptFileNum;
    //	EMVConfig.pExceptPAN = ExceptFile;
    //	iResult = FILE_ReadRecordNumExternal(IPKREVOKEFILE, &ucIPKRevokeNum);
    //}
    //if(!iResult)
    //	for(uiI =0;(!iResult)&&(uiI<ucIPKRevokeNum)&&(uiI<MAXIPKREVOKENUMS);uiI++)
    //if(!iResult)
    //{
    //	EMVConfig.ucIPKRevokeNum = ucIPKRevokeNum;
    //	EMVConfig.pIPKRevoke = IPKRevoke;
    //}
    //if(!ConstParam.ucFlag)
    //	TERMSETTING_LoadDefaultSetting();
    //EMVConfig.pfnEMVICC = EMVICC;
    //EMVConfig.pfnOfflinePIN = &InputDialog::InputOfflinePIN;
    //   EMVConfig.pfnOnlinePIN = &InputDialog::InputOnlinePIN;
    //   EMVConfig.pfnEMVInformMsg = &MsgReport::ShowEMVInformMsg;
    //   WriteValidFile(EMVCONFIGFILE);
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
int	xDATA::FILE_ReadRecordNumInternal(int iHandle,unsigned int * puiRecordNum)
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
int	xDATA::FILE_UpdateRecord(const unsigned char ucFileIndex,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen)
{
    int iResult = SALEERR_SUCCESS;
    int iHandle = OSAPP_OpenFile((char *)FileInfo[ucFileIndex].aucFileName,O_READ|O_WRITE);
    if(iHandle>=0)
        iResult = FILE_WriteRecord(iHandle,uiRecordNum,pRecordBuff,uiRecordLen);
    else iResult = SALEERR_OPENFILE;
    OSAPP_FileClose(iHandle);
    return iResult;
}
void xDATA::DeletaAllEMVfileData(void)
{
    ClearValidFile(CONSTPARAMFILE);
    ClearValidFile(EMVCONFIGFILE);
    ClearSubsectionFile(CAPKFILE);
    ClearSubsectionFile(TERMSUPPORTAPPFILE);
    ClearSubsectionFile(TRANSLOG);
    ClearSubsectionFile(EXCEPTFILE);
    ClearSubsectionFile(BATCHRECORD);
    ClearSubsectionFile(IPKREVOKEFILE);
    ClearSubsectionFile(ERRICCTRANS);
}
void xDATA::ClearEMVInterfaceData(void)
{
    //memset(&ICTransInfo , 0 ,sizeof(ICTRANSINFO));
    //memset(AIDParam , 0 ,sizeof(AIDParam));
    //ucTermCAPKNum=0;
    //ucTermAIDNum=0;
    //memset(AppListTerm , 0 ,sizeof(AppListTerm));
    //ucCAPKNum=0;
    //memset(CAPKInfo , 0 ,sizeof(CAPKInfo));
    //SupportAIDNum=0;
    //memset(SupportAID , 0 ,sizeof(SupportAID));
}

unsigned char xDATA::SaveCAPKFile(void)
{
    //UCHAR ucResult = ClearSubsectionFile(CAPKFILE);
    //for(unsigned int ucI=0;!ucResult && ucI<ucTermCAPKNum;ucI++)
    //{
    //	USHORT uiRecordNum = FILE_InsertRecord(CAPKFILE,(unsigned char *)&TermCAPK[ucI],sizeof(CAPK));
    //	if(uiRecordNum==0) ucResult = EMVERROR_SAVEFILE;
    //}
    //return ucResult;
    return 0;
}

unsigned char xDATA::SaveAIDFile(void)
{
    //UCHAR ucResult = ClearSubsectionFile(TERMSUPPORTAPPFILE);
    //for(unsigned int ucI=0;!ucResult && ucI<ucTermAIDNum;ucI++)
    //{
    //	USHORT uiRecordNum = FILE_InsertRecord(TERMSUPPORTAPPFILE,(unsigned char *)&TermAID[ucI],sizeof(TERMSUPPORTAPP));
    //	if(uiRecordNum==0) ucResult = EMVERROR_SAVEFILE;
    //}
    //return ucResult;
    return 0;
}
unsigned char xDATA::SaveAIDParamFile(void)
{
    //   UCHAR ucResult = ClearSubsectionFile(EMVAIDPARAMFILE);
    //for(unsigned int ucI=0;!ucResult && ucI<ucTermAIDNum;ucI++)
    //{
    //	USHORT uiRecordNum = FILE_InsertRecord(EMVAIDPARAMFILE,(unsigned char *)&AIDParam[ucI],sizeof(EMVAIDPARAM));
    //	if(uiRecordNum==0) ucResult = EMVERROR_SAVEFILE;
    //}
    //return ucResult;
    return 0;
}
unsigned char xDATA::SaveExceptFile(void)
{
    //UCHAR ucResult = ClearSubsectionFile(EXCEPTFILE);
    //for(unsigned int ucI=0;!ucResult && ucI<ucExceptFileNum;ucI++)
    //{
    //	USHORT uiRecordNum = FILE_InsertRecord(EXCEPTFILE,(unsigned char *)&ExceptFile[ucI],sizeof(EXCEPTPAN));
    //	if(uiRecordNum==0) ucResult = EMVERROR_SAVEFILE;
    //}
    //return ucResult;
    return 0;
}

unsigned char xDATA::SaveIPKRevokeFile(void)
{
    //   UCHAR ucResult = ClearSubsectionFile(IPKREVOKEFILE);
    //for(unsigned int ucI=0;!ucResult && ucI<ucIPKRevokeNum;ucI++)
    //{
    //	USHORT uiRecordNum = FILE_InsertRecord(IPKREVOKEFILE,(unsigned char *)&IPKRevoke[ucI],sizeof(IPKREVOKE));
    //	if(uiRecordNum==0) ucResult = EMVERROR_SAVEFILE;
    //}
    //return ucResult;
    return 0;

}

void xDATA::TERMSETTING_LoadDefaultSetting(void)
{
    //    qDebug() << Q_FUNC_INFO;
    //    UCHAR ucResult = 0x00;
    //    memset(&EMVConfig,0x00,sizeof(TERMCONFIG));
    //    memset(&ConstParam,0x00,sizeof(EMV_CONSTPARAM));
    //#if SANDREADER
    //    memset(&QReaderParam,0x00,sizeof(QREADERPARAM));
    //#endif
    //    ucIPKRevokeNum=0;
    //    ucExceptFileNum =0;
    //    ucTermAIDNum = 0;
    //    ucTermCAPKNum = 0;
    //
    //    memcpy(&EMVConfig,&TermConfigDefault,sizeof(TERMCONFIG));
    //
    //    EMVConfig.pfnEMVICC = EMVICC;
    //    EMVConfig.pfnOfflinePIN = &InputDialog::InputOfflinePIN;
    //    EMVConfig.pfnOnlinePIN = &InputDialog::InputOnlinePIN;
    //    EMVConfig.pfnEMVInformMsg = &MsgReport::ShowEMVInformMsg;
    //    memcpy(&ConstParam,&ConstParamDefault,sizeof(EMV_CONSTPARAM));
    //#if SANDREADER
    //    memcpy(&QReaderParam,&ConstReaderDefault,sizeof(QREADERPARAM));
    //#endif
    //    ConstParam.ucFlag=TRUE;
    //	ClearValidFile(CONSTPARAMFILE);
    //	ucResult = WriteValidFile(CONSTPARAMFILE);
    //	if(!ucResult)
    //    {
    //		ClearValidFile(EMVCONFIGFILE);
    //        ucResult = WriteValidFile(EMVCONFIGFILE);
    //    }
    //    if(!ucResult)
    //        ucResult = SaveCAPKFile();
    //    if(!ucResult)
    //        ucResult = SaveAIDFile();
    //    if(!ucResult)
    //        ucResult = SaveAIDParamFile();
    //    if(!ucResult)
    //        ucResult = SaveExceptFile();
    //    if(!ucResult)
    //        ucResult = SaveIPKRevokeFile();
    //    if(!ucResult)
    //        ucResult = ClearSubsectionFile(TRANSLOG);
    //    if(!ucResult)
    //        ucResult = ClearSubsectionFile(BATCHRECORD);
    //    if(!ucResult)
    //	{
    //		qDebug("\n ----------- EMV_LOAD_OK -----------\n");
    //        //MsgReport::ShowMsg(EMV_LOAD_OK);
    //	}
    //    else
    //        MsgReport::ShowEMVError(ucResult);
}

void xDATA::Read_LoadTransType(void)
{
    //	unsigned char ucTransBit1;
    //	unsigned char ucTransBit2;
    //	unsigned char ucTransBit3;
    //	unsigned char ucTransBit4;
    //
    //    ucTransBit1 = g_constIninfo.aucGlobalTransEnable[0];
    //    ucTransBit2 = g_constIninfo.aucGlobalTransEnable[1];
    //    ucTransBit3 = g_constIninfo.aucGlobalTransEnable[2];
    //    ucTransBit4 = g_constIninfo.aucGlobalTransEnable[3];		//扩展位
    //
    //	qDebug("\n ---0001---= [%02x,%02x,%02x,%02x]\n", ucTransBit1,ucTransBit2,ucTransBit3,ucTransBit4);
    //
    //	if(!g_constantParam.ucLoadFlag)
    //	{
    //		ucTransBit1 = 0xFF;
    //		ucTransBit2 = 0xFF;
    //		ucTransBit3 = 0xFF;
    //		ucTransBit4 = 0xC0;
    //	}
    //	I_Inquiry = ucTransBit1&0x80;					//查询
    //	I_Auth = ucTransBit1&0x40;					//预授权
    //	I_UnAuth = ucTransBit1&0x20;				//预授权撤销
    //	I_AuthFin = ucTransBit1&0x10;				//预授权完成(请求)
    //
    //	I_UnAuthFin = ucTransBit1&0x08;			//预授权完成撤销
    //	I_Sale = ucTransBit1&0x04;						//消费
    //	I_UnSale = ucTransBit1&0x02;					//消费撤销
    //	I_Refund = ucTransBit1&0x01;					//退货
    //
    //	I_Offline = ucTransBit2&0x80;					//离线结算
    //	I_TipFee = ucTransBit2&0x40;					//结算调整
    //	I_AuthFinishOff = ucTransBit2&0x20;		//预授权完成(通知)
    //	I_ICScript = ucTransBit2&0x10;				//PBOC　ＩＣ卡脚本结果通知
    //
    //	I_OffSale = ucTransBit2&0x08;				//电子现金脱机消费
    //	I_PurseDeposit = ucTransBit2&0x04;		//PBOC电子钱包圏存类交易
    //	I_PurseStage = ucTransBit2&0x01;			//分期付款消费
    //	I_VPurseStage= ucTransBit3&0x80;			//分期付款消费撤销
    //
    //	I_PurseInteg= ucTransBit3&0x40;			//积分消费
    //	I_VPurseInteg= ucTransBit3&0x20;			//积分消费撤销
    //	I_ucPBOCPrepaid = ucTransBit3&0x10;			//PBOC借代记圈存类交易
    //	I_ucMobileBooking = ucTransBit3&0x08;			//预约消费
    //
    //	I_ucVMobileBooking = ucTransBit3&0x04;			//预约消费撤销
    //	I_ucOrderSale = ucTransBit3&0x02;						//订购消费
    //	I_ucVOrderSale = ucTransBit3&0x01;					//订购消费撤销
    //	I_ucMagneticPrepaid = ucTransBit4&0x80;				//磁条卡现金(账户)充值
    //
    //
    //    G_NORMALTRANS_ucTransType=g_constIninfo.ucDefaultTransParam;
    //#if 0
    //	qDebug("\aucGlobalTransEnable ---001---= %02x \n", g_constantParam.aucGlobalTransEnable[0]);
    //	qDebug("aucGlobalTransEnable ---010---= %02x \n",g_constantParam.aucGlobalTransEnable[1]);
    //	qDebug("aucGlobalTransEnable ---100---= %02x \n",g_constantParam.aucGlobalTransEnable[2]);
    //	qDebug("ucTransBit1 ---001---= %02x \n", ucTransBit1);
    //	qDebug("ucTransBit2 ---010---= %02x \n", ucTransBit2);
    //	qDebug("ucTransBit3 ---100---= %02x \n", ucTransBit3);
    //	qDebug("I_Inquiry = %02x \n",I_Inquiry);
    //	qDebug("I_Auth = %02x \n",I_Auth);
    //	qDebug("I_UnAuth = %02x \n",I_UnAuth);
    //	qDebug("I_AuthFin = %02x \n",I_AuthFin);
    //	qDebug"I_UnAuthFin = %02x \n",I_UnAuthFin);
    //	qDebug("I_Sale = %02x \n",I_Sale);
    //	qDebug("I_UnSale = %02x \n",I_UnSale);
    //	qDebug("I_Refund = %02x \n",I_Refund);
    //	qDebug("I_Offline = %02x \n",I_Offline);
    //	qDebug("I_TipFee = %02x \n",I_TipFee);
    //	qDebug("I_AuthFinishOff = %02x \n",I_AuthFinishOff);
    //	qDebug("I_ICScript = %02x \n",I_ICScript);
    //	qDebug("I_OffSale = %02x \n",I_OffSale);
    //	qDebug("I_PurseDeposit = %02x \n",I_PurseDeposit);
    //	qDebug("I_PurseStage = %02x \n",I_PurseStage);
    //	qDebug("I_VPurseStage = %02x \n",I_VPurseStage);
    //	qDebug("I_PurseInteg = %02x \n",I_PurseInteg);
    //	qDebug("I_VPurseInteg = %02x \n",I_VPurseInteg);
    //#endif
    qDebug() << Q_FUNC_INFO;
}

void xDATA::ClrAccumulatDial(void)
{
    //unsigned long ulTemp = 0;
    //Os__saved_copy(	(unsigned char *)&ulTemp,(unsigned char *)&g_changeParam.ulDialConnectCnt,sizeof(unsigned long));
    //Os__saved_copy(	(unsigned char *)&ulTemp,(unsigned char *)&g_changeParam.ulDialTotalCount,sizeof(unsigned long));
    //Os__saved_copy(	(unsigned char *)&ulTemp,(unsigned char *)&g_changeParam.ulDialTimeTotal,sizeof(unsigned long));
    //Os__saved_copy(	(unsigned char *)&ulTemp,(unsigned char *)&g_changeParam.ulDialConnectRate,sizeof(unsigned long));
    //WriteValidFile(DataSaveChange);
}

void xDATA::IncreaseTraceNumber(void)
{
    //ULONG_C51 ulTraceNumber;
    //TransReqInfo.uiTraceNumber = ConstParam.uiTraceNumber;
    //   ulTraceNumber = g_changeParam.ulTraceNumber;
    //G_NORMALTRANS_ulTraceNumber = ulTraceNumber;
    //   	ulTraceNumber ++;
    //   if(ulTraceNumber > 999999)ulTraceNumber = 1;
    //ConstParam.uiTraceNumber = ulTraceNumber;
    //Os__saved_copy((unsigned char *)&ulTraceNumber,(unsigned char *)&g_changeParam.ulTraceNumber,sizeof(ULONG_C51));
    //WriteValidFile(CONSTPARAMFILE);
    //WriteValidFile(DataSaveChange);
}

void xDATA::AddTraceNumber(void)
{
    //ULONG_C51 ulTraceNumber;

    //g_changeParam.ulTraceNumber += 50;
    //ConstParam.uiTraceNumber += 50;
    //TransReqInfo.uiTraceNumber = ConstParam.uiTraceNumber ;
    //ulTraceNumber = g_changeParam.ulTraceNumber;
    //G_NORMALTRANS_ulTraceNumber = ulTraceNumber;
    //   	ulTraceNumber ++;
    //if( ulTraceNumber > 999999 )
    //{
    //	ulTraceNumber = 1;
    //}
    //ConstParam.uiTraceNumber = ulTraceNumber;
    //g_changeParam.ulTraceNumber = ulTraceNumber;
    //WriteValidFile(CONSTPARAMFILE);
    //WriteValidFile(DataSaveChange);
}

unsigned char xDATA::TransIsEmpty(void)
{
    //unsigned int uiTransNum=0;
    //if((g_transInfo.ForeignTransTotal.uiTotalNb == 0)&&(g_transInfo.TransTotal.uiTotalNb == 0))
    //	{
    //		if(G_NORMALTRANS_ucTransType==TRANS_SETTLE)
    //		{
    //			int iResult = FILE_ReadRecordNumExternal(ERRICCTRANS, &uiTransNum);
    //			qDebug("xDATA::TransIsEmpty()----FILE_ReadRecord ---uiTransNum=%d ----ucResult=%02x \n",uiTransNum,iResult);
    //			if(!iResult)
    //			{
    //				if(uiTransNum)
    //					return SUCCESS;
    //				else
    //					return(ERR_TRANSEMPTY);
    //			}else
    //			{
    //				MsgReport::ShowResultMsg(ERR_EMPTYICCTRANS);
    //				return(ERR_CANCEL);
    //			}
    //		}
    //		else return(ERR_TRANSEMPTY);
    //	}
    //else
        return(SUCCESS);
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

unsigned char xDATA::SavPrinterParamToPOS(unsigned char ucFlag)
{
    ucFlag = 0;
    //   unsigned char ch = ucFlag;
    //if(ch=='1') //------ 热敏------------
    //{
    //       Os__saved_set(&g_constIninfo.prnflag,1,1);
    //       Os__saved_set(&g_constIninfo.prnflagNew,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagOld,0,1);
    //       Os__saved_set(&g_constIninfo.linenum,0x16,1);
    //       Os__saved_set(&g_constIninfo.font,1,1);
    //       //Os__saved_set(&g_constIninfo.printtimes,2,1);
    //}else if(ch=='2') //------ 针打------------
    //{
    //       Os__saved_set(&g_constIninfo.prnflag,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagNew,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagOld,0,1);
    //       Os__saved_set(&g_constIninfo.linenum,0x15,1);
    //       Os__saved_set(&g_constIninfo.font,1,1);
    //       //Os__saved_set(&g_constIninfo.printtimes,1,1);
    //}else if(ch=='3') //------ 套打老纸------
    //{
    //       Os__saved_set(&g_constIninfo.prnflag,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagNew,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagOld,1,1);
    //       Os__saved_set(&g_constIninfo.linenum,0x08,1);
    //       Os__saved_set(&g_constIninfo.font,0x01,1);
    //       //Os__saved_set(&g_constIninfo.printtimes,1,1);
    //}else if(ch=='4') //------ 套打新纸------
    //{
    //       Os__saved_set(&g_constIninfo.prnflag,0,1);
    //       Os__saved_set(&g_constIninfo.prnflagNew,1,1);
    //       Os__saved_set(&g_constIninfo.prnflagOld,0,1);
    //       Os__saved_set(&g_constIninfo.linenum,0x08,1);
    //       Os__saved_set(&g_constIninfo.font,0x01,1);
    //       //Os__saved_set(&g_constIninfo.printtimes,1,1);
    //}
    //INIFILE_UpdateParamViaID("PNTYPE");
    //WriteValidFile(DataSaveConstant);
    //return (SUCCESS);
    return 0;
}

unsigned char xDATA::SLEAccumulatRunTime(void)
{
    /*unsigned char * aucBuf = NULL;
    unsigned char * aucTmp = NULL;
    unsigned long ulTemp = 0;
    aucBuf = (unsigned char *)malloc(sizeof(unsigned char)*32);memset(aucBuf,0,32);
    aucTmp = (unsigned char *)malloc(sizeof(unsigned char)*32);memset(aucTmp,0,32);
    Os__read_time(aucBuf);
    asc_bcd(aucTmp,2,aucBuf,4);
    if( aucTmp[0]!=g_changeParam.ucDialStartStamp )
    {
        Os__saved_set(&g_changeParam.ucDialStartStamp,
                        aucTmp[0],1);
        ulTemp=g_changeParam.ulDialTimeTotal;
        ulTemp++;
        if( ulTemp>=9999 )
        {
            ulTemp=0;
            Os__saved_copy(	(unsigned char *)&ulTemp,
                (unsigned char *)&g_changeParam.ulDialConnectCnt,
                sizeof(unsigned long));
            Os__saved_copy(	(unsigned char *)&ulTemp,
                    (unsigned char *)&g_changeParam.ulDialTotalCount,
                    sizeof(unsigned long));
            Os__saved_copy(	(unsigned char *)&ulTemp,
                    (unsigned char *)&g_changeParam.ulDialConnectRate,
                    sizeof(unsigned long));
        }
        Os__saved_copy(	(unsigned char *)&ulTemp,
            (unsigned char *)&g_changeParam.ulDialTimeTotal,
            sizeof(unsigned long));
        WriteValidFile(DataSaveChange);
    }
    free(aucBuf);free(aucTmp);
    return 1;*/
    return 0;
}

