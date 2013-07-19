#ifndef ISO8583DATA_H
#define ISO8583DATA_H

/* ISO8583 Define */
#define ISO8583_MSGIDLEN            2
#define ISO8583_BITMAPLEN           16
#define ISO8583_MAXBITNUM           128
#define ISO8583_MAXBITBUFLEN        1024        /* Max unpacked ISO8583 buffer */
#define ISO8583_MAXCOMMBUFLEN       1024        /* Max Iso8583 communication buffer */
#define ISO8583_BITNOTEXISTFLAG     0x00
#define ISO8583_BITEXISTFLAG        0xff
#define ISO8583_BIT48NUM            8

typedef struct
{
    unsigned char   ucExistFlag;
    unsigned short  uiOffset;
    unsigned short  uiLen;
}ISO8583Bit;

typedef struct
{
    ISO8583Bit      Bit[ISO8583_MAXBITNUM];
    unsigned short  uiBitBufLen;
    unsigned char   aucMsgID[ISO8583_MSGIDLEN];
    unsigned char   aucBitBuf[ISO8583_MAXBITBUFLEN];
    unsigned short  uiCommBufLen;
    unsigned char   aucCommBuf[ISO8583_MAXCOMMBUFLEN];
}ISO8583;

typedef struct
{
    ISO8583Bit      Bit[ISO8583_MAXBITNUM];
    unsigned short  uiBitBufLen;
    unsigned char   aucMsgID[ISO8583_MSGIDLEN];
    unsigned char   aucBitBuf[ISO8583_MAXBITBUFLEN];
}BACKUPISO8583;

typedef struct
{
    unsigned char   ucValid;        //FLAG_ON 有冲正
    unsigned char   ucRevealEvent[2];
    unsigned char   ucAuthFlag;
    unsigned char   aucAuthData[6];
    BACKUPISO8583   BackupISO8583Data;
}REVERSEDISO8583;

typedef struct
{
    BACKUPISO8583	SaveISO8583Data;        //临时备份ISO8583结构数据(不包括COMMLEN,COMMBUF)
    BACKUPISO8583	SendISO8583Data;        //用于对ISO关键域数据的校验,
}TRANS_8583_DATA;

#endif // ISO8583DATA_H
