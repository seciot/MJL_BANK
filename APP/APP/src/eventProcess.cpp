#include "eventProcess.h"
#include <QDebug>

#include "global.h"

#define POS_SUCCESS 0x00

DRV_OUT *pxMifare;
DRV_OUT *pxMag;
DRV_OUT *pxIcc;
DRV_OUT *pxKey;
NEW_DRV_TYPE  Driver;

unsigned char DrvPro;
EventType eventType;

void ClearEventDrvAddr()
{
    pxMifare = NULL;
    pxMag = NULL;
    pxIcc = NULL;
    pxKey = NULL;
    DrvPro = 0;
    eventType = NoCard;
}

unsigned char DriverProcess()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = POS_SUCCESS;

    qDebug("Driver.drv_type = %02x", Driver.drv_type);
    qDebug("Driver.drv_data.gen_status = %02x",Driver.drv_data.gen_status);
    switch(Driver.drv_type)
    {
    case KEY_DRV:
        if(Driver.drv_data.gen_status== DRV_ENDED)
            pxKey = (DRV_OUT *)&Driver.drv_data;
        break;
    case MAG_DRV:
        if(Driver.drv_data.gen_status== DRV_ENDED)
            pxMag = (DRV_OUT *)&Driver.drv_data;
        break;
    case MFR_DRV:
        if(Driver.drv_data.gen_status== DRV_ENDED)
            pxMifare = (DRV_OUT *)&Driver.drv_data;
        break;
    case ICC_DRV:
        if(Driver.drv_data.gen_status== DRV_ENDED)
            pxIcc = (DRV_OUT *)&Driver.drv_data;
        break;
    case DRV_TIMEOUT:
        ucResult = ERR_TIMEOUT;
        break;
    default:
        break;
    }
    qDebug("DriverProcess() = %02x", ucResult);
    return ucResult;
}

unsigned char DrvExceptionProcess(unsigned int _result)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = POS_SUCCESS;

    switch(_result)
    {
    case KEY_DRV_ERROR:
        ucResult = ERR_KEYDRV;
        break;
    case MAG_DRV_ERROR:
        ucResult = ERR_MAGDRV;
        break;
    case MFR_DRV_ERROR:
        ucResult = ERR_MFCDRV;
        break;
    case ICC_DRV_ERROR:
        ucResult = ERR_ICCDRV;
        break;
    default:
        ucResult = ERR_UNKNOWNDRV;
        break;
    }
    return ucResult;
}

unsigned char KEYDrv_Process()
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = POS_SUCCESS;
    unsigned char ucKey = pxKey->xxdata[1];

    if(ucKey == KEY_CLEAR)
        ucResult = ERR_CANCEL;
    if(ucKey == KEY_ENTER)
    {
        qDebug()<<"KEY_ENTER";
        ucResult = ERR_CARDNOACT;
    }

    qDebug("KEYDrv_Process() = %02x", ucResult);
    return ucResult;
}

void RemoveKeyEventBug(void)
{
    qDebug() << Q_FUNC_INFO;
    unsigned int uiRet = POS_SUCCESS;
    NEW_DRV_TYPE  new_drv;

    for(;;)
    {
        uiRet = Os_Wait_Event(KEY_DRV, &new_drv, 1);
        if(uiRet == 0)
        {
            if(new_drv.drv_type == KEY_DRV)
                qDebug("ABORT KEY VALUE = %02x\n",new_drv.drv_data.xxdata[1]);
            else if (new_drv.drv_type == MAG_DRV)
                qDebug("ABORT MAG VALUE = %02x\n",new_drv.drv_data.xxdata[1]);
            else if (new_drv.drv_type == MFR_DRV)
                qDebug("ABORT MFR VALUE = %02x\n",new_drv.drv_data.xxdata[1]);
            else if (new_drv.drv_type == ICC_DRV)
                qDebug("ABORT ICC VALUE = %02x\n",new_drv.drv_data.xxdata[1]);
            else if(new_drv.drv_type == DRV_TIMEOUT)
            {
                qDebug("Excessive KEYs are destroyed !!\n");
                break;
            }
        }
        else
            break;
    }
}

unsigned char EventProcess(unsigned char Type)
{
    qDebug() << Q_FUNC_INFO;
    unsigned char ucResult = POS_SUCCESS;
    unsigned char uiRet = POS_SUCCESS;

    RemoveKeyEventBug();
    ClearEventDrvAddr();

    DrvPro = 0;
    if(NoCard == Type || Mag == Type)
        DrvPro = DrvPro|MAG_DRV;
    if(NoCard == Type || Touch == Type)
        DrvPro = DrvPro|ICC_DRV;
    if(NoCard == Type || Untouch == Type)
        DrvPro = DrvPro|MFR_DRV;
    DrvPro = DrvPro|KEY_DRV;
    memset(&Driver ,0, sizeof(NEW_DRV_TYPE));

    qDebug("DrvPro = %02x\n", DrvPro);
    uiRet = Os_Wait_Event(DrvPro, &Driver, 1000*30);
    qDebug("\nOs_Wait_Event result is %02x\n", uiRet);

    if(uiRet == 0)
        ucResult = DriverProcess();
    else            //异常处理
        ucResult = DrvExceptionProcess(uiRet);

    printf("Driver.drv_data.gen_status = %02x\n",Driver.drv_data.gen_status);
    if(!ucResult)
    {
        if((pxIcc != NULL) && (pxIcc->gen_status== DRV_ENDED))              /*ICC事件具体处理*/
            eventType = Touch;
        else if((pxMag != NULL) && (pxMag->gen_status == DRV_ENDED))        /*MAG事件具体处理*/
            eventType = Mag;
        else if((pxMifare != NULL) && (pxMifare->gen_status == DRV_ENDED))  /*MFC事件具体处理*/
            eventType = Untouch;
        else if((pxKey != NULL) && (pxKey->gen_status == DRV_ENDED))        /*KEY事件具体处理*/
            eventType = Key;
    }
    return ucResult;
}

EventType getEventType()
{
    return eventType;
}

DRV_OUT* GetPxMag()
{
    return pxMag;
}
