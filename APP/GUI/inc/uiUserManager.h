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
    UIInputPassword *uiInPass;
    QThread *passThread;

    QTimer *closeTimer;

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void slotAllowEdit(UserType,QString ID);
    void slotProcessUserManager();

    void setPasswdWidgetLen(int);
    void quitFromInputPass();

    void setAutoClose(int timeout);
    void slotQuitMenu();


signals:
    void sigLogInSuccess();
    void sigQuitTrans();
};

#endif  
