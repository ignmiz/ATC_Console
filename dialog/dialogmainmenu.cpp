#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"

DialogMainMenu::DialogMainMenu(QWidget *parent) :
    ATCDialog(parent, "Main Menu", 640, 480),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();
}

DialogMainMenu::~DialogMainMenu()
{
    delete uiInner;
}

void DialogMainMenu::on_buttonTest_clicked()
{
    emit signalConstructDialogFlight();
}
