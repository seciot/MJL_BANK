#ifndef UICONFIGPAY_H_
#define UICONFIGPAY_H_
#include "ui.h"
#include <QAction>


class UIConfigPay : public QDialog
{
    Q_OBJECT
public:
    explicit UIConfigPay(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIConfigPay();


    QLabel *lbHead;
    QLabel *lbSeHead;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;
//    QMouseEvent *saveEve;

    QLabel *lbVendorID;
    QLabel *lbVendorName;
    QLabel *lbVendorAccNo;
    QLabel *lbText1;
    QLabel *lbText2;
    QLabel *lbText3;
    QLabel *lbText4;
    QLabel *lbText5;

    QLineEdit *leVendorID;
    QLineEdit *leVendorName;
    QLineEdit *leVendorAccNo;
    QLineEdit *leText1;
    QLineEdit *leText2;
    QLineEdit *leText3;
    QLineEdit *leText4;
    QLineEdit *leText5;
    QCheckBox *chkText1;
    QCheckBox *chkText2;
    QCheckBox *chkText3;
    QCheckBox *chkText4;
    QCheckBox *chkText5;


    QPushButton *btnCancel;
    QPushButton *btnSubmit;
private:
protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif  
