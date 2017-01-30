#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"

DialogMainMenu::DialogMainMenu(QTime *startTime, QWidget *parent) :
    ATCDialog(parent, "Main Menu", 600, 650),
    startTime(startTime),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->lineEditActiveScenario->setReadOnly(true);
    uiInner->timeEditStart->setTime(*startTime);
}

DialogMainMenu::~DialogMainMenu()
{
    delete uiInner;
}

QTime DialogMainMenu::getSimStartTime()
{
    return QTime(uiInner->timeEditStart->time());
}

void DialogMainMenu::slotSetSimStartTime(QTime time)
{
    uiInner->timeEditStart->setTime(time);
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

void DialogMainMenu::on_buttonStart_clicked()
{
    emit signalStartSimulation();
}

void DialogMainMenu::on_buttonStop_clicked()
{
    emit signalStopSimulation();
}

void DialogMainMenu::on_buttonCloseMenu_clicked()
{
    emit closed();
    close();
}

void DialogMainMenu::slotActiveScenarioPath(QString path)
{
    uiInner->lineEditActiveScenario->setText(path);
}
