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

// RegExp
#define REGEX_NUMBER "^[0-9]\\d*$"


#endif
