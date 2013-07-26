#ifndef UIREPORTDETAIL_H_
#define UIREPORTDETAIL_H_
#include "ui.h"
//#include "widgetReport.h"

#include <QAction>

class UIReportDetail : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportDetail(int type=0,QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportDetail();

    QLabel *lbHead;

    QTableWidget *tbDetailList;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:
    QTimer *closeTimer;
    bool FLAG_NEEDVOID;
protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void slotSetDetailList(QString transType, QString cardNo, QString amount, QString refNo, QString apprNo, QString operatorNo);
    void slotSetFun(QString);
    void slotFunClicked();
    void slotQuitMenu();
private slots:
    void restartTimeOut();
    void setAutoClose(int timeout);

signals:
    void sigFun();
};

#endif  
