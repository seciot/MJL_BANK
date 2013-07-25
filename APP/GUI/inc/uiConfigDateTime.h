#ifndef UICONFIGDATETIME_H_
#define UICONFIGDATETIME_H_
#include "ui.h"
#include <QAction>

extern "C"
{
#include "sand_time.h"
}

class UIConfigDateTime : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigDateTime(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigDateTime();

    QLabel *lbHead;

    QLabel *lbDate;
    QLabel *lbTime;
    QDateEdit *dateEdit;
    QTimeEdit *timeEdit;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

    unsigned char *qstring_auc(QString);

private slots:
    void setDateTime();

protected:
    void keyPressEvent(QKeyEvent *event);
private:
    QTimer *closeTimer;
public slots:
    void setAutoClose(int timeout);
    void slotQuitCfg();

};

#endif  
