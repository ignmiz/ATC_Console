#ifndef DIALOGMAINMENU_H
#define DIALOGMAINMENU_H

#include "atcdialog.h"
#include "atcpaths.h"
#include "atcsimulation.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class DialogMainMenu;
}

class DialogMainMenu : public ATCDialog
{
    Q_OBJECT

public:
    explicit DialogMainMenu(ATCPaths *paths, QTime *time, bool simValid, bool simRunning, bool dataLogged, QWidget *parent = 0);
    ~DialogMainMenu();

    QTime getSimStartTime();

    void setLogPath(QString path);

signals:
    void signalConstructDialogFlightNew();
    void signalConstructDialogFlightEdit();
    void signalImportScenario();
    void signalExportScenario();

    void signalStartSimulation();
    void signalPauseSimulation();
    void signalStopSimulation();

    void signalDataLogged(bool flag, QString path);

public slots:
    void slotSetSimStartTime(QTime time);
    void slotSetFlagSimulationValid(bool simValid);

private slots:
    ATC_MOUSE_HANDLER

    void on_buttonNewSimulation_clicked();
    void on_buttonEditSimulation_clicked();
    void on_buttonImportSimulation_clicked();
    void on_buttonExportSimulation_clicked();

    void on_buttonStart_clicked();
    void on_buttonPause_clicked();
    void on_buttonStop_clicked();
    void on_buttonCloseMenu_clicked();
    void on_checkBoxLog_clicked(bool checked);
    void on_buttonLogDir_clicked();

    void slotActiveScenarioPath(QString path);

private:
    ATCPaths *paths;
    Ui::DialogMainMenu *uiInner;
    QTime *time;

    bool simulationValid;
    bool simulationRunning;
    bool dataLogged;
};

#endif // DIALOGMAINMENU_H
