#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "dialogtextconsole.h"
#include "dialogmainmenu.h"
#include "dialogsectorsetup.h"
#include "dialogsettings.h"
#include "dialogflight.h"
#include "dialogflightcreator.h"
#include "dialogflightplan.h"

#include "atcpaths.h"
#include "atcairspace.h"
#include "atcsettings.h"
#include "atcflightfactory.h"
#include "atcsimulation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent = 0);
    ~MainWindow();

    bool isDialogTextConsoleVisible() const;

    bool getFlagDialogMainMenuExists() const;
    bool getFlagDialogSectorSetupExists() const;
    bool getFlagDialogSettingsExists() const;

    void setFlagDialogMainMenuExists(bool flagBool);
    void setFlagDialogSectorSetupExists(bool flagBool);
    void setFlagDialogSettingExists(bool flagBool);

    void setSituationalDisplayFocus();

private slots:
    void on_buttonMainMenu_clicked();
    void on_buttonSctSetup_clicked();
    void on_buttonClose_clicked();
    void on_buttonSettings_clicked();
    void on_buttonShowConsole_clicked();

    void dialogMainMenuClosed();
    void dialogSectorSetupClosed();
    void dialogSettingsClosed();

    void changeFocusToDisplay();

    void slotConstructDialogFlight();
    void slotCloseDialogFlight();

    void slotCreateDialogFlightPlan(ATCFlight *flight);
    void slotDialogFlightPlanClosed();

    void slotConstructFlightCreator();
    void slotConstructFlightCreator(ATCFlight *flight);
    void slotCloseFlightCreator();

private:
    Ui::MainWindow *ui;
    DialogTextConsole *dialogTextConsole;
    DialogMainMenu *dialogMainMenu;
    DialogSectorSetup *dialogSectorSetup;
    DialogSettings *dialogSettings;

    DialogFlight *dialogFlight = nullptr;
    DialogFlightCreator *dialogFlightCreator = nullptr;

    DialogFlightPlan *dialogFlightPlan = nullptr;

    ATCPaths *paths;
    ATCAirspace *airspaceData;
    ATCSettings *settings;
    ATCFlightFactory *flightFactory;
    ATCSimulation *simulation;

    bool flagDialogMainMenuExists = false;
    bool flagDialogSectorSetupExists = false;
    bool flagDialogSettingsExists = false;

    void mainWindowSetup();

    void connectDialogFlightCreatorSlots();
};

#endif // MAINWINDOW_H
