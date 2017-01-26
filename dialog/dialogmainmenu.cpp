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

void DialogMainMenu::on_buttonNewSimulation_clicked()
{
    emit signalConstructDialogFlightNew();
}

void DialogMainMenu::on_buttonEditSimulation_clicked()
{
    emit signalConstructDialogFlightEdit();
}

void DialogMainMenu::on_buttonImportSimulation_clicked()
{
    //TO BE IMPLEMENTED
}

void DialogMainMenu::on_buttonExportSimulation_clicked()
{
    emit signalExportScenario();
}
