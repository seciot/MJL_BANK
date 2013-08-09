#include "menuBase.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QToolButton>
#include <QCommandLinkButton>
#include <QTimer>
#include "sand_lcd.h"

#define menuTimeOut     30

void gifStart(void)
{
    Os__gif_start((char*)"BigLoad.gif", 60, 80, 124, 124);
}

void gifStop(void)
{
    Os__gif_stop();
}

MenuBase::MenuBase(bool isMainMenu, QDialog *parent, Qt::WindowFlags f) :
    QDialog(parent, f)
{
    /*============== 初始化数据 ==============*/
    setObjectName("menuDialog");            //此处的设置，是为了重置背景图
    initData();
    /*============== 创建控件 ==============*/
    QLabel *lbTitle = new QLabel(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    buttonGroup = new QButtonGroup(this);
    QToolButton *tbPageUp = new QToolButton(this);
    QToolButton *tbPageDn = new QToolButton(this);
    QToolButton *tbClose = new QToolButton(this);
    QFont fontH("Helvetica",16,QFont::Bold);
    tbClose->setStyleSheet("color: rgb(255, 255, 255);	background-color: rgba(96, 96, 96, 50%);border-radius: 6px;");
    tbClose->setText(tr("Exit"));
    tbClose->setFont(fontH);
    tbClose->setMinimumHeight(30);
    tbClose->setMinimumWidth(150);
    QTimer *timer = new QTimer(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QVBoxLayout *scrollAreaLayout = new QVBoxLayout;
    buttonLayout = new QGridLayout(scrollArea);
    QHBoxLayout *hbLayout = new QHBoxLayout;
    /*============== 控件布局 ==============*/
    layout->addWidget(lbTitle);
    layout->addLayout(scrollAreaLayout);
    layout->addLayout(hbLayout);

    //    hbLayout->addWidget(tbPageUp);
    //    hbLayout->addStretch(0);
    //    hbLayout->addWidget(tbPageDn);
    //    hbLayout->addStretch(0);
    hbLayout->addWidget(tbClose);

    scrollAreaLayout->addWidget(scrollArea);
    /*============== 参数设置 ==============*/
    if(isMainMenu == false)
        setAttribute(Qt::WA_DeleteOnClose);
    setAutoFillBackground(true);

    layout->setContentsMargins(0, 30, 0, 5);
    scrollAreaLayout->setContentsMargins(5, 0, 5, 0);
    hbLayout->setContentsMargins(2, 0, 2, 0);

    lbTitle->setAlignment(Qt::AlignHCenter);
    lbTitle->setFont(QFont("Helvetica", 14, QFont::Bold));

    scrollArea->setObjectName("scrollArea");
    scrollArea->setStyleSheet("#scrollArea{border: 0px solid; border-radius:8px; background-color:rgba(0, 0, 0, 30);}");

    setButton(tbPageUp, true, tr("上一页"), ":/icons/page_first.png", QSize(32, 32));
    setButton(tbPageDn, true, tr("下一页"), ":/icons/page_last.png", QSize(32, 32));

    //    setButton(tbClose, true, tr("退出"), ":/icons/签退.png", QSize(32, 32));


    timer->setInterval(menuTimeOut * 1000);
    /*============== 信号连接 ==============*/
    connect(buttonGroup, SIGNAL(buttonClicked(int)), SLOT(clickMenu(int)));
    connect(tbClose, SIGNAL(clicked()), this, SLOT(closeMenu()));
    //    connect(timer, SIGNAL(timeout()), tbClose, SLOT(click()));   // 超时关闭
    connect(tbPageUp, SIGNAL(clicked()), this, SLOT(pageUp()));
    connect(tbPageDn, SIGNAL(clicked()), this, SLOT(pageDn()));

    connect(this, SIGNAL(signal_pageUpVisible(bool)), tbPageUp, SLOT(setVisible(bool)));
    connect(this, SIGNAL(signal_pageDnVisible(bool)), tbPageDn, SLOT(setVisible(bool)));
    connect(this, SIGNAL(signal_setTitle(QString)), lbTitle, SLOT(setText(QString)));
    connect(this, SIGNAL(signal_setTitleFontColor(QString)), lbTitle, SLOT(setStyleSheet(QString)));
    connect(this, SIGNAL(signal_timerStart()), timer, SLOT(start()));
    connect(this, SIGNAL(signal_timerStop()), timer, SLOT(stop()));
}

MenuBase::~MenuBase()
{
    qDebug() << Q_FUNC_INFO;

    if(NULL != buttonGroup)
    {
        const int btnCount = buttonGroup->buttons().count();
        for(int id = 0; id < btnCount; id++)
        {
            delete buttonGroup->button(id);
        }
        delete buttonGroup;
    }
    initData();
}

void MenuBase::keyPressEvent(QKeyEvent *event)
{
    qDebug() << Q_FUNC_INFO;

    if(iskeyPress == true)
    {
        iskeyPress = false;
    }
    else
    {
        iskeyPress = true;
        event->ignore();
    }
}

void MenuBase::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << Q_FUNC_INFO;

    if(iskeyPress == false)
        return;

    iskeyPress = false;
    signal_timerStart();
    int iKey =  event->key();

    if(iKey >= '1' && iKey <= '9')
    {
        clickMenu(iKey - '1');
    }
    else
    {
        switch(iKey)
        {
        case Qt::Key_Escape:
            closeMenu();
            break;
        case Qt::Key_F1:
            pageUp();
            break;
        case Qt::Key_F2:
            pageDn();
            break;
        default:
            event->ignore();
            break;
        }
    }
}

void MenuBase::setBgImage(const QString &_bgImage, const QString &_fontColor)
{
    if(styleSheet() != QString("#menuDialog{border-image:url(%1);}").arg(_bgImage))
    {
        fontColor = _fontColor;
        signal_setTitleFontColor(QString("color:%1").arg(fontColor));
        setStyleSheet(QString("#menuDialog{border-image:url(%1);}").arg(_bgImage));
    }
}

void MenuBase::closeMenu(void)
{
    qDebug() << Q_FUNC_INFO;

    signal_timerStop();

    //回退
    if(menuBackList.count() > 1)
    {
        menuFunc _menuFunc = menuBackList[menuBackList.count() - 2];
        if(_menuFunc)
        {
            menuBackList.takeLast();
            return (_menuFunc)();
        }
    }
    //退出
    else
    {
        //        close();
        hide();
        signal_close();
    }
}

void MenuBase::initData(void)
{
    maxRow = 0;         //行最大数
    maxCol = 0;         //列最大数
    maxMenu = 0;        //最大数
    currentPage = 0;    //当前页索引

    fontColor = "black";    //默认黑色字体

    isMenu = false;
    iskeyPress = false;
    menuList.clear();
    menuBackList.clear();

    buttonGroup = NULL;
}

void MenuBase::clickMenu(int _id)
{
    signal_timerStop();
    int iIndex = (currentPage * maxMenu) + _id;
    if(iIndex < menuList.count() && _id < maxMenu)
    {
        if(menuList[iIndex].click)
            (menuList[iIndex].click)();
    }
    signal_timerStart();
}

void MenuBase::setButton(QAbstractButton *_button, bool _enabled, const QString &_text, const QString &_icon)
{
    _button->setText(_text);
    _button->setIcon(QIcon(_icon));
    _button->setEnabled(_enabled);
}

void MenuBase::setButton(QAbstractButton *_button, bool _enabled, const QString &_text, const QString &_icon, const QSize &_iconSize)
{
    _button->setFocus();
    _button->setFont(QFont("Helvetica", 12));
    _button->setStyleSheet(QString("background-color:transparent; color:%1;font:Bold;").arg(fontColor));
    _button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    _button->setIconSize(_iconSize);
    setButton(_button, _enabled, _text, _icon);
}

void MenuBase::createMenu(MenuType _menuType, int _row, int _col)
{
    qDebug() << Q_FUNC_INFO;

    //如果当前 "行" 或 "列" 或 "类型" 不一致, 则重新创建
    if(_row != maxRow
            || _col != maxCol
            || _menuType != menuType)
    {
        //赋行、列
        maxRow = _row;
        maxCol = _col;
        maxMenu = maxRow * maxCol;

        //清容器指针
        const int btnCount = buttonGroup->buttons().count();
        for(int id = 0; id < btnCount; id++)
        {
            delete buttonGroup->button(id);
        }
        //创建按钮
        if(commandLinkButton == _menuType)
            createCommLinkButton(maxRow, maxCol, QSize(48, 48));
        else
            createToolButton(maxRow, maxCol, QSize(48, 48));
    }
}

void MenuBase::createToolButton(int _row, int _col, const QSize &_iconSize)
{
    qDebug() << Q_FUNC_INFO;

    menuType = toolButton;
    for(int iRow = 0, id = 0; iRow < _row; iRow++)
    {
        for(int iCol = 0; iCol < _col; iCol++, id++)
        {
            QToolButton *button = new QToolButton(this);
            buttonLayout->addWidget(button, iRow, iCol);
            buttonGroup->addButton(button, id);

            button->setAutoRaise(true);
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            setButton(button, false, NULL, "", _iconSize);
        }
    }
}

void MenuBase::createCommLinkButton(int _row, int _col, const QSize &_iconSize)
{
    qDebug() << Q_FUNC_INFO;

    menuType = commandLinkButton;
    for(int iRow = 0, id = 0; iRow < _row; iRow++)
    {
        for(int iCol = 0; iCol < _col; iCol++, id++)
        {
            QCommandLinkButton *button = new QCommandLinkButton(this);
            buttonLayout->addWidget(button, iRow, iCol, Qt::AlignLeft);
            buttonGroup->addButton(button, id);

            setButton(button, false, NULL, "", _iconSize);
            button->setFlat(true);
        }
    }
}

void MenuBase::loadMenu(int _pageIndex)
{
    qDebug() << Q_FUNC_INFO;
    int iPage = 0;

    currentPage = _pageIndex;
    iPage = currentPage * maxMenu;

    signal_timerStart();
    //翻页键Visible设置
    if(iPage == 0)
    {
        signal_pageUpVisible(false);
        if(maxMenu >= menuList.count())
            signal_pageDnVisible(false);
        else
            signal_pageDnVisible(true);
    }
    else if((iPage + maxMenu) >= menuList.count())
    {
        signal_pageUpVisible(true);
        signal_pageDnVisible(false);
    }
    else
    {
        signal_pageUpVisible(true);
        signal_pageDnVisible(true);
    }

    if(menuList.count())
    {
        for(int i = 0; i < maxMenu; i++)
        {
            setButton(buttonGroup->button(i), false, NULL, "");
            if((iPage + i) < menuList.count())
            {
                setButton(buttonGroup->button(i), true,
                          QString("%1").arg(menuList[iPage + i].title),
                          menuList[iPage + i].icon);
            }
        }
    }
}

void MenuBase::pageDn(void)
{
    qDebug() << Q_FUNC_INFO;

    if(((currentPage + 1) * maxMenu) < menuList.count())
    {
        currentPage++;
        loadMenu(currentPage);
    }
}

void MenuBase::pageUp(void)
{
    qDebug() << Q_FUNC_INFO;

    if(currentPage > 0)
    {
        currentPage--;
        loadMenu(currentPage);
    }
}

void MenuBase::addMenu(const QString &_caption, const QString &_icon, menuFunc _click)
{
    if(true == isMenu)
    {
        menuList.clear();
        isMenu = !isMenu;
    }

    _Menu menu;
    menu.title = _caption;
    menu.icon = _icon;
    menu.click = _click;

    menuList << menu;
}

void MenuBase::addMenu(bool isHid, const QString &_caption, const QString &_icon, menuFunc _click)
{
    if(false == isHid)
        return addMenu(_caption, _icon, _click);
}

void MenuBase::setBackMenu(menuFunc _click)
{
    if(menuBackList.indexOf(_click) < 0)
        menuBackList << _click;
}

void MenuBase::displayMenu(MenuType _menuType, const QString &_title, const QString &_bgImage, const QString &_fontColor, int _row, int _col)
{
    qDebug() << Q_FUNC_INFO;

    gifStart();
    isMenu = true;                          //置已有数据
    setBgImage(_bgImage, _fontColor);       //置背景及字体颜色
    signal_setTitle(_title);                //置标题信息
    createMenu(_menuType, _row, _col);      //创建菜单
    loadMenu(0);                            //载入菜单
    gifStop();
    if(isHidden() == true)
        showFullScreen();
}
