#ifndef _XDATA_H
#define _XDATA_H


#include <QDebug>

#include "MsgReport.h"

//#include "EMVParam.h"
//#include "EMVInterface.h"


//#include "mySQLite3.h"
 
#define DATASAVE_TEST 1


typedef struct
{
	char aucFileName[10];
}FILEINFO;




class xDATA
{   
public:
    explicit xDATA();
    ~xDATA();
     //文件名static枚举
	
	enum
    {
		DataSaveConstant = 0,
		DataSaveChange,
		DataSaveCashier,
        DataSaveTransInfo,
        DataSaveReversal,
		DataSaveTrans8583,
		DataSaveScript,
        DataSaveTempMax,

        CONSTPARAMFILE,
		EMVCONFIGFILE,
		DataSaveSaveTrans,
		SaveCollectData,
        BlackFile,

		CAPKFILE,
        TERMSUPPORTAPPFILE,
        TRANSLOG,
        EXCEPTFILE,
		BATCHRECORD,
		IPKREVOKEFILE,
		EMVAIDPARAMFILE,
		ERRICCTRANS,

		
        

		DataSaveMaxIndex
    };
    enum
	{
	    SALEERR_SUCCESS  =  0,
		SALEERR_SEEKFILE,
		SALEERR_READFILE,
		SALEERR_OPENFILE  =  4,
		SALEERR_WRITEFILE,
		SALEERR_DELETERECORD
	};
	
    const static FILEINFO FileInfo[DataSaveMaxIndex + 1];
public:
	static void CheckFileExist();
    static unsigned char CheckValidFile(const unsigned char ucFileIndex);
    static unsigned char WriteValidFile(const unsigned char ucFileIndex);
	static unsigned char ReadValidFile(const unsigned char ucFileIndex);
	static unsigned char ClearValidFile(const unsigned char ucFileIndex);
    static unsigned char CheckSubsectionFile(const unsigned char ucFileIndex);
    static unsigned char WriteSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex,int operate = 0);
    static unsigned char ReadSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex);
    static unsigned char ClearSubsectionFile(const unsigned char ucFileIndex);

    static void getEmvFiles();
	static void ClearGlobalData();
    static unsigned char SetDefaultValue(const unsigned char ucFileIndex);
	static unsigned char DefaultParam(unsigned char ucTermType);
public:
    static void * GetFile(const unsigned char ucFileIndex, long *puiDataSize);
    static void * GetConstantData(long *puiDataSize);
    static void * GetChangeData(long *puiDataSize);	
	static void * GetCashierData(long *puiDataSize);
	static void * GetTransInfoData(long *puiDataSize);
    static void * GetSaveCollData(long *puiDataSize);
	static void * GetScriptData(long *puiDataSize);
    static void * GetReversalData(long *puiDataSize);
    static void * GetTempData(long *puiDataSize);
    static void * Get8583Backup(long *puiDataSize);
	static void * GetTransactionData(long *puiDataSize);
	static void * GetBlackListData(long *puiDataSize);
    static void * GetConstParamFile(long *puiDataSize);
	static void * GetEMVConfigFile(long *puiDataSize);
	static void * GetSubsectionFile(const unsigned char ucFileIndex, long *puiDataSize);
     
	static unsigned char SaveCAPKFile(void);
    static unsigned char SaveAIDFile(void);
    static unsigned char SaveAIDParamFile(void);
    static unsigned char SaveExceptFile(void);
    static unsigned char SaveIPKRevokeFile(void);

    static void LoadIniFileDefaultData(void);
    static void LoadConstantDefaultData(void);
    static void LoadChangeDefaultData(void);
	static void LoadCashierDefaultData(void);	

    static void DeletaAllEMVfileData(void);
    static void ClearEMVInterfaceData(void);
    static void Read_LoadTransType(void);
    static void TERMSETTING_LoadDefaultSetting(void);
    static void WriteSuperLogonFlag(unsigned char ucFlag);
	static void WriteCashierLogonIndex(unsigned char ucIndex);
	static unsigned char CheckCashierNo(unsigned char *pucIndex,unsigned char *pucCashierNo);
	static void UpdateCashierData(unsigned char ucNo,unsigned char *pucCashierNo,unsigned char *pucCashierPass);
    static void AddTraceNumber(void);
    static void IncreaseTraceNumber(void);
    static void ClrAccumulatDial(void);
    static unsigned char TransIsEmpty(void);


    static unsigned char SavPrinterParamToPOS(unsigned char ucFlag);
	static unsigned char SLEAccumulatRunTime(void);
public:
	static int FILE_ReadRecordNumExternal(const unsigned char ucFileIndex,unsigned int *puiRecordNum);
	static int FILE_ReadRecordNumInternal(int iHandle,unsigned int * puiRecordNum);
    static int FILE_ReadRecordExternal(const unsigned char ucFileIndex,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen);
    static int FILE_ReadRecordInternal(int iHandle,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen);
    static unsigned int FILE_SearchRecord(const unsigned char ucFileIndex,unsigned short uiRecordSize,unsigned short uiRecordItemAddrOffset,
															unsigned char *pucItemValue,unsigned short uiItemValueLen,unsigned char *pRecordBuff);
    static unsigned int FILE_InsertRecord(const unsigned char ucFileIndex,unsigned char *pucRecord,unsigned short uiRecordSize);
    static int FILE_WriteRecordNumInternal(int iHandle,unsigned int uiRecordNum);
    static int FILE_WriteRecord(int iHandle,unsigned int uiRecordNum,unsigned char* pRecordBuff,unsigned short uiRecordLen);
    static int FILE_DeleteRecord(const unsigned char ucFileIndex,unsigned short uiRecordLen,unsigned int uiStartNum,unsigned int uiDeleteNums);
	static int FILE_UpdateRecord(const unsigned char ucFileIndex,unsigned int uiRecordNum,unsigned char * pRecordBuff,unsigned short uiRecordLen);
};

#endif 

