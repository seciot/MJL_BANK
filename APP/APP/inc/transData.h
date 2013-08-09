#ifndef TRANSDATA_H
#define TRANSDATA_H

#define TRANS_ISO1LEN               76
#define TRANS_ISO2LEN               37
#define TRANS_ISO3LEN               104

#define TRANS_DATE_LEN              4
#define TRANS_TIME_LEN              3
#define TRANS_AUTHCODE_LEN          6
#define TRANS_REFNUM_LEN            12
#define TRANS_AMOUNT_LEN            12

#define TRANS_PINDATALEN            8

#define TRANS_ACCLEN                19
#define TRANS_CASHIERLEN            2

//:- 交易transinfo统计
#define TRANS_SAVEMAXNB                 500
#define TRANS_SAVEMINNB                 300

/*cashier */
#define CASH_MAXSUPERNO                 2
#define CASH_MAXSUPERPASSWDLEN          6
#define CASH_SUPERPASSWDLEN             8

#define CASH_MAXCASHIER                 25
#define CASH_CASHIERNOLEN               2
#define CASH_CASHIERPASSLEN             4
#define CASH_SYSCASHIERNOLEN            2
#define CASH_SYSCASHIERPASSLEN          6
#define CASH_MAXTime                    4
#define CASH_MINTime                    4

/*
银行POS：
取钱
存钱
撤销
余额查询
转账
修改密码
签到
公共事业账单付费
*/

//trans type index
typedef enum TRANS_MODE
{
    TransMode_DownWK,           //签到(工作密钥)
    TransMode_DownEWK,          //签到(传输密钥)
    TransMode_Settle,           //结算(一次)
    TransMode_Settle2,          //结算(二次)
    TransMode_BatchUpload,      //批上送
    TransMode_BalanceInquiry,   //查余
    TransMode_CashDeposit,      //存款
    TransMode_DepositVoid,      //存款撤销
    TransMode_DepositAdjust,    //存款调整
    TransMode_CashAdvance,      //取款
    TransMode_AdvanceVoid,      //取款撤销
    TransMode_AdvanceAdjust,    //取款调整
    TransMode_CardTransfer,     //转账
    TransMode_PINChange,        //改密
    TransMode_MaxIndex
}TransMode;

//input type index
typedef enum INPUT_MODE_INDEX
{
    INPUTMODE_Manual,
    INPUTMODE_MagneticStripe2,
    INPUTMODE_ICCRead,
    INPUTMODE_ContactlessEMV,
    INPUTMODE_MagneticStripe3,
    INPUTMODE_EMV_Fallback,
    INPUTMODE_MagneticStripe1,
    INPUTMODE_ContactlessMSD,
    INPUTMODE_MaxIndex
}InputModeIndex;

typedef enum ACCOUNT_TYPE
{
    defaultAcc  = 0x30,
    savingsAcc  = 0x31,
    checkAcc    = 0x32,
    creditAcc   = 0x33
}AccType;

typedef enum USER_TYPE
{
    typeAdmin       = 0,
    typeManager     = 1,
    typeCashier     = 2
}UserType;

typedef struct MAG_DATA
{
    unsigned char   ucISO1Status;
    unsigned short  uiISO1Len;
    unsigned char   aucISO1[TRANS_ISO1LEN];
    unsigned char   ucISO2Status;
    unsigned short  uiISO2Len;
    unsigned char   aucISO2[TRANS_ISO2LEN];
    unsigned char   ucISO3Status;
    unsigned short  uiISO3Len;
    unsigned char   aucISO3[TRANS_ISO3LEN];
}MagData;

/* Extra Transaction Information */
typedef struct EXTRA_TRANS
{
    MagData         magData;
    unsigned char   aucPINData[TRANS_PINDATALEN];
    unsigned char   aucChangePINData[TRANS_PINDATALEN];
    unsigned char   aucCardPan_UnAssign[6 + 1];
}ExtraTrans;

typedef struct NORMAL_TRANS
{
    unsigned char   cardType;                               //卡类型
    TransMode       transType;                              //交易类型
    InputModeIndex  inputMode;                              //输入方式
    AccType         accType;                                //账户类型
    AccType         toAccType;                              //转出账户类型

    unsigned char   aucSourceAcc[TRANS_ACCLEN + 1];         //账号
    unsigned char   ucSourceAccLen;                         //账号长度
    unsigned char   ucExpiredDate[TRANS_DATE_LEN + 1];      //账号有效期
    unsigned char   aucToAcc[TRANS_ACCLEN + 1];             //接收账号
    unsigned char   ucToAccLen;                             //接收账号长度
    bool            isInputPin;                             //主账号是否带密码

    unsigned long   ulBatchNumber;                          //批次号
    unsigned long   ulTraceNumber;                          //POS流水号
    unsigned char   aucAuthCode[TRANS_AUTHCODE_LEN + 1];    //授权号
    unsigned char   aucRefNum[TRANS_REFNUM_LEN + 1];        //参考号

    unsigned long   ulAmount;                               //消费金额
    unsigned long   ulAdjustAmount;                         //调整金额
    unsigned long   ulBalance;                              //余额
    unsigned char   aucDate[TRANS_DATE_LEN + 1];
    unsigned char   aucTime[TRANS_TIME_LEN + 1];

    unsigned char   aucCashier[TRANS_CASHIERLEN + 1];

    //void
    unsigned long   ulOrgTraceNumber;                           //原POS流水号
    unsigned char   aucOrgRefNum[TRANS_REFNUM_LEN + 1];         //原参考号
}NormalTrans;

typedef struct
{
    unsigned int    uiNb;           //笔数
    unsigned long   ulAmount;       //金额
}Total;

typedef enum _TotalType
{
    TotalDeposit,       //存款
    TotalDepositVoid,   //存款撤销
    TotalDepositAdjust, //存款调整
    TotalAdvance,       //取款
    TotalAdvanceVoid,   //取款撤销
    TotalAdvanceAdjust, //取款调整
    TotalTransfer,      //转账
    TotalVoid,          //撤销
    TotalMaxIndex
}TotalType;

typedef enum _TotalBusinessType
{
    TotalCreditBusiness,            //贷记业务
    TotalDebitBusiness,             //借记业务
    TotalBusinessMaxIndex
}TotalBusinessType;

//统计信息
typedef struct
{
    unsigned int    uiTotalNb;                  //总笔数
    Total           total[TotalMaxIndex];
    Total           totalBusiness[4][TotalBusinessMaxIndex];
}TRANSTOTAL;

typedef struct
{
    unsigned long   ulLastTransNumber;                  //上笔流水
    unsigned long   ulTransNumber;                      //交易流水

    unsigned short  auiTransIndex[TRANS_SAVEMAXNB];     //交易是否存在标志，如果存在设置交易标志种类
    TRANSTOTAL      TransTotal;                         //当前批次交易统计
    TRANSTOTAL      LastTransTotal;                     //上一批次交易统计

    unsigned long   ulLastBatchNumber;                      //上一批次号
    unsigned char   aucLastCashierNo[TRANS_CASHIERLEN];     //上一批次柜员号
    unsigned char   ucLasSettleTime[TRANS_TIME_LEN];        //上一批次结算时间
    unsigned char   ucLasSettleData[TRANS_DATE_LEN];        //上一批次结算日期
    unsigned char   ucLastSettleFlag;                       //上一批次结算成功标志
}TRANSINFO;

// 柜员管理
typedef struct
{
    unsigned char   aucSuperNo[CASH_MAXSUPERNO];
    unsigned char   aucSuperPSW[9];
    unsigned char   ucSYSCashierExitFlag;
    unsigned char   aucSYSCashierNo[CASH_SYSCASHIERNOLEN];
    unsigned char   aucSYSCashierPass[CASH_SYSCASHIERPASSLEN];
    unsigned char   aucCashierNo[CASH_MAXCASHIER][CASH_CASHIERNOLEN];
    unsigned char   aucCashierPass[CASH_MAXCASHIER][CASH_CASHIERPASSLEN];
    unsigned char   aucSuperPassWord[7];
    unsigned char   aucSafePSW[7];
}SYSCASHIER;

typedef struct
{
    unsigned short  ucErrorExtCode;
//    unsigned char   ucTerminalType;
    unsigned char   bReversalFlag;              //:- 开始交易时设置的冲正标志
}RUNDATA;//临时使用.

#endif // TRANSDATA_H
