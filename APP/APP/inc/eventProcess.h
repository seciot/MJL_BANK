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
unsigned char EventProcess(bool magEvent, bool touchEvent, bool untouchEvent);
DRV_OUT *GetPxMag();
DRV_OUT *GetMifare();

#endif // EVENTPROCESS_H
