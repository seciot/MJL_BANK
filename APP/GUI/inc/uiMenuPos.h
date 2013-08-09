#ifndef UIMENUPOS_H_
#define UIMENUPOS_H_
#include "ui.h"


class UIMenuPos : public QDialog
{
    Q_OBJECT
public:
    explicit UIMenuPos(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIMenuPos();

    QLabel *lbMenu;
    QLabel *lbMenuIndexText;
    QPushButton *btnTrans;
    QPushButton *btnConfig;
    QPushButton *btnBatch;
    QPushButton *btnReceipt;
    QPushButton *btnSettle;
    QPushButton *btnCashierManager;
    QPushButton *btnReport;
    QPushButton *btnParDown;

    QPushButton *btnPageOneCancel;
    QPushButton *btnPageTwoCancel;
    QPushButton *btnPageOneNext;
    QPushButton *btnPageTwoBack;

    QStackedWidget *stackWidget;
    QWidget *pageOne;
    QWidget *pageTwo;
    QWidget *pageThree;

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void pageOneNextClicked();
    void pageTwoBackClicked();

    void showTransMenu();
    void showConfigMenu();
    void showCashierManager();
    void showReport();
    void showSettle();
    void Param_Down_Click();

private:
    static void styleWidget(QWidget * btn,int iFontSize=13);
    QTimer *closeTimer;

public slots:
    void setAutoClose(int timeout);
    void slotQuitMenu();
};

#endif  
