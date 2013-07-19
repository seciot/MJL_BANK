#ifndef _MSGREPORT_H
#define _MSGREPORT_H
#include "msg.h"

 void CleanError();
 void SetHostError(HostErrIndex hostIndex);
 void SetCommsError(CommsErrIndex commsIndex);
 void SetFileError(FileErrIndex fileIndex);
 void SetLineError(LineErrIndex lineIndex);

 HostErrIndex GetHostError();
 CommsErrIndex GetCommsError();
 FileErrIndex GetFileError();
 LineErrIndex GetLineError();

#endif
