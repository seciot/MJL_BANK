#include "pinpad.h"
#include <string.h>
#include "ostools.h"
#include "encrypt.h"
#include "msg.h"

ENCRYPT_DRV_DATA    DrvData;

/*******************************************************************
 *  函数名：gz_xor
 *  说明：异或2组数据
 *  输入：a      异或数据
 *       b      异或数据，同时也是异或后数据
 *       lg     数据长度
 ******************************************************************/
void gz_xor(unsigned char *a, unsigned char *b, char lg)
{
    while (lg--)
        *(b++) ^= *(a++);
}

/*******************************************************************
 *  函数名：LoadSingleKeyUseSingleKey_42
 *  说明：单倍长（8字节）密钥“解存”单倍长（8字节）数据
 *  输入：ucKeyArray           需要解存的密钥行
 *       ucDecryptKeyIndex    解密密钥所在的列
 *       ucDestKeyIndex       解密后数据所存的列
 *       pucInData            需要解密存储的数据
 *  输出：0x00成功 其他表示失败
 ******************************************************************/
unsigned char LoadSingleKeyUseSingleKey_42(unsigned char ucKeyArray, unsigned char ucDecryptKeyIndex, unsigned char ucDestKeyIndex, unsigned char *pucInData)
{
    memset(&DrvData, 0x00, sizeof(ENCRYPT_DRV_DATA));
    DrvData.keytype = ENCRYPT_SAVE_BYTRANSKEY;
//    DrvData.saveappid  = ucKeyArray;
//    DrvData.cryptappid = ucKeyArray;
    DrvData.cryptindex = ((ucKeyArray*KEYINDEX_MAX_NUM)+ucDecryptKeyIndex);
    DrvData.saveindex  = ((ucKeyArray*KEYINDEX_MAX_NUM)+ucDestKeyIndex);
    DrvData.crypttype  = ENCRYPT_DES_SINGLE_DECRYPT;
    hex_asc(&DrvData.keyvalue[0], pucInData, 16);
    if(!ENCRYPT_Decrypt_Load_Key(&DrvData))
        return(SUCCESS_TRACKDATA);
    else
        return(ERR_DRIVER);
}

/*******************************************************************
 *  函数名：Encrypt8ByteSingleKey_47
 *  说明：单倍长（8字节）密钥“加密”单倍长（8字节）数据
 *  输入：ucKeyArray           加密密钥所在的行
 *       ucDecryptKeyIndex    加密密钥所在的列
 *       pucInData            需要加密的数据
 *       pucOutData           加密后的数据
 *  输出：0x00成功 其他表示失败
 ******************************************************************/
unsigned char Encrypt8ByteSingleKey_47(unsigned char ucKeyArray, unsigned char ucKeyIndex, unsigned char *pucInData, unsigned char *pucOutData)
{
    memset(&DrvData, 0x00, sizeof(ENCRYPT_DRV_DATA));
    DrvData.keytype = ENCRYPT_DECRYPT_BYTRANSKEY;
//    DrvData.cryptappid = ucKeyArray;
    DrvData.cryptindex = ((ucKeyArray*KEYINDEX_MAX_NUM)+ucKeyIndex);
    DrvData.crypttype  = ENCRYPT_DES_SINGLE;
    hex_asc(&DrvData.keyvalue[0], pucInData, 16);
    if(!ENCRYPT_Decrypt_Load_Key(&DrvData))
    {
        asc_hex(pucOutData, 8, &DrvData.outdata[0], 16);
        return(SUCCESS_TRACKDATA);
    }
    else
        return(ERR_DRIVER);
}

/*******************************************************************
 *  函数名：Crypt8ByteSingleKey_47
 *  说明：单倍长（8字节）密钥“解密”单倍长（8字节）数据
 *  输入：ucKeyArray           解密密钥所在的行
 *       ucDecryptKeyIndex    解密密钥所在的列
 *       pucInData            需要解密的数据
 *       pucOutData           解密后的数据
 *  输出：0x00成功 其他表示失败
 ******************************************************************/
unsigned char Crypt8ByteSingleKey_47(unsigned char ucKeyArray, unsigned char ucKeyIndex, unsigned char *pucInData, unsigned char *pucOutData)
{
    memset(&DrvData, 0x00, sizeof(ENCRYPT_DRV_DATA));
    DrvData.keytype = ENCRYPT_DECRYPT_BYTRANSKEY;
//    DrvData.cryptappid = ucKeyArray;
    DrvData.cryptindex = ((ucKeyArray*KEYINDEX_MAX_NUM)+ucKeyIndex);
    DrvData.crypttype  = ENCRYPT_DES_SINGLE_DECRYPT;
    hex_asc(&DrvData.keyvalue[0], pucInData, 16);
    if(!ENCRYPT_Decrypt_Load_Key(&DrvData))
    {
        asc_hex(pucOutData, 8, &DrvData.outdata[0], 16);
        return(SUCCESS_TRACKDATA);
    }
    else
        return(ERR_DRIVER);
}

/*******************************************************************
 *  函数名：CryptByDoubleKey
 *  说明：双倍长（16字节）密钥“解密”单倍长（8字节）或双倍长（16字节）数据
 *       如果数据后8字节为0x00，则解密后为8字节数据，反之则为16字节数据
 *  输入：ucKeyArray           解密密钥所在的行
 *       ucDecryptKeyIndex    解密密钥所在的列
 *       pucInData            需要解密的数据
 *       pucOutData           解密后的数据
 *  输出：0x00成功 其他表示失败
 ******************************************************************/
unsigned char CryptByDoubleKey(unsigned char ucKeyArray, unsigned char ucKeyIndex, unsigned char *pucInData, unsigned char *pucOutData)
{
    unsigned char tmp1[8+1],tmp2[8+1],tmp3[8+1];
    unsigned char ucResult = ERR_DRIVER;
    unsigned char ucTime = 0,ucI = 0;

    memset(tmp1,0,sizeof(tmp1));
    memset(tmp2,0,sizeof(tmp2));
    memset(tmp3,0,sizeof(tmp3));

    if(!memcmp(&pucInData[8],"\x00\x00\x00\x00\x00\x00\x00\x00",8))
        ucTime = 1;
    else
        ucTime = 2;

    for(ucI = 0;ucI < ucTime;ucI ++)
    {
        if((ucResult = Crypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex,pucInData + 8*ucI,tmp1)) != SUCCESS_TRACKDATA)
            return ucResult;
        if((ucResult = Encrypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex+1,tmp1,tmp2)) != SUCCESS_TRACKDATA)
            return ucResult;
        if((ucResult = Crypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex,tmp2,tmp3)) != SUCCESS_TRACKDATA)
            return ucResult;

        if(ucTime == 1)
            memcpy(pucOutData,tmp3,8);
        else
            memcpy(&pucOutData[8],tmp3,8);
    }
    return ucResult;
}

/*******************************************************************
 *  函数名：EncryptByDoubleKey
 *  说明：双倍长（16字节）密钥“加密”单倍长（8字节）或双倍长（16字节）数据
 *       如果数据后8字节为0x00，则加密后为8字节数据，反之则为16字节数据
 *  输入：ucKeyArray           加密密钥所在的行
 *       ucDecryptKeyIndex    加密密钥所在的列
 *       pucInData            需要加密的数据
 *       pucOutData           加密后的数据
 *  输出：0x00成功 其他表示失败
 ******************************************************************/
unsigned char EncryptByDoubleKey(unsigned char ucKeyArray, unsigned char ucKeyIndex, unsigned char *pucInData, unsigned char *pucOutData)
{
    unsigned char tmp1[8+1],tmp2[8+1],tmp3[8+1];
    unsigned char ucResult = ERR_DRIVER;
    unsigned char ucTime = 0,ucI = 0;

    memset(tmp1, 0, sizeof(tmp1));
    memset(tmp2, 0, sizeof(tmp2));
    memset(tmp3, 0, sizeof(tmp3));

    if(!memcmp(&pucInData[8],"\x00\x00\x00\x00\x00\x00\x00\x00",8))
        ucTime = 1;
    else
        ucTime = 2;

    for(ucI = 0;ucI < ucTime;ucI ++)
    {
        if((ucResult = Encrypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex,pucInData + 8*ucI,tmp1)) != SUCCESS_TRACKDATA)
            return ucResult;
        if((ucResult = Crypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex+1,tmp1,tmp2)) != SUCCESS_TRACKDATA)
            return ucResult;
        if((ucResult = Encrypt8ByteSingleKey_47(ucKeyArray,ucKeyIndex,tmp2,tmp3)) != SUCCESS_TRACKDATA)
            return ucResult;

        if(ucTime == 1)
            memcpy(pucOutData,tmp3,8);
        else
            memcpy(&pucOutData[8],tmp3,8);
    }

    return ucResult;
}

/*******************************************************************
 *  函数名：LoadKeyUseDoubleKey
 *  说明：双倍长（16字节）密钥“解存”单倍长（8字节）或双倍长（16字节）数据
 *       如果数据后8字节为0x00，则解存后为8字节数据，反之则为16字节数据
 *       解存双倍长数据，则后8位数据存入下个密钥单位
 *  输入：ucKeyArray           需要解存的密钥行
 *       ucDecryptKeyIndex    解密密钥所在的列
 *       ucDestKeyIndex       解密后数据所存的列
 *       pucInData            需要解密存储的数据
 *  输出：0x00成功 其他表示失败
 *******************************************************************/
unsigned char LoadKeyUseDoubleKey(unsigned char ucKeyArray, unsigned char ucDecryptKeyIndex, unsigned char ucDestKeyIndex, unsigned char *pucInData)
{
    unsigned char ucTime = 0,ucI = 0;
    unsigned char ucResult = ERR_DRIVER;

    if(!memcmp(&pucInData[8],"\x00\x00\x00\x00\x00\x00\x00\x00",8))
        ucTime = 1;
    else
        ucTime = 2;

    for(ucI = 0;ucI < ucTime;ucI ++)
    {
        if(LoadSingleKeyUseSingleKey_42(ucKeyArray,ucDecryptKeyIndex,ucDestKeyIndex+ucI,pucInData+ucI*8) != SUCCESS_TRACKDATA)
            return ucResult;
    }
    return ucResult;
}
