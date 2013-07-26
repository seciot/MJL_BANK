#ifndef TRANSONLINEPRO_H
#define TRANSONLINEPRO_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QString>
#include <QMessageBox>

#include "sand_lcd.h"
#include "comms.h"

class TransOnlinePro : public CommsSocket 
{
    Q_OBJECT
public:
    explicit TransOnlinePro(int onlineType,bool RetryFlag = true,QObject *parent = 0);
    ~TransOnlinePro();
    void process(){start(HighestPriority);}
private:
    unsigned char DownWK(void);                 //工作密钥下载 Work Key Down
    unsigned char DownEWK(void);
    unsigned char SettlementPro(void);          //结算 Settlement
    unsigned char ReversalPro(void);            //冲正

    unsigned char DepositPro(void);             //存款 Deposit
    unsigned char AdvancePro(void);             //取款 Advance
    unsigned char BalanceInquiryPro(void);      //余额查询 Balance Inquiry
    unsigned char PINChangePro(void);           //改密 PIN Change
    unsigned char TransferPro(void);            //卡卡转帐 P2P Transfer
public:
    int iOnlineType;
    bool offRetry;
protected:
    void run();
};
#endif
