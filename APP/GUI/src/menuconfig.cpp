#include "menuconfig.h"
#include <QDebug>
#include "menuBase.h"

#include "frame.h"
#include "uiCashAdvance.h"
#include "uiCashDeposit.h"
#include "uiMenuPos.h"
#include "uiLogon.h"
#include "uiBalanceInquiry.h"
#include "uiRePrint.h"
#include "uiMenuConfig.h"
#include "uiP2PTransfer.h"
#include "uiLogon.h"

#include "print.h"
#include "global.h"

MenuBase *menu = NULL;

MenuConfig::MenuConfig()
{
    qDebug() << Q_FUNC_INFO;
}

MenuConfig::~MenuConfig()
{
    qDebug() << Q_FUNC_INFO;
}

void MenuConfig::createMenu()
{
    if(menu == NULL)
    {
        menu = new MenuBase(true);
        QObject::connect(menu, SIGNAL(signal_close()), Frame::mainEntryThread, SLOT(wakeUp()));
    }
}

void MenuConfig::mainMenu()
{
    MenuConfig::createMenu();

    menu->addMenu(QObject::tr("Cash Advance"), QObject::tr(":/icons/QCash/advance.png"), &MenuConfig::Cash_Advance_Click);
    menu->addMenu(QObject::tr("Cash Deposit"), QObject::tr(":/icons/QCash/deposit.png"), &MenuConfig::Cash_Deposit_Click);
    menu->addMenu(QObject::tr("Balance"), QObject::tr(":/icons/QCash/balance.png"), &MenuConfig::Balance_Inquiry_Click);
    menu->addMenu(QObject::tr("P2P Transfer"), QObject::tr(":/icons/QCash/p2p.png"), &MenuConfig::P2P_Click);
    menu->addMenu(QObject::tr("Settings"), QObject::tr(":/icons/QCash/settings.png"), &MenuConfig::Settings_Click);
    menu->addMenu(QObject::tr("Print"), QObject::tr(":/icons/QCash/print.png"), &MenuConfig::Print_Click);
    menu->addMenu(QObject::tr("Menu"), QObject::tr(":/icons/QCash/menu.png"), &MenuConfig::Pos_Menu_Click);
    menu->addMenu(QObject::tr("Logon"), QObject::tr(":/icons/QCash/logon.png"), &MenuConfig::Logon_Click);

    menu->setBackMenu(MenuConfig::mainMenu);
    menu->displayMenu(MenuBase::toolButton, QObject::tr("QCash"), ":/images/bg14.png", "white", 2, 2);
}

void MenuConfig::Cash_Advance_Click()
{
    qDebug() << Q_FUNC_INFO;

    UICashAdvance *uiCashAd = new UICashAdvance();
    uiCashAd->exec();
}

void MenuConfig::Cash_Deposit_Click()
{
    qDebug() << Q_FUNC_INFO;

    UICashDeposit *uiCashDp = new UICashDeposit();
    uiCashDp->exec();
}

void MenuConfig::Settle_Click()
{
    qDebug() << Q_FUNC_INFO;
}

void MenuConfig::Pos_Menu_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIMenuPos *uiPM = new UIMenuPos();
    uiPM->exec();
}

void MenuConfig::Balance_Inquiry_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIBalanceInquiry *uiBI = new UIBalanceInquiry();
    uiBI->exec();
}

void MenuConfig::Print_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIRePrint *uiRPint = new UIRePrint();
    uiRPint->exec();
}

void MenuConfig::Settings_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIMenuConfig *uiMC = new UIMenuConfig();
    uiMC->exec();
}

void MenuConfig::P2P_Click()
{
    qDebug() << Q_FUNC_INFO;

    UIP2PTransfer *uip2p = new UIP2PTransfer();
    uip2p->exec();
}

void MenuConfig::Logon_Click()
{
    qDebug() << Q_FUNC_INFO;

    UILogon *uiLogon = new UILogon();
    uiLogon->exec();
}
