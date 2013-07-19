#ifndef _PINPAD_H
#define _PINPAD_H

typedef enum KEY_INDEX
{
    KEYARRAY_GOLDENCARDSH                   = 0,
    KEYINDEX_GOLDENCARDSH_MK                = 1,
    KEYINDEX_GOLDENCARDSH_MK2               = 2,
    KEYINDEX_GOLDENCARDSH_PIK               = 3,
    KEYINDEX_GOLDENCARDSH_PIK2              = 4,
    KEYINDEX_GOLDENCARDSH_MAK               = 5,
    KEYINDEX_GOLDENCARDSH_MAK2              = 6,
    KEYINDEX_MAX_NUM                        = 15
}KeyIndex;


void gz_xor(unsigned char *a, unsigned char *b, char lg);
unsigned char LoadSingleKeyUseSingleKey_42(
        unsigned char ucKeyArray,
        unsigned char ucDecryptKeyIndex,
        unsigned char ucDestKeyIndex,
        unsigned char *pucInData);

unsigned char Encrypt8ByteSingleKey_47(
        unsigned char ucKeyArray,
        unsigned char ucKeyIndex,
        unsigned char *pucInData,
        unsigned char *pucOutData);

unsigned char Crypt8ByteSingleKey_47(
        unsigned char ucKeyArray,
        unsigned char ucKeyIndex,
        unsigned char *pucInData,
        unsigned char *pucOutData);

unsigned char LoadKeyUseDoubleKey(
        unsigned char ucKeyArray,
        unsigned char ucDecryptKeyIndex,
        unsigned char ucDestKeyIndex,
        unsigned char *pucInData);

unsigned char EncryptByDoubleKey(
        unsigned char ucKeyArray,
        unsigned char ucKeyIndex,
        unsigned char *pucInData,
        unsigned char *pucOutData);

unsigned char CryptByDoubleKey(
        unsigned char ucKeyArray,
        unsigned char ucKeyIndex,
        unsigned char *pucInData,
        unsigned char *pucOutData);

#endif
