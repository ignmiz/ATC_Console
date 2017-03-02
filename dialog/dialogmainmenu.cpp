#include "dialogmainmenu.h"
#include "ui_dialogmainmenu.h"

DialogMainMenu::DialogMainMenu(ATCPaths *paths, QTime *time, bool simValid, bool simRunning, bool dataLogged, QWidget *parent) :
    ATCDialog(parent, "Main Menu", 600, 650),
    paths(paths),
    time(time),
    simulationValid(simValid),
    simulationRunning(simRunning),
    dataLogged(dataLogged),
    uiInner(new Ui::DialogMainMenu)
{
    uiInner->setupUi(this);
    windowSetup();

    uiInner->lineEditActiveScenario->setReadOnly(true);
    uiInner->lineEditLogDir->setReadOnly(true);
    uiInner->timeEditStart->setTime(*time);

    uiInner->checkBoxLog->setChecked(dataLogged);
    uiInner->lineEditLogDir->setEnabled(dataLogged);
    uiInner->buttonLogDir->setEnabled(dataLogged);
    uiInner->lineEditLogDir->setText(paths->DATA_LOG_DIR_PATH);

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

            uiInner->checkBoxLog->setEnabled(false);
            uiInner->lineEditLogDir->setEnabled(false);
            uiInner->buttonLogDir->setEnabled(false);
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

void DialogMainMenu::setLogPath(QString path)
{
    uiInner->lineEditLogDir->setText(path);
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
    emit signalDataLogged(dataLogged, uiInner->lineEditLogDir->text());
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

    uiInner->checkBoxLog->setEnabled(true);
    uiInner->lineEditLogDir->setEnabled(true);
    uiInner->buttonLogDir->setEnabled(true);
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

    uiInner->checkBoxLog->setEnabled(true);
    uiInner->lineEditLogDir->setEnabled(true);
    uiInner->buttonLogDir->setEnabled(true);
}

void DialogMainMenu::on_buttonCloseMenu_clicked()
{
    emit signalDataLogged(dataLogged, uiInner->lineEditLogDir->text());
    emit closed();
    close();
}

void DialogMainMenu::on_checkBoxLog_clicked(bool checked)
{
    uiInner->lineEditLogDir->setEnabled(checked);
    uiInner->buttonLogDir->setEnabled(checked);
    dataLogged = checked;
}

void DialogMainMenu::on_buttonLogDir_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Export to..."), paths->DATA_LOG_DIR_PATH);
    if(dirPath.isEmpty()) return;

    uiInner->lineEditLogDir->setText(dirPath);
}

void DialogMainMenu::slotActiveScenarioPath(QString path)
{
    uiInner->lineEditActiveScenario->setText(path);
}


