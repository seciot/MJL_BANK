#ifndef _MENUCONFIG_H
#define _MENUCONFIG_H

class MenuConfig
{
public:
    explicit MenuConfig();
    ~MenuConfig();

    static void createMenu();
    static void mainMenu();

    static void Settle_Click();
    static void Cash_Advance_Click();
    static void Cash_Deposit_Click();
    static void Pos_Menu_Click();
    static void Balance_Inquiry_Click();
    static void Print_Click();
    static void Settings_Click();
    static void P2P_Click();
    static void Logon_Click();

};


#endif
