#include "comms.h"
#include "sand_lcd.h"
#include "iso8583.h"
#include "global.h"
#include "xdata.h"

extern "C"
{
#include "sand_network.h"
#include "key.h"
}

#define OUT_DATA
#define ERR_STOPCOMM        0xFD
#define COMMS_CHECK_GPRS_TIMEOUT    100 //1000

unsigned char SendReceive_Test(unsigned char *pucOutData,unsigned short *puiOutLen)
{
    unsigned char aucBuf[512];
    int iBufLen = 0;

    memset(aucBuf, 0, sizeof(aucBuf));
    switch(NormalTransData.transType)
    {
    case TransMode_DownWK:           //签到(工作密钥)
    case TransMode_DownEWK:          //签到(传输密钥)
        iBufLen = sprintf((char *)aucBuf, "%s", (char *)
                          "081020380100028000049200000000051611190702000130304D42425230313031001800168BF66021B899F683FFFFFFFFFFFFFFFF");
        break;
    case TransMode_CashDeposit:      //存钱
        iBufLen = sprintf((char *)aucBuf, "%s", (char *)
                          "0210703801000E808000187060010000521705122100000000004567890000061319250711000130303030313639333435323237393832313230304D42425230313031303530");
        break;
    case TransMode_CashAdvance:      //取钱
        iBufLen = sprintf((char*)aucBuf, "%s", (char *)
                          "0210703801000E808000187060010000521705120100000000000000010000081730570702000130303030313639333337313830303439373630304D42425230313031303530");
        break;
    case TransMode_AdvanceVoid:         //撤销
        break;
    case TransMode_DepositVoid:         //撤销
        break;
    case TransMode_BalanceInquiry:   //查余
        iBufLen = sprintf((char*)aucBuf, "%s", (char *)
                          "0110703801000E808002187060010000521705123100000000004979970000031236410704000130303030313639333431363538373137323230304D4242523031303130353000124244542020343937392E3937");
        break;
    case TransMode_CardTransfer:     //转账
        iBufLen = sprintf((char*)aucBuf, "%s", (char *)
                          "0210703801000E808000187060010000521705126100000000000000010000041212290715000130303030313639333438313438373138373730304D42425230313031303530");
        break;
    default:
        break;
    }

    *puiOutLen = iBufLen/2;
    asc_hex(pucOutData, iBufLen/2, aucBuf, iBufLen);

    return SUCCESS;
}

CommsSocket::CommsSocket(QObject *parent):QThread(parent)
{
    qDebug() << Q_FUNC_INFO;

    G_uiBlackNum = 0;
    continueFlag = true;ucDialCount = 0;
    pDialTimeOut = new QTimer(this);
    connect(pDialTimeOut, SIGNAL(timeout()), this, SLOT(RetryConnect()));

    aucCommuBuf = NULL;
    aucCommuBuf = (unsigned char *)malloc(sizeof(unsigned char)*512);
    memset(aucCommuBuf, 0, 512);

    aucSendBuff = NULL;
    aucSendBuff = (unsigned char *)malloc(sizeof(unsigned char)*ISO8583_MAXCOMMBUFLEN);
    memset(aucSendBuff, 0, ISO8583_MAXCOMMBUFLEN);

    aucReceiveBuff = NULL;
    aucReceiveBuff = (unsigned char *)malloc(sizeof(unsigned char)*ISO8583_MAXCOMMBUFLEN);
    memset(aucReceiveBuff, 0, ISO8583_MAXCOMMBUFLEN);
}

CommsSocket::~CommsSocket()
{ 
    qDebug() << Q_FUNC_INFO;

    free(aucCommuBuf);
    free(aucSendBuff);
    free(aucReceiveBuff);
    continueFlag = false;
    if(pDialTimeOut != NULL)
        delete pDialTimeOut;

    exit();/*wait();*/
}

void CommsSocket::gifRefresh(const char *name, int x, int y, int w, int h)
{
    Os__gif_stop();
    QThread::msleep(100);
    Os__gif_start((char *)name,x,y,w,h);
}

void CommsSocket::UpdatedMsg(const QString str,const QString backup)
{
    qDebug()<<Q_FUNC_INFO<<str<<backup;
    emit notify(str,backup);
//    QThread::msleep(100);
}

unsigned char CommsSocket::PreComm(void)
{
    unsigned char ucResult = SUCCESS;
    unsigned char ucSetApnResult = 0;

    qDebug("正在预拨号...");
    gifRefresh("003.gif",80,120,100,100);
    UpdatedMsg(tr("Precomm"),tr(""));

//    G_RUNDATA_ucHostConnectFlag = false;
    switch(g_constantParam.commMode)
    {
    case PARAM_COMMMODE_GPRS:
        ucSetApnResult = OSGSM_GprsSetAPN(g_constantParam.GSM.aucAPN);
        qDebug("[OSGSM_GprsSeAPN = %02x]", ucSetApnResult);
    case PARAM_COMMMODE_CDMA:
        ucResult = OSGSM_GprsDial();
        qDebug("[OSGSM_GprsDial = %02x]", ucResult);
        break;
    default:
        break;
    }
    msleep(100);
//    Os__gif_stop();
    return ucResult;
}

void CommsSocket::DestroyNotify()
{
    qDebug() << Q_FUNC_INFO;
    pDialTimeOut->stop();
    FinComm();
}

void CommsSocket::ExitFromRecv()
{
    qDebug() << Q_FUNC_INFO;
    continueFlag = false;
    DestroyNotify();
    emit sendResult(0xD4);
}

void CommsSocket::ExitFromExCAN(unsigned char ret)
{
    qDebug() << Q_FUNC_INFO;
    continueFlag = false;
    DestroyNotify();
    emit sendResult(ret);
}

void CommsSocket::RetryConnect()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = SUCCESS;

    /* Retry Dial */
    emit EableNotify(false);
    pDialTimeOut->start();
    FinComm();
//    if(ucDialCount < g_constIninfo.ucDialRetry)
//    {
//        qDebug("CHECKPRE 重拨号 尝试!\n");
        do
        {
            qDebug("ucDialCount = %u\n",ucDialCount);
            //Thread Destruction exception
            if(continueFlag == false)
                break;
            ucResult = PreComm();
            //ucDialCount++;
            if(ucResult == SUCCESS)
            {
                qDebug("CHECKPRE 重拨号 成功!\n");
                break;
            }
            else
                FinComm();
            //Thread Destruction exception
            if(continueFlag == false)
                break;
        }while(1);
//        while(ucDialCount < g_constIninfo.ucDialRetry);
//    }
//    else
//    {
//        qDebug("CHECKPRE 超时 退出!\n");
//        return ExitFromRecv();
//    }
//    if(ucResult == SUCCESS)
//        CheckPreComm();
    if(ucResult != SUCCESS)
    {
        qDebug("CHECKPRE 异常 退出!\n");
        ExitFromExCAN(ERR_COMMUNICATION);
    }
    else
    {
        Os__gif_stop();
        qDebug("CHECKPRE 重拨号 成功XXX !\n");
        emit EableNotify(true);
    }
}

unsigned char CommsSocket::FinComm(void)
{
    qDebug() << Q_FUNC_INFO;

    switch(g_constantParam.commMode)
    {
    case PARAM_COMMMODE_GPRS:
    case PARAM_COMMMODE_CDMA:
        OSGSM_GprsHangupDial();
        break;
    default:
        break;
    }
    return SUCCESS;
}

unsigned char CommsSocket::CheckPreComm(void)
{
    qDebug() << Q_FUNC_INFO;

    unsigned char ucResult;
    unsigned char ucDialDispFlag;

    for(;;)
    {
        //Thread Destruction exception
        if(continueFlag == false)
            break;
        ucDialDispFlag = true;
        //check dial
        do
        {
            //Thread Destruction exception
            if(continueFlag == false)
                break;
            ucResult = OSGSM_GprsCheckDial(COMMS_CHECK_GPRS_TIMEOUT);
            qDebug("OSGSM_GprsCheckDial %02x", ucResult);
            if(!ucResult)
            {
                disconnect(pDialTimeOut, SIGNAL(timeout()), this, SLOT(RetryConnect()));
                break;
            }
            //this if clause is useless now.
            else if(ucDialDispFlag == true)
            {
                /*emit EableNotify(true);*/
                ucDialDispFlag = false;
            }
            //Thread Destruction exception
            if(continueFlag == false)
                break;
        }while(ucResult == STILL_DIALING || ucResult == 0x01);

        //Thread Destruction exception
        if(continueFlag == false)
            break;

        if(!ucResult)
        {
            pDialTimeOut->stop();
            emit EableNotify(false);
//            G_RUNDATA_ucHostConnectFlag = true;
            break;
        }
        //Thread Destruction exception
        if(continueFlag == false)
            break;
    }
    /*pDialTimeOut->stop();*/
    qDebug("CheckPreComm = %02x!\n", ucResult);
    return ucResult;
}

unsigned char CommsSocket::TCPIP_SendReceive(unsigned char *pucInData,unsigned short uiInLen,unsigned char *pucOutData,unsigned short *puiOutLen)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = SUCCESS;
    unsigned char aucBuf[20] = {0};
    unsigned char aucTPDU[6] = {0};
    unsigned short uiTimeout = 30, ucTPDULen = 5, usOffSet = 0;
    int iTCPHandle = 0, uiI = 0;

    memcpy(aucTPDU, "\x60\x00\x01\x00\x00", 5);

    //连接
    gifRefresh("003.gif", 80, 120, 100, 100);
    for(uiI = 0; uiI < 2; uiI++)
    {
        if(uiI > 1)
            UpdatedMsg(tr("Switch to Backup IP"), tr(""));
        else
            UpdatedMsg(tr("Connecting Main IP"), tr(""));

        /*================ TCP Open ================*/
        qDebug("TCP Open");
        ucResult = TCP_Open(&iTCPHandle);
        qDebug("TCP Open ucResult = %02x", ucResult);
        if(ucResult)
        {
            TCP_Close(iTCPHandle);
            break;
        }
        /*================ TCP Connect ================*/
        if(!ucResult)
        {
            ucResult = TCP_Connect(iTCPHandle,
                                   g_constantParam.ulHostIP[g_constantParam.ucHostIndex],
                                   g_constantParam.uiHostPort[g_constantParam.ucHostIndex]);
        }
        //TCP Connect Fail
        if(ucResult)
        {
            TCP_Close(iTCPHandle);
            if(g_constantParam.commMode == PARAM_COMMMODE_GPRS
            || g_constantParam.commMode == PARAM_COMMMODE_CDMA)
                OSGSM_GprsReset();
        }
        qDebug("TCP Connect Num %d result is %02x", uiI, ucResult);
    }

    //发送
    if(!ucResult)
    {
        qDebug("Connect Server OK");

        gifRefresh((char*)"sending3.gif", 0, 120, 240, 154);
        UpdatedMsg(tr("Connected,Sending\nPlease Wait"), tr(""));

        //数据准备
        usOffSet = 0;
        memset(aucSendBuff, 0x00, ISO8583_MAXCOMMBUFLEN);
        memcpy(&aucSendBuff[2+usOffSet], aucTPDU, ucTPDULen);        //TPDU
        usOffSet += ucTPDULen;
        memcpy(&aucSendBuff[2+usOffSet], pucInData, uiInLen);        //数据体
        usOffSet += uiInLen;
        short_tab(aucSendBuff, 2, &usOffSet);

        //数据发送
        qDebug("Send Data");
        ucResult = TCP_Send(iTCPHandle, aucSendBuff, (2+usOffSet));
        qDebug("Send Data result is %02x", ucResult);
    }
    //接收
    if(!ucResult)
    {
        gifRefresh((char*)"receive.gif", 5, 120, 230, 35);

        memset(aucBuf, 0x00, sizeof(aucBuf));
        memcpy(aucBuf, "Total used    sec", 14);
        uiTimeout = 30;
        for(uiI = 1; uiI <= uiTimeout; uiI++)
        {
            sprintf((char *)aucBuf, "Total Used %d sec", uiI);
            UpdatedMsg(tr("Received, Processing\nPlease Wait"), tr((const char *)aucBuf));

            *puiOutLen = ISO8583_MAXCOMMBUFLEN;
            memset(aucReceiveBuff, 0x00, ISO8583_MAXCOMMBUFLEN);
            qDebug("Recv Data");
            ucResult = TCP_Recv(iTCPHandle,aucReceiveBuff, puiOutLen, 1);
            qDebug("Recv Data result is %02x", ucResult);
            if(!ucResult)
            {
                if(*puiOutLen < 2)
                    continue;
                break;
            }
        }
    }

    //赋数据
    if(!ucResult)
    {
        usOffSet = 2 + ucTPDULen;
        if(aucReceiveBuff[0] == 0x60 || aucReceiveBuff[0] == 0x80) //first byte of APTU
            usOffSet -= 2;
        *puiOutLen -= usOffSet;
        memcpy(pucOutData, &aucReceiveBuff[usOffSet], *puiOutLen);
    }

    TCP_Close(iTCPHandle);
    if(g_constantParam.commMode == PARAM_COMMMODE_GPRS
    || g_constantParam.commMode == PARAM_COMMMODE_CDMA)
        OSGSM_GprsReset();
    return ucResult;
}

unsigned char CommsSocket::SendReceive(unsigned char *pucInData,unsigned short uiInLen,unsigned char *pucOutData,unsigned short *puiOutLen)
{
    unsigned char ucResult;

    if(g_changeParam.simTrans == true)
        ucResult = SendReceive_Test(pucOutData, puiOutLen);
    else
        ucResult = TCPIP_SendReceive(pucInData, uiInLen, pucOutData, puiOutLen);

    qDebug("SendReceive result is %02x", ucResult);
    if(ucResult)
    {
        SetCommsError((CommsErrIndex)ucResult);
        ucResult = ERR_COMMUNICATION;
    }
    return ucResult;
}

unsigned char CommsSocket::GenSendReceive(void)
{
    unsigned char ucResult;
    unsigned char aucMAC[8] = {0};
    unsigned short uiLen = 0;
    unsigned short SendMsgID, RecvMsgID = 0;

    /* Save package to SendISO8583Data */
    memcpy(&Trans_8583Data.SendISO8583Data, &ISO8583Data, sizeof(BACKUPISO8583));

    /* Pack Data */
    ucResult = ISO8583_PackData(ISO8583Data.aucCommBuf, &ISO8583Data.uiCommBufLen, ISO8583_MAXCOMMBUFLEN);

    /* Set MAC */
    if(!ISO8583_CheckBit(64) || !ISO8583_CheckBit(128))
    {
        uiLen = ISO8583Data.uiCommBufLen - 8;
        ucResult = KEY_CalcMAC(ISO8583Data.aucCommBuf, uiLen, aucMAC);
        /* Copy MAC to Commbuf */
        memcpy(&ISO8583Data.aucCommBuf[uiLen], aucMAC, 8);
    }

    /* SendReceive with host */
    if(!ucResult)
    {
        ISO8583_DumpData();
        uiLen = sizeof(ISO8583Data.aucCommBuf);
        ucResult = SendReceive(ISO8583Data.aucCommBuf, ISO8583Data.uiCommBufLen, ISO8583Data.aucCommBuf, &uiLen);
    }

    /* unPack Data */
    if(!ucResult)
    {
        ISO8583Data.uiCommBufLen = uiLen;
        ucResult = ISO8583_UnpackData(ISO8583Data.aucCommBuf, ISO8583Data.uiCommBufLen);
        ISO8583_DumpData();
        if(!ucResult)
        {
            SendMsgID = bcd_long(Trans_8583Data.SendISO8583Data.aucMsgID, ISO8583_MSGIDLEN*2);
            RecvMsgID = bcd_long(ISO8583Data.aucMsgID, ISO8583_MSGIDLEN*2);
            if(RecvMsgID != (SendMsgID + 10))
                return(ERR_UTIL_OVERBUFLIMIT);
        }
    }
    return(ucResult);
}
