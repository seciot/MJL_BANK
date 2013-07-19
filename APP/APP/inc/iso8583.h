#ifndef ISO8583_H
#define ISO8583_H
#include <QDebug>

void ISO8583_DumpData(void);

void ISO8583_Clear(void);
void ISO8583_ClearBit(void);
void ISO8583_ClearBitBuf(void);
unsigned char ISO8583_GetMsgID(unsigned short *piMsgID);
unsigned char ISO8583_SetMsgID(unsigned short iMsgID);
unsigned char ISO8583_GetBitValue(short iBitNo, unsigned char *pucOutData, short *piOutDataLen, short iOutBufLen);
unsigned char ISO8583_SetBitValue(short iBitNo, const unsigned char *pucInData,unsigned short uiInLen);
unsigned char ISO8583_SetBitHexValue(short iBitNo, unsigned char *pucInData, unsigned short uiInLen);
unsigned char ISO8583_CheckBit(short iBitNo);
unsigned char ISO8583_RemoveBit(short iBitNo);
unsigned char ISO8583_PackData(unsigned char *pucOutData, unsigned short *piOutDataLen, short iOutLen);
unsigned char ISO8583_UnpackData(unsigned char *pucInData, short iInDataLen);
unsigned char ISO8583_CompareSentBit(short iBitNo);
void ISO8583_SaveISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
void ISO8583_RestoreISO8583Data(unsigned char *pucSrc,unsigned char *pucDest);
unsigned char ISO8583_SaveReversalISO8583Data(const unsigned char *aucEvent);

#endif
