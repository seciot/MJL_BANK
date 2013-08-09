#ifndef _MIFARECLASS_H
#define _MIFARECLASS_H





#define CLASSIC_ORDER_READ	    	0x30
#define CLASSIC_ORDER_WRITE	      	0xA0
#define CLASSIC_ORDER_INC	      	0xC1
#define CLASSIC_ORDER_DEC			0xC0
#define CLASSIC_ORDER_RESTORE		0xC2
#define CLASSIC_ORDER_TRANSFER		0xB0
#define CLASSIC_ORDER_AUTHA			0x60
#define CLASSIC_ORDER_AUTHB			0x61

unsigned char CLASSIC_ReadBlock( unsigned char ucBlockIndex,unsigned char *pucReadData );
unsigned char CLASSIC_WriteBlock( unsigned char ucBlockIndex,unsigned char *pucWriteData );
unsigned char CLASSIC_Increment( unsigned char ucBlockIndex,unsigned long ulValue );
unsigned char CLASSIC_Decrement( unsigned char ucBlockIndex,unsigned long ulValue );
unsigned char CLASSIC_Restore( unsigned char ucBlockIndex );
unsigned char CLASSIC_Transfer( unsigned char ucBlockIndex );
unsigned char CLASSIC_Auth( unsigned char ucAuthType,unsigned char ucSectorIndex,unsigned char ucKeyType,unsigned char *pucKey );







#endif




