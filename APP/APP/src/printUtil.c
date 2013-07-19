#include "osmain.h"
#include "printUtil.h"
#include "printer.h"

void PRINT_CheckState(void)
{
    while(Os__check_printBuf() != 1);
}

void PRINT_ChangeFont_xprint(unsigned char _defaultFont, unsigned char *pInData, unsigned int uiInDataLen,
                             unsigned char _changeFont1, int _changeBegin1, int _changeEnd1,
                             unsigned char _changeFont2, int _changeBegin2, int _changeEnd2,
                             unsigned char _changeFont3, int _changeBegin3, int _changeEnd3,
                             unsigned char _changeFont4, int _changeBegin4, int _changeEnd4)
{
    VARIAPRINTER printBuf;

    memset(&printBuf, 0, sizeof(printBuf));

    //计算BUF长度
    printBuf.sum = uiInDataLen;
    //分配内存空间
    printBuf.Text = (unsigned char*)malloc(printBuf.sum);
    printBuf.ucGBFont = (unsigned char*)malloc(printBuf.sum);
    printBuf.ucASCIIFont = (unsigned char*)malloc(printBuf.sum);
    //数据初始化
    memset(printBuf.Text, 0, printBuf.sum);
    memset(printBuf.ucGBFont, _defaultFont, printBuf.sum);
    memset(printBuf.ucASCIIFont, 0x31, printBuf.sum);

    memcpy(printBuf.Text, pInData, printBuf.sum);
    if(_changeEnd1 != 0)
        memset(&printBuf.ucGBFont[_changeBegin1], _changeFont1, _changeEnd1);

    if(_changeEnd2 != 0)
        memset(&printBuf.ucGBFont[_changeBegin2], _changeFont2, _changeEnd2);

    if(_changeEnd3 != 0)
        memset(&printBuf.ucGBFont[_changeBegin3], _changeFont3, _changeEnd3);

    if(_changeEnd4 != 0)
        memset(&printBuf.ucGBFont[_changeBegin4], _changeFont4, _changeEnd4);

    Os__variable_printf(&printBuf);

    free(printBuf.Text);
    free(printBuf.ucGBFont);
    free(printBuf.ucASCIIFont);
}

void PRINT_GB2312_xprint(unsigned char *pucBuffer, unsigned char ucFont)
{
    Os__GB2312_xprint(pucBuffer, ucFont);
}

void PRINT_xprint(unsigned char *pucBuffer, unsigned char ucFont)
{
    ucFont = 0;
    Os__xprint(pucBuffer);
}

void PRINT_xlinefeed(unsigned char ucLineNum)
{
    Os__linefeed(ucLineNum);
}

void PRINT_BMP(const char *ucPath)
{
    Os__sign_xprint((unsigned char *)ucPath);
}

/*************************************************
  Function:		unsigned char PRINT_UTIL_GetMaxLength(PrintFont a_font)
  Description:	get maximum length in one line of printer by font
  Param:        PrintFont a_font:				font of printer
  Return:		void
  remake:
  history:
    <author>  	<time>			<version >   		<desc>
        yyf		2007.07.02		1.0					build this module
*************************************************/
unsigned char PRINT_UTIL_GetMaxLength(PrintFont a_font, PrinterType a_type)
{
    unsigned char ucPrintChar = 0;
    switch(a_type)
    {
    case STYLUS:
        ucPrintChar = MAX_PRINT_LARGE_FONT_LEN;
        break;
    case THERMAL:
        switch(a_font)
        {
        case PF_SMALL:
        case PF_DOUBLE_HEIGHT:
            ucPrintChar = MAX_PRINT_MIN_FONT_LEN;
            break;
        case PF_NORMAL:
            ucPrintChar = MAX_PRINT_LEN;
            break;
        case PF_QUAD_WIDTH:
            ucPrintChar = MAX_PRINT_LARGER_FONT_LEN;
            break;
        case PF_QUAD_SIZE:
        case PF_EQUAL_SIZE:
            ucPrintChar = MAX_PRINT_MAX_FONT_LEN;
            break;
        default:
            ucPrintChar = MAX_PRINT_LARGE_FONT_LEN;
            break;
        }
        break;
    case LARGE_FORMAT:
    case SMALL_FORMAT:
        switch(a_font)
        {
        case PF_SMALL:
        case PF_SMALL_EN:
            ucPrintChar = MAX_PRINT_FORMAT_FONT_LEN;
            break;
        default:
            ucPrintChar = MAX_PRINT_FORMAT_LARGER_FONT_LEN;
            break;
        }
        break;
    }
    return ucPrintChar;
}

/*************************************************
  Function:		void PRINT_UTIL_InfoChar(const char *a_acInfo, PrintLocalType a_infoLocalType, PrintFont a_font)
  Description:	print information
  Param:        const char *a_acInfo:			information need print
                PrintLocalType a_infoLocalType:	print location
                PrintFont a_font:				font of printer
  Return:		void
  remake:
  history:
    <author>  	<time>			<version >   		<desc>
        yyf		2007.07.02		1.0					build this module
*************************************************/
void PRINT_UTIL_Info(const unsigned char* a_aucInfo, PrintLocalType a_infoLocalType, PrintFont a_font, PrinterType a_type)
{
    unsigned char aucPrintBuf[MAX_PRINT_MIN_FONT_LEN + 1];
    unsigned char ucPrintChar;
    unsigned int uiPrintLen = strlen((const char*)a_aucInfo);
    memset(aucPrintBuf, 0, sizeof(aucPrintBuf));

    if(a_type != LARGE_FORMAT
        && a_type != SMALL_FORMAT
        && a_font == PF_SMALL_EN)
    {
        a_font = PF_SMALL;
    }
    ucPrintChar = PRINT_UTIL_GetMaxLength(a_font, a_type);

    switch(a_infoLocalType)
    {
    case PRINT_LEFT:
        strncpy((char *)aucPrintBuf, (const char*)a_aucInfo, ucPrintChar);
        break;
    case PRINT_MIDDLE:
        if(uiPrintLen <= ucPrintChar)
        {
            memset(aucPrintBuf, ' ', (ucPrintChar + 1 - uiPrintLen) / 2);
            strcat((char *)aucPrintBuf, (const char*)a_aucInfo);
        }
        else
        {
            strncpy((char *)aucPrintBuf, (const char*)&a_aucInfo[(uiPrintLen - ucPrintChar) / 2], ucPrintChar);
        }
        break;
    case PRINT_RIGHT:
        if(uiPrintLen <= ucPrintChar)
        {
            memset(aucPrintBuf, ' ', ucPrintChar - uiPrintLen);
            strcat((char *)aucPrintBuf, (const char*)a_aucInfo);
        }
        else
        {
            strncpy((char *)aucPrintBuf, (const char*)&a_aucInfo[uiPrintLen - ucPrintChar], ucPrintChar);
        }
        break;
    default:
        break;
    }

    if(PF_SMALL_EN == a_font)
        PRINT_xprint(aucPrintBuf, a_font);
    else
        PRINT_GB2312_xprint(aucPrintBuf, a_font);
}

/*************************************************
  Function:		void PRINT_UTIL_InfoChar(const char *a_acInfo, PrintLocalType a_infoLocalType, PrintFont a_font)
  Description:	print information
  Param:        const char *a_acInfo:			information need print
                PrintLocalType a_infoLocalType:	print location
                PrintFont a_font:				font of printer
  Return:		void
  remake:
  history:
    <author>  	<time>			<version >   		<desc>
        yyf		2007.07.02		1.0					build this module
*************************************************/
void PRINT_UTIL_InfoChar(const char *a_acInfo, PrintLocalType a_infoLocalType, PrintFont a_font, PrinterType a_type)
{
    PRINT_UTIL_Info((const unsigned char*)a_acInfo, a_infoLocalType, a_font, a_type);
}

/*************************************************
  Function:		void PRINT_UTIL_InfoChar(const char *a_acInfo, PrintLocalType a_infoLocalType, PrintFont a_font)
  Description:	print information
  Param:        const unsigned char *a_aucInfoName: information title
                const unsigned char *a_aucInfoParam:information data
                const unsigned int a_uiInfoParamLen:information data length
                PrintLocalType a_infoLocalType:     location type(don't support PRINT_MIDDLE)
                PrintFont a_font:					font of printer
  Return:		void
  remake:   	if title length add data length greater than one line length,
                the function will print them in two line.
  history:
    <author>  	<time>			<version >   		<desc>
        yyf		2007.07.02		1.0					build this module
*************************************************/
void PRINT_UTIL_TransInfo(const unsigned char *a_aucInfoName,
                        const unsigned char *a_aucInfoParam,
                        const unsigned int a_uiInfoParamLen,
                        PrintLocalType a_infoLocalType,
                        PrintFont a_font,
                        PrinterType a_type)
{
    unsigned char aucPrintBuf[MAX_PRINT_MIN_FONT_LEN + 1];
    unsigned int uiLen;
    unsigned char ucPrintChar;

    if(a_type != LARGE_FORMAT
        && a_type != SMALL_FORMAT
        && a_font == PF_SMALL_EN)
    {
        a_font = PF_SMALL;
    }

    ucPrintChar = PRINT_UTIL_GetMaxLength(a_font, a_type);

    memset(aucPrintBuf, ' ', ucPrintChar);
    aucPrintBuf[ucPrintChar] = 0;
    uiLen = strlen((char *)a_aucInfoName);
    memcpy(aucPrintBuf, a_aucInfoName, uiLen);

    if((uiLen + a_uiInfoParamLen) > ucPrintChar)
    {
        PRINT_GB2312_xprint(aucPrintBuf, a_font);
        memset(aucPrintBuf, ' ', ucPrintChar);
        aucPrintBuf[ucPrintChar] = 0;
        if(a_infoLocalType == ucPrintChar)
        {
            if(a_uiInfoParamLen > ucPrintChar)
            {
                memcpy(aucPrintBuf, a_aucInfoParam, ucPrintChar);
            }
            else
            {
                memcpy(aucPrintBuf, a_aucInfoParam, a_uiInfoParamLen);
            }
        }
        else
        {
            if(a_uiInfoParamLen > ucPrintChar)
            {
                memcpy(aucPrintBuf, a_aucInfoParam, ucPrintChar);
            }
            else
            {
                memcpy(&aucPrintBuf[ucPrintChar - a_uiInfoParamLen], a_aucInfoParam, a_uiInfoParamLen);
            }
        }
        if(PF_SMALL_EN == a_font)
        {
            PRINT_xprint(aucPrintBuf, a_font);
        }
        else
        {
            PRINT_GB2312_xprint(aucPrintBuf, a_font);
        }
    }
    else
    {
        if(a_infoLocalType == PRINT_LEFT)
        {
            memcpy(&aucPrintBuf[uiLen], a_aucInfoParam, a_uiInfoParamLen);
        }
        else if(a_infoLocalType == PRINT_MIDDLE)
        {
            memcpy(&aucPrintBuf[ucPrintChar/2], a_aucInfoParam, a_uiInfoParamLen);
        }
        else
        {
            memcpy(&aucPrintBuf[ucPrintChar - a_uiInfoParamLen], a_aucInfoParam, a_uiInfoParamLen);
        }
        if(PF_SMALL_EN == a_font)
        {
            PRINT_xprint(aucPrintBuf, a_font);
        }
        else
        {
            PRINT_GB2312_xprint(aucPrintBuf, a_font);
        }
    }
}

/*************************************************
  Function:		void PRINT_UTIL_PrintLine(void)
  Description:	print a line
  Param:        void
  Return:		void
  remake:
  history:
    <author>  	<time>			<version >   		<desc>
        yyf		2007.07.09		1.0					build this module
*************************************************/
void PRINT_UTIL_PrintLine(PrinterType a_type)
{
    unsigned char aucPrintBuf[MAX_PRINT_MIN_FONT_LEN + 1];
    memset(aucPrintBuf, '-', MAX_PRINT_MIN_FONT_LEN);
    aucPrintBuf[MAX_PRINT_MIN_FONT_LEN] = 0;
    PRINT_UTIL_Info(aucPrintBuf, PRINT_LEFT, PF_SMALL, a_type);
}

void PRINT_UTIL_PrintInfoLine(unsigned char *ptStr, PrinterType a_type)
{
    unsigned char aucPrintBuf[MAX_PRINT_MIN_FONT_LEN + 1];
    memset(aucPrintBuf, '-', MAX_PRINT_MIN_FONT_LEN);
    memcpy(aucPrintBuf, ptStr, strlen((const char*)ptStr));
    aucPrintBuf[MAX_PRINT_MIN_FONT_LEN] = 0;
    PRINT_UTIL_Info(aucPrintBuf, PRINT_LEFT, PF_SMALL, a_type);
}

unsigned char PrintGB2312Info(unsigned char *ptSrc,
                              unsigned int uiDataLen,
                              unsigned char ucPrinterType,
                              unsigned char ucLineNum)
{
    unsigned char aucPrintBuf[64+1];
    unsigned int  uiI;
    unsigned int  uiPrintLen,uiLen,uiNum;
    unsigned char ucASCNum;
    unsigned int  uiRowNum,uiRowLen;
    unsigned char ucFont;

    if (ucPrinterType == 0x03)      //套打
    {
        uiRowLen = 30;
        ucFont = 0x01;
    }
    else if (ucPrinterType == 0x01) //热敏
    {
        uiRowLen = 24;
        ucFont = 0x1D;      //previous 0x1E;
    }
    else                            //针打
    {
        uiRowLen = 24;
        ucFont = 0x00;
    }

    for (uiPrintLen = 0; uiPrintLen < uiDataLen; uiPrintLen++)
    {
        if (ptSrc[uiPrintLen] == 0)
            break;
    }

    //现在统一规定，打印两行，多余的不打印，不足两行的第二行打印空格,无需打印的两行空格
    if ((uiDataLen == 0) || ((uiDataLen == 1) && (*ptSrc >= 0x80))
    || (uiPrintLen == 0))
    {
        PRINT_GB2312_xprint((unsigned char *) " ", ucFont);
        PRINT_xlinefeed(ucLineNum * 1/2);
        PRINT_GB2312_xprint((unsigned char *) " ", ucFont);
        return 0;
    }

    uiLen = 0;
    uiRowNum = 0;
    do
    {
        for (uiNum = 0; ((uiLen + uiNum) < uiPrintLen) && (uiNum < uiRowLen); uiNum++);

        if (uiNum >= uiRowLen)
        {
            uiI = 0;
            ucASCNum = 0;
            while (uiI < uiNum)
            {
                if (ptSrc[uiLen + uiI] < 0x80)
                {
                    ucASCNum++;
                    uiI++;
                }
                else
                {
                    uiI += 3;   //2
                }
            }
            if ((ucASCNum % 2) != (uiRowLen % 2))
            {
                uiNum--;
            }
        }

        memset(aucPrintBuf, 0, sizeof(aucPrintBuf));
        memcpy(aucPrintBuf, &ptSrc[uiLen], uiNum);

        PRINT_GB2312_xprint(aucPrintBuf, ucFont);
        uiRowNum++;
        uiLen += uiNum;
        //现在统一规定，打印两行，多余的不打印，不足两行的第二行打印空格,无需打印的两行空格
        if(uiRowNum == 0x02)
            break;
    }while(uiLen < uiPrintLen);

    if (uiRowNum == 0x00)
    {
        PRINT_GB2312_xprint((unsigned char *) " ", ucFont);
        PRINT_xlinefeed(ucLineNum * 1/2);
        PRINT_GB2312_xprint((unsigned char *) " ", ucFont);
    }
    if (uiRowNum == 0x01 && ucPrinterType == 0x03)
    {
        PRINT_xlinefeed(ucLineNum * 1/2);
        PRINT_GB2312_xprint((unsigned char *) " ", ucFont);
    }
    return uiRowNum;
}

void PRINT_UTIL_ChangeFont_GroupInfo(PrintFont _defFont,
                                     unsigned char *pCaption1, PrintFont _captionFont1, unsigned char *pText1, PrintFont _textFont1,
                                     unsigned char *pCaption2, PrintFont _captionFont2, unsigned char *pText2, PrintFont _textFont2)
{
    unsigned char aucPrintBuf[MAX_PRINT_MIN_FONT_LEN + 1];
    unsigned char ucDefFontMaxLen = 0;
    unsigned int uiCaption1Len = 0, uiText1Len = 0;
    unsigned int uiCaption2Len = 0, uiText2Len = 0;
    unsigned int uiOffset = 0;

    ucDefFontMaxLen = PRINT_UTIL_GetMaxLength(_defFont, THERMAL);

    if(NULL != pCaption1)
        uiCaption1Len = strlen((char *)pCaption1);
    if(NULL != pText1)
        uiText1Len = strlen((char *)pText1);
    if(NULL != pCaption2)
        uiCaption2Len = strlen((char *)pCaption2);
    if(NULL != pText2)
        uiText2Len = strlen((char *)pText2);

    memset(aucPrintBuf, ' ', ucDefFontMaxLen);
    aucPrintBuf[ucDefFontMaxLen] = 0;

    uiOffset = 0;
    if(NULL != pCaption1)
    {
        memcpy(&aucPrintBuf[uiOffset], pCaption1, uiCaption1Len);
        uiOffset += uiCaption1Len;
    }
    if(NULL != pText1)
    {
        memcpy(&aucPrintBuf[uiOffset], pText1, uiText1Len);

    }
    uiOffset = ucDefFontMaxLen/2;
    if(NULL != pCaption2)
    {
        memcpy(&aucPrintBuf[uiOffset], pCaption2, uiCaption2Len);
        uiOffset += uiCaption2Len;
    }
    if(NULL != pText2)
    {
        memcpy(&aucPrintBuf[uiOffset], pText2, uiText2Len);
        uiOffset += uiText2Len;
    }
    PRINT_ChangeFont_xprint(_defFont, aucPrintBuf, uiOffset,
                            _captionFont1,  0, uiCaption1Len,
                            _textFont1,     uiCaption1Len, uiText1Len,
                            _captionFont2,  (uiOffset - uiText2Len - uiCaption2Len), uiCaption2Len,
                            _textFont2,     ucDefFontMaxLen/2 + uiCaption2Len, uiText2Len);
}
