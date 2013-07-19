#ifndef UIUSERMANAGER_H_
#define UIUSERMANAGER_H_
#include "ui.h"
#include <QAction>
#include "uiInputPassword.h"

class UIUserManager : public QDialog
{
    Q_OBJECT
public:
    explicit UIUserManager(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIUserManager();

    QLabel *lbHead;

    QLabel *lbUserId;
    QLabel *lbUserType;
    QLabel *lbUserNewPass;
    QLabel *lbUserRePass;

    QLineEdit *leUserId;
    QLabel *lbUserTypeText;
    QLabel *lbNotice;
    QLineEdit *leUserNewPass;
    QLineEdit *leUserRePass;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:
    UIInputPassword *uiIP;
    QThread *passThread;


protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void slotAllowEdit(UserType,QString ID);
    void slotProcessUserManager();
    void slotQuitTrans();

    void setPasswdWidgetLen(int);
    void quitFromInputPass();

signals:
    void sigLogInSuccess();
    void sigQuitTrans();
};

#endif  
