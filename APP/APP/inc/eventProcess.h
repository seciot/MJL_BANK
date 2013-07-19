#ifndef EVENTPROCESS_H
#define EVENTPROCESS_H
#include "new_drv.h"
#include "msg.h"

enum EventType
{
    NoCard = 0,
    Untouch,
    Touch,
    Mag,
    Key
};

EventType getEventType();
void RemoveKeyEventBug(void);
unsigned char KEYDrv_Process();
unsigned char EventProcess(unsigned char Type);
DRV_OUT* GetPxMag();

#endif // EVENTPROCESS_H
