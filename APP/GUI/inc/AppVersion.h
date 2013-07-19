#ifndef APPVERSION_H_
#define APPVERSION_H_
#include <QDialog>
#include <QKeyEvent>

class AppVersion : public QDialog
{
public:
    explicit AppVersion(QDialog *parent = 0,Qt::WindowFlags f = Qt::FramelessWindowHint);
    ~AppVersion();
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif  
