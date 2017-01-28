#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"

DialogMainMenu::DialogMainMenu(QWidget *parent) :
    ATCDialog(parent, "Main Menu", 600, 650),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->lineEditActiveScenario->setReadOnly(true);
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
    emit signalImportScenario();
}

void DialogMainMenu::on_buttonExportSimulation_clicked()
{
    emit signalExportScenario();
}

void DialogMainMenu::slotActiveScenarioPath(QString path)
{
    uiInner->lineEditActiveScenario->setText(path);
}
