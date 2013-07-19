#ifndef MSG_CONFIG_H
#define MSG_CONFIG_H


const  Msg MsgReport::msgTab[MSG_MAXNB] =
{
	{""},	
	{"收银员积分\n签到成功!"},
	{"请拔出IC卡"},
	{"数据采集成功!"},
	{"请拔出转出卡 -->>"},
	{"请再试一次!"},
	{"请注意!\n该交易要收现金"},
	{"状态上送成功!"},
	{"回响测试成功!"},
	{"EMV参数成功!"},
	{"EMVDefaultLoad OK!"},
	{"清除成功!"},
	{"交易清除成功!"},
	{"串口初始化成功!"},
	{"数据存储成功!"},
	{"交易成功!"},
	{"卡柄删除成功!"},
	{"卡柄增加成功!"},	
	{"清除成功!"},
    {"恢复成功!"},
	{"帐务调整成功!"},
	{"增加柜员成功!"},
	{"删除柜员成功!"},
	{"修改密码成功!"},
	{"密码恢复成功!"},
	{"超时!"},	
	{"[签到] 成功!"},
	{"签退成功!"},
	{"主密钥载入成功!"},
	{"下载参数成功!"},	
};

const COMPISO MsgReport::ISOErrMsg[] =
{
	{COMP_ERROR_CODE,			{"处理码出错"}},
	{COMP_ERROR_AMOUNT,			{"金额比较出错"}},
	{COMP_ERROR_TRANS_NO,		{"流水比较出错"}},
	{COMP_ERROR_MESSAGE,		{"交易报文出错"}},
	{COMP_ERROR_TID,			{"终端号出错"}},
	{COMP_ERROR_MID,			{"商户号出错"}},
	{COMP_ERROR_PCODE,			{"受理方标识码错"}},
	{COMP_ERROR_COMPARE,		{"比较出错"}}
};


const FileErrMsg MsgReport::fileErrTab[]=
{
	{FILE_ERROR_INDEX,			{"文件索引满"}},
	{FILE_ERROR_PARAM,			{"文件参数有误"}},
	{FILE_ERROR_AREA,			{"文件区设置失败"}},
    {FILE_ERROR_NO_EXIST,		{"文件系统不存在"}},
    {FILE_ERROR_NOMORE_MEMORY,	{"内存不足请先结算"}},

    {FILE_ERROR_WRITE,			{"写文件失败"}},
    {FILE_ERROR_OPERTATE,		{"文件操作有误"}},
    {FILE_ERROR_HANDLEOPEN,		{"文件未打开"}},
    {FILE_ERROR_CURSOR,			{"文件偏移错误"}},
    {FILE_ERROR_ATTR,			{"文件属性错误"}},
    {FILE_ERROR_NAME,			{"文件名错误"}},

    {FILE_ERROR_SYSTEM,			{"文件系统出错"}},
};

const CommsErrMsg MsgReport::commsErrTab[] =
{
	{NET_PATTERN_ERROR,				{"类型错误"}},
	{NET_NO_IDLE_DIALOG,			{"无空闲会话"}},
	{NET_ID_ERROR,					{"错误句柄"}},
	{NET_ID_NOT_OPEN,				{"句柄未打开"}},
	{NET_DIAL_CONNECT_ERROR,		{"会话连接错误"}},
	{NET_DIAL_CONNECT_REJECT,		{"会话连接拒绝"}},
	{NET_PARAM_ERROR,				{"参数错误"}},
	{NET_IP_CATEGORY_ERROR,			{"IP层错误"}},
	{NET_DATA_CHECK_ERROR,			{"数据较验错误"}},
	{NET_NO_MATCH_DIALOG,			{"无匹配的会话"}},
	{NET_TAG_ERROR,					{"标志错误"}},
	{NET_NO_MATCH_SERIES_NUMBER,	{"无匹配的序列号"}},
	{NET_SEND_DATA_LEN_ERROR,		{"发送数据长度错误"}},
	{NET_REVIEVE_OVERTIME,			{"接收超时"}},
	{NET_NO_ANSWER,					{"数据无应答"}},
	{NET_DIAL_OVERTIME,				{"网络连接超时"}},
	{NET_REVIEVE_DATA_LEN_ERROR,	{"接收数据长度错误"}},
	{NET_NETCOMMS_VERSION,	{"通讯版本错"}},
	{NET_NETCOMMS_OVERBUFLIMIT,	{"缓存超限"}},
	{NET_NETCOMMS_INDEXERROR,	{"密钥索引错"}},
	{NET_NETCOMMS_MACERROR,	{"MAC错"}},
	{ERR_COMMS_UNKNOW,				{"交易失败"}}
};

const LineErrMsg MsgReport::lineErrTab[] =
{
    {MID_INVALID,				{"商户号输入无效"}},
	{MNAME_INVALID,				{"商户名输入无效"}},
    {TID_INVALID,				{"终端号输入无效"}},
	{TRACE_INVALID,				{"流水号输入无效"}},
	{BATCH_INVALID,				{"批次号输入无效"}},
	{TRANSMAX_INVALID,			{"最大交易笔数无效"}},
	{TPDU_INVALID,				{"TPDU输入无效"}},
	{TIMEOUT_INVALID,			{"超时输入无效"}},
	{DIALRETRY_INVALID,			{"重拨次数输入无效"}},
	{SWITCHBOARD_INVALID,		{"分机号输入无效"}},
	{TEL_INVALID,				{"电话号码输入无效"}},
	{PORT_INVALID,				{"端口号输入无效"}},
	{IP_INVALID,				{"IP输入无效"}},
	{APN_INVALID,				{"APN输入无效"}},
	{TIPFEERATE_INVALID,		{"小费率输入无效"}},
	{CURRENCYRATE_INVALID,		{"货币扣率输入无效"}},
	{TRANSRETRY_INVALID,		{"交易重发输入无效"}},
    {ANTOSENDSUM_INVALID,		{"上送笔数输入无效"}},
	{TERMTYPE_INVALID,          {"请先选择终端类型"}},
	{KEYINDEX_INVALID,          {"密钥索引不正确!"}},
	{TMKEYINPUT_INVALID,        {"密钥输入不合法!"}},

    {REGULAR_INVALID,           {"当前输入无效,请重输!"}},
	{PININPUT_INVALID,          {"密码长度无效,请重输!"}},
	{AMOUNT_INVALID,			{"金额无效,请重输!"}},
	{UNKNOW_INVALID,			{"未知异常"}}

};

HostErrIndex MsgReport::hostErrIndex = (HostErrIndex)0;
FileErrIndex MsgReport::fileErrIndex = (FileErrIndex)0;
CommsErrIndex MsgReport::commsErrIndex = (CommsErrIndex)0;
LineErrIndex MsgReport::lineErrIndex = (LineErrIndex)0;



#endif /* MSG_CONFIG_H_ */
