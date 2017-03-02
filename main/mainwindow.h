#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QStringList>

#include "dialogtextconsole.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"
#include "dialogsettings.h"
#include "dialogflight.h"
#include "dialogflightcreator.h"
#include "dialogflightplan.h"
#include "dialogactiverunways.h"
#include "dialogleaders.h"
#include "dialogtrailingdots.h"

#include "atcpaths.h"
#include "atcairspace.h"
#include "atcsettings.h"
#include "atcflightfactory.h"
#include "atcsimulation.h"
#include "atcsimulationcontroller.h"
#include "atcmath.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ATCFlightFactory *flightFactory, QWidget *parent = 0);
    ~MainWindow();

    bool isDialogTextConsoleVisible() const;

    bool getFlagDialogMainMenuExists() const;
    bool getFlagDialogSectorSetupExists() const;
    bool getFlagDialogSettingsExists() const;

    void setFlagDialogMainMenuExists(bool flagBool);
    void setFlagDialogSectorSetupExists(bool flagBool);
    void setFlagDialogSettingExists(bool flagBool);

    void setSituationalDisplayFocus();

signals:
    void signalCreateFlightTag(ATCFlight *flight);
    void signalActiveScenarioPath(QString path);

private slots:
    void on_buttonMainMenu_clicked();
    void on_buttonSctSetup_clicked();
    void on_buttonClose_clicked();
    void on_buttonSettings_clicked();
    void on_buttonShowConsole_clicked();
    void on_buttonLeaderLine_clicked();
    void on_buttonTrailingDots_clicked();

    void dialogMainMenuClosed();
    void dialogSectorSetupClosed();
    void dialogSettingsClosed();
    void dialogLeadersClosed();
    void dialogTrailingDotsClosed();

    void changeFocusToDisplay();

    void slotSimulation(ATCSimulation *sim);
    void slotActiveScenarioPath(QString path);

    void slotConstructDialogFlightNew();
    void slotConstructDialogFlightEdit();
    void slotCloseDialogFlight();

    void slotCreateDialogFlightPlan(ATCFlight *flight);
    void slotCloseDialogFlightPlan();

    void slotConstructDialogFlightCreator();
    void slotConstructDialogFlightCreator(ATCFlight *flight);
    void slotCloseDialogFlightCreator();

    void slotConstructDialogActiveRunways(ATC::SimCreationMode m);
    void slotCloseDialogActiveRunways();

    void slotImportScenario();
    void slotImportScenario(QString path);
    void slotExportScenario();

    void slotStartSimulation();
    void slotPauseSimulation();
    void slotStopSimulation();

    void slotSetSimulationStartTime();
    void slotDataLogged(bool flag, QString path);

private:
    Ui::MainWindow *ui;
    DialogTextConsole *dialogTextConsole;
    DialogMainMenu *dialogMainMenu;
    DialogSectorSetup *dialogSectorSetup;
    DialogSettings *dialogSettings;
    DialogLeaders *dialogLeaders = nullptr;
    DialogTrailingDots *dialogTrailingDots = nullptr;

    DialogFlight *dialogFlight = nullptr;
    DialogFlightCreator *dialogFlightCreator = nullptr;
    DialogActiveRunways *dialogActiveRunways = nullptr;

    DialogFlightPlan *dialogFlightPlan = nullptr;

    ATCPaths *paths;
    ATCAirspace *airspaceData;
    ATCSettings *settings;
    ATCFlightFactory *flightFactory;

    ATCSimulation *simulation = nullptr;
    ATCSimulation *tempSimulation = nullptr;
    ATCSimulationController *simController = nullptr;

    QString simulationPath;
    QTime simulationTime;

    bool flagDialogMainMenuExists = false;
    bool flagDialogSectorSetupExists = false;
    bool flagDialogSettingsExists = false;

    bool dataLogged = false;
    QString dataLoggedPath;

    void mainWindowSetup();

    void connectDialogFlightCreatorSlots();
};

#endif // MAINWINDOW_H
