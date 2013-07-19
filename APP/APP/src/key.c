#include "pinpad.h"
#include <string.h>
#include "ostools.h"
#include "key.h"
#include "msg.h"

#define KEY_DEBUG_FLAG

#define min(a, b)       (((a) < (b)) ? (a) : (b))
//#define max(a, b)       (((a) > (b)) ? (a) : (b))

#define KEY_DEBUG_FLAG

void KEY_DumpData(char *pTitle, unsigned char *pData, short sDataLen)
{
#ifdef KEY_DEBUG_FLAG
    short i;

    Uart_Printf(pTitle);
    for(i = 0; i < sDataLen; i++)
        Uart_Printf("%02x ", pData[i]);
    Uart_Printf("\n");
#endif
}

unsigned char KEY_LoadDefaultKey(void)
{
    unsigned char ucResult;
    unsigned char aucHexData[17];
    int i = 0, iDES = 0;

    memset(aucHexData, 0x00, sizeof(aucHexData));
//    memcpy(aucHexData, "\x01\x23\x45\x67\x89\xAB\xCD\xEF", 8);
    memcpy(aucHexData, "\x01\x23\x45\x67\x89\xAB\xCD\xEF\x01\x23\x45\x67\x89\xAB\xCD\xEF", 16);


    if(!memcmp(&aucHexData[8], "\x00\x00\x00\x00\x00\x00\x00\x00", 8))
        iDES = 1;
    else
        iDES = 2;

    for(i = 0; i < iDES; i++)
    {
        ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                            0,
                                            &aucHexData[i*8],
                                            &aucHexData[i*8]);
        if(!ucResult)
            ucResult = LoadSingleKeyUseSingleKey_42(KEYARRAY_GOLDENCARDSH,
                                                    0,
                                                    KEYINDEX_GOLDENCARDSH_MK+i,
                                                    &aucHexData[i*8]);
        if(ucResult)
            break;
    }
    return(ucResult);
}

unsigned char KEY_StoreSingleNewKey(unsigned char *aucHexData, short iLen)
{
    unsigned char ucResult;
    unsigned char aucPinKey[9];
    unsigned char aucMacKey[9];
    unsigned char ucOffset = 0;

    if(iLen != 16)
        return ERR_ISO8583_INVALIDLEN;

    //PIN KEY
    ucOffset = 0;
    memset(aucPinKey, 0, sizeof(aucPinKey));
    memcpy(aucPinKey, &aucHexData[ucOffset], 8);
    ucOffset += 8;

    //MAC KEY
    memset(aucMacKey, 0, sizeof(aucMacKey));
    memcpy(aucMacKey, &aucHexData[ucOffset], 8);
    ucOffset += 8;

    KEY_DumpData("=========== Encrypt Pin Key  ===========\n", aucPinKey, 8);
    KEY_DumpData("=========== Encrypt Mac Key  ===========\n", aucMacKey, 8);

    //PIN KEY
#if 0
    ucResult = Crypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                      KEYINDEX_GOLDENCARDSH_MK,
                                      aucPinKey,
                                      aucPinKey);

    if(!ucResult)
        ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                            KEYINDEX_GOLDENCARDSH_MK2,
                                            aucPinKey,
                                            aucPinKey);
    if(!ucResult)
#endif
    ucResult = LoadSingleKeyUseSingleKey_42(KEYARRAY_GOLDENCARDSH,
                                            KEYINDEX_GOLDENCARDSH_MK,
                                            KEYINDEX_GOLDENCARDSH_PIK,
                                            aucPinKey);

    //MAC KEY
#if 0
    if(!ucResult)
        ucResult = Crypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                          KEYINDEX_GOLDENCARDSH_MK,
                                          aucMacKey,
                                          aucMacKey);

    if(!ucResult)
        ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                            KEYINDEX_GOLDENCARDSH_MK2,
                                            aucMacKey,
                                            aucMacKey);
#endif
    if(!ucResult)
        ucResult = LoadSingleKeyUseSingleKey_42(KEYARRAY_GOLDENCARDSH,
                                                KEYINDEX_GOLDENCARDSH_MK,
                                                KEYINDEX_GOLDENCARDSH_MAK,
                                                aucMacKey);
#if 0
    //对0x00进行加密
    if(!ucResult)
    {
        memset(aucPinKey, 0x30, sizeof(aucPinKey));
        Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                 KEYINDEX_GOLDENCARDSH_PIK,
                                 aucPinKey,
                                 aucPinKey);
        KEY_DumpData("=========== Pin Check Data ===========\n", aucPinKey, 8);

        memset(aucMacKey, 0x30, sizeof(aucMacKey));
        Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                 KEYINDEX_GOLDENCARDSH_MAK,
                                 aucMacKey,
                                 aucMacKey);
        KEY_DumpData("=========== Mac Check Data ===========\n", aucMacKey, 8);
    }
#endif
    return ucResult;
}

unsigned char KEY_EncryptPIN_X98(const unsigned char *pInPinData,
                                 const unsigned char *pCardPan,
                                 unsigned char isDoubleDes,
                                 unsigned char *pOutPinData)
{
    unsigned char ucResult = SUCCESS_TRACKDATA;
    unsigned char aucPinBuf[17], aucHexPinBuf[8], aucPan[8];
    unsigned int uiPinLen = 0;

    uiPinLen = strlen((const char *)pInPinData);
    KEY_DumpData("=========== PIN ===========\n", (unsigned char *)pInPinData, uiPinLen);
    KEY_DumpData("=========== PAN ===========\n", (unsigned char *)pCardPan, 6);
    if(uiPinLen != 0)
    {
        memset(aucPinBuf, 0, sizeof(aucPinBuf));
        memset(aucPinBuf, 'F', sizeof(aucPinBuf) - 1);
        memset(aucHexPinBuf, 0, sizeof(aucHexPinBuf));

        aucPinBuf[0] = '0';
        aucPinBuf[1] = uiPinLen + '0';
        memcpy(&aucPinBuf[2], pInPinData, uiPinLen);
        asc_hex(aucHexPinBuf, 8, aucPinBuf, 16);
        KEY_DumpData("=========== PIN BLOCK ===========\n", aucHexPinBuf, 8);

        memset(aucPan, 0, sizeof(aucPan));
        aucPan[0] = aucPan[1] = 0x00;
        memcpy(&aucPan[2], pCardPan, 6);
        KEY_DumpData("=========== PAN BLOCK ===========\n", aucPan, 8);

        gz_xor(aucPan, aucHexPinBuf, 8);
        KEY_DumpData("=========== Xor Data ===========\n", aucHexPinBuf, 8);

        if(!isDoubleDes)
        {
#ifdef KEY_DEBUG_FLAG
            Uart_Printf("\n------------ PIN 单DES加密 ----------------\n");
#endif
            ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                                KEYINDEX_GOLDENCARDSH_PIK,
                                                aucHexPinBuf,
                                                pOutPinData);
        }
        else
        {
#ifdef KEY_DEBUG_FLAG
            Uart_Printf("\n------------ PIN 3DES加密 ----------------\n");
#endif
            ucResult = EncryptByDoubleKey(KEYARRAY_GOLDENCARDSH,
                                          KEYINDEX_GOLDENCARDSH_PIK,
                                          aucHexPinBuf,
                                          pOutPinData);
        }
    }
    KEY_DumpData("=========== PIN Encrypt ===========\n", pOutPinData, 8);
    return ucResult;
}

unsigned char KEY_CalcMAC_1(unsigned char *pucInData, unsigned short uiInLen, unsigned char *pucOutMAC)
{
    unsigned char ucResult;
    unsigned char aucHexData[8], aucAscData[17];
    unsigned char *pucPtr;
    unsigned short uiI, uiJ, uiLen;

    pucPtr = pucInData;
    KEY_DumpData("初始数据:\n", pucPtr, uiInLen);

    memset(aucHexData, 0, sizeof(aucHexData));
    for(uiI = 0; uiI < uiInLen; uiI += 8)
    {
        uiLen = min(8, pucInData+uiInLen-pucPtr);
        for(uiJ = 0; uiJ < uiLen; uiJ++)
        {
            aucHexData[uiJ] ^= *(pucPtr+uiJ);
        }
        pucPtr += 8;
    }

    memset(aucAscData, 0, sizeof(aucAscData));
    hex_asc(aucAscData, aucHexData, 16);
    aucAscData[16] = 0;

    memset(aucHexData, 0, sizeof(aucHexData));
    ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH, KEYINDEX_GOLDENCARDSH_MAK, aucAscData, aucHexData);
    KEY_DumpData("DES加密后数据:\n", aucHexData, 8);

    if(!ucResult)
    {
        for(uiI = 0; uiI < 8; uiI++)
            aucHexData[uiI] ^= aucAscData[8+uiI];
        ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH, KEYINDEX_GOLDENCARDSH_MAK, aucHexData, aucHexData);
        KEY_DumpData("再次DES加密后数据:\n", aucHexData, 8);
    }

    if(!ucResult)
    {
        memset(aucAscData, 0, sizeof(aucAscData));
        hex_asc(aucAscData, aucHexData, 16);
        KEY_DumpData("最后转换16位后数据:\n", aucAscData, 16);
    }

    if(!ucResult)
        memcpy(pucOutMAC, aucAscData, 8);

    return ucResult;
}

unsigned char KEY_CalcMAC_2(unsigned char *pucInData, unsigned short uiInLen, unsigned char *pucOutMAC)
{
    unsigned char ucResult;
    unsigned char aucHexData[9];
    unsigned char *pucPtr;
    unsigned short uiI, uiJ, uiLen;

    pucPtr = pucInData;
    KEY_DumpData("初始数据:\n", pucPtr, uiInLen);

    memset(aucHexData, 0, sizeof(aucHexData));
    for(uiI = 0; uiI < uiInLen; uiI += 8)
    {
        uiLen = min(8, pucInData+uiInLen-pucPtr);
        for(uiJ = 0; uiJ < uiLen; uiJ++)
        {
            aucHexData[uiJ] ^= *(pucPtr+uiJ);
        }
        pucPtr += 8;
    }

    ucResult = Encrypt8ByteSingleKey_47(KEYARRAY_GOLDENCARDSH,
                                        KEYINDEX_GOLDENCARDSH_MAK,
                                        aucHexData,
                                        aucHexData);

    KEY_DumpData("DES加密后数据:\n", aucHexData, 8);

    if(!ucResult)
        memcpy(pucOutMAC,aucHexData,8);

    return ucResult;
}

unsigned char KEY_CalcMAC(unsigned char *pucInData, unsigned short uiInLen, unsigned char *pucOutMAC)
{
//    return KEY_CalcMAC_1(pucInData, uiInLen, pucOutMAC);
    return KEY_CalcMAC_2(pucInData, uiInLen, pucOutMAC);
}
