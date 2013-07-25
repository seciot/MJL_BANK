#ifndef UIREPORTSUMMARY_H_
#define UIREPORTSUMMARY_H_
#include "ui.h"
#include <QAction>

class UIReportSummary : public QDialog
{
    Q_OBJECT
public:
    explicit UIReportSummary(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIReportSummary();

    QLabel *lbHead;

    QTableWidget *tbSummary;

    QPushButton *btnCancel;
private:

    QTimer *closeTimer;
protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void slotQuitMenu();

    void setTransTotal();
private slots:

    void restartTimeOut();
    void setAutoClose(int timeout);


signals:
};

#endif  
