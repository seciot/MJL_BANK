#ifndef TRANSONLINE_H
#define TRANSONLINE_H

#include "transData.h"

//工作密钥下载 Work Key Down
unsigned char TRANS_ONLINE_DownWK_pack(void);
unsigned char TRANS_ONLINE_DownWK_unpack(void);

//传输密钥下载
unsigned char TRANS_ONLINE_DownEWK_pack(void);
unsigned char TRANS_ONLINE_DownEWK_unpack(void);

//冲正 Reversal
void TRANS_ONLINE_ReversalPack(void);

//存款 Deposit
unsigned char TRANS_ONLINE_Deposit_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans);
unsigned char TRANS_ONLINE_Deposit_unpack(NormalTrans *pNormalTrans);

//取款 Advance
unsigned char TRANS_ONLINE_Advance_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans);
unsigned char TRANS_ONLINE_Advance_unpack(NormalTrans *pNormalTrans);

//余额查询 Balance Inquiry
unsigned char TRANS_ONLINE_BalanceInquiry_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans);
unsigned char TRANS_ONLINE_BalanceInquiry_unpack(NormalTrans *pNormalTrans);

//改密 Change PIN
unsigned char TRANS_ONLINE_PINChange_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans);
unsigned char TRANS_ONLINE_PINChange_unpack(NormalTrans *pNormalTrans);

//卡卡转帐 P2P Transfer
unsigned char TRANS_ONLINE_Transfer_pack(NormalTrans *pNormalTrans, ExtraTrans *pExtraTrans);
unsigned char TRANS_ONLINE_Transfer_unpack(NormalTrans *pNormalTrans);

//结算 Settlement
unsigned char TRANS_ONLINE_Settlement_pack(TRANSTOTAL *pTransTotal);
unsigned char TRANS_ONLINE_Settlement_unpack(void);

//批上送
unsigned char TRANS_ONLINE_BatchUpload_pack(NormalTrans *pNormalTrans);
unsigned char TRANS_ONLINE_BatchUpload_unpack(void);

//批上送结束
unsigned char TRANS_ONLINE_BatchUploadEnd_pack(TRANSTOTAL *pTransTotal);
unsigned char TRANS_ONLINE_BatchUploadEnd_unpack(void);

#endif // TRANSONLINE_H
