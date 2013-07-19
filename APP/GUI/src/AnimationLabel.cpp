#include "AnimationLabel.h"


/*
自动动态滚动图片QLabel类 
作者:Dean.Pan
日期:2013/1/9
*/


AnimationLabel::AnimationLabel(QWidget *parent) :
    QLabel(parent)
{
	qDebug() << Q_FUNC_INFO;
	m_pixmapList.clear();
	this->setAttribute(Qt::WA_DeleteOnClose);  
    resize(parent->size());
    m_timer = new QTimer(this);
    m_timer->setInterval(700);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timer()));
}

AnimationLabel::~AnimationLabel()
{
    qDebug() <<"delete:: "<< Q_FUNC_INFO;
}

void AnimationLabel::addPixmap(const QPixmap &pixmap)
{
    m_pixmapList.push_back(pixmap);
}

void AnimationLabel::ClearPixmap()
{
    m_pixmapList.clear();
}

void AnimationLabel::start()
{
    if(m_pixmapList.isEmpty())
        return;
    m_index = -1;
    m_timer->start();
}

void AnimationLabel::stop()
{
    m_timer->stop();
}

void AnimationLabel::timer()
{
    m_index++;
    if(m_index > m_pixmapList.count() -1)
        m_index = 0;
    setPixmap(m_pixmapList.at(m_index));
}
