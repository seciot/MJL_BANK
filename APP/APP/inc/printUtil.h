#ifndef PRINTUTIL_H
#define PRINTUTIL_H

typedef enum PRINTER_TYPE
{
    STYLUS,//针打
    THERMAL,//热敏
    LARGE_FORMAT,
    SMALL_FORMAT
}PrinterType;

typedef enum PRINT_FONT
{
    PF_NORMAL = 0,
    PF_SMALL = 1,
    PF_QUAD_WIDTH = 0x0C,
    PF_QUAD_HEIGHT = 0x0D,
    PF_QUAD_SIZE = 0x0E,
    PF_EQUAL_SIZE = 0x1B,
    PF_DOUBLE_WIDTH = 0x1C,
    PF_DOUBLE_HEIGHT = 0x1D,
    PF_DOUBLE_SIZE = 0x1E,
    PF_SMALL_EN = 0xF1//only for format printer
}PrintFont;

enum
{
    MAX_PRINT_MAX_FONT_LEN = 12,
    MAX_PRINT_LARGER_FONT_LEN = 16,
    MAX_PRINT_LARGE_FONT_LEN = 24,
    MAX_PRINT_LEN = 24, //32
    MAX_PRINT_MIN_FONT_LEN = 48
};

enum
{
    MAX_PRINT_FORMAT_FONT_LEN = 30,
    MAX_PRINT_FORMAT_LARGER_FONT_LEN = 22
};

typedef enum PRINT_LOCAL_TYPE
{
    PRINT_LEFT = 0,
    PRINT_MIDDLE,
    PRINT_RIGHT
}PrintLocalType;

void PRINT_CheckState(void);
void PRINT_xlinefeed(unsigned char ucLineNum);
void PRINT_xprint(unsigned char *pucBuffer, unsigned char ucFont);
void PRINT_GB2312_xprint(unsigned char *pucBuffer, unsigned char ucFont);
void PRINT_BMP(const char *ucPath);

unsigned char PRINT_UTIL_GetMaxLength(PrintFont a_font, PrinterType a_type);
void PRINT_UTIL_Info(const unsigned char* a_aucInfo, PrintLocalType a_infoLocalType, PrintFont a_font, PrinterType a_type);
void PRINT_UTIL_InfoChar(const char *a_acInfo, PrintLocalType a_infoLocalType, PrintFont a_font, PrinterType a_type);
void PRINT_UTIL_PrintLine(PrinterType a_type);
void PRINT_UTIL_PrintInfoLine(unsigned char *ptStr, PrinterType a_type);
void PRINT_UTIL_TransInfo(const unsigned char *a_aucInfoName,
                        const unsigned char *a_aucInfoParam,
                        const unsigned int a_uiInfoParamLen,
                        PrintLocalType a_infoLocalType,
                        PrintFont a_font,
                        PrinterType a_type);
unsigned char PrintGB2312Info(unsigned char *ptSrc,
                              unsigned int uiDataLen,
                              unsigned char ucPrinterType,
                              unsigned char ucLineNum);
void PRINT_UTIL_ChangeFont_GroupInfo(PrintFont _defFont,
                                     unsigned char *pCaption1, PrintFont _captionFont1, unsigned char *pText1, PrintFont _textFont1,
                                     unsigned char *pCaption2, PrintFont _captionFont2, unsigned char *pText2, PrintFont _textFont2);

#endif // PRINTUTIL_H
