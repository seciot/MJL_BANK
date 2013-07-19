#ifndef UIINPUTPASSWORD_H_
#define UIINPUTPASSWORD_H_
#include "ui.h"
#include <QAction>

#include "transData.h"


class UIInputPassword : public QDialog
{
    Q_OBJECT
public:
    explicit UIInputPassword(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIInputPassword();

    QLabel *lbHead;

    QLabel *lbCashier;
    QLabel *lbPass;
    QLineEdit *leCashier;
    QLineEdit *lePass;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void startAuthorize();
    void slotQuitTrans();
    void slotFinishTrans();

    void resetLine();


signals:
    void sigLogInSuccess(UserType ut,QString ID);
    void sigQuitTrans();
    void sigFinishTrans();
};

#endif  
