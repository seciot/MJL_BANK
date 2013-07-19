#ifndef _COMMS_H
#define _COMMS_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QString>

#include "paramData.h"
#include "transData.h"

class CommsSocket : public QThread
{
    Q_OBJECT
public:	
	explicit CommsSocket(QObject *parent = 0);
    ~CommsSocket();
    bool continueFlag;
	unsigned char ucDialCount;
	QTimer * pDialTimeOut;
    
    unsigned long G_uiBlackNum;
	unsigned char * aucCommuBuf;
	unsigned char * aucSendBuff;
    unsigned char * aucReceiveBuff;

    unsigned char PreComm(void);
    static unsigned char FinComm(void);
    unsigned char CheckPreComm(void);
	unsigned char GenSendReceive(void);
    unsigned char TCPIP_SendReceive(unsigned char *pucInData,unsigned short uiInLen,unsigned char *pucOutData, unsigned short *puiOutLen);
    unsigned char SendReceive(unsigned char *pucInData,unsigned short uiInLen,unsigned char *pucOutData,unsigned short *puiOutLen);
    void UpdatedMsg(const QString str,const QString backup);

    void DestroyNotify();
	void ExitFromRecv();
	void ExitFromExCAN(unsigned char ret);
    
    static void gifRefresh(const char *name, int x, int y, int w, int h);
signals:
    void notify(const QString str,const QString backup);
    void sendResult(unsigned char index);
    void EableNotify(bool);
private slots:
    void RetryConnect();
};

#endif
