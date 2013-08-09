#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "include.h"
#include "paramData.h"
#include "transData.h"
#include "iso8583Data.h"
#include "msg.h"

#ifndef EXTGLOBAL
#define EXTGLOBAL  extern
#endif

EXTGLOBAL   CONSTANT_PARAM      g_constantParam;
EXTGLOBAL   CHANGE_PARAM        g_changeParam;
EXTGLOBAL   BIN_DATA        g_binData;
EXTGLOBAL   BIN_STATUS        g_binStatus;
EXTGLOBAL   SYSCASHIER          g_cashier;

EXTGLOBAL   RUNDATA             RunData;
EXTGLOBAL   NORMAL_TRANS        NormalTransData;
EXTGLOBAL   EXTRA_TRANS         ExtraTransData;

EXTGLOBAL   ISO8583             ISO8583Data;
EXTGLOBAL   TRANS_8583_DATA     Trans_8583Data;

EXTGLOBAL   NORMAL_TRANS        g_saveTrans;                //保存交易
EXTGLOBAL   TRANSINFO           g_transInfo;                //统计信息
EXTGLOBAL   TRANSINFO           g_oldtransInfo;             //统计信息
EXTGLOBAL   REVERSEDISO8583     g_reversedTrans;            //冲正

// added by xtf
#define G_NORMALTRANS_ucMainAccountLen              NormalTransData.ucSourceAccLen
#define G_NORMALTRANS_aucSourceAcc                  NormalTransData.aucSourceAcc
#define G_EXTRATRANS_aucCardPan_UnAssign  			ExtraTransData.aucCardPan_UnAssign

#define G_EXTRATRANS_uiISO2Len						ExtraTransData.magData.uiISO2Len
#define G_EXTRATRANS_uiISO3Len						ExtraTransData.magData.uiISO3Len
#define G_EXTRATRANS_aucISO2						ExtraTransData.magData.aucISO2
#define G_EXTRATRANS_aucISO3						ExtraTransData.magData.aucISO3
#define G_EXTRATRANS_aucPINData						ExtraTransData.aucPINData
#define G_EXTRATRANS_aucChangePINData               ExtraTransData.aucChangePINData

//:- RUNDATA
#define G_RUNDATA_bReversalFlag						RunData.bReversalFlag					//:- 开始交易时设置的冲正标志
#endif
