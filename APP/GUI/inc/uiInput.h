#ifndef UIINPUT_H_
#define UIINPUT_H_
#include "ui.h"
#include <QAction>


class UIInput : public QDialog
{
    Q_OBJECT
public:


    explicit UIInput(QString title,QString showMsg,QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    explicit UIInput(QString title,QString showMsg,QString regExp,int length, QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);

    ~UIInput();

    QLabel *lbHead;

    QLabel *lbMsg;
    QLineEdit *leInput;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;


    static int getInt(QString title,QString showMsg,QString regExp,int legnth,bool *ok);
    static QString getText(QString title,QString showMsg,QString regExp,bool *ok);
    static bool getConfirm(QString title,QString showMsg);

    static bool flagFinish;
    static QString inputedText;

private:

private slots:

    void slotCancel();
    void slotSubmit();
    void slotConfirm();


protected:
    void keyPressEvent(QKeyEvent *event);

public slots:

signals:
};

#endif  
