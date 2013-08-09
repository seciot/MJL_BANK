#ifndef KEY_H
#define KEY_H

void KEY_DumpData(const char *pTitle, unsigned char *pData, short sDataLen);
unsigned char KEY_LoadDefaultKey(void);

unsigned char KEY_CalcMAC(unsigned char *pucInData,
                          unsigned short uiInLen,
                          unsigned char *pucOutMAC);

unsigned char KEY_StoreSingleNewKey(unsigned char *aucHexData, short iLen);

unsigned char KEY_EncryptPIN_X98(const unsigned char *pInPinData,
                                 const unsigned char *pCardPan,
                                 unsigned char isDoubleDes,
                                 unsigned char *pOutPinData);

#endif // KEY_H
