#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"

DialogMainMenu::DialogMainMenu(QTime *time, bool simValid, bool simRunning, QWidget *parent) :
    ATCDialog(parent, "Main Menu", 600, 650),
    time(time),
    simulationValid(simValid),
    simulationRunning(simRunning),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->lineEditActiveScenario->setReadOnly(true);
    uiInner->timeEditStart->setTime(*time);

    if(simValid)
    {
        if(simRunning)
        {
            uiInner->buttonNewSimulation->setEnabled(false);
            uiInner->buttonEditSimulation->setEnabled(false);
            uiInner->buttonImportSimulation->setEnabled(false);
            uiInner->buttonExportSimulation->setEnabled(false);
            uiInner->timeEditStart->setEnabled(false);
            uiInner->buttonStart->setEnabled(false);
        }
        else
        {
            uiInner->buttonPause->setEnabled(false);
            uiInner->buttonStop->setEnabled(false);
        }
    }
    else
    {
        uiInner->buttonStart->setEnabled(false);
        uiInner->buttonPause->setEnabled(false);
        uiInner->buttonStop->setEnabled(false);

        uiInner->buttonEditSimulation->setEnabled(false);
        uiInner->buttonExportSimulation->setEnabled(false);
    }
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

void DialogMainMenu::slotSetFlagSimulationValid(bool simValid)
{
    if(simValid)
    {
        uiInner->buttonEditSimulation->setEnabled(true);
        uiInner->buttonExportSimulation->setEnabled(true);
        uiInner->buttonStart->setEnabled(true);
    }
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

void DialogMainMenu::on_buttonPause_clicked()
{
    emit signalPauseSimulation();

    uiInner->buttonNewSimulation->setEnabled(true);
    uiInner->buttonEditSimulation->setEnabled(true);
    uiInner->buttonImportSimulation->setEnabled(true);
    uiInner->buttonExportSimulation->setEnabled(true);

    uiInner->buttonStart->setEnabled(true);
    uiInner->buttonPause->setEnabled(false);
    uiInner->buttonStop->setEnabled(true);
}

void DialogMainMenu::on_buttonStop_clicked()
{
    emit signalStopSimulation();

    uiInner->timeEditStart->setTime(*time);

    uiInner->buttonNewSimulation->setEnabled(true);
    uiInner->buttonImportSimulation->setEnabled(true);
    uiInner->lineEditActiveScenario->setText("No scenario selected");
    uiInner->timeEditStart->setEnabled(true);
    uiInner->buttonStart->setEnabled(false);
    uiInner->buttonPause->setEnabled(false);
    uiInner->buttonStop->setEnabled(false);
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
