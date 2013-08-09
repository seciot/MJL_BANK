#ifndef _XDATA_H
#define _XDATA_H

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
        //Vaild File
        DataSaveConstant = 0,
        DataSaveChange,
        DataSaveCashier,
        DataSaveTransInfo,
        DataSaveOldTransInfo,
        DataSaveReversal,
        DataSaveBINStatus,
        // Subsection File
        DataSaveSaveTrans,
        DataSaveBINTable,
        DataSaveMaxIndex
    };

    enum
    {
        SALEERR_SUCCESS         = 0,
        SALEERR_SEEKFILE,
        SALEERR_READFILE,
        SALEERR_OPENFILE        = 4,
        SALEERR_WRITEFILE,
        SALEERR_DELETERECORD
    };

    const static FILEINFO FileInfo[DataSaveMaxIndex + 1];
private:
    static void * GetFile(const unsigned char ucFileIndex, long *puiDataSize);
    static void * GetSubsectionFile(const unsigned char ucFileIndex, long *puiDataSize);

    static void LoadConstantDefaultData(void);
    static void LoadChangeDefaultData(void);
    static void LoadCashierDefaultData(void);
    static void LoadCardBINDefaultData(void);
    static unsigned char SetDefaultValue(const unsigned char ucFileIndex);
public:
    static unsigned char CheckValidFile(const unsigned char ucFileIndex);
    static unsigned char WriteValidFile(const unsigned char ucFileIndex);
    static unsigned char ReadValidFile(const unsigned char ucFileIndex);
    static unsigned char ClearValidFile(const unsigned char ucFileIndex);

    static unsigned char CheckSubsectionFile(const unsigned char ucFileIndex);
    static unsigned char WriteSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex,int operate = 0);
    static unsigned char ReadSubsectionFile(const unsigned char ucFileIndex, const unsigned short uiIndex);
    static unsigned char ClearSubsectionFile(const unsigned char ucFileIndex);

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
public:
    static void CheckFileExist();
    static void ClearGlobalData();

    static unsigned char CheckCashierNo(unsigned char *pucIndex,unsigned char *pucCashierNo);
    static void UpdateCashierData(unsigned char ucNo,unsigned char *pucCashierNo,unsigned char *pucCashierPass);
};

#endif
