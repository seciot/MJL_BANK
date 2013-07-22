#ifndef UIMSG_H_
#define UIMSG_H_
#include "ui.h"
#include <QAction>

#include "msg.h"

class UIMsg : public QDialog
{
    typedef enum ERROR_TYPE
    {
        NORMAL_ERROR     = 0,
        NOTICE_ERROR    = 1,
        FILE_ERROR    = 2,
        COMM_ERROR    = 3,
        HOST_ERROR    = 4
    }ErrorType;

    Q_OBJECT
public:
    explicit UIMsg(ErrorType errortype,long errorCode,QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    explicit UIMsg(ErrorType errortype,unsigned char ucErrorCode,QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    explicit UIMsg(ErrorType errortype,QString errMsg, bool needBeep,QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIMsg();

    QLabel *lbHead;

    QLabel *lbMsg;
    QLabel *lbReturnCodeLabel;
    QLabel *lbReturnCode;

    QPushButton *btnSubmit;

    void gifRefresh(char *name, int x, int y, int w, int h);

    static void showCombineErrMsg(const ErrIndex ucErrorCode);
    static void showErrMsg(const ErrIndex ucErrorCode);
    static void showErrMsg(QString);
    static void showFileErrMsg(const FileErrIndex ucErrorCode);
    static void showCommErrMsg(const CommsErrIndex ucErrorCode);
    static void showHostErrMsg(const HostErrIndex longErrorCode);
    static void showNoticeMsg(const MsgTabIndex index);
    static void showNoticeMsg(QString);

    static void showCombineErrMsgWithAutoClose(const ErrIndex ucErrorCode,int timeout);
    static void showErrMsgWithAutoClose(const ErrIndex ucErrorCode,int timeout);
    static void showErrMsgWithAutoClose(QString,int timeout);
    static void showFileErrMsgWithAutoClose(const FileErrIndex ucErrorCode,int timeout);
    static void showCommErrMsgWithAutoClose(const CommsErrIndex ucErrorCode,int timeout);
    static void showHostErrMsgWithAutoClose(const HostErrIndex longErrorCode,int timeout);
    static void showNoticeMsgWithAutoClose(const MsgTabIndex index,int timeout);
    static void showNoticeMsgWithAutoClose(QString,int);

private:
    QTimer *noticeTimer;

    /// ----------错误代码大全-------------///
    QHash<ErrIndex,QString> hashError;
    /// ----------提示信息大全-------------///
    QHash<MsgTabIndex,QString> hashNotice;
    /// ----------文件错误大全-------------///
    QHash<FileErrIndex,QString> hashFileError;
    /// ----------通信错误大全-------------///
    QHash<CommsErrIndex,QString> hashCommError;
    /// ----------后台错误大全-------------///
    QHash<HostErrIndex,QString> hashHostError;

    void initalErrorMsg();
    void initalNoticeMsg();
    void initalFileErrorMsg();
    void initalCommErrorMsg();
    void initalHostErrorMsg();

    void setAutoClose(int timeout);
private slots:
    void thisClose();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:

    void beepTwice();

signals:
};

#endif  
