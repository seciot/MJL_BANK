#ifndef UI_H
#define UI_H
#include <QApplication>
#include <QComboBox>
#include <QCommandLinkButton>
#include <QCompleter>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QGridLayout>
#include <QHashIterator>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <qnamespace.h>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QToolButton>
#include <QAbstractButton>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QScrollArea>
#include <QScrollBar>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QMovie>
#include <QKeyEvent>
#include <QHash>
#include <QThread>
#include <QPropertyAnimation>
#include <QTableWidget>
#include <QHeaderView>
#include <QVector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "uiMsg.h"
#include "uiInput.h"
#include "MyIpAddrEdit.h"
#include "eventProcess.h"

//Display UI
#define     FRAME420_THVALUE    25
#define		FRAME420_WIDTH		qApp->desktop()->screen(0)->width()
#define		FRAME420_HEIGHT		qApp->desktop()->screen(0)->height()-25
#define     DEVICE420_HEIGHT    qApp->desktop()->screen(0)->height()
#define     RIGHT_WHITE     40

// StyleSheet
#define BTN_CANCEL_STYLE " QPushButton {color: rgb(255, 255, 255);	background-color: rgb(0, 153, 255);border-radius: 6px;}  QPushButton:pressed {color: rgb(0, 0, 0);	background-color: rgb(0, 50, 255);border-radius: 6px;}"
#define BTN_SUBMIT_STYLE " QPushButton {color: rgb(0, 0, 0);	background-color: rgb(0, 255, 0);border-radius: 6px;}  QPushButton:pressed {color: rgb(255, 255, 255);	background-color: rgb(0, 100, 0);border-radius: 6px;}"
#define BTN_MENU_CANCEL_STYLE "QPushButton {color: rgb(255, 255, 255);	background-color: rgb(139,139,122);border-radius: 6px;}  QPushButton:pressed {color: rgb(0, 0, 0);	background-color: rgb(139,139,122);border-radius: 6px;}"
#define BTN_MENU_STYLE " QPushButton {color: rgb(255, 255, 255);	background-color: rgb(0, 153, 255);border-radius: 6px;}  QPushButton:pressed {color: rgb(0, 0, 0);	background-color: rgb(0, 50, 255);border-radius: 6px;}"
#define HEAD_STYLE "background-color: rgb(220, 220, 220);"
#define CHECK_BOX_STYLE "QCheckBox::indicator:unchecked {image: url(:/styles/checkBox.png); }QCheckBox::indicator:unchecked:hover {image: url(:/styles/checkBox_hover.png);    }QCheckBox::indicator:unchecked:pressed {image: url(:/styles/checkBox_pressed.png);    }QCheckBox::indicator:unchecked:focus {image: url(:/styles/checkBox_focus.png);    }QCheckBox::indicator:unchecked:focus:pressed {image: url(:/styles/checkBox_pressed.png);    }QCheckBox::indicator:checked {image: url(:/styles/checkBox_checked.png);    }QCheckBox::indicator:checked:hover {image: url(:/styles/checkBox_checked_hover.png);    }QCheckBox::indicator:checked:pressed {image: url(:/styles/checkBox_checked_pressed.png);    }QCheckBox::indicator:checked:focus {image: url(:/styles/checkBox_checked_focus.png);    }QCheckBox::indicator:checked:focus:pressed {image: url(:/styles/checkBox_checked_pressed.png);    }QCheckBox::indicator:disabled {image: url(:/styles/checkBox_disabled.png);    }QCheckBox::indicator:checked:disabled {image: url(:/styles/checkBox_checked_disabled.png);    }"
#define COMBO_BOX_STYLE "QComboBox {border-image: url(:/styles/normalButton.png) 0 8 2 8 stretch;border-width: 0x 8px 2px 8px;}QComboBox:hover {border-image: url(:/styles/normalButton_hover.png) 2 8 2 8 stretch;border-width: 2px 8px 2px 8px;}QComboBox:on {border-image: url(:/styles/normalButton_pressed.png) 2 8 2 8 stretch;border-width: 2px 8px 2px 8px;}QComboBox:focus {border-image: url(:/styles/normalButton_focus.png) 2 8 2 8 stretch;border-width: 2px 8px 2px 8px;}QComboBox:focus:hover {border-image: url(:/styles/normalButton_hover.png) 2 8 2 8 stretch;border-width: 2px 8px 2px 8px;}QComboBox:disabled {border-image: url(:/styles/normalButton_disabled.png) 2 8 2 8 stretch;border-width: 2px 8px 2px 8px;}QComboBox::down-arrow {margin-left: 5px;width: 14px;height: 12px;image: url(:/styles/arrowDown.png);}QComboBox::downarrow:hover {image: url(:/styles/arrowDown_hover.png);}QComboBox::downarrow:pressed {image: url(:/styles/arrowDown_pressed.png);}QComboBox::downarrow:focus {image: url(:/styles/arrowDown_focus.png);}QComboBox::downarrow:disabled {image: url(:/styles/arrowDown_disabled.png);}QComboBox::editable {border-image: url(:/styles/lineEditBg.png) 3 3 3 3 stretch;border-width: 3px 3px 3px 3px;}QComboBox::editable:hover {border-image: url(:/styles/lineEditBg_hover.png) 3 3 3 3 stretch;border-width: 3px 3px 3px 3px;}QComboBox::editable:pressed {border-image: url(:/styles/lineEditBg_pressed.png) 3 3 3 3 stretch;border-width: 3px 3px 3px 3px;}QComboBox::editable:disabled {border-image: url(:/styles/lineEditBg_disabled.png) 3 3 3 3 stretch;border-width: 3px 3px 3px 3px;}QComboBox::drop-down {border: 0px}QComboBox QAbstractItemView {padding: 2px;border-top-color: #fdfdfd;border-bottom-color: #cfcecc;border-left-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fdfdfd, stop: 0.3 #e3e3e1, stop: 0.7 #e3e3e1, stop: 1 transparent);border-right-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fdfdfd, stop: 0.3 #e3e3e1, stop: 0.7 #e3e3e1, stop: 1 transparent);background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e9e8e8, stop: 1 #cfcecc);border-style: solid;border-width: 1px;}"
#define SCROLL_VERTICAL_STYLE "QScrollBar:vertical {border-image: url(:/styles/scrollbar_bg.png) 6 0 6 0 stretch;width: 15px;margin: 15px 0 15px 0;padding-top: -6px;padding-bottom: -6px;border-width: 6px 0px 6px 0px;}QScrollBar::handle:vertical {border-image: url(:/styles/scrollbar_body.png) 6 0 6 0 stretch;min-height: 15px;border-width: 6px 0px 6px 0px;}QScrollBar::handle:vertical:hover {border-image: url(:/styles/scrollbar_body_hover.png) 6 0 6 0 stretch;border-width: 6px 0px 6px 0px;}QScrollBar::handle:vertical:pressed {border-image: url(:/styles/scrollbar_body_pressed.png) 6 0 6 0 stretch;border-width: 6px 0px 6px 0px;}QScrollBar::sub-line:vertical {border: 0px;background: none;height: 15px;subcontrol-position: top;subcontrol-origin: margin;}QScrollBar::add-line:vertical {border: 0px;background: none;height: 15px;subcontrol-position: bottom;subcontrol-origin: margin;}QScrollBar::up-arrow:vertical {background-image: url(:/styles/scrollbar_upArrow.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::up-arrow:vertical:hover {background-image: url(:/styles/scrollbar_upArrow_hover.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::up-arrow:vertical:pressed {background-image: url(:/styles/scrollbar_upArrow_pressed.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::down-arrow:vertical {background-image: url(:/styles/scrollbar_downArrow.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::down-arrow:vertical:hover {background-image: url(:/styles/scrollbar_downArrow_hover.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::down-arrow:vertical:pressed {background-image: url(:/styles/scrollbar_downArrow_pressed.png);background-repeat: no repeat;width: 15px;height: 15px;}QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;} "
// RegExp
#define REGEX_NUMBER "^[0-9]\\d*$"


#endif
