#include <stdio.h>
#include "commontools.h"
#include <string.h>

extern "C"
{
    #include "ostools.h"
}

void getAmount(unsigned char *pucOut,unsigned long ulAmount,unsigned char ucDecimalPos)
{
    unsigned char   i;
    unsigned char   j;

    long_str(pucOut, 9, &ulAmount);//8
    j = ucDecimalPos;
    for (i = 8; j; i--, j--)//7
        pucOut[i + 1] = pucOut[i];
    if (ucDecimalPos)
    {
        pucOut[i + 1] ='.';
        pucOut[i + 1 + ucDecimalPos + 1] = 0;
    }
    for (j = 0; ((j < i) && (pucOut[j]=='0')); pucOut[j++]=' ');
}

void getFormTime(unsigned char *pInTime, unsigned char *pOutTime)
{
    unsigned char aucBuf[7];

    memset(aucBuf, 0, sizeof(aucBuf));

    bcd_asc(aucBuf, pInTime, 6);
    memcpy(&pOutTime[0], &aucBuf[0], 2);
    pOutTime[2] = ':';
    memcpy(&pOutTime[3], &aucBuf[2], 2);
    pOutTime[5] = ':';
    memcpy(&pOutTime[6], &aucBuf[4], 2);
}

void getFormDate(unsigned char *pInDate, unsigned char *pOutDate)
{
    unsigned char aucBuf[9];

    memset(aucBuf, 0, sizeof(aucBuf));
    bcd_asc(aucBuf, pInDate, 8);
#if 0
    //YYYYMMDD
    memcpy(&pOutDate[0], &aucBuf[0], 4);
    pOutDate[4] = '/';
    memcpy(&pOutDate[5], &aucBuf[4], 2);
    pOutDate[7] = '/';
    memcpy(&pOutDate[8], &aucBuf[6], 2);
#else
    //DDMMYYYY
    memcpy(&pOutDate[0], &aucBuf[6], 2);
    pOutDate[2] = '.';
    memcpy(&pOutDate[3], &aucBuf[4], 2);
    pOutDate[5] = '.';
    memcpy(&pOutDate[6], &aucBuf[0], 4);
#endif
}

// 字符串ip转ul
void CONV_IPStringULong(unsigned char *pucIP,unsigned long *ulIP)
{
    unsigned int IpDest[4];
    memset(IpDest,0,sizeof(IpDest));

    qDebug("IP aucBuf = %s",pucIP);
    sscanf((char*)pucIP,"%u.%u.%u.%u", IpDest, IpDest + 1, IpDest + 2, IpDest + 3);
    for(unsigned char i =0;i<sizeof(unsigned long);i++)
        pucIP[i] = IpDest[i];

    *ulIP=tab_long(pucIP,4);
}

// ul IP 转字符串
QString FormIpAddressFromArray(unsigned long ulIPValue)
{
    unsigned char aucBuf[4]={0};
    unsigned char pucIpAddress[20] = {0};
    unsigned char pucUnformatedIpAddress[4] = {0};

    memset(aucBuf, 0x00, sizeof(aucBuf));
    long_tab(aucBuf, 4, &ulIPValue);
    memcpy(pucUnformatedIpAddress, aucBuf, 4);

    /*IP_ADDRESS_FIELD_LEN for Unit Output Length*/
    sprintf((char*)pucIpAddress, "%*u.%*u.%*u.%*u", 3, pucUnformatedIpAddress[0],
                                                    3, pucUnformatedIpAddress[1],
                                                    3, pucUnformatedIpAddress[2],
                                                    3, pucUnformatedIpAddress[3]);
    return QString::fromAscii((const char *)pucIpAddress);
}
