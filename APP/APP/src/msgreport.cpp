#include "msgreport.h"

static HostErrIndex  saveHostErrIndex;
static FileErrIndex  saveFileErrIndex;
static CommsErrIndex saveCommsErrIndex;
static LineErrIndex  saveLineErrIndex;

void CleanError()
{
    saveHostErrIndex = (HostErrIndex)0;
    saveCommsErrIndex = (CommsErrIndex)0;
    saveFileErrIndex = (FileErrIndex)0;
    saveLineErrIndex = (LineErrIndex)0;
}


void SetHostError(HostErrIndex hostIndex)
{
    saveHostErrIndex = hostIndex;
}

void SetCommsError(CommsErrIndex commsIndex)
{
    saveCommsErrIndex = commsIndex;
}

void SetFileError(FileErrIndex fileIndex)
{
    saveFileErrIndex = fileIndex;
}

void SetLineError(LineErrIndex lineIndex)
{
    saveLineErrIndex = lineIndex;
}

HostErrIndex GetHostError()
{
    return saveHostErrIndex;

}
CommsErrIndex GetCommsError()
{
    return saveCommsErrIndex;

}
FileErrIndex GetFileError()
{
    return saveFileErrIndex;

}
LineErrIndex GetLineError()
{
    return saveLineErrIndex;

}
