#ifndef PARAMDATA_H
#define PARAMDATA_H
#include "appdef.h"

#define PARAM_TERMINALID_LEN            8       //终端号长度
#define PARAM_MERCHANTID_LEN            15      //商户号长度
#define PARAM_TERMINALNAME_LEN          8
#define PARAM_CURRENCY_CODE_LEN         3       //货币代码长度
#define PARAM_NII_LEN                   4       //NII长度

typedef enum COMMS_MODE
{
    PARAM_COMMMODE_MODEM    = 1,
    PARAM_COMMMODE_HDLC     = 2,
    PARAM_COMMMODE_GPRS     = 3,
    PARAM_COMMMODE_CDMA     = 4
}CommsMode;

typedef enum HOST_TYPE
{
    HOST_IP,
    HOST_DOMAINNAME
}HostType;

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

    unsigned long   ulMinAmount;            //最小金额
    unsigned long   ulMaxAmount;            //最大金额
    unsigned int    uiMaxTotalNb;           //:- 最大交易笔数
}ConstantParam;

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

typedef struct CHANGE_PARAM
{
    /*========== test ======*/
    bool    simTrans;
    bool    simCard;
    /*====================*/
    CashAdvance advance;
    CashDeposit deposit;
    P2P p2p;
    BalanceInquiry balance;
    TransVoid transvoid;
    Adjust adjust;
    PinChange pinchange;

    bool            boolMacKeyFlag;           //用于标注MAC KEY是否有下发
    unsigned long   ulTransNum;             //交易流水号(注:长度6位)
    unsigned long   ulBatchNumber;          //批次号
    unsigned long   ulInvoiceNum;           //(注:长度6位)

    bool            boolCashierLogonFlag;
    unsigned char   ucCashierLogonIndex;


    // timeout
    unsigned int TIMEOUT_PAPERTEAR;   // 打印撕纸等待时间
    unsigned int TIMEOUT_ERRMSG;         // 报错窗口自动关闭时间

    unsigned int TIMEOUT_HOST;    // 主机超时
    unsigned int TIMEOUT_UI;    // 界面超时

    unsigned int connectionRetry;     // 连接重试次数
}ChangeParam;

#endif // PARAMDATA_H
