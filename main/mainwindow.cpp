#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(ATCFlightFactory *flightFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    flightFactory(flightFactory)
{
    ui->setupUi(this);
    dialogTextConsole = new DialogTextConsole(this);

    ui->situationalDisplay->setFlightFactory(flightFactory);
    ui->situationalDisplay->setParent(this);

    paths = ui->situationalDisplay->getPaths();
    airspaceData = ui->situationalDisplay->getAirspaceData();
    settings = ui->situationalDisplay->getSettings();

    mainWindowSetup();
    setSituationalDisplayFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete flightFactory;
    if(simulation != nullptr) delete simulation;
}

bool MainWindow::isDialogTextConsoleVisible() const
{
    return dialogTextConsole->isVisible();
}

void MainWindow::on_buttonMainMenu_clicked()
{
    if(!getFlagDialogMainMenuExists())
    {
        dialogMainMenu = new DialogMainMenu(this);
        dialogMainMenu->show();

        connect(dialogMainMenu, SIGNAL(closed()), this, SLOT(dialogMainMenuClosed()));
        connect(dialogMainMenu, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogMainMenuExists(true);
        setSituationalDisplayFocus();

        connect(dialogMainMenu, SIGNAL(signalConstructDialogFlightNew()), this, SLOT(slotConstructDialogFlightNew()));
        connect(dialogMainMenu, SIGNAL(signalConstructDialogFlightEdit()), this, SLOT(slotConstructDialogFlightEdit()));
        connect(dialogMainMenu, SIGNAL(signalExportScenario()), this, SLOT(slotExportScenario()));
    }
}

void MainWindow::on_buttonSctSetup_clicked()
{
    if(!getFlagDialogSectorSetupExists())
    {
        dialogSectorSetup = new DialogSectorSetup(this);
        dialogSectorSetup->show();

        connect(dialogSectorSetup, SIGNAL(closed()), this, SLOT(dialogSectorSetupClosed()));
        connect(dialogSectorSetup, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogSectorSetupExists(true);
        setSituationalDisplayFocus();
    }
}

void MainWindow::on_buttonSettings_clicked()
{
    if(!getFlagDialogSettingsExists())
    {
        dialogSettings = new DialogSettings(ui->situationalDisplay ,this);
        dialogSettings->show();

        connect(dialogSettings, SIGNAL(closed()), this, SLOT(dialogSettingsClosed()));
        connect(dialogSettings, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));

        setFlagDialogSettingExists(true);
        setSituationalDisplayFocus();
    }
}

void MainWindow::on_buttonShowConsole_clicked()
{
    if(dialogTextConsole->isHidden())
    {
        if(!(dialogTextConsole->isMaximized()))
            dialogTextConsole->maximizeWindow();

        dialogTextConsole->show();
        dialogTextConsole->raise();
        dialogTextConsole->setConsoleDisplayBottom();
        dialogTextConsole->setConsoleInputFocus();
    }
    else
    {
        dialogTextConsole->hide();
    }
}

void MainWindow::dialogMainMenuClosed()
{
    setFlagDialogMainMenuExists(false);
}

void MainWindow::dialogSectorSetupClosed()
{
    setFlagDialogSectorSetupExists(false);
}

void MainWindow::dialogSettingsClosed()
{
    setFlagDialogSettingExists(false);
}

void MainWindow::changeFocusToDisplay()
{
    setSituationalDisplayFocus();
}

void MainWindow::slotSimulation(ATCSimulation *sim)
{
    if(simulation != nullptr) delete simulation;

    simulation = sim;
    ui->situationalDisplay->setSimulation(sim);
}

void MainWindow::slotConstructDialogFlightNew()
{
    dialogMainMenu->hide();

    dialogFlight = new DialogFlight(new ATCSimulation(), airspaceData, ATC::New, this);
    dialogFlight->show();

    tempSimulation = dialogFlight->getSimulation();

    if(simulation != nullptr)   //Hide all data tags & routes
    {
        for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
        {
            ATCFlight *current = simulation->getFlight(i);

            current->getFlightTag()->hideTag();
            if(current->getRoutePrediction() != nullptr) emit current->signalDisplayRoute(current);
        }
    }

    connect(dialogFlight, SIGNAL(closed()), this, SLOT(slotCloseDialogFlight()));
    connect(dialogFlight, SIGNAL(signalConstructDialogFlightCreator()), this, SLOT(slotConstructDialogFlightCreator()));
    connect(dialogFlight, SIGNAL(signalConstructDialogFlightCreator(ATCFlight*)), this, SLOT(slotConstructDialogFlightCreator(ATCFlight*)));
    connect(dialogFlight, SIGNAL(signalConstructDialogActiveRunways(ATC::SimCreationMode)), this, SLOT(slotConstructDialogActiveRunways(ATC::SimCreationMode)));
    connect(dialogFlight, SIGNAL(signalSimulation(ATCSimulation*)), this, SLOT(slotSimulation(ATCSimulation*)));
}

void MainWindow::slotConstructDialogFlightEdit()
{
    if(simulation != nullptr)
    {
        dialogMainMenu->hide();

        dialogFlight = new DialogFlight(simulation, airspaceData, ATC::Edit, this);
        dialogFlight->show();

        connect(dialogFlight, SIGNAL(closed()), this, SLOT(slotCloseDialogFlight()));
        connect(dialogFlight, SIGNAL(signalConstructDialogFlightCreator()), this, SLOT(slotConstructDialogFlightCreator()));
        connect(dialogFlight, SIGNAL(signalConstructDialogFlightCreator(ATCFlight*)), this, SLOT(slotConstructDialogFlightCreator(ATCFlight*)));
        connect(dialogFlight, SIGNAL(signalConstructDialogActiveRunways(ATC::SimCreationMode)), this, SLOT(slotConstructDialogActiveRunways(ATC::SimCreationMode)));
    }
}

void MainWindow::slotCloseDialogFlight()
{
    dialogFlight = nullptr;
    if((tempSimulation != simulation) && (tempSimulation != nullptr))   //When DialogFlight@New was cancelled or closed
    {
        delete tempSimulation;

        if(simulation != nullptr)   //Show all data tags
        {
            for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
            {
                ATCFlight *current = simulation->getFlight(i);
                current->getFlightTag()->showTag();
            }
        }
    }

    tempSimulation = nullptr;
    dialogMainMenu->show();
}

void MainWindow::slotCreateDialogFlightPlan(ATCFlight *flight)
{
    if((dialogFlightPlan == nullptr) && (dialogFlight == nullptr) && (dialogFlightCreator == nullptr))
    {
        dialogFlightPlan = new DialogFlightPlan(flight, airspaceData, simulation, flightFactory, this);
        dialogFlightPlan->show();

        connect(dialogFlightPlan, SIGNAL(closed()), this, SLOT(slotCloseDialogFlightPlan()));
        connect(dialogFlightPlan, SIGNAL(signalUpdateRoute(ATCFlight*)), ui->situationalDisplay, SLOT(slotUpdateRoute(ATCFlight*)));
    }
}

void MainWindow::slotCloseDialogFlightPlan()
{
    dialogFlightPlan = nullptr;
}

void MainWindow::slotConstructDialogFlightCreator()
{
    dialogFlight->hide();

    if(tempSimulation != nullptr) //New simulation option, new flight
    {
        dialogFlightCreator = new DialogFlightCreator(airspaceData, settings, flightFactory, tempSimulation, this);
    }
    else //Edit simulation option, new flight
    {
        dialogFlightCreator = new DialogFlightCreator(airspaceData, settings, flightFactory, simulation, this);
    }

    dialogFlightCreator->show();

    connectDialogFlightCreatorSlots();
}

void MainWindow::slotConstructDialogFlightCreator(ATCFlight *flight)
{
    dialogFlight->hide();

    if(tempSimulation != nullptr) //New simulation option, edit flight
    {
        dialogFlightCreator = new DialogFlightCreator(flight, airspaceData, settings, flightFactory, tempSimulation, this);
    }
    else //Edit simulation option, edit flight
    {
        dialogFlightCreator = new DialogFlightCreator(flight, airspaceData, settings, flightFactory, simulation, this);
    }

    dialogFlightCreator->show();

    connectDialogFlightCreatorSlots();
}

void MainWindow::slotCloseDialogFlightCreator()
{
    disconnect(dialogFlightCreator, SIGNAL(closed()), this, SLOT(slotCloseDialogFlightCreator()));
    disconnect(dialogFlightCreator, SIGNAL(signalGetLocation(QStringList)), ui->situationalDisplay, SLOT(slotGetLocation(QStringList)));
    disconnect(ui->situationalDisplay, SIGNAL(signalShowFlightCreator()), dialogFlightCreator, SLOT(slotShowFlightCreator()));
    disconnect(ui->situationalDisplay, SIGNAL(signalDisplayClicked(double,double)), dialogFlightCreator, SLOT(slotDisplayClicked(double,double)));
    disconnect(dialogFlightCreator, SIGNAL(signalCreateFlightTag(ATCFlight*)), ui->situationalDisplay, SLOT(slotCreateFlightTag(ATCFlight*)));

    dialogFlightCreator = nullptr;

    dialogFlight->show();
}

void MainWindow::slotConstructDialogActiveRunways(ATC::SimCreationMode m)
{
    dialogFlight->hide();

    if(m == ATC::New)
    {
        dialogActiveRunways = new DialogActiveRunways(airspaceData, tempSimulation->getActiveRunways(), this);
    }
    else
    {
        dialogActiveRunways = new DialogActiveRunways(airspaceData, simulation->getActiveRunways(), this);
    }

    dialogActiveRunways->show();

    connect(dialogActiveRunways, SIGNAL(closed()), this, SLOT(slotCloseDialogActiveRunways()));
}

void MainWindow::slotCloseDialogActiveRunways()
{
    dialogActiveRunways = nullptr;
    dialogFlight->show();
}

void MainWindow::slotExportScenario()
{
    if(simulation != nullptr)
    {
        ATCPaths paths;

        QString filePath = QFileDialog::getSaveFileName(this, tr("Export to..."), paths.SCENARIO_EXPORT_PATH, tr("Text files(*.scn)"));
        if(filePath.isEmpty()) return;

        QStringList pathElements = filePath.split("/", QString::KeepEmptyParts);
        QString fileName = pathElements.at(pathElements.size() - 1).trimmed();
        QString nameWithoutExtension = fileName.split(".", QString::KeepEmptyParts).at(0).trimmed();

        QFile file(filePath);

        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("DialogMainMenu: Failed to open path: " + filePath);
            msgBox.exec();

            return;
        }

        QTextStream out(&file);

        out << "[INFO]" << endl;
        out << "NAME = " << nameWithoutExtension << endl;
        out << endl;

        out << "[RUNWAY]" << endl;
        QVector<ActiveAirport> activeAirports = simulation->getActiveRunways()->getActiveAirports();
        for(int i = 0; i < activeAirports.size(); i++)
        {
            ActiveAirport current = activeAirports.at(i);

            out << "ICAO = " << current.airportCode << endl;
            out << "DEP = " << (current.dep ? "1" : "0") << endl;
            out << "ARR = " << (current.arr ? "1" : "0") << endl;
            out << "DEP RWY = " << (current.depRwys.isEmpty() ? "NIL" : current.depRwys.join(" ")) << endl;
            out << "ARR RWY = " << (current.arrRwys.isEmpty() ? "NIL" : current.arrRwys.join(" ")) << endl;
            out << endl;
        }
        out << endl;

        out << "[FLIGHTS]" << endl  ;
        QVector<ATCFlight*> flights = simulation->getFlightsVector();
        for(int i = 0; i < flights.size(); i++)
        {
            ATCFlight *current = flights.at(i);
            ATC::NavMode mode = current->getNavMode();

            out << "COMPANY = " << current->getFlightPlan()->getCompany()->getCode() << endl;
            out << "FLIGHT NO = " << current->getFlightPlan()->getFlightNumber() << endl;
            out << "TYPE = " << current->getFlightPlan()->getType()->getAcType().ICAOcode << endl;
            out << "ADEP = " << current->getFlightPlan()->getRoute().getDeparture() << endl;
            out << "ADES = " << current->getFlightPlan()->getRoute().getDestination() << endl;
            out << "AALT = " << current->getFlightPlan()->getRoute().getAlternate() << endl;
            out << "ROUTE = " << current->getFlightPlan()->getRoute().getRoute().join(" ") << endl;
            out << "FILED TAS = " << QString::number(current->getFlightPlan()->getTAS()) << endl;
            out << "RFL = " << current->getFlightPlan()->getAltitude() << endl;
            out << "DEP TIME = " << current->getFlightPlan()->getDepartureTime().toString("HH:mm") << endl;
            out << "ENR TIME = " << current->getFlightPlan()->getEnrouteTime().toString("HH:mm") << endl;
            out << "FUEL TIME = " << current->getFlightPlan()->getFuelTime().toString("HH:mm") << endl;
            out << "FIX LIST = " << current->getFixList().join(" ") << endl;
            out << "MAIN FIX LIST = " << current->getMainFixList().join(" ") << endl;
            out << "ASSR = " << current->getAssignedSquawk() << endl;
            out << "PSSR = " << current->getSquawk() << endl;
            out << "NAV TYPE = " << (mode == ATC::Nav ? "NAV" : "HDG") << endl;
            out << "POS = " << QString::number(ATCMath::rad2deg(current->getState().y)) << " " << QString::number(ATCMath::rad2deg(current->getState().x)) << endl;
            out << "AFL = " << QString::number(ATCMath::m2ft(current->getState().h)) << endl;
            out << "TAS = " << QString::number(ATCMath::mps2kt(current->getState().v)) << endl;
            out << "THDG = " << QString::number(ATCMath::rad2deg(current->getState().hdg)) << endl;
            out << "CFL = " << current->getTargetAltitude() << endl;
            out << "SPEED RES = " << (current->getTargetSpeed().isEmpty() ? "NIL" : current->getTargetSpeed()) << endl;
            out << "MHDG RES = " << (mode == ATC::Nav ? "NIL" : QString::number(current->getHdgRestriction())) << endl;
            out << "NEXT FIX = " << (mode == ATC::Nav ? current->getNextFix() : "NIL") << endl;
            out << "SIM TIME = " << current->getSimStartTime().toString("HH:mm:ss") << endl;
            out << "RWY DEP = " << current->getRunwayDeparture() << endl;
            out << "RWY ARR = " << current->getRunwayDestination() << endl;
            out << "SID = " << current->getSID() << endl;
            out << "STAR = " << current->getSTAR() << endl;
            out << endl;
        }
        out << endl;

        QMessageBox msgBox(this);
        msgBox.setText("Scenario successfuly exported to: " + filePath);
        msgBox.exec();
    }
}

void MainWindow::on_buttonClose_clicked()
{
    close();
}

void MainWindow::mainWindowSetup()
{
    QDesktopWidget desktop;
    QRect mainScreenSize = desktop.screenGeometry(desktop.primaryScreen());

    int desktopWidth = mainScreenSize.width();
    int desktopHeight = mainScreenSize.height();

    this->setFixedSize(desktopWidth, desktopHeight);
    this->setWindowState(Qt::WindowFullScreen);

    ui->mainToolBar->hide();
    ui->statusBar->hide();
}

void MainWindow::connectDialogFlightCreatorSlots()
{
    connect(dialogFlightCreator, SIGNAL(closed()), this, SLOT(slotCloseDialogFlightCreator()));
    connect(dialogFlightCreator, SIGNAL(signalGetLocation(QStringList)), ui->situationalDisplay, SLOT(slotGetLocation(QStringList)));
    connect(ui->situationalDisplay, SIGNAL(signalShowFlightCreator()), dialogFlightCreator, SLOT(slotShowFlightCreator()));
    connect(ui->situationalDisplay, SIGNAL(signalDisplayClicked(double,double)), dialogFlightCreator, SLOT(slotDisplayClicked(double,double)));
    connect(dialogFlightCreator, SIGNAL(signalCreateFlightTag(ATCFlight*)), ui->situationalDisplay, SLOT(slotCreateFlightTag(ATCFlight*)));
    connect(dialogFlightCreator, SIGNAL(signalUpdateFlightList(ATCFlight*)), dialogFlight, SLOT(slotUpdateFlightList(ATCFlight*)));
    connect(dialogFlightCreator, SIGNAL(signalUpdateFlightTag(ATCFlight*)), ui->situationalDisplay, SLOT(slotUpdateFlightTag(ATCFlight*)));
}

bool MainWindow::getFlagDialogMainMenuExists() const
{
    return flagDialogMainMenuExists;
}

bool MainWindow::getFlagDialogSectorSetupExists() const
{
    return flagDialogSectorSetupExists;
}

bool MainWindow::getFlagDialogSettingsExists() const
{
    return flagDialogSettingsExists;
}

void MainWindow::setFlagDialogMainMenuExists(bool flagBool)
{
    flagDialogMainMenuExists = flagBool;
}

void MainWindow::setFlagDialogSectorSetupExists(bool flagBool)
{
    flagDialogSectorSetupExists = flagBool;
}

void MainWindow::setFlagDialogSettingExists(bool flagBool)
{
    flagDialogSettingsExists = flagBool;
}

void MainWindow::setSituationalDisplayFocus()
{
    ui->situationalDisplay->setFocus();
}
