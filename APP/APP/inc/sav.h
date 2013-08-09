#ifndef SAV_H
#define SAV_H

#include "transData.h"

enum SAV_TRANS_TYPE
{
    SAV_TRANS_NORMAL        = 1,
    SAV_TRANS_NIIVOID       = 2,
    SAV_TRANS_ADJUST        = 3
};

unsigned char SAV_CleanTransaction(void);
unsigned char SAV_CleanCurrentBatch(void);
unsigned char SAV_CheckNormalTransIndex(void);
unsigned char SAV_CheckVoidTransIndex(NormalTrans *pNormalTrans);
unsigned char SAV_CheckAdjustTransIndex(NormalTrans *pNormalTrans);
unsigned char SAV_TransSave(void);
unsigned char SAV_SettleSave(void);

#endif // SAV_H
