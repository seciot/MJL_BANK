#ifndef _EMVPARAM_H
#define _EMVPARAM_H

#ifndef EMVPARAM_DATA
#define EMVPARAM_DATA extern
#endif

#include "EMV41.h"

extern UCHAR EMVICC(EMVICCIN *pEMVICCIn, EMVICCOUT *pEMVICCOut);
extern const TERMSUPPORTAPP TermSupportAppDefault[];
extern const CAPK TermSupportCAPKDefault[];
extern const CAPK TermSupportCAPKTest[];
extern const TERMCONFIG TermConfigDefault;

#endif // _EMVPARAM_H
