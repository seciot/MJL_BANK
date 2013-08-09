#ifndef _MIFAREPRO_H
#define _MIFAREPRO_H
#include "new_drv.h"
#include "fsync_drvs.h"
/*
param : ucDataBlockIndex
type:   unsigned char 
Mifare Classic 4k Blocks - Minimum Index : 0
						 - Maximum Index : 255
*/
unsigned char MIFARE_readCardNo(DRV_OUT *pxMifare,unsigned char ucDataBlockIndex,unsigned char *pucOutCardNo,unsigned short *puiOutLen);
/*
return value List:
typedef enum MIFARE_ERR_INDEX
{
     HALMIFAREERR_SUCCESS    =  0,  //正确
     HALMIFAREERR_NOTAG      =  1,  //无卡
     HALMIFAREERR_CRC        =  2,  //卡片CRC 校验错误
     HALMIFAREERR_EMPTY      =  3, //数值溢出错误
     HALMIFAREERR_AUTH       =  4, //验证不成功
     HALMIFAREERR_PARITY     =  5, //卡片奇偶校验错误
     HALMIFAREERR_CODE       =  6, //通讯错误(BCC 校验错) 
     HALMIFAREERR_SERNR      =  8, //卡片序列号错误(anti-collision 错误) 
     HALMIFAREERR_SELECT     =  9, //卡片数据长度字节错误(SELECT 错误) 
     HALMIFAREERR_NOTAUTH    =  10, //卡片没有通过验证
     HALMIFAREERR_BITCOUNT   =  11, //从卡片接收到的位数错误
     HALMIFAREERR_BYTECOUNT  =  12, //从卡片接收到的字节数错误（仅读函数有效）
     HALMIFAREERR_REST       =  13, //调用restore 函数出错
     HALMIFAREERR_TRANS      =  14, //调用transfer 函数出错
     HALMIFAREERR_WRITE		 =  15, //调用write 函数出错
     HALMIFAREERR_INCR		 =  16, //调用increment 函数出错
     HALMIFAREERR_DECR       =  17, //调用decrement 函数出错
     HALMIFAREERR_READ       =  18, //调用read 函数出错
     HALMIFAREERR_LOADKEY    =  19, //调用LOADKEY 函数出错
     HALMIFAREERR_FRAMING    =  20, //FM1715 帧错误
     HALMIFAREERR_RESQ       =  21, //调用req 函数出错
     HALMIFAREERR_SEL        =  22, //调用sel 函数出错
     HALMIFAREERR_ANTICOLL   =  23, //调用anticoll 函数出错
     HALMIFAREERR_INTIVAL    =  24, //调用初始化函数出错
     HALMIFAREERR_READVAL    =  25, //调用高级读块值函数出错
     HALMIFAREERR_DESELECT   =  26, //取消选定出错
     HALMIFAREERR_CMD        =  42, //命令错误
     HALMIFAREERR_INIT       =  43,  //初始化失败
     HALMIFAREERR_UNKNOWN    =  44, //未找到硬件
     HALMIFAREERR_COLLISION  =  45, //位冲突
     HALMIFAREERR_FRAME      =  46, //Frame 错误
     HALMIFAREERR_ATS_LEN    =  47, //Ats 长度错误
     HALMIFAREERR_ATS_ERR    =  48, //Ats 取ats错误 
	 HALMIFAREERR_APP_ERR    =  49, //应用异常


	 MIFARE_UNKNOWN_ERR      =  0xFF
}MifareErrIndex;
	

	for English instance:
	
		mifareErrTab[]=
    {
	{HALMIFAREERR_SUCCESS,				{"SUCCESS !"}},
	{HALMIFAREERR_NOTAG,				{"No Tag"}},
	{HALMIFAREERR_CRC,					{"CRC Err"}},
	{HALMIFAREERR_EMPTY,				{"DATA ABORT!"}},
	{HALMIFAREERR_AUTH,					{"AUTH FAIL"}},
	{HALMIFAREERR_PARITY,				{"PARITY Err"}},
	{HALMIFAREERR_CODE,					{"BCC Err"}},
	{HALMIFAREERR_SERNR,				{"anti-collision Err"}},
	{HALMIFAREERR_SELECT,				{"SELECT Err"}},
	{HALMIFAREERR_NOTAUTH,				{"Not Auth yet"}},
	{HALMIFAREERR_BITCOUNT,				{"INVALID BITS"}},
    {HALMIFAREERR_BYTECOUNT,			{"INVALID BYTES"}},
	{HALMIFAREERR_REST,					{"Restore Err"}},
	{HALMIFAREERR_TRANS,				{"transfer Err"}},
	{HALMIFAREERR_WRITE,				{"Write Err"}},
	{HALMIFAREERR_INCR,					{"increment Err"}},
	{HALMIFAREERR_DECR,					{"decrement Err"}},
	{HALMIFAREERR_READ,					{"read File Err"}},
	{HALMIFAREERR_LOADKEY,				{"LoadKey Err"}},
	{HALMIFAREERR_FRAMING,				{"FM1715 Fail"}},
	{HALMIFAREERR_RESQ,					{"Request Err"}},
	{HALMIFAREERR_SEL,					{"sel Err"}},
	{HALMIFAREERR_ANTICOLL,				{"anticoll Err"}},
	{HALMIFAREERR_INTIVAL,				{"INITIAL Err"}},
	{HALMIFAREERR_READVAL,				{"Err READVAL"}},
	{HALMIFAREERR_DESELECT,				{"Deselect Err"}},
	{HALMIFAREERR_CMD,					{"CMD Err"}},
	{HALMIFAREERR_INIT,					{"INITIAL FAIL"}},
	{HALMIFAREERR_UNKNOWN,				{"HARDWARE NOT FND"}},
	{HALMIFAREERR_COLLISION,			{"COLLISION !"}},
	{HALMIFAREERR_FRAME,				{"WRONG FRAME"}},
	{HALMIFAREERR_ATS_LEN,			    {"Invalid Ats"}},
	{HALMIFAREERR_ATS_ERR,			    {"Ats Err"}},
	{MIFARE_UNKNOWN_ERR,				{"UNKNOWN ERR"}}
};
*/



#endif
