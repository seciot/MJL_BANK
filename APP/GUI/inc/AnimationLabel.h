#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H


#include <QObject>
#include <QDebug>
#include <QLabel>
#include <QList>
#include <QPixmap>
#include <QTimer>


class AnimationLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AnimationLabel(QWidget *parent = 0);
    ~AnimationLabel();
    void addPixmap(const QPixmap &pixmap);
    void ClearPixmap();


    
public slots:
    void start();
    void stop();
private slots:
    void timer();
private:
    QTimer *m_timer;
    int m_index;
    QList<QPixmap> m_pixmapList;
};

#endif // ANIMATIONLABEL_H
