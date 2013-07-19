#ifndef COMMONTOOLS_H
#define COMMONTOOLS_H
#include <QString>

void getAmount(unsigned char *pucOut,unsigned long ulAmount,unsigned char ucDecimalPos);
void getFormTime(unsigned char *pInTime, unsigned char *pOutTime);
void getFormDate(unsigned char *pInDate, unsigned char *pOutDate);
void CONV_IPStringULong(unsigned char *pucIP,unsigned long *ulIP);
QString FormIpAddressFromArray(unsigned long ulIPValue);

#endif
