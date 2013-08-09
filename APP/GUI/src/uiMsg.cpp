#include "uiMsg.h"
#include "msgreport.h"
#include "sand_lcd.h"
#include "global.h"

UIMsg::UIMsg(ErrorType errortype, long errorCode, QDialog *parent, Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 初始化错误代码
    this->initalErrorMsg();
    this->initalNoticeMsg();
    this->initalFileErrorMsg();
    this->initalCommErrorMsg();
    this->initalHostErrorMsg();

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(20,FRAME420_THVALUE+50,FRAME420_WIDTH,FRAME420_HEIGHT-50);
    this->setFixedSize(FRAME420_WIDTH-40,FRAME420_HEIGHT-80);
    this->setStyleSheet("QDialog{border: 6px solid silver;}");

    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    //--------------define--------------------//
    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lbMsg=new QLabel();
    lbReturnCode=new QLabel();
    lbMsg->setFont(font2);
    lbReturnCode->setFont(font2);
    lbMsg->setAlignment(Qt::AlignCenter);
    lbReturnCode->setAlignment(Qt::AlignCenter);
    lbMsg->setMinimumHeight(40);
    lbReturnCode->setMinimumHeight(40);

    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("OK"));
    btnSubmit->setFont(font2);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_GREY_STYLE);


    this->beepTwice();   //beep

    switch(errortype)
    {
    case HOST_ERROR:
    {
        lbHead->setText("HOST ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");

        lbMsg->setText(hashHostError.value(HostErrIndex(errorCode)));
        break;
    }
    default:
        break;
    }

    QString hostCode=QString::number(errorCode);
    lbReturnCode->setText("Return Code : "+hostCode);

    // -----------layout------------//
    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp3=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(6);
    h1Lay->addWidget(lbHead);
    h1Lay->addSpacing(6);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addSpacing(6);
    v1Lay->addLayout(h1Lay);
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbMsg);
    v1Lay->addItem(sp2);
    v1Lay->addWidget(lbReturnCode);

    v1Lay->addItem(sp3);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,10);

    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(close()));

}


UIMsg::UIMsg(ErrorType errortype, unsigned char ucErrorCode, QDialog *parent, Qt::WindowFlags f) :
    QDialog(parent,f)
{
    // 初始化错误代码
    this->initalErrorMsg();
    this->initalNoticeMsg();
    this->initalFileErrorMsg();
    this->initalCommErrorMsg();
    this->initalHostErrorMsg();

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(20,FRAME420_THVALUE+50,FRAME420_WIDTH,FRAME420_HEIGHT-50);
    this->setFixedSize(FRAME420_WIDTH-40,FRAME420_HEIGHT-80);
    this->setStyleSheet("QDialog{border: 6px solid silver;}");

    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    //--------------define--------------------//
    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lbMsg=new QLabel();
    lbReturnCode=new QLabel();
    lbMsg->setFont(font2);
    lbReturnCode->setFont(font2);
    lbMsg->setAlignment(Qt::AlignCenter);
    lbReturnCode->setAlignment(Qt::AlignCenter);
    lbMsg->setMinimumHeight(40);
    lbReturnCode->setMinimumHeight(40);

    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("OK"));
    btnSubmit->setFont(font2);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_GREY_STYLE);


    this->beepTwice();   //beep

    switch(errortype)
    {
    case NORMAL_ERROR:
    {
        lbHead->setText("ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");

        lbMsg->setText(hashError.value(ErrIndex(ucErrorCode)));

        break;
    }
    case NOTICE_ERROR:
    {
        lbHead->setText("NOTICE");
        lbHead->setStyleSheet("background-color: rgb(0, 153, 255);");

        lbMsg->setText(hashNotice.value(MsgTabIndex(ucErrorCode)));

        break;
    }
    case FILE_ERROR:
    {
        lbHead->setText("FILE ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");
        lbMsg->setText(hashFileError.value(FileErrIndex(ucErrorCode)));


        break;
    }
    case COMM_ERROR:
    {
        lbHead->setText("COMM ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");
        lbMsg->setText(hashCommError.value(CommsErrIndex(ucErrorCode)));

        break;
    }
    case HOST_ERROR:
    {
        qDebug()<<"不应该在这里处理";
        break;
    }
    }

    unsigned char  ucCode;
    ucCode = ucErrorCode;

    unsigned char  aucErrorCode[PARAM_ANSWER_LEN + 1] = {0};
    hex_str(aucErrorCode, &ucCode, PARAM_ANSWER_LEN);
    lbReturnCode->setText("Return Code : "+QString::fromAscii((const char*)aucErrorCode));

    // -----------layout------------//
    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp3=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(6);
    h1Lay->addWidget(lbHead);
    h1Lay->addSpacing(6);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addSpacing(6);
    v1Lay->addLayout(h1Lay);
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbMsg);
    v1Lay->addItem(sp2);
    if(errortype!=NOTICE_ERROR)
    {
        v1Lay->addWidget(lbReturnCode);
    }
    v1Lay->addItem(sp3);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,10);

    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(close()));

}

UIMsg::UIMsg(ErrorType errortype, QString errMsg, bool needBeep, QDialog *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    // 初始化错误代码
    this->initalErrorMsg();
    this->initalNoticeMsg();
    this->initalFileErrorMsg();
    this->initalCommErrorMsg();
    this->initalHostErrorMsg();

    QPixmap bg;
    bg.load(":/images/commonbg.png");
    QPalette palette;
    palette.setBrush(backgroundRole(),QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setGeometry(20,FRAME420_THVALUE+50,FRAME420_WIDTH,FRAME420_HEIGHT-50);
    this->setFixedSize(FRAME420_WIDTH-40,FRAME420_HEIGHT-80);
    this->setStyleSheet("QDialog{border: 6px solid silver;}");

    QFont font("Helvetica",12,QFont::Bold);
    QFont font2("Helvetica",14,QFont::Bold);
    QFont font3("Helvetica",8,QFont::Bold);

    //--------------define--------------------//
    lbHead=new QLabel();
    QFont fontH("Helvetica",18,QFont::Bold);
    lbHead->setFont(fontH);
    lbHead->setAlignment(Qt::AlignCenter);
    lbHead->setMinimumHeight(40);
    lbHead->setMaximumHeight(40);
    lbHead->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    lbMsg=new QLabel();
    lbReturnCode=new QLabel();
    lbMsg->setFont(font2);
    lbReturnCode->setFont(font2);
    lbMsg->setAlignment(Qt::AlignCenter);
    lbReturnCode->setAlignment(Qt::AlignCenter);
    lbMsg->setMinimumHeight(40);
    lbReturnCode->setMinimumHeight(40);

    btnSubmit=new QPushButton;
    btnSubmit->setText(tr("OK"));
    btnSubmit->setFont(font2);
    btnSubmit->setMinimumHeight(30);
    btnSubmit->setStyleSheet(BTN_GREY_STYLE);


    if(needBeep==true)
        this->beepTwice();   //beep

    switch(errortype)
    {
    case NORMAL_ERROR:
    {
        lbHead->setText("ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");

        lbMsg->setText(errMsg);

        break;
    }
    case NOTICE_ERROR:
    {
        lbHead->setText("NOTICE");
        lbHead->setStyleSheet("background-color: rgb(0, 153, 255);");

        lbMsg->setText(errMsg);

        break;
    }
    case FILE_ERROR:
    {
        lbHead->setText("FILE ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");
        lbMsg->setText(errMsg);


        break;
    }
    case COMM_ERROR:
    {
        lbHead->setText("COMM ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");
        lbMsg->setText(errMsg);

        break;
    }
    case HOST_ERROR:
    {
        lbHead->setText("HOST ERROR");
        lbHead->setStyleSheet("background-color: rgb(255,0,0);");
        lbMsg->setText(errMsg);

        break;
    }
    }

    // -----------layout------------//
    QSpacerItem *sp1=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp2=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *sp3=new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *h1Lay=new QHBoxLayout();
    h1Lay->addSpacing(6);
    h1Lay->addWidget(lbHead);
    h1Lay->addSpacing(6);

    QVBoxLayout *v1Lay=new QVBoxLayout();
    v1Lay->addSpacing(6);
    v1Lay->addLayout(h1Lay);
    v1Lay->addItem(sp1);
    v1Lay->addWidget(lbMsg);
    v1Lay->addItem(sp2);
    if(errortype!=NOTICE_ERROR)
    {
        v1Lay->addWidget(lbReturnCode);
    }
    v1Lay->addItem(sp3);

    QHBoxLayout *h2Lay=new QHBoxLayout();
    h2Lay->addSpacing(10);
    h2Lay->addWidget(btnSubmit);
    h2Lay->addSpacing(10);

    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addLayout(v1Lay);
    layout->addLayout(h2Lay);

    layout->setContentsMargins(0,0,0,10);

    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(close()));

}

void UIMsg::setAutoClose(int timeout)
{
    qDebug()<<timeout;
    closeTimer= new QTimer(this);
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(thisClose()));
    closeTimer->start(timeout);
}

void UIMsg::thisClose()
{
    closeTimer->stop();
    this->close();
}

void UIMsg::gifRefresh(char *name, int x, int y, int w, int h)
{
    Os__gif_stop();
    //    QThread::msleep(100);
    Os__gif_start(name,x,y,w,h);
}

UIMsg::~UIMsg()
{
    qDebug() << Q_FUNC_INFO;
}

void UIMsg::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();
    switch(event->key())
    {
    case Qt::Key_Escape:
        this->close();
        break;
    case Qt::Key_F4:
        //        vBar->setValue(vBar->value()+150);
        break;
    default:
        qDebug()<<event->key();
        //        event->ignore();
        break;
    }
}

void UIMsg::showErrMsg(const ErrIndex ucErrorCode)
{
    UIMsg *msg=new UIMsg(NORMAL_ERROR,(unsigned char)ucErrorCode);
    msg->exec();
}


void UIMsg::showErrMsg(QString str)
{
    UIMsg *msg=new UIMsg(NORMAL_ERROR,str,true);
    msg->exec();
}


void UIMsg::showFileErrMsg(const FileErrIndex ucErrorCode)
{
    UIMsg *msg=new UIMsg(FILE_ERROR,(unsigned char)ucErrorCode);
    msg->exec();

}

void UIMsg::showCommErrMsg(const CommsErrIndex ucErrorCode)
{
    UIMsg *msg=new UIMsg(COMM_ERROR,(unsigned char)ucErrorCode);
    msg->exec();
}


void UIMsg::showHostErrMsg(const HostErrIndex longErrorCode)
{
    UIMsg *msg=new UIMsg(HOST_ERROR,(long)longErrorCode);
    msg->exec();
}


void UIMsg::showNoticeMsg(const MsgTabIndex index)
{
    UIMsg *msg=new UIMsg(NOTICE_ERROR,(unsigned char)index);
    msg->exec();
}


void UIMsg::showNoticeMsg(QString str)
{
    UIMsg *msg=new UIMsg(NOTICE_ERROR,str,true);
    msg->exec();
}

void UIMsg::showCombineErrMsg(const ErrIndex ucErrorCode)
{
    unsigned char ucCode;
    long hostErr;

    switch(ucErrorCode)
    {
    case ERR_HOSTCODE:
        hostErr = GetHostError();
        showHostErrMsg(HostErrIndex(hostErr));
        break;
    case ERR_XFILE_WRITE:
        ucCode = GetFileError();
        showFileErrMsg(FileErrIndex(ucCode));
        break;
    case ERR_COMMUNICATION:
        ucCode = GetCommsError();
        showCommErrMsg(CommsErrIndex(ucCode));
        break;
    default:
        ucCode = ucErrorCode;
        showErrMsg(ErrIndex(ucCode));
        break;
    }
}

/// -------------------------------------------------------------------- ///
/// -----------------------------Auto Close-------------------------- ///
/// -------------------------------------------------------------------- ///
void UIMsg::showErrMsgWithAutoClose(const ErrIndex ucErrorCode, int timeout)
{
    UIMsg *msg=new UIMsg(NORMAL_ERROR,(unsigned char)ucErrorCode);
    msg->setAutoClose(timeout);
    msg->exec();
}


void UIMsg::showErrMsgWithAutoClose(QString str, int timeout)
{
    UIMsg *msg=new UIMsg(NORMAL_ERROR,str,true);
    msg->setAutoClose(timeout);
    msg->exec();
}


void UIMsg::showFileErrMsgWithAutoClose(const FileErrIndex ucErrorCode, int timeout)
{
    UIMsg *msg=new UIMsg(FILE_ERROR,(unsigned char)ucErrorCode);
    msg->setAutoClose(timeout);
    msg->exec();

}

void UIMsg::showCommErrMsgWithAutoClose(const CommsErrIndex ucErrorCode, int timeout)
{
    UIMsg *msg=new UIMsg(COMM_ERROR,(unsigned char)ucErrorCode);
    msg->setAutoClose(timeout);
    msg->exec();
}


void UIMsg::showHostErrMsgWithAutoClose(const HostErrIndex longErrorCode, int timeout)
{
    UIMsg *msg=new UIMsg(HOST_ERROR,(long)longErrorCode);
    msg->setAutoClose(timeout);
    msg->exec();
}


void UIMsg::showNoticeMsgWithAutoClose(const MsgTabIndex index, int timeout)
{
    UIMsg *msg=new UIMsg(NOTICE_ERROR,(unsigned char)index);
    msg->setAutoClose(timeout);
    msg->exec();
}

void UIMsg::showNoticeMsgWithAutoClose(QString str, int timeout)
{
    UIMsg *msg=new UIMsg(NOTICE_ERROR,str,true);
    msg->setAutoClose(timeout);
    msg->exec();
}

void UIMsg::showNoticeMsgWithAutoCloseNoBeep(QString str, int timeout)
{
    UIMsg *msg=new UIMsg(NOTICE_ERROR,str,false);
    msg->setAutoClose(timeout);
    msg->exec();
}

void UIMsg::showCombineErrMsgWithAutoClose(const ErrIndex ucErrorCode, int timeout)
{
    unsigned char ucCode;
    long hostErr;

    switch(ucErrorCode)
    {
    case ERR_HOSTCODE:
        hostErr = GetHostError();
        showHostErrMsgWithAutoClose(HostErrIndex(hostErr),timeout);
        break;
    case ERR_XFILE_WRITE:
        ucCode = GetFileError();
        showFileErrMsgWithAutoClose(FileErrIndex(ucCode),timeout);
        break;
    case ERR_COMMUNICATION:
        ucCode = GetCommsError();
        showCommErrMsgWithAutoClose(CommsErrIndex(ucCode),timeout);
        break;
    default:
        ucCode = ucErrorCode;
        showErrMsgWithAutoClose(ErrIndex(ucCode),timeout);
        break;
    }
}

void UIMsg::initalErrorMsg()
{
    hashError.insert(ERR_END,                    "TRANSACTION\nEND");
    hashError.insert(ERR_CANCEL,                 "TRANSACTION\nCANCLED");
    hashError.insert(ERR_DRIVER,                 "DRIVER ERROR");
    hashError.insert(ERR_READWRITECARD,          "READ WRITE CARD\nERROR");
    hashError.insert(ERR_OSFUNC,                 "OS FUNC ERROR");
    hashError.insert(ERR_NOTPROC,                "TRANSACTION\nNOT SETTLE");
    hashError.insert(ERR_AMOUNT,                 "金额比较出错");
    hashError.insert(ERR_SYSPASS,                "系统密码错");
    hashError.insert(ERR_DIAL,                   "拨号出错");
    hashError.insert(ERR_HOSTCODE,               "主机返回错误");
    hashError.insert(ERR_DATAINVALID,            "无效数据");
    hashError.insert(ERR_INVALIDXDATAPAGE,       "无效数据页");
    hashError.insert(ERR_INITCARD,               "请先清余额");
    hashError.insert(ERR_TRANSFILEFULL,          "TRANSACTION  FULL\nPLEASE SETTLE");
    hashError.insert(ERR_NOTVALIDPSAM,           "GPRS拨号出错");
    hashError.insert(ERR_NOTLOGON,               "LOGON FIRST");
    hashError.insert(ERR_NOTINIT,                "请先初始化");
    hashError.insert(ERR_INBLACKLIST,            "黑名单卡");
    hashError.insert(ERR_NOTINZONE,              "不满足区域条件");
    hashError.insert(ERR_NOTRANS,                "TRANS NOT EXIST");
    hashError.insert(ERR_TERMINALID,             "终端号比较出错");
    hashError.insert(ERR_CARDNOFUN,              "卡片不支持此功能");
    hashError.insert(ERR_OVERAMOUNT,             "余额超限");
    hashError.insert(ERR_ZONECODE,               "该发卡方不受理");
    hashError.insert(ERR_TRACENO,                "流水号不连续");
    hashError.insert(ERR_CARDUNTREAT,            "该卡表不受理");
    hashError.insert(ERR_LOCKEDCARD,             "已锁卡,不能使用");
    hashError.insert(ERR_CARDRECYCLE,            "回收卡无此功能");
    hashError.insert(ERR_EXPIREDATE,             "有效期已过或无效");
    hashError.insert(ERR_CODE,                   "密钥验证码错");
    hashError.insert(ERR_CARDWRITE,              "写卡失败!");
    hashError.insert(ERR_CARD,                   "卡片认证出错");
    hashError.insert(ERR_CARDSTATUS,             "未知卡状态");
    hashError.insert(ERR_NOTCASHIERLOGON,        "CASHIER LOGON FIRST");
    hashError.insert(ERR_ALREADYVOID,            "交易已撤销");
    hashError.insert(ERR_CARDSUPPORT,            "CARD NOT SUPPORT");
    hashError.insert(ERR_DIFPT,                  "不同平台卡");
    hashError.insert(ERR_ONLINECARD,             "联机卡须批量激活");
    hashError.insert(ERR_CARDTYPE,               "卡类型不匹配");
    hashError.insert(ERR_AMOUNTNOTZERO,          "余额不为零");
    hashError.insert(ERR_DISCOUNT,               "折扣不能大于100");
    hashError.insert(ERR_CARDMACERROR,           "累计充资认证错");
    hashError.insert(ERR_HANDNOPROC,             "不支持手工交易");
    hashError.insert(ERR_NOPSAM,                 "PSAM卡检测出错");
    hashError.insert(ERR_DIFFCARDTYPE,           "卡类型不一致");
    hashError.insert(ERR_AMOUNTERR,              "无效金额");
    hashError.insert(ERR_TRANSTIMEOUT,           "等待超时");
    hashError.insert(ERR_EXPNOFEE,               "无手续费信息");
    hashError.insert(ERR_EXPNOPARAM,             "无续期参数");
    hashError.insert(ERR_OFFLINECARD,          "非联机卡");
    hashError.insert(ERR_READMEMORY,             "读内存错");
    hashError.insert(ERR_WRITEMEMORY,            "写内存错");
    hashError.insert(ERR_CHECKSECRET,            "认证密钥错");
    hashError.insert(ERR_MODIFYSECRET,           "更改密钥失败");
    hashError.insert(ERR_CARDLOCK,               "卡应用被锁");
    hashError.insert(ERR_CARDCRC,                "卡校验位错");
    hashError.insert(ERR_MAXBLACK,               "黑名单空间已满");
    hashError.insert(ERR_MAXCARDTABLE,           "卡表已满");
    hashError.insert(ERR_MAXZONE,                "区位码表已满");
    hashError.insert(ERR_MAXRATE,                "扣率表已满");
    hashError.insert(ERR_CARDAMOUNTCRC,          "金额校验位错");
    hashError.insert(ERR_NOTSAFT,                "不满足安全状态");
    hashError.insert(ERR_FILENOTFOUND,           "文件未找到");
    hashError.insert(ERR_RECORDNOTFOUND,         "记录未找到");
    hashError.insert(ERR_NOTSUCHAPP,             "非本应用卡");
    hashError.insert(ERR_FULLCOUNT,              "计数器已满");
    hashError.insert(ERR_NOKEYINDEX,             "密钥未找到");
    hashError.insert(ERR_MACERROR,               "MAC ERROR");
    hashError.insert(ERR_CASENOTFULL,            "使用条件不满足");
    hashError.insert(ERR_UNKNOWOPERATCODE,       "未知操作类型");
    hashError.insert(ERR_DETAILDIF,              "卡明细比较错");
    hashError.insert(ERR_MEDIADIFFRENT,          "卡介质不匹配");
    hashError.insert(ERR_NOTDRIVERCARD,          "非司机卡");
    hashError.insert(ERR_PACKERROR,              "卡钱包无效");
    hashError.insert(ERR_DEVALCARD,              "无效卡");
    hashError.insert(ERR_TRANSERROR,             "卡交易失败");
    hashError.insert(ERR_NOTINSYS,               "非本系统卡");
    hashError.insert(ERR_DISTRUST,               "卡可疑交易");
    hashError.insert(ERR_KEYVERIFY,              "卡密钥验证失败");
    hashError.insert(ERR_NOTUNACT,               "卡已启用");
    hashError.insert(ERR_CASH_EXIST,             "操作员已存在");
    hashError.insert(ERR_CASH_NOTEXIST,          "CASHIER NOT EXIST");
    hashError.insert(ERR_CASH_PASS,              "WRONG PASSWORD");
    hashError.insert(ERR_CASH_FILEFULL,          "操作员已满");
    hashError.insert(ERR_CASH_NOTLOGON,          "无操作员卡");
    hashError.insert(ERR_CASH_ALREADYLOGON,      "柜员已签到");
    hashError.insert(ERR_UNKNOWTRANSTYPE,        "未知交易类型");
    hashError.insert(ERR_PINPADVERSION,          "PINPAD版本错");
    hashError.insert(ERR_MAXTESTAMOUNT,          "超过测试卡上限");
    hashError.insert(ERR_TESTTRANSFER,           "只能转入测试卡");
    hashError.insert(ERR_LESSBALANCE,            "金额不足");
    hashError.insert(ERR_KEYDRV,                 "按键错误");
    hashError.insert(ERR_ICCDRV,                 "卡槽驱动出错");
    hashError.insert(ERR_MFCDRV,                 "非接驱动出错");
    hashError.insert(ERR_MAGDRV,                 "磁条驱动错误");
    hashError.insert(ERR_UNKNOWNDRV,             "未知驱动错误");
    hashError.insert(ERR_TIMEOUT,                "超时");
    hashError.insert(ERR_ISO83,                  "8583 ERROR");
    hashError.insert(ERR_ISO8583_INVALIDVALUE,   "无效值");
    hashError.insert(ERR_ISO8583_INVALIDBIT,     "无效位");
    hashError.insert(ERR_ISO8583_INVALIDPACKLEN, "无效压缩长度");
    hashError.insert(ERR_ISO8583_OVERBITBUFLIMIT,"域缓冲超限");
    hashError.insert(ERR_ISO8583_BITNOTEXIST,    "域名不存在");
    hashError.insert(ERR_ISO8583_INVALIDPARAM,   "无效参数");
    hashError.insert(ERR_ISO8583_NODATAPACK,     "无压缩数据");
    hashError.insert(ERR_ISO8583_OVERBUFLIMIT,   "缓存超限");
    hashError.insert(ERR_ISO8583_INVALIDBITATTR, "域定义无效");
    hashError.insert(ERR_ISO8583_UNPACKDATA,     "未压缩数据");
    hashError.insert(ERR_ISO8583_SETBITLEN,      "SET BIT LEN");
    hashError.insert(ERR_ISO8583_COMPARE,        "比较出错");
    hashError.insert(ERR_ISO8583_MACERROR,       "ＭAC ERROR");
    hashError.insert(ERR_ISO8583_INVALIDLEN,     "长度无效");
    hashError.insert(ERR_VOIDDEPOSITAMOUNT,      "消费超限");
    hashError.insert(ERR_NOFUNCTION,             "功能未开通");
    hashError.insert(ERR_DIFFERENT_AMOUNT,       "金额不一致");
    hashError.insert(ERR_XFILE_WRITE,            "文件系统错误");
    hashError.insert(ERR_COMMUNICATION,          "通讯错误");
    hashError.insert(ERR_NO_SETTLE,              "无结算单");
    hashError.insert(ERR_SQLITE3,                "本地数据库出错");
    hashError.insert(ERR_SETTLE_UNBALANCE,                "SETTLE UNBALANCE");
    hashError.insert(ERR_UTIL_OVERBUFLIMIT,      "镜像存储出错");
    hashError.insert(ERR_MSG_INVALIDINDEX,       "索引无效");
    hashError.insert(ERR_MSG_INVALIDVALUE,       "数据无效");
    hashError.insert(ERR_RECYCLE,                "回收卡");
    hashError.insert(ERR_MAG_TRACKDATA,          "磁轨信息出错");
    hashError.insert(ERR_TRANS_CARDTYPE,		 "卡片类型不一致");
    hashError.insert(ERR_COMMS_SENDCHAR,         "数据发送出错");
    hashError.insert(ERR_COMMS_RECVCHAR,         "数据接收出错");
    hashError.insert(ERR_COMMS_PROTOCOL,         "协议出错");
    hashError.insert(ERR_COMMS_RECVBUFFOVERLIMIT,"接收数据超限");
    hashError.insert(ERR_COMMS_RECVTIMEOUT,      "接收超时");
    hashError.insert(ERR_COMMS_LRC,              "包校验位错");
    hashError.insert(ERR_COMMS_ERRTPDU,          "TPDU出错");
    hashError.insert(ERR_COMMS_NOMAC,            "没有MAC");
    hashError.insert(ERR_WORKSWITCH,             "操作模式不支持");
    hashError.insert(ERR_MUSTRETURN,             "此卡应回收");
    hashError.insert(ERR_SAMECARD,               "同卡不可移资");
    hashError.insert(ERR_TRANSEMPTY,             "交易流水为空");
    hashError.insert(ERR_SETTLE_FIRST,           "请先结算");
    hashError.insert(ERR_DIFFERENT_TERM,         "记录终端不一致");
    hashError.insert(ERR_DIFFERENT_TIME,         "记录时间不一致");
    hashError.insert(ERR_DIFFERENT_TRACE,        "记录流水不一致");
    hashError.insert(ERR_DIFFERENT_MONEY,        "记录金额不一致");
    hashError.insert(ERR_DIFFERENT_BALANCE,      "记录前金额不一致");
    hashError.insert(ERR_BAR_REPEAT,             "帐单重复");
    hashError.insert(ERR_CARDNOACT,              "卡片未启用");
    hashError.insert(ERR_TRANS,                  "TRANSACTION\nCANCELED");
}

void UIMsg::initalNoticeMsg()
{
    hashNotice.insert(MSG_NULL, "");
    hashNotice.insert(LOGONCREDIT_SUCCESS,  "收银员积分\n签到成功!");
    hashNotice.insert(PULL_CARD,"请拔出IC卡");
    hashNotice.insert(COLLECT_SUCCESS, "数据采集成功!");
    hashNotice.insert(PULLCARD_OUT,"请拔出转出卡 -->>");
    hashNotice.insert(TRY_AGAIN, "请再试一次!");
    hashNotice.insert(TIP_CASHNEED,  "请注意!\n该交易要收现金");
    hashNotice.insert(SENDSTATS_SUCCESS,"状态上送成功!");
    hashNotice.insert(ECHO_SUCCESS,  "回响测试成功!");
    hashNotice.insert(EMVDOWNLOAD_OK, "EMV参数成功!");
    hashNotice.insert(EMV_LOAD_OK,  "EMVDefaultLoad OK!");
    hashNotice.insert(CLEAR_SUCCESS,"清除成功!");
    hashNotice.insert(CLEAR_TRANS_SUCCESS, "交易清除成功!");
    hashNotice.insert(RS232_INITSUCCESS, "串口初始化成功!");
    hashNotice.insert(SAV_SUCCESS, "数据存储成功!");
    hashNotice.insert(TRANS_SUCCESS, "交易成功!");
    hashNotice.insert(DELETE_BIN_SUCCESSFULLY, "卡柄删除成功!");
    hashNotice.insert(ADD_BIN_SUCCESSFULLY, "卡柄增加成功!");
    hashNotice.insert(CLEAN_OK, "清除成功!");
    hashNotice.insert(RECOVER_OK, "恢复成功!");
    hashNotice.insert(TIP_ADJUSTMENT_OK,"帐务调整成功!");
    hashNotice.insert(ADD_USER_OK, "ADD CASHIER\nSUCCESS!");
    hashNotice.insert(DELETE_USER_OK,  "DELETE CASHIER\nSUCCESS!");
    hashNotice.insert(CHANGE_PASSWORD_OK, "PASSWORD CHANGED!");
    hashNotice.insert(PASSWD_RECOVERY_OK,  "密码恢复成功!");
    hashNotice.insert(TIME_OUT,  "超时!");
    hashNotice.insert(LOGON_SUCCESS,  "[LOGON] SUCCESS!");
    hashNotice.insert(TRANSACTION_SUCCESS,  "TRANSACTION SUCCESS!");
    hashNotice.insert(LOGOFF_SUCCESS, "签退成功!");
    hashNotice.insert(INITIAL_ONLINE_SUCCESS,   "主密钥载入成功!");
    hashNotice.insert(DOWNLOAD_INFORMATION_OK,   "下载参数成功!");
    hashNotice.insert(PASSWORD_MISMATCHING,   "PASSWORD MISMATCH");
    hashNotice.insert(INCOMPLETE_INFORMATION,   "FILL IN THE BLANK");
    hashNotice.insert(NO_PERMISSION,   "PERMISSION DENIED");
    hashNotice.insert(WRONG_EXP,   "WRONG EXP DATE");
}

void UIMsg::initalFileErrorMsg()
{
    hashFileError.insert(FILE_ERROR_INDEX,			"文件索引满");
    hashFileError.insert(FILE_ERROR_PARAM,			"文件参数有误");
    hashFileError.insert(FILE_ERROR_AREA,			"文件区设置失败");
    hashFileError.insert(FILE_ERROR_NO_EXIST,		"文件系统不存在");
    hashFileError.insert(FILE_ERROR_NOMORE_MEMORY,	"内存不足请先结算");

    hashFileError.insert(FILE_ERROR_WRITE,			"写文件失败");
    hashFileError.insert(FILE_ERROR_OPERTATE,		"文件操作有误");
    hashFileError.insert(FILE_ERROR_HANDLEOPEN,		"文件未打开");
    hashFileError.insert(FILE_ERROR_CURSOR,			"文件偏移错误");
    hashFileError.insert(FILE_ERROR_ATTR,			"文件属性错误");
    hashFileError.insert(FILE_ERROR_NAME,			"文件名错误");

    hashFileError.insert(FILE_ERROR_SYSTEM,			"文件系统出错");
}

void UIMsg::initalCommErrorMsg()
{
    hashCommError.insert(NET_PATTERN_ERROR,             "PATTERN ERROR");
    hashCommError.insert(NET_NO_IDLE_DIALOG,            "NO IDLE DIALOG");
    hashCommError.insert(NET_ID_ERROR,                  "错误句柄");
    hashCommError.insert(NET_ID_NOT_OPEN,               "句柄未打开");
    hashCommError.insert(NET_DIAL_CONNECT_ERROR,        "会话连接错误");
    hashCommError.insert(NET_DIAL_CONNECT_REJECT,       "会话连接拒绝");
    hashCommError.insert(NET_PARAM_ERROR,               "参数错误");
    hashCommError.insert(NET_IP_CATEGORY_ERROR,         "IP层错误");
    hashCommError.insert(NET_DATA_CHECK_ERROR,          "数据较验错误");
    hashCommError.insert(NET_NO_MATCH_DIALOG,           "无匹配的会话");
    hashCommError.insert(NET_TAG_ERROR,                 "标志错误");
    hashCommError.insert(NET_NO_MATCH_SERIES_NUMBER,    "无匹配的序列号");
    hashCommError.insert(NET_SEND_DATA_LEN_ERROR,       "发送数据长度错误");
    hashCommError.insert(NET_REVIEVE_OVERTIME,          "接收超时");
    hashCommError.insert(NET_NO_ANSWER,                 "数据无应答");
    hashCommError.insert(NET_DIAL_OVERTIME,             "网络连接超时");
    hashCommError.insert(NET_REVIEVE_DATA_LEN_ERROR,    "接收数据长度错误");
    hashCommError.insert(NET_NETCOMMS_VERSION,          "通讯版本错");
    hashCommError.insert(NET_NETCOMMS_OVERBUFLIMIT,     "缓存超限");
    hashCommError.insert(NET_NETCOMMS_INDEXERROR,       "密钥索引错");
    hashCommError.insert(NET_NETCOMMS_MACERROR,         "MAC错");
    hashCommError.insert(ERR_COMMS_UNKNOW,              "交易失败");
}

void UIMsg::initalHostErrorMsg()
{
    hashHostError.insert(HOSTERR_1, "Contact\n Card Issuer");
    hashHostError.insert(HOSTERR_3, "Format Error");
    hashHostError.insert(HOSTERR_5, " External Decline");
    hashHostError.insert(HOSTERR_12, " Invalid Transaction");
    hashHostError.insert(HOSTERR_13, " Merchant Limit\n Exceeded");
    hashHostError.insert(HOSTERR_14, " Invalid Track 2");
    hashHostError.insert(HOSTERR_30, " Invalid Format");
    hashHostError.insert(HOSTERR_41, " Lost Card");
    hashHostError.insert(HOSTERR_43, " Stolen Card");
    hashHostError.insert(HOSTERR_51 , "Insufficient Funds");
    hashHostError.insert(HOSTERR_54, " Expired Card");
    hashHostError.insert(HOSTERR_55 , "Invalid PIN\nPIN Tries Exceeded");
    hashHostError.insert(HOSTERR_58 , "Invalid \nProcessing Code");
    hashHostError.insert(HOSTERR_62 , "Invalid MAC");
    hashHostError.insert(HOSTERR_78 , "Original Request not Found");
    hashHostError.insert(HOSTERR_81 , "Wrong Format of\nCustomer Information Field");
    hashHostError.insert(HOSTERR_82 , "Prepaid Code\n Not Found");
    hashHostError.insert(HOSTERR_89 , "Invalid Terminal ID.");
    hashHostError.insert(HOSTERR_91 , "Destination\n Not Available");
    hashHostError.insert(HOSTERR_94 , "Duplicate Transmission");
    hashHostError.insert(HOSTERR_96 , "System Error");

}



void UIMsg::beepTwice()
{
    for(char i=0;i<2;i++)
        Os__beep();
}
