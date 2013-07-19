#ifndef MENUBASE_H_
#define MENUBASE_H_

#include <QDialog>
#include <QGridLayout>
#include <QButtonGroup>
#include <QResizeEvent>

typedef void (*menuFunc)();

class MenuBase : public QDialog
{
    Q_OBJECT

    struct _Menu
    {
        QString     title;
        QString     icon;
        menuFunc    click;
    };
public:
    enum MenuType
    {
        toolButton              = 0,
        commandLinkButton
    };

    explicit MenuBase(bool isMainMenu, QDialog *parent = 0, Qt::WindowFlags f = 0);
    ~MenuBase();

    void addMenu(const QString &_caption, const QString &_icon, menuFunc _click);
    void addMenu(bool isHid, const QString &_caption, const QString &_icon, menuFunc _click);
    void setBackMenu(menuFunc _click);
    void displayMenu(MenuType _menuType, const QString &_title, const QString &_bgImage, const QString &_fontColor, int _row, int _col);
private:
    int maxMenu;        //最大数
    int maxRow;         //行最大数
    int maxCol;         //列最大数
    int currentPage;    //当前页索引

    bool isMenu;
    bool iskeyPress;

    QString fontColor;              //字体颜色
    MenuType menuType;              //菜单类型
    QList<_Menu> menuList;          //菜单列表
    QList<menuFunc> menuBackList;
    QButtonGroup *buttonGroup;
    QGridLayout *buttonLayout;
private:
    void initData(void);
    void setBgImage(const QString &_bgImage, const QString &_fontColor);
    void setButton(QAbstractButton *_button, bool _enabled, const QString &_text, const QString &_icon);
    void setButton(QAbstractButton *_button, bool _enabled, const QString &_text, const QString &_icon, const QSize &_iconSize);
    void createMenu(MenuType _menuType, int _row, int _col);
    void createToolButton(int _row, int _col, const QSize &_iconSize);
    void createCommLinkButton(int _row, int _col, const QSize &_iconSize);
    void loadMenu(int _pageIndex);
private slots:
    void closeMenu(void);
    void clickMenu(int _id);
    void pageDn(void);
    void pageUp(void);
signals:
    void signal_setTitle(const QString);
    void signal_setTitleFontColor(const QString);
    void signal_pageDnVisible(bool);
    void signal_pageUpVisible(bool);
    void signal_close(void);
    void signal_timerStop(void);
    void signal_timerStart(void);
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif /* MENUBASE_H_ */
