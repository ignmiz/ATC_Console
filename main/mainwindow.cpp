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
    if(simController != nullptr) delete simController;
}

bool MainWindow::isDialogTextConsoleVisible() const
{
    return dialogTextConsole->isVisible();
}

void MainWindow::on_buttonMainMenu_clicked()
{
    if(!getFlagDialogMainMenuExists())
    {
        QTime *time = ui->buttonTime->getTime();

        dialogMainMenu = new DialogMainMenu(time, this);
        dialogMainMenu->show();

        setFlagDialogMainMenuExists(true);
        setSituationalDisplayFocus();

        connect(dialogMainMenu, SIGNAL(closed()), this, SLOT(dialogMainMenuClosed()));
        connect(dialogMainMenu, SIGNAL(changeFocusToDisplay()), this, SLOT(changeFocusToDisplay()));
        connect(dialogMainMenu, SIGNAL(signalConstructDialogFlightNew()), this, SLOT(slotConstructDialogFlightNew()));
        connect(dialogMainMenu, SIGNAL(signalConstructDialogFlightEdit()), this, SLOT(slotConstructDialogFlightEdit()));
        connect(dialogMainMenu, SIGNAL(signalImportScenario()), this, SLOT(slotImportScenario()));
        connect(dialogMainMenu, SIGNAL(signalExportScenario()), this, SLOT(slotExportScenario()));
        connect(dialogMainMenu, SIGNAL(signalStartSimulation()), this, SLOT(slotStartSimulation()));
        connect(dialogMainMenu, SIGNAL(signalStopSimulation()), this, SLOT(slotStopSimulation()));
        connect(this, SIGNAL(signalActiveScenarioPath(QString)), dialogMainMenu, SLOT(slotActiveScenarioPath(QString)));

        connect(this, SIGNAL(signalCreateFlightTag(ATCFlight*)), ui->situationalDisplay, SLOT(slotCreateFlightTag(ATCFlight*)));

        if(simulation == nullptr)
        {
            emit signalActiveScenarioPath("No scenario selected");
        }
        else
        {
            emit signalActiveScenarioPath(simulationPath);
            dialogMainMenu->slotSetSimStartTime(simulationTime);
        }
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

    disconnect(this, SIGNAL(signalCreateFlightTag(ATCFlight*)), ui->situationalDisplay, SLOT(slotCreateFlightTag(ATCFlight*)));
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

void MainWindow::slotActiveScenarioPath(QString path)
{
    simulationPath = path;
    simulationTime = dialogMainMenu->getSimStartTime();
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
    connect(dialogFlight, SIGNAL(signalActiveScenarioPath(QString)), dialogMainMenu, SLOT(slotActiveScenarioPath(QString)));
    connect(dialogFlight, SIGNAL(signalActiveScenarioPath(QString)), this, SLOT(slotActiveScenarioPath(QString)));
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

void MainWindow::slotImportScenario()
{
    ATCPaths paths;

    QString filePath = QFileDialog::getOpenFileName(this, tr("Load symbology..."), paths.SCENARIO_EXPORT_PATH, tr("Text files(*.scn)"));
    if(filePath.isEmpty()) return;

    QFile file(filePath);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("DialogMainMenu: Failed to open path: " + filePath);
        msgBox.exec();

        return;
    }

    ATCSimulation *newSim = new ATCSimulation();
    ATCActiveRunways *active = newSim->getActiveRunways();
    ATCFlight *flight;

    ActiveAirport airport;
    State state;
    state.x = 1;

    QString flag;

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString textLine = stream.readLine();
        textLine = textLine.trimmed();

        if(!textLine.isEmpty())
        {
            QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

            if(textLine.contains("[INFO]", Qt::CaseInsensitive))
            {
                flag = "[INFO]";
            }
            else if(textLine.contains("[RUNWAYS]", Qt::CaseInsensitive))
            {
                flag = "[RUNWAYS]";
            }
            else if(textLine.contains("[FLIGHTS]", Qt::CaseInsensitive))
            {
                flag = "[FLIGHTS]";
            }
            else if(flag == "[INFO]")
            {
                if(stringList.at(0).trimmed() == "TIME")
                {
                    int h = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(0).toInt();
                    int m = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(1).toInt();
                    int s = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(2).toInt();

                    dialogMainMenu->slotSetSimStartTime(QTime(h, m, s));
                }
            }
            else if(flag == "[RUNWAYS]")
            {
                if(stringList.at(0).trimmed() == "ICAO")
                {
                    airport = ActiveAirport();
                    airport.airportCode = stringList.at(1).trimmed();
                }
                else if(stringList.at(0).trimmed() == "DEP")
                {
                    bool dep;
                    stringList.at(1).trimmed() == "1" ? dep = true : dep = false;

                    airport.dep = dep;
                }
                else if(stringList.at(0).trimmed() == "ARR")
                {
                    bool arr;
                    stringList.at(1).trimmed() == "1" ? arr = true : arr = false;

                    airport.arr = arr;
                }
                else if(stringList.at(0).trimmed() == "DEP RWY")
                {
                    if(stringList.size() == 2) airport.depRwys = stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts);
                }
                else if(stringList.at(0).trimmed() == "ARR RWY")
                {
                    if(stringList.size() == 2) airport.arrRwys = stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts);

                    active->appendActiveAirport(airport);
                }
            }
            else if(flag == "[FLIGHTS]")
            {
                if(stringList.at(0).trimmed() == "COMPANY")
                {
                    flight = new ATCFlight(state, new ATCFlightPlan());

                    if(flightFactory->getCompanyFactory().getCompany(stringList.at(1).trimmed()) == nullptr)
                    {
                        flightFactory->getCompanyFactory().appendCompany(new ATCCompany(stringList.at(1).trimmed(), "UNKNOWN", "Unknown Airline"));
                    }

                    flight->getFlightPlan()->setCompany(flightFactory->getCompanyFactory().getCompany(stringList.at(1).trimmed()));
                }
                else if(stringList.at(0).trimmed() == "FLIGHT NO")
                {
                    if(stringList.size() == 2) flight->getFlightPlan()->setFlightNumber(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "TYPE")
                {
                    flight->getFlightPlan()->setType(flightFactory->getAircraftTypeFactory().getType(stringList.at(1).trimmed()));
                }
                else if(stringList.at(0).trimmed() == "ADEP")
                {
                    flight->getFlightPlan()->getRoute().setDeparture(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "ADES")
                {
                    flight->getFlightPlan()->getRoute().setDestination(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "AALT")
                {
                    if(stringList.size() == 2) flight->getFlightPlan()->getRoute().setAlternate(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "ROUTE")
                {
                    flight->getFlightPlan()->getRoute().setRoute(stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts));
                }
                else if(stringList.at(0).trimmed() == "FILED TAS")
                {
                    flight->getFlightPlan()->setTAS(stringList.at(1).trimmed().toInt());
                }
                else if(stringList.at(0).trimmed() == "RFL")
                {
                    flight->getFlightPlan()->setAltitude(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "DEP TIME")
                {
                    int h = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(0).toInt();
                    int m = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(1).toInt();

                    flight->getFlightPlan()->setDepartureTime(QTime(h, m));
                }
                else if(stringList.at(0).trimmed() == "ENR TIME")
                {
                    int h = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(0).toInt();
                    int m = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(1).toInt();

                    flight->getFlightPlan()->setEnrouteTime(QTime(h, m));
                }
                else if(stringList.at(0).trimmed() == "FUEL TIME")
                {
                    int h = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(0).toInt();
                    int m = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(1).toInt();

                    flight->getFlightPlan()->setFuelTime(QTime(h, m));
                }
                else if(stringList.at(0).trimmed() == "FIX LIST")
                {
                    flight->setFixList(stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts));
                }
                else if(stringList.at(0).trimmed() == "MAIN FIX LIST")
                {
                    if(stringList.size() == 2) flight->setMainFixList(stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts));
                }
                else if(stringList.at(0).trimmed() == "ASSR")
                {
                    flight->setAssignedSquawk(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "PSSR")
                {
                    flight->setSquawk(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "NAV TYPE")
                {
                    QString type = stringList.at(1).trimmed();
                    type == "NAV" ? flight->setNavMode(ATC::Nav) : flight->setNavMode(ATC::Hdg);
                }
                else if(stringList.at(0).trimmed() == "POS")
                {
                    state = State();

                    double lat = ATCMath::deg2rad(stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts).at(0).toDouble());
                    double lon = ATCMath::deg2rad(stringList.at(1).trimmed().split(" ", QString::SkipEmptyParts).at(1).toDouble());

                    state.x = lon;
                    state.y = lat;
                }
                else if(stringList.at(0).trimmed() == "AFL")
                {
                    state.h = ATCMath::ft2m(stringList.at(1).trimmed().toDouble());
                }
                else if(stringList.at(0).trimmed() == "TAS")
                {
                    state.v = ATCMath::kt2mps(stringList.at(1).trimmed().toDouble());
                }
                else if(stringList.at(0).trimmed() == "THDG")
                {
                    state.hdg = ATCMath::deg2rad(stringList.at(1).trimmed().toDouble());

                    flight->setState(state);
                }
                else if(stringList.at(0).trimmed() == "CFL")
                {
                    flight->setTargetAltitude(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "SPEED RES")
                {
                    if(stringList.size() == 2) flight->setTargetAltitude(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "MHDG RES")
                {
                    if(flight->getNavMode() == ATC::Hdg) flight->setHdgRestriction(stringList.at(1).trimmed().toInt());
                }
                else if(stringList.at(0).trimmed() == "NEXT FIX")
                {
                    if(flight->getNavMode() == ATC::Nav) flight->setNextFix(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "SIM TIME")
                {
                    int h = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(0).toInt();
                    int m = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(1).toInt();
                    int s = stringList.at(1).trimmed().split(":", QString::SkipEmptyParts).at(2).toInt();

                    flight->setSimStartTime(QTime(h, m, s));
                }
                else if(stringList.at(0).trimmed() == "RWY DEP")
                {
                    if(stringList.size() == 2) flight->setRunwayDeparture(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "RWY ARR")
                {
                    if(stringList.size() == 2) flight->setRunwayDestination(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "SID")
                {
                    if(stringList.size() == 2) flight->setSID(stringList.at(1).trimmed());
                }
                else if(stringList.at(0).trimmed() == "STAR")
                {
                    if(stringList.size() == 2) flight->setSTAR(stringList.at(1).trimmed());

                    newSim->appendFlight(flight);
                }
            }

        }
    }

    if(simulation != nullptr) delete simulation;
    simulation = newSim;
    ui->situationalDisplay->setSimulation(simulation);

    for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
    {
        emit signalCreateFlightTag(simulation->getFlight(i));
    }

    emit signalActiveScenarioPath(filePath);
    simulationPath = filePath;
    simulationTime = dialogMainMenu->getSimStartTime();

    QMessageBox msgBox(this);
    msgBox.setText("Scenario successfuly imported from: " + filePath);
    msgBox.exec();
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
        out << "TIME = " << dialogMainMenu->getSimStartTime().toString("HH:mm:ss") << endl;
        out << endl;

        out << "[RUNWAYS]" << endl;
        QVector<ActiveAirport> activeAirports = simulation->getActiveRunways()->getActiveAirports();
        for(int i = 0; i < activeAirports.size(); i++)
        {
            ActiveAirport current = activeAirports.at(i);

            out << "ICAO = " << current.airportCode << endl;
            out << "DEP = " << (current.dep ? "1" : "0") << endl;
            out << "ARR = " << (current.arr ? "1" : "0") << endl;
            out << "DEP RWY = " << (current.depRwys.isEmpty() ? "" : current.depRwys.join(" ")) << endl;
            out << "ARR RWY = " << (current.arrRwys.isEmpty() ? "" : current.arrRwys.join(" ")) << endl;
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
            out << "FLIGHT NO = " << (current->getFlightPlan()->getFlightNumber().isEmpty() ? "" : current->getFlightPlan()->getFlightNumber()) << endl;
            out << "TYPE = " << current->getFlightPlan()->getType()->getAcType().ICAOcode << endl;
            out << "ADEP = " << current->getFlightPlan()->getRoute().getDeparture() << endl;
            out << "ADES = " << current->getFlightPlan()->getRoute().getDestination() << endl;
            out << "AALT = " << (current->getFlightPlan()->getRoute().getAlternate().isEmpty() ? "" : current->getFlightPlan()->getRoute().getAlternate()) << endl;
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
            out << "SPEED RES = " << (current->getTargetSpeed().isEmpty() ? "" : current->getTargetSpeed()) << endl;
            out << "MHDG RES = " << (mode == ATC::Nav ? "" : QString::number(current->getHdgRestriction())) << endl;
            out << "NEXT FIX = " << (mode == ATC::Nav ? current->getNextFix() : "") << endl;
            out << "SIM TIME = " << current->getSimStartTime().toString("HH:mm:ss") << endl;
            out << "RWY DEP = " << (current->getRunwayDeparture().isEmpty() ? "" : current->getRunwayDeparture()) << endl;
            out << "RWY ARR = " << (current->getRunwayDestination().isEmpty() ? "" : current->getRunwayDestination()) << endl;
            out << "SID = " << (current->getSID().isEmpty() ? "" : current->getSID()) << endl;
            out << "STAR = " << (current->getSTAR().isEmpty() ? "" : current->getSTAR()) << endl;
            out << endl;
        }
        out << endl;

        emit signalActiveScenarioPath(filePath);
        simulationPath = filePath;
        simulationTime = dialogMainMenu->getSimStartTime();
        ui->situationalDisplay->setSimulation(simulation);

        QMessageBox msgBox(this);
        msgBox.setText("Scenario successfuly exported to: " + filePath);
        msgBox.exec();
    }
}

void MainWindow::slotStartSimulation()
{
    if(simulation != nullptr)
    {
        simController = new ATCSimulationController(simulation);
        simController->start();

        emit dialogMainMenu->closed();
        dialogMainMenu->close();
    }
}

void MainWindow::slotStopSimulation()
{
    if(simController != nullptr)
    {
        simController->stop();
        delete simController;
        simController = nullptr;

        simulation->moveToThread(QThread::currentThread());
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
