#ifndef UIPAYMENT_H_
#define UIPAYMENT_H_
#include "ui.h"
#include <QAction>

class UIPayment : public QDialog
{
    Q_OBJECT
public:
    explicit UIPayment(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~UIPayment();


    QLabel *lbHead;
    QLabel *lbHeadText;
    QScrollArea *scArea;
    QWidget *scWidget;
    QScrollBar *vBar;
    QScrollBar *hBar;
    int saveVValue;
//    QMouseEvent *saveEve;

    QLabel *lbSelVendor;
    QLabel *lbPayCardNum;
    QLabel *lbCHName;
    QLabel *lbCardExp;
    QLabel *lbPayerPIN;
    QLabel *lbInsuranceNum;
    QLabel *lbInstallNum;
    QLabel *lbInstallAmount;
    QLabel *lbMobileNum;

    QComboBox *cmbSelVendor;
    QLineEdit *lePayCardNum;
    QLineEdit *leCHName;
    QLineEdit *leCardExp;
    QLineEdit *lePayerPIN;
    QLineEdit *leInsuranceNum;
    QLineEdit *leInstallNum;
    QLineEdit *leInstallAmount;
    QLineEdit *leMobileNum;

    QPushButton *btnCancel;
    QPushButton *btnSubmit;

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
//    void mouseReleaseEvent(QMouseEvent *);
};

#endif  
