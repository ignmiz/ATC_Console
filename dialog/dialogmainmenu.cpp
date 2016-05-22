#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"
#include "mainwindow.h"

DialogMainMenu::DialogMainMenu(QWidget *parent) :
    ATCDialog(parent, "Main Menu", 640, 480),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogMainMenu::~DialogMainMenu()
{
    dynamic_cast<MainWindow*>(getParentWindowAdress())->setFlagDialogMainMenuExists(false);
    delete uiInner;
}
