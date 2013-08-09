#ifndef PARAMDATA_H
#define PARAMDATA_H
#include "appdef.h"

#define PARAM_TERMINALID_LEN            8       //终端号长度
#define PARAM_MERCHANTID_LEN            15      //商户号长度
#define PARAM_TERMINALNAME_LEN          8
#define PARAM_CURRENCY_CODE_LEN         3       //货币代码长度
#define PARAM_NII_LEN                   4       //NII长度

#define PARAM_ISSUER_ID_LEN             2       // issuer id len
#define PARAM_ISSUER_TEXT_LEN           20      // issuer text len
#define PARAM_ACQUIRER_ID_LEN           2       // acquirer id len
#define PARAM_CARDBIN_TEXT_LEN          20      // card bin text len
#define PARAM_CARDBIN_LEN               6       // card bin len
#define PARAM_MAX_BIN_QUANTITY          50      // max accepted card bin

#define PARAM_IP_LEN                    16      // max IP bin
#define PARAM_DOMAIN_LEN                50      // max domain len

#define PARAM_PRINT_BIG_FONT_LEN        24      // max print in one len
#define PARAM_PRINT_SMALL_FONT_LEN      48      // max print in one len

typedef enum COMMS_MODE
{
    PARAM_COMMMODE_MODEM    = 1,
    PARAM_COMMMODE_HDLC     = 2,
    PARAM_COMMMODE_GPRS     = 3,
    PARAM_COMMMODE_CDMA     = 4,
    PARAM_COMMMODE_WIFI     = 5
}CommsMode;

typedef enum HOST_TYPE
{
    HOST_IP,
    HOST_DOMAINNAME
}HostType;

typedef struct CASH_ADVANCE
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}CashAdvance;

typedef struct CASH_DEPOSIT
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}CashDeposit;

typedef struct P2P_TRANSFER
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}P2P;

typedef struct BALANCE_INQUIRY
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}BalanceInquiry;

typedef struct TRANS_VOID
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}TransVoid;

typedef struct ADJUST
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}Adjust;

typedef struct PIN_CHANGE
{
    bool  TRANS_ENABLE;
    bool  PIN_ENABLE;
    bool  MANUAL_ENABLE;
}PinChange;

typedef struct CONSTANT_PARAM
{
    unsigned char   aucTerminalID[PARAM_TERMINALID_LEN + 1];
    unsigned char   aucMerchantID[PARAM_MERCHANTID_LEN + 1];

    //currency
    unsigned char   aucCurrencySign[PARAM_CURRENCY_CODE_LEN + 1];       //huo'bi
    unsigned short  usCurrencyId;

    //common communicate parameter
    CommsMode       commMode;
    HostType        hostType;
    WIRELESS_PARAM  GSM;

    unsigned char   aucNii[PARAM_NII_LEN + 1];

    unsigned char   ucHostIndex;
    unsigned long   ulHostIP[3];
    unsigned int    uiHostPort[3];
    unsigned char   aucDomainName[PARAM_DOMAIN_LEN+1];

    unsigned long   ulMinAmount;            //最小金额
    unsigned long   ulMaxAmount;            //最大金额
    unsigned int    uiMaxTotalNb;           //最大交易笔数

    // timeout
    unsigned int    TIMEOUT_PAPERTEAR;         //打印撕纸等待时间
    unsigned int    TIMEOUT_ERRMSG;            //报错窗口自动关闭时间

    unsigned int    TIMEOUT_HOST;              //主机超时
    unsigned int    TIMEOUT_UI;                //界面超时

    unsigned int    connectionRetry;           //连接重试次数

    // flag
    bool flagSettleReceipt;
    bool flagSelectAccount;

    CashAdvance     advance;
    CashDeposit     deposit;
    P2P             p2p;
    BalanceInquiry  balance;
    TransVoid       transvoid;
    Adjust          adjust;
    PinChange       pinchange;

    unsigned char   aucReceiptLine1[PARAM_PRINT_BIG_FONT_LEN + 1];
    unsigned char   aucReceiptLine2[PARAM_PRINT_BIG_FONT_LEN + 1];
    unsigned char   aucReceiptLine3[PARAM_PRINT_BIG_FONT_LEN + 1];
    unsigned char   aucReceiptLine4[PARAM_PRINT_BIG_FONT_LEN + 1];
    unsigned char   aucAgreeLine1[PARAM_PRINT_SMALL_FONT_LEN + 1];
    unsigned char   aucAgreeLine2[PARAM_PRINT_SMALL_FONT_LEN + 1];
    unsigned char   aucAgreeLine3[PARAM_PRINT_SMALL_FONT_LEN + 1];
    unsigned char   aucAgreeLine4[PARAM_PRINT_SMALL_FONT_LEN + 1];
    unsigned char   aucCompanyCopyLabel[PARAM_PRINT_BIG_FONT_LEN +1];
}ConstantParam;

typedef struct CHANGE_PARAM
{
    /*========== test ======*/
    bool    simTrans;
    bool    simCard;
    /*====================*/

    bool            boolMacKeyFlag;         //用于标注MAC KEY是否有下发
    unsigned long   ulTransNum;             //交易流水号(注:长度6位)     结算后置1
    unsigned long   ulBatchNumber;          //批次号                   结算后累加1
    unsigned long   ulInvoiceNum;           //(注:长度6位)              结算后?

    bool            boolCashierLogonFlag;
}ChangeParam;

typedef struct BIN_STATUS
{
    bool            status[PARAM_MAX_BIN_QUANTITY];
    unsigned int    uiQuantity;
}BinStatus;

typedef struct BIN_DATA
{
    unsigned char   aucIssuerText[PARAM_ISSUER_TEXT_LEN];
    unsigned int    uiIssuerID;
    unsigned int    uiAcquirerID;
    HostType        hostType;
    unsigned long   ulHostIP[2];
    unsigned int    uiHostPort[2];
    unsigned char   aucDomainName[PARAM_DOMAIN_LEN];

    unsigned char   aucCardBINText[PARAM_CARDBIN_TEXT_LEN];
    unsigned long   ulCardBINLow;
    unsigned long   ulCardBINHigh;
}BinData;

#endif // PARAMDATA_H
