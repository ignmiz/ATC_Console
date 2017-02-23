#include "atcsituationaldisplay.h"
#include "mainwindow.h"
#include "atcconst.h"

#include <QtMath>
#include <QWheelEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

ATCSituationalDisplay::ATCSituationalDisplay(QWidget *parent) :
    QGraphicsView(parent)
{
    paths = new ATCPaths();
    airspaceData = new ATCAirspace(paths->SCT_PATH, paths->ESE_PATH);
    settings = new ATCSettings(paths);

    situationalDisplaySetup();
    connectSlots();

    rescaleScene();

    projectSectorsARTCCLow();
    projectSectorsARTCCHigh();
    projectSectorsARTCC();

    calculateSectorParameters();

    calculateSectorsARTCCLow();
    calculateSectorsARTCCHigh();
    calculateSectorsARTCC();
    calculateAirwayLow();
    calculateAirwayHigh();
    calculateSTARs();
    calculateSIDs();
    calculateExtendedCentrelines();
    calculateCentrelineTicks();
    calculateNDBs();
    calculateVORs();
    calculateFixes();
    calculateAirports();

    loadInitialDisplay(paths->DISPLAY_DFLT_PATH);
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    if(airspaceData != nullptr) delete airspaceData;
    if(settings != nullptr) delete settings;
    if(paths != nullptr) delete paths;

    if(dialogAltitude != nullptr) delete dialogAltitude;
    if(dialogSpeed != nullptr) delete dialogSpeed;
    if(dialogHeading != nullptr) delete dialogHeading;
    if(dialogRoute != nullptr) delete dialogRoute;
    if(dialogHandoff != nullptr) delete dialogHandoff;

    if(ruler != nullptr) delete ruler;

    currentScene->clear();
}

qreal ATCSituationalDisplay::getBaseScale() const
{
    return baseScale;
}

void ATCSituationalDisplay::setBaseScale(qreal scale)
{
    baseScale = scale;
}

double ATCSituationalDisplay::getCurrentScale()
{
    return currentScale;
}

void ATCSituationalDisplay::setFlightFactory(ATCFlightFactory *factory)
{
    flightFactory = factory;
}

void ATCSituationalDisplay::setSimulation(ATCSimulation *sim)
{
    simulation = sim;
}

ATCSettings* ATCSituationalDisplay::getSettings()
{
    return settings;
}

ATCAirspace *ATCSituationalDisplay::getAirspaceData()
{
    return airspaceData;
}

ATCPaths *ATCSituationalDisplay::getPaths()
{
    return paths;
}

QCursor& ATCSituationalDisplay::getAcftCursor()
{
    return acftCursor;
}

void ATCSituationalDisplay::exportDisplay(QString path)
{
    QStringList pathElements = path.split("/", QString::KeepEmptyParts);
    QString fileName = pathElements.at(pathElements.size() - 1).trimmed();
    QString nameWithoutExtension = fileName.split(".", QString::KeepEmptyParts).at(0).trimmed();

    QPointF viewportCentre = mapToScene(viewport()->rect().center());

    QTransform viewportTransformation(transform());
    double scaleX = viewportTransformation.m11();
    double scaleY = viewportTransformation.m22();

    QFile file(path);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " + path;
        return;
    }

    QTextStream out(&file);

    out << "[INFO]" << endl;
    out << "NAME = " << nameWithoutExtension << endl;
    out << "CENTRE = " << viewportCentre.x() << ", " << viewportCentre.y() << endl;
    out << "SCALE = " << scaleX << ", " << scaleY << endl;
    out << endl;

    out << "[ARTCC LOW]" << endl;
    for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
    {
        out << visibleSectorsARTCCLow.at(i)->getName() << endl;
    }
    out << endl;

    out << "[ARTCC HIGH]" << endl;
    for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
    {
        out << visibleSectorsARTCCHigh.at(i)->getName() << endl;
    }
    out << endl;

    out << "[ARTCC]" << endl;
    for(int i = 0; i < visibleSectorsARTCC.size(); i++)
    {
        out << visibleSectorsARTCC.at(i)->getName() << endl;
    }
    out << endl;

    out << "[VOR]" << endl;
    for(int i = 0; i < visibleVORs.size(); i++)
    {
        out << visibleVORs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[NDB]" << endl;
    for(int i = 0; i < visibleNDBs.size(); i++)
    {
        out << visibleNDBs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[FIXES]" << endl;
    for(int i = 0; i < visibleFixes.size(); i++)
    {
        out << visibleFixes.at(i)->getName() << endl;
    }
    out << endl;

    out << "[AIRPORT]" << endl;
    for(int i = 0; i < visibleAirports.size(); i++)
    {
        out << visibleAirports.at(i)->getName() << endl;
    }
    out << endl;

    out << "[RUNWAY]" << endl;
    for(int i = 0; i < visibleCentrelines.size(); i++)
    {
        for(int j = 0; j < airspaceData->getAirportsVectorSize(); j++)
        {
            for(int k = 0; k < airspaceData->getAirport(j)->getRunwayVectorSize(); k++)
            {
                if(airspaceData->getAirport(j)->getRunway(k)->getExtendedCentreline1() == visibleCentrelines.at(i))
                {
                    out << airspaceData->getAirport(j)->getName() << " " << airspaceData->getAirport(j)->getRunway(k)->getRunwayID1() << endl;
                }
                else if(airspaceData->getAirport(j)->getRunway(k)->getExtendedCentreline2() == visibleCentrelines.at(i))
                {
                    out << airspaceData->getAirport(j)->getName() << " " << airspaceData->getAirport(j)->getRunway(k)->getRunwayID2() << endl;
                }
            }
        }
    }
    out << endl;

    out << "[STAR]" << endl;
    for(int i = 0; i < visibleSTARs.size(); i++)
    {
        out << visibleSTARs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[SID]" << endl;
    for(int i = 0; i < visibleSIDs.size(); i++)
    {
        out << visibleSIDs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[LOW AIRWAY]" << endl;
    for(int i = 0; i < visibleLowAirways.size(); i++)
    {
        out << visibleLowAirways.at(i)->getName() << endl;
    }
    out << endl;

    out << "[HIGH AIRWAY]" << endl;
    for(int i = 0; i < visibleHighAirways.size(); i++)
    {
        out << visibleHighAirways.at(i)->getName() << endl;
    }

    file.close();
}

void ATCSituationalDisplay::slotSetColorSectorARTCCLow(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        airspaceData->getSectorARTCCLow(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCCHigh(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        airspaceData->getSectorARTCCHigh(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCC(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        airspaceData->getSectorARTCC(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorVOR(QColor color)
{
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        airspaceData->getVOR(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorNDB(QColor color)
{
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        airspaceData->getNDB(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorFix(QColor color)
{
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        airspaceData->getFix(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirport(QColor color)
{
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        airspaceData->getAirport(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorCentreline(QColor color)
{
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
        {
            airspaceData->getAirport(i)->getRunway(j)->getExtendedCentreline1()->setColor(color);
            airspaceData->getAirport(i)->getRunway(j)->getExtendedCentreline2()->setColor(color);
        }
    }
}

void ATCSituationalDisplay::slotSetColorSTAR(QColor color)
{
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        airspaceData->getSTARSymbol(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSID(QColor color)
{
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        airspaceData->getSIDSymbol(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirwayLow(QColor color)
{
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        airspaceData->getAirwayLow(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirwayHigh(QColor color)
{
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        airspaceData->getAirwayHigh(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCCLow(QString name)
{
    ATCSectorARTCCLow *current = airspaceData->findSectorARTCCLow(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCCLow);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCCHigh(QString name)
{
    ATCSectorARTCCHigh *current = airspaceData->findSectorARTCCHigh(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCCHigh);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCC(QString name)
{
    ATCSectorARTCC *current = airspaceData->findSectorARTCC(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCC);
    }
}

void ATCSituationalDisplay::slotHideFix(QString name)
{
    ATCNavFix *current = airspaceData->findFix(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleFixes);
    }
}

void ATCSituationalDisplay::slotHideNDB(QString name)
{
    ATCBeaconNDB *current = airspaceData->findNDB(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleNDBs);
    }
}

void ATCSituationalDisplay::slotHideVOR(QString name)
{
    ATCBeaconVOR *current = airspaceData->findVOR(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleVORs);
    }
}

void ATCSituationalDisplay::slotHideAirport(QString name)
{
    ATCAirport *current = airspaceData->findAirport(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleAirports);
    }
}

void ATCSituationalDisplay::slotHideCentreline(QString name)
{
    QStringList stringList = name.split(" ", QString::SkipEmptyParts);

    QString airportName = stringList.at(0);
    QString rwyID = stringList.at(1);

    ATCRunwayExtendedCentreline *current = airspaceData->findCentreline(airportName, rwyID);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleCentrelines);
    }
}

void ATCSituationalDisplay::slotHideSID(QString name)
{
    ATCProcedureSIDSymbol *current = airspaceData->findSIDSymbol(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSIDs);
    }
}

void ATCSituationalDisplay::slotHideSTAR(QString name)
{
    ATCProcedureSTARSymbol *current = airspaceData->findSTARSymbol(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSTARs);
    }
}

void ATCSituationalDisplay::slotHideAirwayLow(QString name)
{
    ATCAirwayLow *current = airspaceData->findAirwayLow(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleLowAirways);
    }
}

void ATCSituationalDisplay::slotHideAirwayHigh(QString name)
{
    ATCAirwayHigh *current = airspaceData->findAirwayHigh(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleHighAirways);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCCLow(QString name)
{
    ATCSectorARTCCLow *current = airspaceData->findSectorARTCCLow(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCCLow.append(current);
        rescaleSectorARTCCLow(current);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCCHigh(QString name)
{
    ATCSectorARTCCHigh *current = airspaceData->findSectorARTCCHigh(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCCHigh.append(current);
        rescaleSectorARTCCHigh(current);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCC(QString name)
{
    ATCSectorARTCC *current = airspaceData->findSectorARTCC(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCC.append(current);
        rescaleSectorARTCC(current);
    }
}

void ATCSituationalDisplay::slotShowFix(QString name)
{
    ATCNavFix *current = airspaceData->findFix(name);

    if(current != nullptr)
    {
        current->show();
        visibleFixes.append(current);
        rescaleFix(current);
    }
}

void ATCSituationalDisplay::slotShowNDB(QString name)
{
    ATCBeaconNDB *current = airspaceData->findNDB(name);

    if(current != nullptr)
    {
        current->show();
        visibleNDBs.append(current);
        rescaleNDB(current);
    }
}

void ATCSituationalDisplay::slotShowVOR(QString name)
{
    ATCBeaconVOR *current = airspaceData->findVOR(name);

    if(current != nullptr)
    {
        current->show();
        visibleVORs.append(current);
        rescaleVOR(current);
    }
}

void ATCSituationalDisplay::slotShowAirport(QString name)
{
    ATCAirport *current = airspaceData->findAirport(name);

    if(current != nullptr)
    {
        current->show();
        visibleAirports.append(current);
        rescaleAirport(current);
    }
}

void ATCSituationalDisplay::slotShowCentreline(QString name)
{
    QStringList stringList = name.split(" ", QString::SkipEmptyParts);

    QString airportName = stringList.at(0);
    QString rwyID = stringList.at(1);

    ATCRunwayExtendedCentreline *current = airspaceData->findCentreline(airportName, rwyID);

    if(current != nullptr)
    {
        current->show();
        visibleCentrelines.append(current);
        rescaleExtendedCentreline(current);
    }
}

void ATCSituationalDisplay::slotShowSID(QString name)
{
    ATCProcedureSIDSymbol *current = airspaceData->findSIDSymbol(name);

    if(current != nullptr)
    {
        current->show();
        visibleSIDs.append(current);
        rescaleSID(current);
    }
}

void ATCSituationalDisplay::slotShowSTAR(QString name)
{
    ATCProcedureSTARSymbol *current = airspaceData->findSTARSymbol(name);

    if(current != nullptr)
    {
        current->show();
        visibleSTARs.append(current);
        rescaleSTAR(current);
    }
}

void ATCSituationalDisplay::slotShowAirwayLow(QString name)
{
    ATCAirwayLow *current = airspaceData->findAirwayLow(name);

    if(current != nullptr)
    {
        current->show();
        visibleLowAirways.append(current);
        rescaleAirwayLow(current);
    }
}

void ATCSituationalDisplay::slotShowAirwayHigh(QString name)
{
    ATCAirwayHigh *current = airspaceData->findAirwayHigh(name);

    if(current != nullptr)
    {
        current->show();
        visibleHighAirways.append(current);
        rescaleAirwayHigh(current);
    }
}

void ATCSituationalDisplay::slotApplySettings()
{
    rescaleSectorsARTCCLow();
    rescaleSectorsARTCCHigh();
    rescaleSectorsARTCC();
    rescaleFixes();
    rescaleFixLabels();
    rescaleVORs();
    rescaleVORLabels();
    rescaleNDBs();
    rescaleNDBLabels();
    rescaleAirports();
    rescaleAirportLabels();
    rescaleExtendedCentrelines();
    rescaleSTARs();
    rescaleSIDs();
    rescaleAirwaysLow();
    rescaleAirwaysHigh();
}

void ATCSituationalDisplay::slotShowFlightTag(ATCFlightTag *tag)
{
    if((tag != nullptr) && (!tag->getDiamond()->isVisible()))
    {
        tag->showTag();
        visibleTags.append(tag);
        rescaleTag(tag);
    }
}

void ATCSituationalDisplay::slotHideFlightTag(ATCFlightTag *tag)
{
    if(tag != nullptr)
    {
        tag->hideTag();
        removeFromVisible(tag, visibleTags);
    }
}

void ATCSituationalDisplay::slotGetLocation()
{
    flagGetLocation = true;
    viewport()->setCursor(acftCursor);
}

void ATCSituationalDisplay::slotGetLocation(QStringList fixList)
{
    slotGetLocation();
    slotDisplayRoute(fixList);
}

void ATCSituationalDisplay::slotCreateFlightTag(ATCFlight *flight)
{
    createFlightTag(flight);

    ATCTagRect *tagBox = flight->getFlightTag()->getTagBox();

    connect(tagBox, SIGNAL(signalCreateDialogAltitude(QPoint)), flight, SLOT(slotCreateDialogAltitude(QPoint)));
    connect(flight, SIGNAL(signalCreateDialogAltitude(ATCFlight*,QPoint)), this, SLOT(slotCreateDialogAltitude(ATCFlight*,QPoint)));

    connect(tagBox, SIGNAL(signalCreateDialogSpeed(QPoint)), flight, SLOT(slotCreateDialogSpeed(QPoint)));
    connect(flight, SIGNAL(signalCreateDialogSpeed(ATCFlight*,QPoint)), this, SLOT(slotCreateDialogSpeed(ATCFlight*,QPoint)));

    connect(tagBox, SIGNAL(signalCreateDialogHeading(QPoint)), flight, SLOT(slotCreateDialogHeading(QPoint)));
    connect(flight, SIGNAL(signalCreateDialogHeading(ATCFlight*,QPoint)), this, SLOT(slotCreateDialogHeading(ATCFlight*,QPoint)));

    connect(tagBox, SIGNAL(signalCreateDialogRoute(QPoint)), flight, SLOT(slotCreateDialogRoute(QPoint)));
    connect(flight, SIGNAL(signalCreateDialogRoute(ATCFlight*,QPoint)), this, SLOT(slotCreateDialogRoute(ATCFlight*,QPoint)));

    connect(tagBox, SIGNAL(signalCreateDialogHandoff(QPoint)), flight, SLOT(slotCreateDialogHandoff(QPoint)));
    connect(flight, SIGNAL(signalCreateDialogHandoff(ATCFlight*,QPoint)), this, SLOT(slotCreateDialogHandoff(ATCFlight*,QPoint)));

    connect(tagBox, SIGNAL(signalCreateDialogFlightPlan()), flight, SLOT(slotCreateDialogFlightPlan()));
    connect(flight, SIGNAL(signalCreateDialogFlightPlan(ATCFlight*)), dynamic_cast<MainWindow*>(this->parentWidget()), SLOT(slotCreateDialogFlightPlan(ATCFlight*)));

    connect(tagBox, SIGNAL(signalDisplayRoute()), flight, SLOT(slotDisplayRoute()));
    connect(flight, SIGNAL(signalDisplayRoute(ATCFlight*)), this, SLOT(slotDisplayRoute(ATCFlight*)));

    connect(flight, SIGNAL(signalClearFlightElements(ATCFlight*)), this, SLOT(slotClearFlightElements(ATCFlight*)));
}

void ATCSituationalDisplay::slotUpdateFlightTag(ATCFlight *flight)
{
    ATCFlightTag *tag = flight->getFlightTag();

    for(int i = 0; i < visibleTags.size(); i++)
    {
        if(visibleTags.at(i) == tag) visibleTags.remove(i);
    }

    if(tag != nullptr) delete tag;
    flight->setFlightTag(nullptr);

    createFlightTag(flight);
    if(flight->getRoutePrediction() != nullptr) slotUpdateRoute(flight);

    ATCTagRect *tagBox = flight->getFlightTag()->getTagBox();

    connect(tagBox, SIGNAL(signalCreateDialogAltitude(QPoint)), flight, SLOT(slotCreateDialogAltitude(QPoint)));
    connect(tagBox, SIGNAL(signalCreateDialogSpeed(QPoint)), flight, SLOT(slotCreateDialogSpeed(QPoint)));
    connect(tagBox, SIGNAL(signalCreateDialogHeading(QPoint)), flight, SLOT(slotCreateDialogHeading(QPoint)));
    connect(tagBox, SIGNAL(signalCreateDialogRoute(QPoint)), flight, SLOT(slotCreateDialogRoute(QPoint)));
    connect(tagBox, SIGNAL(signalCreateDialogHandoff(QPoint)), flight, SLOT(slotCreateDialogHandoff(QPoint)));
    connect(tagBox, SIGNAL(signalCreateDialogFlightPlan()), flight, SLOT(slotCreateDialogFlightPlan()));
    connect(tagBox, SIGNAL(signalDisplayRoute()), flight, SLOT(slotDisplayRoute()));
}

void ATCSituationalDisplay::slotCreateDialogAltitude(ATCFlight *flight, QPoint point)
{
    if(dialogAltitudeExists)
    {
        dialogAltitude->close();
        slotDialogAltitudeClosed();
    }

    dialogAltitude = new DialogAltitude(flight, settings, this);
    dialogAltitude->move(point.x() - dialogAltitude->width()/2, point.y() - dialogAltitude->height()/2 - settings->TAG_BOX_HEIGHT_FULL/2 + 10); //+10 due ILS button at the bottom

    dialogAltitude->show();
    dialogAltitudeExists = true;

    QTimer::singleShot(100, this, SLOT(slotDialogAltitudeCloseOnClick()));
    connect(dialogAltitude, SIGNAL(signalClosed()), this, SLOT(slotDialogAltitudeClosed()));
}

void ATCSituationalDisplay::slotDialogAltitudeClosed()
{
    dialogAltitude = nullptr;
    dialogAltitudeExists = false;
    dialogAltitudeCloseOnClick = false;
}

void ATCSituationalDisplay::slotDialogAltitudeCloseOnClick()
{
    dialogAltitudeCloseOnClick = true;
}

void ATCSituationalDisplay::slotCreateDialogSpeed(ATCFlight *flight, QPoint point)
{
    if(dialogSpeedExists)
    {
        dialogSpeed->close();
        slotDialogSpeedClosed();
    }

    dialogSpeed = new DialogSpeed(flight, settings, this);
    dialogSpeed->move(point.x() - dialogSpeed->width()/2, point.y() - dialogSpeed->height()/2 - settings->TAG_BOX_HEIGHT_FULL/2);

    dialogSpeed->show();
    dialogSpeedExists = true;

    QTimer::singleShot(100, this, SLOT(slotDialogSpeedCloseOnClick()));
    connect(dialogSpeed, SIGNAL(signalClosed()), this, SLOT(slotDialogSpeedClosed()));
}

void ATCSituationalDisplay::slotDialogSpeedClosed()
{
    dialogSpeed = nullptr;
    dialogSpeedExists = false;
    dialogSpeedCloseOnClick = false;
}

void ATCSituationalDisplay::slotDialogSpeedCloseOnClick()
{
    dialogSpeedCloseOnClick = true;
}

void ATCSituationalDisplay::slotCreateDialogHeading(ATCFlight *flight, QPoint point)
{
    if(dialogHeadingExists)
    {
        dialogHeading->close();
        slotDialogHeadingClosed();
    }

    dialogHeading = new DialogHeading(flight, settings, this);
    dialogHeading->move(point.x() - dialogHeading->width()/2, point.y() - dialogHeading->height()/2 - settings->TAG_BOX_HEIGHT_FULL/2 + 10); //+10 due ILS button at the bottom

    dialogHeading->show();
    dialogHeadingExists = true;

    connect(dialogHeading, SIGNAL(signalUpdateRoute(ATCFlight*)), this, SLOT(slotUpdateRoute(ATCFlight*)));

    QTimer::singleShot(100, this, SLOT(slotDialogHeadingCloseOnClick()));
    connect(dialogHeading, SIGNAL(signalClosed()), this, SLOT(slotDialogHeadingClosed()));
}

void ATCSituationalDisplay::slotDialogHeadingClosed()
{
    dialogHeading = nullptr;
    dialogHeadingExists = false;
    dialogHeadingCloseOnClick = false;
}

void ATCSituationalDisplay::slotDialogHeadingCloseOnClick()
{
    dialogHeadingCloseOnClick = true;
}

void ATCSituationalDisplay::slotCreateDialogRoute(ATCFlight *flight, QPoint point)
{
    if(dialogRouteExists)
    {
        dialogRoute->close();
        slotDialogRouteClosed();
    }

    dialogRoute = new DialogRoute(flight, settings, this);
    dialogRoute->move(point.x() - dialogRoute->width()/2, point.y() - dialogRoute->height()/2 - settings->TAG_BOX_HEIGHT_FULL/2 + 10); //+10 due ILS button at the bottom

    dialogRoute->show();
    dialogRouteExists = true;

    connect(dialogRoute, SIGNAL(signalUpdateRoute(ATCFlight*)), this, SLOT(slotUpdateRoute(ATCFlight*)));

    QTimer::singleShot(100, this, SLOT(slotDialogRouteCloseOnClick()));
    connect(dialogRoute, SIGNAL(signalClosed()), this, SLOT(slotDialogRouteClosed()));
}

void ATCSituationalDisplay::slotDialogRouteClosed()
{
    dialogRoute = nullptr;
    dialogRouteExists = false;
    dialogRouteCloseOnClick = false;
}

void ATCSituationalDisplay::slotDialogRouteCloseOnClick()
{
    dialogRouteCloseOnClick = true;
}

void ATCSituationalDisplay::slotCreateDialogHandoff(ATCFlight *flight, QPoint point)
{
    if(dialogHandoffExists)
    {
        dialogHandoff->close();
        slotDialogHandoffClosed();
    }

    dialogHandoff = new DialogHandoff(flight, settings, this);
    dialogHandoff->move(point.x() - dialogHandoff->width()/2, point.y() - dialogHandoff->height()/2 - settings->TAG_BOX_HEIGHT_FULL/2);

    dialogHandoff->show();
    dialogHandoffExists = true;

    QTimer::singleShot(100, this, SLOT(slotDialogHandoffCloseOnClick()));
    connect(dialogHandoff, SIGNAL(signalClosed()), this, SLOT(slotDialogHandoffClosed()));
}

void ATCSituationalDisplay::slotDialogHandoffClosed()
{
    dialogHandoff = nullptr;
    dialogHandoffExists = false;
    dialogHandoffCloseOnClick = false;
}

void ATCSituationalDisplay::slotDialogHandoffCloseOnClick()
{
    dialogHandoffCloseOnClick = true;
}

void ATCSituationalDisplay::slotDisplayRoute(ATCFlight *flight)
{
    ATCRoutePrediction *prediction = flight->getRoutePrediction();
    QVector<QPointF> polyVertices;

    if(prediction != nullptr)
    {
        slotClearRoute(flight);
    }
    else
    {
        if(flight->getNavMode() == ATC::Nav)
        {
            //Calculate positions of leg fixes & leg lines
            QStringList fixList = flight->getFixList();
            QString nextFix = flight->getNextFix();
            int nextIndex = -1;

            polyVertices.append(flight->getFlightTag()->getDiamondPosition());

            for(int i = 0; i < fixList.size(); i++)
            {
                if(fixList.at(i) == nextFix) nextIndex = i;
            }

            ATCNavFix *fix;
            ATCBeaconVOR *vor;
            ATCAirport *airport;
            ATCBeaconNDB *ndb;

            if(nextIndex != -1)
            {
                for(int i = nextIndex; i < fixList.size(); i++)
                {
                    if((fix = airspaceData->findFix(fixList.at(i))) != nullptr)
                    {
                        polyVertices.append(*fix->getScenePosition());
                    }
                    else if((airport = airspaceData->findAirport(fixList.at(i))) != nullptr)
                    {
                        polyVertices.append(*airport->getScenePosition());
                    }
                    else if((vor = airspaceData->findVOR(fixList.at(i))) != nullptr)
                    {
                        polyVertices.append(*vor->getScenePosition());
                    }
                    else if((ndb = airspaceData->findNDB(fixList.at(i))) != nullptr)
                    {
                        polyVertices.append(*ndb->getScenePosition());
                    }
                }
            }
            else
            {
                if((fix = airspaceData->findFix(nextFix)) != nullptr)
                {
                    polyVertices.append(*fix->getScenePosition());
                }
                else if((airport = airspaceData->findAirport(nextFix)) != nullptr)
                {
                    polyVertices.append(*airport->getScenePosition());
                }
                else if((vor = airspaceData->findVOR(nextFix)) != nullptr)
                {
                    polyVertices.append(*vor->getScenePosition());
                }
                else if((ndb = airspaceData->findNDB(nextFix)) != nullptr)
                {
                    polyVertices.append(*ndb->getScenePosition());
                }
            }

            QPainterPath path;
            path.addPolygon(polyVertices);

            QGraphicsPathItem *poly = new QGraphicsPathItem(path);
            poly->setPen(QPen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH/currentScale));

            prediction = new ATCRoutePrediction();
            prediction->setPolygon(poly);

            //Create labels
            if(nextIndex != -1)
            {
                for(int i = nextIndex; i < fixList.size(); i++)
                {
                    QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(fixList.at(i));
                    prediction->appendLabel(label);
                }
            }
            else
            {
                QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(nextFix);
                prediction->appendLabel(label);
            }

            flight->setRoutePrediction(prediction);
        }
        else
        {
            //Calculate positions of leg fixes & leg lines
            QStringList fixList = flight->getFixList();

            ATCNavFix *fix;
            ATCBeaconVOR *vor;
            ATCAirport *airport;
            ATCBeaconNDB *ndb;

            for(int i = 0; i < fixList.size(); i++)
            {
                if((fix = airspaceData->findFix(fixList.at(i))) != nullptr)
                {
                    polyVertices.append(*fix->getScenePosition());
                }
                else if((airport = airspaceData->findAirport(fixList.at(i))) != nullptr)
                {
                    polyVertices.append(*airport->getScenePosition());
                }
                else if((vor = airspaceData->findVOR(fixList.at(i))) != nullptr)
                {
                    polyVertices.append(*vor->getScenePosition());
                }
                else if((ndb = airspaceData->findNDB(fixList.at(i))) != nullptr)
                {
                    polyVertices.append(*ndb->getScenePosition());
                }
            }

            QPainterPath path;
            path.addPolygon(polyVertices);

            QGraphicsPathItem *poly = new QGraphicsPathItem(path);
            poly->setPen(QPen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH/currentScale));

            prediction = new ATCRoutePrediction();
            prediction->setPolygon(poly);

            //Create labels
            for(int i = 0; i < fixList.size(); i++)
            {
                QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(fixList.at(i));
                prediction->appendLabel(label);
            }

            flight->setRoutePrediction(prediction);
        }

        //Display route prediction
        if(prediction != nullptr)
        {
            prediction->getPolygon()->setPen(QPen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH / currentScale));
            currentScene->addItem(prediction->getPolygon());

            QBrush textBrush(Qt::white);

            QFont textFont("Arial");
            textFont.setPointSizeF(settings->ROUTE_LABEL_HEIGHT / currentScale);

            if(flight->getNavMode() == ATC::Nav)
            {
                for(int i = 0; i < prediction->getLabels().size(); i++)
                {
                    prediction->getLabels().at(i)->setBrush(textBrush);
                    prediction->getLabels().at(i)->setFont(textFont);
                    prediction->getLabels().at(i)->setPos(polyVertices.at(i + 1).x() + settings->ROUTE_LABEL_DX / currentScale,
                                                          polyVertices.at(i + 1).y() + settings->ROUTE_LABEL_DY / currentScale);

                    currentScene->addItem(prediction->getLabels().at(i));
                }
            }
            else
            {
                for(int i = 0; i < prediction->getLabels().size(); i++)
                {
                    prediction->getLabels().at(i)->setBrush(textBrush);
                    prediction->getLabels().at(i)->setFont(textFont);
                    prediction->getLabels().at(i)->setPos(polyVertices.at(i).x() + settings->ROUTE_LABEL_DX / currentScale,
                                                          polyVertices.at(i).y() + settings->ROUTE_LABEL_DY / currentScale);

                    currentScene->addItem(prediction->getLabels().at(i));
                }
            }

            visibleRoutes.append(prediction);
        }
    }
}

void ATCSituationalDisplay::slotDisplayRoute(QStringList fixList)
{
    //Calculate positions of leg fixes & leg lines
    tempPrediction = new ATCRoutePrediction();
    QVector<QPointF> polyVertices;

    ATCNavFix *fix;
    ATCBeaconVOR *vor;
    ATCAirport *airport;
    ATCBeaconNDB *ndb;

    for(int i = 0; i < fixList.size(); i++)
    {
        if((fix = airspaceData->findFix(fixList.at(i))) != nullptr)
        {
            polyVertices.append(*fix->getScenePosition());
        }
        else if((airport = airspaceData->findAirport(fixList.at(i))) != nullptr)
        {
            polyVertices.append(*airport->getScenePosition());
        }
        else if((vor = airspaceData->findVOR(fixList.at(i))) != nullptr)
        {
            polyVertices.append(*vor->getScenePosition());
        }
        else if((ndb = airspaceData->findNDB(fixList.at(i))) != nullptr)
        {
            polyVertices.append(*ndb->getScenePosition());
        }
    }

    QPainterPath path;
    path.addPolygon(polyVertices);

    QGraphicsPathItem *poly = new QGraphicsPathItem(path);
    poly->setPen(QPen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH/currentScale));

    tempPrediction->setPolygon(poly);

    //Create labels
    for(int i = 0; i < fixList.size(); i++)
    {
        QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem(fixList.at(i));
        tempPrediction->appendLabel(label);
    }

    //Display route prediction
    tempPrediction->getPolygon()->setPen(QPen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH / currentScale));
    currentScene->addItem(tempPrediction->getPolygon());

    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->ROUTE_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < tempPrediction->getLabels().size(); i++)
    {
        tempPrediction->getLabels().at(i)->setBrush(textBrush);
        tempPrediction->getLabels().at(i)->setFont(textFont);
        tempPrediction->getLabels().at(i)->setPos(polyVertices.at(i).x() + settings->ROUTE_LABEL_DX / currentScale,
                                              polyVertices.at(i).y() + settings->ROUTE_LABEL_DY / currentScale);

        currentScene->addItem(tempPrediction->getLabels().at(i));
    }

    visibleRoutes.append(tempPrediction);
}

void ATCSituationalDisplay::slotClearRoute(ATCFlight *flight)
{
    ATCRoutePrediction *prediction = flight->getRoutePrediction();

    currentScene->removeItem(prediction->getPolygon());
    for(int i = 0; i < prediction->getLabels().size(); i++)
    {
        currentScene->removeItem(prediction->getLabels().at(i));
    }

    for(int i = 0; i < visibleRoutes.size(); i++)
    {
        if(visibleRoutes.at(i) == prediction) visibleRoutes.remove(i);
    }

    delete prediction;
    flight->setRoutePrediction(nullptr);
}

void ATCSituationalDisplay::slotUpdateRoute(ATCFlight *flight)
{
    slotClearRoute(flight);
    slotDisplayRoute(flight);
}

void ATCSituationalDisplay::slotUpdateTags()
{
    for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
    {
        ATCFlight *flight = simulation->getFlight(i);
        ATCFlightTag *tag = flight->getFlightTag();
        ATCRoutePrediction *prediction = flight->getRoutePrediction();

        State state = flight->getState();

        //Update diamond position
        QPointF diamond = geo2local(state.y, state.x, ATCConst::AVG_DECLINATION);
        tag->moveTo(diamond);

        //Update leader line length & orientation

        double length;
        if(settings->TAG_LEADER_UNIT == ATC::LeaderNM)
        {
            length = ATCMath::nm2m(settings->TAG_LEADER_LENGTH);
        }
        else
        {
            length = flight->getState().v * settings->TAG_LEADER_LENGTH * 60;
        }

        double startLon = ATCMath::rad2deg(state.x);
        double startLat = ATCMath::rad2deg(state.y);
        double endLon = ATCMath::rad2deg(state.x + length / (ATCConst::WGS84_A * qCos(state.y)));
        double endLat = ATCMath::rad2deg(state.y);

        startLon = mercatorProjectionLon(startLon);
        startLat = mercatorProjectionLat(startLat);
        endLon = mercatorProjectionLon(endLon);
        endLat = mercatorProjectionLat(endLat);

        double delta = qFabs(endLon - startLon);

        endLon = startLon + delta * qSin(state.hdg);
        endLat = startLat + delta * qCos(state.hdg);

        double xStart = rotateX(startLon, startLat, ATCConst::AVG_DECLINATION);
        double yStart = rotateY(startLon, startLat, ATCConst::AVG_DECLINATION);
        double xEnd = rotateX(endLon, endLat, ATCConst::AVG_DECLINATION);
        double yEnd = rotateY(endLon, endLat, ATCConst::AVG_DECLINATION);

        xStart = translateToLocalX(xStart);
        yStart = translateToLocalY(yStart);
        xEnd = translateToLocalX(xEnd);
        yEnd = translateToLocalY(yEnd);

        tag->getLeader()->setLine(QLineF(xStart, yStart, xEnd, yEnd));

        //Update etiquettes
        updateEtiquettesQuick(flight);

        //If exist, update route predictions
        if((prediction != nullptr) && (flight->getNavMode() == ATC::Nav)) updateRoutePrediction(flight);
    }
}

void ATCSituationalDisplay::slotUpdateLeaders()
{
    if(simulation != nullptr)
    {
        for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
        {
            ATCFlight *flight = simulation->getFlight(i);
            QGraphicsLineItem *leader = flight->getFlightTag()->getLeader();

            QPointF p1 = leader->line().p1();
            QPointF p2 = leader->line().p2();

            double azimuth = qAtan2(p2.x() - p1.x(), p1.y() - p2.y()) + ATCMath::deg2rad(ATCConst::AVG_DECLINATION);

            double length;
            if(settings->TAG_LEADER_UNIT == ATC::LeaderNM)
            {
                length = ATCMath::nm2m(settings->TAG_LEADER_LENGTH);
            }
            else
            {
                length = flight->getState().v * settings->TAG_LEADER_LENGTH * 60;
            }

            QPointF diamondCoords = local2geo(p1.x(), p1.y(), ATCConst::AVG_DECLINATION); //Format: (lon, lat), deg
            double endLon = diamondCoords.x() + ATCMath::rad2deg(length / (ATCConst::WGS84_A * qCos(ATCMath::deg2rad(diamondCoords.y()))));
            double endLat = diamondCoords.y();

            double startLon = mercatorProjectionLon(diamondCoords.x());
            double startLat = mercatorProjectionLat(diamondCoords.y());
            endLon = mercatorProjectionLon(endLon);
            endLat = mercatorProjectionLat(endLat);

            double delta = qFabs(endLon - startLon);

            endLon = startLon + delta * qSin(azimuth);
            endLat = startLat + delta * qCos(azimuth);

            double xEnd = rotateX(endLon, endLat, ATCConst::AVG_DECLINATION);
            double yEnd = rotateY(endLon, endLat, ATCConst::AVG_DECLINATION);

            xEnd = translateToLocalX(xEnd);
            yEnd = translateToLocalY(yEnd);

            leader->setLine(QLineF(p1.x(), p1.y(), xEnd, yEnd));
        }
    }
}

void ATCSituationalDisplay::slotClearFlightElements(ATCFlight *flight)
{
    for(int i = 0; i < visibleTags.size(); i++)
    {
        if(visibleTags.at(i) == flight->getFlightTag()) visibleTags.remove(i);
    }

    for(int i = 0; i < visibleRoutes.size(); i++)
    {
        if(visibleRoutes.at(i) == flight->getRoutePrediction()) visibleRoutes.remove(i);
    }
}

void ATCSituationalDisplay::slotClearAllFlightElements()
{
    visibleTags.clear();
    visibleRoutes.clear();
}

void ATCSituationalDisplay::situationalDisplaySetup()
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(-0.5 * ATCConst::SCENE_WIDTH, -0.5 * ATCConst::SCENE_HEIGHT, ATCConst::SCENE_WIDTH, ATCConst::SCENE_HEIGHT);

    viewport()->setCursor(Qt::CrossCursor);

    currentScene = new QGraphicsScene(this);
    setScene(currentScene);

    acftCursor = QCursor(QPixmap("../../ATC_Console/ATC_Console/resources/acft_cursor.png"));
}

void ATCSituationalDisplay::rescaleAll()
{
    rescaleSectorsARTCCLow();
    rescaleSectorsARTCCHigh();
    rescaleSectorsARTCC();
    rescaleFixes();
    rescaleFixLabels();
    rescaleVORs();
    rescaleVORLabels();
    rescaleNDBs();
    rescaleNDBLabels();
    rescaleAirports();
    rescaleAirportLabels();
    rescaleExtendedCentrelines();
    rescaleSTARs();
    rescaleSIDs();
    rescaleAirwaysLow();
    rescaleAirwaysHigh();

    rescaleTags();
    rescaleRoutes();
}

void ATCSituationalDisplay::rescaleScene()
{
    qreal scaleX = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);
    qreal scaleY = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);

    scale(scaleX, scaleY);
    currentScale = scaleX;
}

void ATCSituationalDisplay::rescaleSectorsARTCCLow()
{
    if(!visibleSectorsARTCCLow.empty())
    {
        QPen currentPen(visibleSectorsARTCCLow.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCCLow.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCCLow.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorsARTCCHigh()
{
    if(!visibleSectorsARTCCHigh.empty())
    {
        QPen currentPen(visibleSectorsARTCCHigh.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCCHigh.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCCHigh.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorsARTCC()
{
    if(!visibleSectorsARTCC.empty())
    {
        QPen currentPen(visibleSectorsARTCC.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCC.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCC.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCC.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleFixes()
{
    if(!visibleFixes.empty())
    {
        qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleFixes.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleFixes.size(); i++)
        {
            QGraphicsPolygonItem *currentPolygonItem = dynamic_cast<QGraphicsPolygonItem*>(visibleFixes.at(i)->getSymbol());
            QPointF *currentPosition = visibleFixes.at(i)->getScenePosition();

            QVector<QPointF> polygonVertex(4);

            QPointF upperVertex(currentPosition->x(), currentPosition->y() - sideLength * qSqrt(3) / 3);
            QPointF lowerLeftVertex(currentPosition->x() - sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);
            QPointF lowerRightVertex(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);

            polygonVertex[0] = upperVertex;
            polygonVertex[1] = lowerLeftVertex;
            polygonVertex[2] = lowerRightVertex;
            polygonVertex[3] = upperVertex;

            QPolygonF symbolPolygon(polygonVertex);

            currentPolygonItem->setPolygon(symbolPolygon);
            currentPolygonItem->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleFixLabels()
{
    if(!visibleFixes.empty())
    {
        QFont textFont(visibleFixes.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleFixes.size(); i++)
        {
            ATCNavFix *currentFix = visibleFixes.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentFix->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentFix->getScenePosition()->x();
            double positionY = currentFix->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                                 positionY + settings->FIX_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleVORs()
{
    if(!visibleVORs.empty())
    {
        qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleVORs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleVORs.size(); i++)
        {
            QGraphicsRectItem *currentRectItem = dynamic_cast<QGraphicsRectItem*>(visibleVORs.at(i)->getSymbol());
            QPointF *currentPosition = visibleVORs.at(i)->getScenePosition();

            QPointF topLeft(currentPosition->x() - sideLength / 2, currentPosition->y() - sideLength / 2);
            QPointF bottomRight(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength / 2);

            QRectF rect(topLeft, bottomRight);

            currentRectItem->setRect(rect);
            currentRectItem->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleVORLabels()
{
    if(!visibleVORs.empty())
    {
        QFont textFont(visibleVORs.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleVORs.size(); i++)
        {
            ATCBeaconVOR *currentVOR = visibleVORs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentVOR->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentVOR->getScenePosition()->x();
            double positionY = currentVOR->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                                 positionY + settings->VOR_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBs()
{
    if(!visibleNDBs.empty())
    {
        QPen currentPen(visibleNDBs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = dynamic_cast<QGraphicsEllipseItem*>(visibleNDBs.at(i)->getSymbol());
            QPointF *currentPosition = visibleNDBs.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                   settings->NDB_SYMBOL_DIA / currentScale,
                                   settings->NDB_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBLabels()
{
    if(!visibleNDBs.empty())
    {
        QFont textFont(visibleNDBs.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            ATCBeaconNDB *currentNDB = visibleNDBs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentNDB->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentNDB->getScenePosition()->x();
            double positionY = currentNDB->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                                 positionY + settings->NDB_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleAirports()
{
    if(!visibleAirports.empty())
    {
        QPen currentPen(visibleAirports.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = visibleAirports.at(i)->getSymbol();
            QPointF *currentPosition = visibleAirports.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   settings->AIRPORT_SYMBOL_DIA / currentScale,
                                   settings->AIRPORT_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleAirportLabels()
{
    if(!visibleAirports.empty())
    {
        QFont textFont(visibleAirports.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            ATCAirport *currentAirport = visibleAirports.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentAirport->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentAirport->getScenePosition()->x();
            double positionY = currentAirport->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                                 positionY + settings->AIRPORT_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleExtendedCentrelines()
{
    if(!visibleCentrelines.empty())
    {
        QPen currentPen(visibleCentrelines.at(0)->getCentreline()->pen());
        currentPen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleCentrelines.size(); i++)
        {
            ATCRunwayExtendedCentreline *current(visibleCentrelines.at(i));
            current->getCentreline()->setPen(currentPen);

            QVector<QGraphicsLineItem*> ticks(current->getTicksVector());
            for(int i = 0; i < ticks.size(); i++)
            {
                ticks.at(i)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSTARs()
{
    if(!visibleSTARs.empty())
    {
        QPen currentPen(visibleSTARs.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSTARs.size(); i++)
        {
            for(int j = 0; j < visibleSTARs.at(i)->getCoordsVectorSize(); j++)
            {
                visibleSTARs.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSIDs()
{
    if(!visibleSIDs.empty())
    {
        QPen currentPen(visibleSIDs.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSIDs.size(); i++)
        {
            for(int j = 0; j < visibleSIDs.at(i)->getCoordsVectorSize(); j++)
            {
                visibleSIDs.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwaysLow()
{
    if(!visibleLowAirways.empty())
    {
        QPen currentPen(visibleLowAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleLowAirways.size(); i++)
        {
            for(int j = 0; j < visibleLowAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleLowAirways.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwaysHigh()
{
    if(!visibleHighAirways.empty())
    {
        QPen currentPen(visibleHighAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleHighAirways.size(); i++)
        {
            for(int j = 0; j < visibleHighAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleHighAirways.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleTags()
{
    if(!visibleTags.empty())
    {
        double sideLength = settings->TAG_DIAMOND_WIDTH / currentScale;
        double leaderWidth = settings->TAG_LEADER_WIDTH / currentScale;

        double boxWidth = settings->TAG_BOX_WIDTH / currentScale;
        double boxHeight;

        double connectorWidth = settings->TAG_CONNECTOR_WIDTH / currentScale;

        double tagFont = settings->TAG_LABEL_HEIGHT / currentScale;
        double labelMargins = settings->TAG_LABEL_MARGINS / currentScale;

        QPen leaderPen = visibleTags.at(0)->getLeader()->pen();
        leaderPen.setWidthF(leaderWidth);

        QPen connectorPen = visibleTags.at(0)->getConnector()->pen();
        connectorPen.setWidthF(connectorWidth);

        QFont labelFont = visibleTags.at(0)->getText()->font();
        labelFont.setPointSizeF(tagFont);

        for(int i = 0; i < visibleTags.size(); i++)
        {
            if(visibleTags.at(i)->getTagType() == ATC::Short)
            {
                boxHeight = settings->TAG_BOX_HEIGHT / currentScale;
            }
            else
            {
                boxHeight = settings->TAG_BOX_HEIGHT_FULL / currentScale;
            }

            ATCTagDiamond *diamond = visibleTags.at(i)->getDiamond();
            QGraphicsLineItem *connector = visibleTags.at(i)->getConnector();
            ATCTagRect *tagBox = visibleTags.at(i)->getTagBox();

            QPointF currentPosition = visibleTags.at(i)->getDiamondPosition();

            diamond->setRect(currentPosition.x() - sideLength/2, currentPosition.y() - sideLength/2, sideLength, sideLength);
            QPointF topLeft = diamond->mapToScene(diamond->rect().topLeft());
            diamond->moveBy(currentPosition.x() - topLeft.x() - sideLength/2, currentPosition.y() - topLeft.y() - sideLength/2);

            QPointF initial = connector->line().p2();

            double dx = visibleTags.at(i)->getDX() / currentScale;
            double dy = visibleTags.at(i)->getDY() / currentScale;

            connector->setLine(QLineF(currentPosition.x(), currentPosition.y(), currentPosition.x() + dx, currentPosition.y() + dy));
            connector->setPen(connectorPen);

            QPointF final = connector->line().p2();

            double rectX = tagBox->rect().x();
            double rectY = tagBox->rect().y();

            double dw = boxWidth - tagBox->rect().width();
            double dh = boxHeight - tagBox->rect().height();

            double translationX = final.x() - initial.x() - dw/2;
            double translationY = final.y() - initial.y() - dh/2;

            tagBox->setRect(rectX, rectY, boxWidth, boxHeight);
            tagBox->moveBy(translationX, translationY);

            visibleTags.at(i)->getText()->setPos(rectX + labelMargins, rectY + labelMargins);

            visibleTags.at(i)->getLeader()->setPen(leaderPen);
            visibleTags.at(i)->getText()->setFont(labelFont);
        }
    }
}

void ATCSituationalDisplay::rescaleRoutes()
{
    if(!visibleRoutes.isEmpty())
    {
        QPen pen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH / currentScale);

        QFont textFont("Arial");
        textFont.setPointSizeF(settings->ROUTE_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleRoutes.size(); i++)
        {
            QGraphicsPathItem *currentPath = visibleRoutes.at(i)->getPolygon();
            QVector<QGraphicsSimpleTextItem*> labels = visibleRoutes.at(i)->getLabels();

            currentPath->setPen(pen);

            QList<QPointF> pointList = currentPath->path().toFillPolygon().toList();

            for(int j = 0; j < labels.size(); j++)
            {
                if(pointList.size() - labels.size() == 1) //ATC::Hdg mode
                {
                    labels.at(j)->setFont(textFont);
                    labels.at(j)->setPos(pointList.at(j).x() + settings->ROUTE_LABEL_DX / currentScale,
                                         pointList.at(j).y() + settings->ROUTE_LABEL_DY / currentScale);
                }
                else //ATC::Nav mode
                {
                    labels.at(j)->setFont(textFont);
                    labels.at(j)->setPos(pointList.at(j + 1).x() + settings->ROUTE_LABEL_DX / currentScale,
                                         pointList.at(j + 1).y() + settings->ROUTE_LABEL_DY / currentScale);
                }
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorARTCCLow(ATCSectorARTCCLow *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSectorARTCCHigh(ATCSectorARTCCHigh *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSectorARTCC(ATCSectorARTCC *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleFix(ATCNavFix *object)
{
    qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

    QGraphicsPolygonItem *currentPolygonItem = dynamic_cast<QGraphicsPolygonItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    QVector<QPointF> polygonVertex(4);

    QPointF upperVertex(currentPosition->x(), currentPosition->y() - sideLength * qSqrt(3) / 3);
    QPointF lowerLeftVertex(currentPosition->x() - sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);
    QPointF lowerRightVertex(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);

    polygonVertex[0] = upperVertex;
    polygonVertex[1] = lowerLeftVertex;
    polygonVertex[2] = lowerRightVertex;
    polygonVertex[3] = upperVertex;

    QPolygonF symbolPolygon(polygonVertex);

    currentPolygonItem->setPolygon(symbolPolygon);
    currentPolygonItem->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                         positionY + settings->FIX_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleVOR(ATCBeaconVOR *object)
{
    qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

    QGraphicsRectItem *currentRectItem = dynamic_cast<QGraphicsRectItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    QPointF topLeft(currentPosition->x() - sideLength / 2, currentPosition->y() - sideLength / 2);
    QPointF bottomRight(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength / 2);

    QRectF rect(topLeft, bottomRight);

    currentRectItem->setRect(rect);
    currentRectItem->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                         positionY + settings->VOR_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleNDB(ATCBeaconNDB *object)
{
    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

    QGraphicsEllipseItem *currentSymbol = dynamic_cast<QGraphicsEllipseItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    currentSymbol->setRect(currentPosition->x() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                           currentPosition->y() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                           settings->NDB_SYMBOL_DIA / currentScale,
                           settings->NDB_SYMBOL_DIA / currentScale);
    currentSymbol->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                         positionY + settings->NDB_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleAirport(ATCAirport *object)
{
    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

    QGraphicsEllipseItem *currentSymbol = object->getSymbol();
    QPointF *currentPosition = object->getScenePosition();

    currentSymbol->setRect(currentPosition->x() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                           currentPosition->y() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                           settings->AIRPORT_SYMBOL_DIA / currentScale,
                           settings->AIRPORT_SYMBOL_DIA / currentScale);
    currentSymbol->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                         positionY + settings->AIRPORT_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleExtendedCentreline(ATCRunwayExtendedCentreline *object)
{
    QPen currentPen(object->getCentreline()->pen());
    currentPen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

    object->getCentreline()->setPen(currentPen);

    QVector<QGraphicsLineItem*> ticks(object->getTicksVector());
    for(int i = 0; i < ticks.size(); i++)
    {
        ticks.at(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSTAR(ATCProcedureSTARSymbol *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSID(ATCProcedureSIDSymbol *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleAirwayLow(ATCAirwayLow *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleAirwayHigh(ATCAirwayHigh *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleTag(ATCFlightTag *tag)
{
    double sideLength = settings->TAG_DIAMOND_WIDTH / currentScale;
    double leaderWidth = settings->TAG_LEADER_WIDTH / currentScale;

    double boxWidth = settings->TAG_BOX_WIDTH / currentScale;
    double boxHeight;

    double connectorWidth = settings->TAG_CONNECTOR_WIDTH / currentScale;

    double tagFont = settings->TAG_LABEL_HEIGHT / currentScale;
    double labelMargins = settings->TAG_LABEL_MARGINS / currentScale;

    QPen leaderPen = tag->getLeader()->pen();
    leaderPen.setWidthF(leaderWidth);

    QPen connectorPen = tag->getConnector()->pen();
    connectorPen.setWidthF(connectorWidth);

    QFont labelFont = tag->getText()->font();
    labelFont.setPointSizeF(tagFont);

    if(tag->getTagType() == ATC::Short)
    {
        boxHeight = settings->TAG_BOX_HEIGHT / currentScale;
    }
    else
    {
        boxHeight = settings->TAG_BOX_HEIGHT_FULL / currentScale;
    }

    ATCTagDiamond *diamond = tag->getDiamond();
    QGraphicsLineItem *connector = tag->getConnector();
    ATCTagRect *tagBox = tag->getTagBox();

    QPointF currentPosition = tag->getDiamondPosition();

    diamond->setRect(currentPosition.x() - sideLength/2, currentPosition.y() - sideLength/2, sideLength, sideLength);
    QPointF topLeft = diamond->mapToScene(diamond->rect().topLeft());
    diamond->moveBy(currentPosition.x() - topLeft.x() - sideLength/2, currentPosition.y() - topLeft.y() - sideLength/2);

    QPointF initial = connector->line().p2();

    double dx = tag->getDX() / currentScale;
    double dy = tag->getDY() / currentScale;

    connector->setLine(QLineF(currentPosition.x(), currentPosition.y(), currentPosition.x() + dx, currentPosition.y() + dy));
    connector->setPen(connectorPen);

    QPointF final = connector->line().p2();

    double rectX = tagBox->rect().x();
    double rectY = tagBox->rect().y();

    double dw = boxWidth - tagBox->rect().width();
    double dh = boxHeight - tagBox->rect().height();

    double translationX = final.x() - initial.x() - dw/2;
    double translationY = final.y() - initial.y() - dh/2;

    tagBox->setRect(rectX, rectY, boxWidth, boxHeight);
    tagBox->moveBy(translationX, translationY);

    tag->getText()->setPos(rectX + labelMargins, rectY + labelMargins);

    tag->getLeader()->setPen(leaderPen);
    tag->getText()->setFont(labelFont);
}

void ATCSituationalDisplay::rescaleRoute(ATCRoutePrediction *route)
{
    QPen pen(settings->ROUTE_COLOR, settings->ROUTE_LINE_WIDTH / currentScale);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->ROUTE_LABEL_HEIGHT / currentScale);

    QGraphicsPathItem *currentPath = route->getPolygon();
    QVector<QGraphicsSimpleTextItem*> labels = route->getLabels();

    currentPath->setPen(pen);

    QList<QPointF> pointList = currentPath->path().toFillPolygon().toList();

    for(int j = 0; j < labels.size(); j++)
    {
        if(pointList.size() - labels.size() == 1) //ATC::Hdg mode
        {
            labels.at(j)->setFont(textFont);
            labels.at(j)->setPos(pointList.at(j).x() + settings->ROUTE_LABEL_DX / currentScale,
                                 pointList.at(j).y() + settings->ROUTE_LABEL_DY / currentScale);
        }
        else //ATC::Nav mode
        {
            labels.at(j)->setFont(textFont);
            labels.at(j)->setPos(pointList.at(j + 1).x() + settings->ROUTE_LABEL_DX / currentScale,
                                 pointList.at(j + 1).y() + settings->ROUTE_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::projectSectorsARTCCLow()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCCLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCCLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCCLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCCLow(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCCLow(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::projectSectorsARTCCHigh()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCCHigh(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCCHigh(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCCHigh(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCCHigh(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCCHigh(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::projectSectorsARTCC()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCC(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCC(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCC(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCC(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCC(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::calculateSectorsARTCCLow()
{
    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCCLow(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCCLow(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
       }

    }

//Display ARTCC Low lines on scene
    QPen pen(settings->ARTCC_LOW_COLOR);
    pen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCLow(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCCLow(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSectorsARTCCHigh()
{
    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCCHigh(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCCHigh(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
        }
    }

//Display ARTCC High lines on scene
    QPen pen(settings->ARTCC_HIGH_COLOR);
    pen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCHigh(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCCHigh(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSectorsARTCC()
{
    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCC(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCC(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCC(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
        }
    }

//Display ARTCC lines on scene
    QPen pen(settings->ARTCC_COLOR);
    pen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCC(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCC(i)->hide();
    }
}

void ATCSituationalDisplay::calculateFixes()
{
    QVector<coord> tempFixes;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of fixes + rotation
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        coord currentFix;

        currentFix.x = mercatorProjectionLon(airspaceData->getFix(i)->longitude());
        currentFix.y = mercatorProjectionLat(airspaceData->getFix(i)->latitude());

        double xRotated = rotateX(currentFix.x, currentFix.y, rotationDeg);
        double yRotated = rotateY(currentFix.x, currentFix.y, rotationDeg);

        currentFix.x = xRotated;
        currentFix.y = yRotated;

        tempFixes.append(currentFix);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        tempFixes[i].x = translateToLocalX(tempFixes.at(i).x);
        tempFixes[i].y = translateToLocalY(tempFixes.at(i).y);

        airspaceData->getFix(i)->setScenePosition(new QPointF(tempFixes.at(i).x, tempFixes.at(i).y));
    }

//Build and position symbol polygons
    int vertexNumber = 3;
    qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QVector<QPointF> polygonVertex(vertexNumber + 1);

        QPointF upperVertex(tempFixes.at(i).x, tempFixes.at(i).y - sideLength * qSqrt(3) / 3);
        QPointF lowerLeftVertex(tempFixes.at(i).x - sideLength / 2, tempFixes.at(i).y + sideLength * qSqrt(3) / 6);
        QPointF lowerRightVertex(tempFixes.at(i).x + sideLength / 2, tempFixes.at(i).y + sideLength * qSqrt(3) / 6);

        polygonVertex[0] = upperVertex;
        polygonVertex[1] = lowerLeftVertex;
        polygonVertex[2] = lowerRightVertex;
        polygonVertex[3] = upperVertex;

        QPolygonF symbolPolygon(polygonVertex);
        airspaceData->getFix(i)->setSymbol(new QGraphicsPolygonItem(symbolPolygon));
    }

//Display fix symbols on scene
    QPen pen(settings->FIX_COLOR);
    pen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol = airspaceData->getFix(i)->getSymbol();

        currentSymbol->setPen(pen);
        currentScene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        ATCNavFix *currentFix = airspaceData->getFix(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentFix->getName());
        currentFix->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentFix->getScenePosition()->x();
        double positionY = currentFix->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                             positionY + settings->FIX_LABEL_DY / currentScale);

        currentScene->addItem(currentLabel);
        currentFix->hide();
    }
}

void ATCSituationalDisplay::calculateAirports()
{
    QVector<coord> tempAirports;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of airports + rotation
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        coord currentAirport;

        currentAirport.x = mercatorProjectionLon(airspaceData->getAirport(i)->longitude());
        currentAirport.y = mercatorProjectionLat(airspaceData->getAirport(i)->latitude());

        double xRotated = rotateX(currentAirport.x, currentAirport.y, rotationDeg);
        double yRotated = rotateY(currentAirport.x, currentAirport.y, rotationDeg);

        currentAirport.x = xRotated;
        currentAirport.y = yRotated;

        tempAirports.append(currentAirport);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        tempAirports[i].x = translateToLocalX(tempAirports.at(i).x);
        tempAirports[i].y = translateToLocalY(tempAirports.at(i).y);

        airspaceData->getAirport(i)->setScenePosition(new QPointF(tempAirports.at(i).x, tempAirports.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        airspaceData->getAirport(i)->setSymbol(new QGraphicsEllipseItem(tempAirports.at(i).x - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        tempAirports.at(i).y - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        settings->AIRPORT_SYMBOL_DIA / currentScale,
                                                                        settings->AIRPORT_SYMBOL_DIA / currentScale));
    }

//Display airport symbols on scene
    QPen pen(settings->AIRPORT_COLOR);
    pen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        QGraphicsEllipseItem *currentSymbol(airspaceData->getAirport(i)->getSymbol());

        currentSymbol->setPen(pen);
        currentScene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        ATCAirport *currentAirport = airspaceData->getAirport(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentAirport->getName());
        currentAirport->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentAirport->getScenePosition()->x();
        double positionY = currentAirport->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                             positionY + settings->AIRPORT_LABEL_DY / currentScale);

        currentScene->addItem(currentLabel);
        currentAirport->hide();
    }
}

void ATCSituationalDisplay::calculateExtendedCentrelines()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    QVector<coord> rwyCoords1;
    QVector<coord> rwyCoords2;
    QVector<coord> centrelineEnd1;
    QVector<coord> centrelineEnd2;
    QVector<double> azimuths;

//calculate lat and lon of extended centreline ends
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);

                coord rwy1;
                coord rwy2;

                rwy1.x = currentRunway->getStartPoint().longitude();
                rwy1.y = currentRunway->getStartPoint().latitude();
                rwy2.x = currentRunway->getEndPoint().longitude();
                rwy2.y = currentRunway->getEndPoint().latitude();

                coord centreEnd1;
                coord centreEnd2;

                centreEnd1.x = rwy1.x + settings->RUNWAY_CENTELINE_LENGTH * ATCConst::NM_2_M / (ATCConst::WGS84_A * qCos(rwy1.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                centreEnd1.y = rwy1.y;
                centreEnd2.x = rwy2.x + settings->RUNWAY_CENTELINE_LENGTH * ATCConst::NM_2_M / (ATCConst::WGS84_A * qCos(rwy2.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                centreEnd2.y = rwy2.y;

                rwyCoords1.append(rwy1);
                rwyCoords2.append(rwy2);
                centrelineEnd1.append(centreEnd1);
                centrelineEnd2.append(centreEnd2);
            }
        }
    }

//calculate Mercator projection and rotation of points
    for(int i = 0; i < rwyCoords1.size(); i++)
    {
        //Project to 2D
        rwyCoords1[i].x = mercatorProjectionLon(rwyCoords1.at(i).x);
        rwyCoords1[i].y = mercatorProjectionLat(rwyCoords1.at(i).y);

        rwyCoords2[i].x = mercatorProjectionLon(rwyCoords2.at(i).x);
        rwyCoords2[i].y = mercatorProjectionLat(rwyCoords2.at(i).y);

        centrelineEnd1[i].x = mercatorProjectionLon(centrelineEnd1.at(i).x);
        centrelineEnd1[i].y = mercatorProjectionLat(centrelineEnd1.at(i).y);

        centrelineEnd2[i].x = mercatorProjectionLon(centrelineEnd2.at(i).x);
        centrelineEnd2[i].y = mercatorProjectionLat(centrelineEnd2.at(i).y);

        //Translate to local csys and rotate extended centrelines to match runway azimuth
        double deltaProjectedLon1 = centrelineEnd1.at(i).x - rwyCoords1.at(i).x;
        double deltaProjectedLon2 = centrelineEnd2.at(i).x - rwyCoords2.at(i).x;

        double azimuth = qAtan2(rwyCoords2.at(i).x - rwyCoords1.at(i).x, rwyCoords2.at(i).y - rwyCoords1.at(i).y);
        azimuths.append(azimuth);

        centrelineEnd1[i].x = deltaProjectedLon1 * qCos(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwyCoords1.at(i).x;
        centrelineEnd1[i].y = deltaProjectedLon1 * qSin(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwyCoords1.at(i).y;

        centrelineEnd2[i].x = deltaProjectedLon2 * qCos(ATCConst::PI / 2 - azimuth) + rwyCoords2.at(i).x;
        centrelineEnd2[i].y = deltaProjectedLon2 * qSin(ATCConst::PI / 2 - azimuth) + rwyCoords2.at(i).y;

        //Rotate whole map so up matches magnetic north
        double rwyCoords1xRot = rotateX(rwyCoords1.at(i).x, rwyCoords1.at(i).y, rotationDeg);
        double rwyCoords1yRot = rotateY(rwyCoords1.at(i).x, rwyCoords1.at(i).y, rotationDeg);

        double rwyCoords2xRot = rotateX(rwyCoords2.at(i).x, rwyCoords2.at(i).y, rotationDeg);
        double rwyCoords2yRot = rotateY(rwyCoords2.at(i).x, rwyCoords2.at(i).y, rotationDeg);

        double centrelineEnd1xRot = rotateX(centrelineEnd1.at(i).x, centrelineEnd1.at(i).y, rotationDeg);
        double centrelineEnd1yRot = rotateY(centrelineEnd1.at(i).x, centrelineEnd1.at(i).y, rotationDeg);

        double centrelineEnd2xRot = rotateX(centrelineEnd2.at(i).x, centrelineEnd2.at(i).y, rotationDeg);
        double centrelineEnd2yRot = rotateY(centrelineEnd2.at(i).x, centrelineEnd2.at(i).y, rotationDeg);

        rwyCoords1[i].x = rwyCoords1xRot;
        rwyCoords1[i].y = rwyCoords1yRot;

        rwyCoords2[i].x = rwyCoords2xRot;
        rwyCoords2[i].y = rwyCoords2yRot;

        centrelineEnd1[i].x = centrelineEnd1xRot;
        centrelineEnd1[i].y = centrelineEnd1yRot;

        centrelineEnd2[i].x = centrelineEnd2xRot;
        centrelineEnd2[i].y = centrelineEnd2yRot;
    }

//Translate to local & scene coords
    for(int i = 0; i < rwyCoords1.size(); i++)
    {
        rwyCoords1[i].x = translateToLocalX(rwyCoords1.at(i).x);
        rwyCoords1[i].y = translateToLocalY(rwyCoords1.at(i).y);

        rwyCoords2[i].x = translateToLocalX(rwyCoords2.at(i).x);
        rwyCoords2[i].y = translateToLocalY(rwyCoords2.at(i).y);

        centrelineEnd1[i].x = translateToLocalX(centrelineEnd1.at(i).x);
        centrelineEnd1[i].y = translateToLocalY(centrelineEnd1.at(i).y);

        centrelineEnd2[i].x = translateToLocalX(centrelineEnd2.at(i).x);
        centrelineEnd2[i].y = translateToLocalY(centrelineEnd2.at(i).y);
    }

//Construct and assign symbols
    int k = 0;

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);

                QGraphicsLineItem *centreline1 = new QGraphicsLineItem(rwyCoords1.at(k).x, rwyCoords1.at(k).y, centrelineEnd1.at(k).x, centrelineEnd1.at(k).y);
                QGraphicsLineItem *centreline2 = new QGraphicsLineItem(rwyCoords2.at(k).x, rwyCoords2.at(k).y, centrelineEnd2.at(k).x, centrelineEnd2.at(k).y);

                currentRunway->setExtendedCentreline1(new ATCRunwayExtendedCentreline(centreline1));
                currentRunway->setExtendedCentreline2(new ATCRunwayExtendedCentreline(centreline2));
                currentRunway->setAzimuth(azimuths.at(k));

                k++;
            }
        }
    }

//Display extended centrelines
    QPen pen(settings->RUNWAY_COLOR);
    pen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);
                ATCRunwayExtendedCentreline *currentCentreline1 = currentRunway->getExtendedCentreline1();
                ATCRunwayExtendedCentreline *currentCentreline2 = currentRunway->getExtendedCentreline2();

                currentCentreline1->getCentreline()->setPen(pen);
                currentCentreline2->getCentreline()->setPen(pen);

                currentScene->addItem(currentCentreline1->getCentreline());
                currentScene->addItem(currentCentreline2->getCentreline());
            }
        }
    }
}

void ATCSituationalDisplay::calculateCentrelineTicks()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *current(airspaceData->getAirport(i)->getRunway(j));

                for(int k = 0; k < (settings->RUNWAY_CENTELINE_LENGTH - settings->TICK_FIRST_DISTANCE) / settings->TICK_SEPARATION + 1; k++)
                {
                    //Initialize runway tresholds
                    coord rwy1;
                    coord rwy2;

                    rwy1.x = current->getStartPoint().longitude();
                    rwy1.y = current->getStartPoint().latitude();
                    rwy2.x = current->getEndPoint().longitude();
                    rwy2.y = current->getEndPoint().latitude();

                    //Calculate position of tick middle
                    coord tick1mid;
                    coord tick2mid;

                    double localScale = qCos((rwy1.y + rwy2.y) / 2 * ATCConst::DEG_2_RAD);

                    tick1mid.x = rwy1.x + (settings->TICK_FIRST_DISTANCE + k * settings->TICK_SEPARATION) * ATCConst::NM_2_M / (ATCConst::WGS84_A * qCos(rwy1.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                    tick1mid.y = rwy1.y;
                    tick2mid.x = rwy2.x + (settings->TICK_FIRST_DISTANCE + k * settings->TICK_SEPARATION) * ATCConst::NM_2_M / (ATCConst::WGS84_A * qCos(rwy2.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                    tick2mid.y = rwy2.y;

                    //Project to 2D
                    rwy1.x = mercatorProjectionLon(rwy1.x);
                    rwy1.y = mercatorProjectionLat(rwy1.y);

                    rwy2.x = mercatorProjectionLon(rwy2.x);
                    rwy2.y = mercatorProjectionLat(rwy2.y);

                    tick1mid.x = mercatorProjectionLon(tick1mid.x);
                    tick1mid.y = mercatorProjectionLat(tick1mid.y);

                    tick2mid.x = mercatorProjectionLon(tick2mid.x);
                    tick2mid.y = mercatorProjectionLat(tick2mid.y);

                    //Translate to local csys and rotate to match runway azimuth
                    double deltaTickLon1 = tick1mid.x - rwy1.x;
                    double deltaTickLon2 = tick2mid.x - rwy2.x;

                    double azimuth = qAtan2(rwy2.x - rwy1.x, rwy2.y - rwy1.y);

                    tick1mid.x = deltaTickLon1 * qCos(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwy1.x;
                    tick1mid.y = deltaTickLon1 * qSin(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwy1.y;

                    tick2mid.x = deltaTickLon2 * qCos(ATCConst::PI / 2 - azimuth) + rwy2.x;
                    tick2mid.y = deltaTickLon2 * qSin(ATCConst::PI / 2 - azimuth) + rwy2.y;

                    //Calculate tick section ends using localScale
                    coord tick1start;
                    coord tick1end;
                    coord tick2start;
                    coord tick2end;

                    double deltaTickLon;

                    if (((k % static_cast<int>(settings->TICK_MAJOR_SEPARATION / settings->TICK_SEPARATION)) == 0) && (k >= (settings->TICK_FIRST_MAJOR_AT - settings->TICK_FIRST_DISTANCE) / settings->TICK_SEPARATION))
                    {
                        deltaTickLon = mercatorProjectionLon(settings->TICK_MAJOR_LENGTH * ATCConst::NM_2_M / 2 / (ATCConst::WGS84_A * localScale) * ATCConst::RAD_2_DEG);
                    }
                    else
                    {
                        deltaTickLon = mercatorProjectionLon(settings->TICK_MINOR_LENGTH * ATCConst::NM_2_M / 2 / (ATCConst::WGS84_A * localScale) * ATCConst::RAD_2_DEG);
                    }

                    tick1start.x = deltaTickLon * qCos(ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.x;
                    tick1start.y = deltaTickLon * qSin(ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.y;

                    tick1end.x = deltaTickLon * qCos(2 * ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.x;
                    tick1end.y = deltaTickLon * qSin(2 * ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.y;

                    tick2start.x = deltaTickLon * qCos(ATCConst::PI - azimuth) + tick2mid.x;
                    tick2start.y = deltaTickLon * qSin(ATCConst::PI - azimuth) + tick2mid.y;

                    tick2end.x = deltaTickLon * qCos(2 * ATCConst::PI - azimuth) + tick2mid.x;
                    tick2end.y = deltaTickLon * qSin(2 * ATCConst::PI - azimuth) + tick2mid.y;

                    //Rotate tick centre to match magnetic north
                    coord rwy1rot;
                    coord rwy2rot;
                    coord tick1rot;
                    coord tick2rot;

                    rwy1rot.x = rotateX(rwy1.x, rwy1.y, rotationDeg);
                    rwy1rot.y = rotateY(rwy1.x, rwy1.y, rotationDeg);

                    rwy2rot.x = rotateX(rwy2.x, rwy2.y, rotationDeg);
                    rwy2rot.y = rotateY(rwy2.x, rwy2.y, rotationDeg);

                    tick1rot.x = rotateX(tick1mid.x, tick1mid.y, rotationDeg);
                    tick1rot.y = rotateY(tick1mid.x, tick1mid.y, rotationDeg);

                    tick2rot.x = rotateX(tick2mid.x, tick2mid.y, rotationDeg);
                    tick2rot.y = rotateY(tick2mid.x, tick2mid.y, rotationDeg);

                    rwy1.x = rwy1rot.x;
                    rwy1.y = rwy1rot.y;

                    rwy2.x = rwy2rot.x;
                    rwy2.y = rwy2rot.y;

                    tick1mid.x = tick1rot.x;
                    tick1mid.y = tick1rot.y;

                    tick2mid.x = tick2rot.x;
                    tick2mid.y = tick2rot.y;

                    //Rotate tick ends to match magnetick north
                    coord tick1startRot;
                    coord tick1endRot;
                    coord tick2startRot;
                    coord tick2endRot;

                    tick1startRot.x = rotateX(tick1start.x, tick1start.y, rotationDeg);
                    tick1startRot.y = rotateY(tick1start.x, tick1start.y, rotationDeg);

                    tick1endRot.x = rotateX(tick1end.x, tick1end.y, rotationDeg);
                    tick1endRot.y = rotateY(tick1end.x, tick1end.y, rotationDeg);

                    tick2startRot.x = rotateX(tick2start.x, tick2start.y, rotationDeg);
                    tick2startRot.y = rotateY(tick2start.x, tick2start.y, rotationDeg);

                    tick2endRot.x = rotateX(tick2end.x, tick2end.y, rotationDeg);
                    tick2endRot.y = rotateY(tick2end.x, tick2end.y, rotationDeg);

                    tick1start.x = tick1startRot.x;
                    tick1start.y = tick1startRot.y;

                    tick1end.x = tick1endRot.x;
                    tick1end.y = tick1endRot.y;

                    tick2start.x = tick2startRot.x;
                    tick2start.y = tick2startRot.y;

                    tick2end.x = tick2endRot.x;
                    tick2end.y = tick2endRot.y;

                    //Translate to local scene coords
                    rwy1.x = translateToLocalX(rwy1.x);
                    rwy1.y = translateToLocalY(rwy1.y);

                    rwy2.x = translateToLocalX(rwy2.x);
                    rwy2.y = translateToLocalY(rwy2.y);

                    tick1mid.x = translateToLocalX(tick1mid.x);
                    tick1mid.y = translateToLocalY(tick1mid.y);

                    tick2mid.x = translateToLocalX(tick2mid.x);
                    tick2mid.y = translateToLocalY(tick2mid.y);

                    tick1start.x = translateToLocalX(tick1start.x);
                    tick1start.y = translateToLocalY(tick1start.y);

                    tick1end.x = translateToLocalX(tick1end.x);
                    tick1end.y = translateToLocalY(tick1end.y);

                    tick2start.x = translateToLocalX(tick2start.x);
                    tick2start.y = translateToLocalY(tick2start.y);

                    tick2end.x = translateToLocalX(tick2end.x);
                    tick2end.y = translateToLocalY(tick2end.y);

                    //Construct tick symbols
                    QGraphicsLineItem *tickLine1 = new QGraphicsLineItem(tick1start.x, tick1start.y, tick1end.x, tick1end.y);
                    QGraphicsLineItem *tickLine2 = new QGraphicsLineItem(tick2start.x, tick2start.y, tick2end.x, tick2end.y);

                    current->getExtendedCentreline1()->appendTick(tickLine1);
                    current->getExtendedCentreline2()->appendTick(tickLine2);

                    //Display ticks
                    QPen pen(settings->RUNWAY_COLOR);
                    pen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

                    tickLine1->setPen(pen);
                    tickLine2->setPen(pen);

                    currentScene->addItem(tickLine1);
                    currentScene->addItem(tickLine2);
                }

                current->getExtendedCentreline1()->hide();
                current->getExtendedCentreline2()->hide();
            }
        }
    }
}

void ATCSituationalDisplay::calculateVORs()
{
    QVector<coord> tempVORs;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of vors + rotation
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        coord currentVOR;

        currentVOR.x = mercatorProjectionLon(airspaceData->getVOR(i)->longitude());
        currentVOR.y = mercatorProjectionLat(airspaceData->getVOR(i)->latitude());

        double xRotated = rotateX(currentVOR.x, currentVOR.y, rotationDeg);
        double yRotated = rotateY(currentVOR.x, currentVOR.y, rotationDeg);

        currentVOR.x = xRotated;
        currentVOR.y = yRotated;

        tempVORs.append(currentVOR);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        tempVORs[i].x = translateToLocalX(tempVORs.at(i).x);
        tempVORs[i].y = translateToLocalY(tempVORs.at(i).y);

        airspaceData->getVOR(i)->setScenePosition(new QPointF(tempVORs.at(i).x, tempVORs.at(i).y));
    }

//Build and position symbol polygons
    qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QPointF topLeft(tempVORs.at(i).x - sideLength / 2, tempVORs.at(i).y - sideLength / 2);
        QPointF bottomRight(tempVORs.at(i).x + sideLength / 2, tempVORs.at(i).y + sideLength / 2);

        QRectF rect(topLeft, bottomRight);
        airspaceData->getVOR(i)->setSymbol(new QGraphicsRectItem(rect));
    }

//Display VOR symbols on scene
    QPen pen(settings->VOR_COLOR);
    pen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentRect(airspaceData->getVOR(i)->getSymbol());

        currentRect->setPen(pen);
        currentScene->addItem(currentRect);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        ATCBeaconVOR *currentVOR = airspaceData->getVOR(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentVOR->getName());
        currentVOR->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentVOR->getScenePosition()->x();
        double positionY = currentVOR->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                             positionY + settings->VOR_LABEL_DY / currentScale);

        currentScene->addItem(currentLabel);
        currentVOR->hide();
    }
}

void ATCSituationalDisplay::calculateNDBs()
{
    QVector<coord> tempNDBs;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of NDBs + rotation
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        coord currentNDB;

        currentNDB.x = mercatorProjectionLon(airspaceData->getNDB(i)->longitude());
        currentNDB.y = mercatorProjectionLat(airspaceData->getNDB(i)->latitude());

        double xRotated = rotateX(currentNDB.x, currentNDB.y, rotationDeg);
        double yRotated = rotateY(currentNDB.x, currentNDB.y, rotationDeg);

        currentNDB.x = xRotated;
        currentNDB.y = yRotated;

        tempNDBs.append(currentNDB);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        tempNDBs[i].x = translateToLocalX(tempNDBs.at(i).x);
        tempNDBs[i].y = translateToLocalY(tempNDBs.at(i).y);

        airspaceData->getNDB(i)->setScenePosition(new QPointF(tempNDBs.at(i).x, tempNDBs.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        airspaceData->getNDB(i)->setSymbol(new QGraphicsEllipseItem(tempNDBs.at(i).x - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    tempNDBs.at(i).y - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    settings->NDB_SYMBOL_DIA / currentScale,
                                                                    settings->NDB_SYMBOL_DIA / currentScale));
    }

//Display NDB symbols on scene
    QPen pen(settings->NDB_COLOR);
    pen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol(airspaceData->getNDB(i)->getSymbol());

        currentSymbol->setPen(pen);
        currentScene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        ATCBeaconNDB *currentNDB = airspaceData->getNDB(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentNDB->getName());
        currentNDB->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentNDB->getScenePosition()->x();
        double positionY = currentNDB->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                             positionY + settings->NDB_LABEL_DY / currentScale);

        currentScene->addItem(currentLabel);
        currentNDB->hide();
    }
}

void ATCSituationalDisplay::calculateSTARs()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of STAR symbols + rotation
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSTARSymbol(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSTARSymbol(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSTARSymbol(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSTARSymbol(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getSTARSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display STAR symbol lines on scene
    QPen pen(settings->STAR_COLOR);
    pen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSTARSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getSTARSymbol(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSIDs()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of SID symbols + rotation
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSIDSymbol(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSIDSymbol(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSIDSymbol(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSIDSymbol(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getSIDSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display SID symbol lines on scene
    QPen pen(settings->SID_COLOR);
    pen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSIDSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getSIDSymbol(i)->hide();
    }
}

void ATCSituationalDisplay::calculateAirwayLow()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of Low Airways + rotation
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getAirwayLow(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display Low Airway lines on scene
    QPen pen(settings->AIRWAY_LOW_COLOR);
    pen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayLow(i)->getLine(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getAirwayLow(i)->hide();
    }
}

void ATCSituationalDisplay::calculateAirwayHigh()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of High Airways + rotation
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getAirwayHigh(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display High Airway lines on scene
    QPen pen(settings->AIRWAY_HIGH_COLOR);
    pen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayHigh(i)->getLine(j);

            currentSymbol->setPen(pen);
            currentScene->addItem(currentSymbol);
        }

        airspaceData->getAirwayHigh(i)->hide();
    }
}

void ATCSituationalDisplay::deleteRuler()
{
    delete ruler;
    ruler = nullptr;
}

void ATCSituationalDisplay::createFlightTag(ATCFlight *flight)
{
    ATCFlightTag *tag = new ATCFlightTag();
    flight->setFlightTag(tag);

    double longitude = ATCMath::rad2deg(flight->getState().x);
    double latitude = ATCMath::rad2deg(flight->getState().y);

    createTagType(flight);
    ATCTagDiamond *diamond = createDiamond(tag, longitude, latitude);
    QGraphicsLineItem *leader = createLeader(tag, longitude, latitude, flight->getState().hdg, flight->getState().v);
    QGraphicsLineItem *connector = createConnector(tag);
    QGraphicsSimpleTextItem *text = createTagText(tag);
    ATCTagRect *tagBox = createTagBox(tag);

    diamond->setConnector(connector);
    tagBox->setConnector(connector);
    tagBox->setText(text);
    text->setParentItem(tagBox);

    createEtiquettes(flight);

    assignTagPosition(tag);

    currentScene->addItem(diamond);
    currentScene->addItem(leader);
    currentScene->addItem(connector);
    currentScene->addItem(tagBox);

    visibleTags.append(tag);
}

void ATCSituationalDisplay::createTagType(ATCFlight *flight)
{
    QString speedRes = flight->getTargetSpeed();
    QString headingRes = QString::number(flight->getHdgRestriction());

    if((speedRes.isEmpty() || (speedRes == "---")) && (headingRes.isEmpty() || flight->getNavMode() == ATC::Nav))
    {
        flight->getFlightTag()->setTagType(ATC::Short);
    }
    else
    {
        flight->getFlightTag()->setTagType(ATC::Full);
    }
}

ATCTagDiamond* ATCSituationalDisplay::createDiamond(ATCFlightTag *tag, double lon, double lat)
{
    double x = mercatorProjectionLon(lon);
    double y = mercatorProjectionLat(lat);

    double xRot = rotateX(x, y, ATCConst::AVG_DECLINATION);
    double yRot = rotateY(x, y, ATCConst::AVG_DECLINATION);

    x = translateToLocalX(xRot);
    y = translateToLocalY(yRot);

    double width = settings->TAG_DIAMOND_WIDTH / currentScale;

    QPen pen(Qt::green);
    QBrush brush(Qt::green);

    ATCTagDiamond *diamond = new ATCTagDiamond(x - width/2, y - width/2, width, width, settings, &currentScale);
    diamond->setPen(pen);
    diamond->setBrush(brush);

    tag->setDiamond(diamond);
    tag->setDiamondPosition(QPointF(x, y));

    return diamond;
}

QGraphicsLineItem *ATCSituationalDisplay::createLeader(ATCFlightTag *tag, double lon, double lat, double trueHdg, double tasMPS)
{
    double length;
    if(settings->TAG_LEADER_UNIT == ATC::LeaderNM)
    {
        length = ATCMath::nm2m(settings->TAG_LEADER_LENGTH);
    }
    else
    {
        length = tasMPS * settings->TAG_LEADER_LENGTH * 60;
    }

    double leaderStartLon = lon;
    double leaderStartLat = lat;
    double leaderEndLon = lon + ATCMath::rad2deg(length / (ATCConst::WGS84_A * qCos(ATCMath::deg2rad(lat))));
    double leaderEndLat = lat;

    leaderStartLon = mercatorProjectionLon(leaderStartLon);
    leaderStartLat = mercatorProjectionLat(leaderStartLat);
    leaderEndLon = mercatorProjectionLon(leaderEndLon);
    leaderEndLat = mercatorProjectionLat(leaderEndLat);

    double delta = qFabs(leaderEndLon - leaderStartLon);

    leaderEndLon = leaderStartLon + delta * qSin(trueHdg);
    leaderEndLat = leaderStartLat + delta * qCos(trueHdg);

    double xStart = rotateX(leaderStartLon, leaderStartLat, ATCConst::AVG_DECLINATION);
    double yStart = rotateY(leaderStartLon, leaderStartLat, ATCConst::AVG_DECLINATION);
    double xEnd = rotateX(leaderEndLon, leaderEndLat, ATCConst::AVG_DECLINATION);
    double yEnd = rotateY(leaderEndLon, leaderEndLat, ATCConst::AVG_DECLINATION);

    xStart = translateToLocalX(xStart);
    yStart = translateToLocalY(yStart);
    xEnd = translateToLocalX(xEnd);
    yEnd = translateToLocalY(yEnd);

    QPen pen(Qt::green);
    pen.setWidthF(settings->TAG_LEADER_WIDTH / currentScale);

    QGraphicsLineItem *leader = new QGraphicsLineItem(xStart, yStart, xEnd, yEnd);
    leader->setPen(pen);

    tag->setLeader(leader);
    tag->setLeaderEndPosition(QPointF(xEnd, yEnd));

    return leader;
}

QGraphicsLineItem *ATCSituationalDisplay::createConnector(ATCFlightTag *tag)
{
    double x = tag->getDiamondPosition().x();
    double y = tag->getDiamondPosition().y();

    double xTagBox = x + settings->TAG_BOX_DX / currentScale;
    double yTagBox = y + settings->TAG_BOX_DY / currentScale;

    QPen pen(Qt::green);
    pen.setWidthF(settings->TAG_CONNECTOR_WIDTH / currentScale);

    QGraphicsLineItem *connector = new QGraphicsLineItem(x, y, xTagBox, yTagBox);
    connector->setPen(pen);

    tag->setConnector(connector);

    return connector;
}

ATCTagRect *ATCSituationalDisplay::createTagBox(ATCFlightTag *tag)
{
    double width = settings->TAG_BOX_WIDTH / currentScale;;
    double height;

    if(tag->getTagType() == ATC::Short)
    {
        height = settings->TAG_BOX_HEIGHT / currentScale;
    }
    else
    {
        height = settings->TAG_BOX_HEIGHT_FULL / currentScale;
    }

    double dx = settings->TAG_BOX_DX / currentScale;
    double dy = settings->TAG_BOX_DY / currentScale;

    double x = tag->getDiamondPosition().x();
    double y = tag->getDiamondPosition().y();

    QBrush brush(settings->TAG_BOX_COLOR);

    ATCTagRect *tagBox = new ATCTagRect(x + dx - width/2, y + dy - height, width, height, settings, &currentScale, &tag->getTagType());
    tagBox->setBrush(brush);
    tagBox->setOpacity(0.01);
    tagBox->setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren);

    tag->setTagBox(tagBox);

    return tagBox;
}

QGraphicsSimpleTextItem *ATCSituationalDisplay::createTagText(ATCFlightTag *tag)
{
    QString string = "";

    QFont textFont("Consolas");
    textFont.setPointSizeF(settings->TAG_LABEL_HEIGHT / currentScale);

    double xPos = tag->getDiamondPosition().x() + settings->TAG_BOX_DX/currentScale - settings->TAG_BOX_WIDTH/2/currentScale + settings->TAG_LABEL_MARGINS/currentScale;
    double yPos;

    if(tag->getTagType() == ATC::Short)
    {
        yPos = tag->getDiamondPosition().y() + settings->TAG_BOX_DY/currentScale - settings->TAG_BOX_HEIGHT/currentScale + settings->TAG_LABEL_MARGINS/currentScale;
    }
    else
    {
        yPos = tag->getDiamondPosition().y() + settings->TAG_BOX_DY/currentScale - settings->TAG_BOX_HEIGHT_FULL/currentScale + settings->TAG_LABEL_MARGINS/currentScale;
    }

    QBrush brush(settings->TAG_LABEL_COLOR);

    QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(string);
    text->setFont(textFont);
    text->setBrush(brush);
    text->setPos(xPos, yPos);
    text->setOpacity(1);

    tag->setText(text);

    return text;
}

void ATCSituationalDisplay::createEtiquettes(ATCFlight *flight)
{
    QString shortEtiquette = "               \n"
                             "               ";

    QString longEtiquette =  "               \n"
                             "               \n"
                             "               ";

    QString callsign = flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber();
    callsign = callsign.left(9);

    QString groundSpd = QString::number(qRound(ATCMath::mps2kt(flight->getState().v)));

    for(int i = 0; i < callsign.size(); i++)
    {
        shortEtiquette[i] = callsign.at(i);
        longEtiquette[i] = callsign.at(i);
    }

    for(int i = 0; i < groundSpd.size(); i++)
    {
        shortEtiquette[i + 10] = groundSpd.at(i);
        longEtiquette[i + 10] = groundSpd.at(i);
    }

    QString altitude = QString::number(ATCMath::m2ft(flight->getState().h) / 100);

    if(altitude.size() == 2)
    {
        altitude = "0" + altitude;
    }
    else if(altitude.size() == 1)
    {
        altitude = "00" + altitude;
    }

    QString targetAltitude = flight->getTargetAltitude().right(3);
    QString nextFix = flight->getNextFix();

    //CLIMB ARROW ASSIGNMENT HERE

    for(int i = 0; i < altitude.size(); i++)
    {
        shortEtiquette[i + 16] = altitude.at(i);
        longEtiquette[i + 16] = altitude.at(i);

        shortEtiquette[i + 20] = targetAltitude.at(i);
        longEtiquette[i + 20] = targetAltitude.at(i);
    }

    if(flight->getNavMode() == ATC::Nav)
    {
        for(int i = 0; i < nextFix.size(); i++)
        {
            shortEtiquette[i + 24] = nextFix.at(i);
            longEtiquette[i + 24] = nextFix.at(i);
        }
    }

    QString type = flight->getFlightPlan()->getType()->getAcType().ICAOcode;

    ATC::WakeCategory w = flight->getFlightPlan()->getType()->getAcType().wake;
    QString wake;

    if(w == ATC::L)
    {
        wake = "L";
    }
    else if(w == ATC::M)
    {
        wake = "M";
    }
    else if(w == ATC::H)
    {
        wake = "H";
    }
    else if(w == ATC::J)
    {
        wake = "J";
    }

    type = type + wake;

    QString headingRes;
    QString speedRes;

    if(flight->getNavMode() == ATC::Hdg)
    {
        if(!QString::number(flight->getHdgRestriction()).isEmpty())
        {
            int hdg = flight->getHdgRestriction();

            if(hdg < 10)
            {
                headingRes = "H00" + QString::number(flight->getHdgRestriction());
            }
            else if(hdg < 100)
            {
                headingRes = "H0" + QString::number(flight->getHdgRestriction());
            }
            else
            {
                headingRes = "H" + QString::number(flight->getHdgRestriction());
            }
        }
        else
        {
            headingRes = "H---";
        }
    }
    else
    {
        headingRes = "H---";
    }

    if(!flight->getTargetSpeed().isEmpty())
    {
        if(flight->getTargetSpeed().at(1) != '.')
        {
            speedRes = "S" + flight->getTargetSpeed();
        }
        else if(flight->getTargetSpeed().at(0) == '0')
        {
            speedRes = "M." + flight->getTargetSpeed().right(2);
        }
        else
        {
            speedRes = flight->getTargetSpeed();
        }
    }
    else
    {
        speedRes = "S---";
    }

    for(int i = 0; i < type.size(); i++)
    {
        longEtiquette[i + 32] = type.at(i);
    }

    for(int i = 0; i < headingRes.size(); i++)
    {
        longEtiquette[i + 38] = headingRes.at(i);
    }

    for(int i = 0; i < speedRes.size(); i++)
    {
        longEtiquette[i + 43] = speedRes.at(i);
    }

    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

    if(flight->getFlightTag()->getTagType() == ATC::Short)
    {
        flight->getFlightTag()->getText()->setText(shortEtiquette);
    }
    else
    {
        flight->getFlightTag()->getText()->setText(longEtiquette);
    }
}

void ATCSituationalDisplay::updateEtiquettesQuick(ATCFlight *flight)
{
    QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
    QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();
    State state = flight->getState();

    QString groundSpd = QString::number(qRound(ATCMath::mps2kt(state.v)));

    for(int i = 0; i < groundSpd.size(); i++)
    {
        shortEtiquette[i + 10] = groundSpd.at(i);
        longEtiquette[i + 10] = groundSpd.at(i);
    }

    QString altitude = QString::number(qRound(ATCMath::m2ft(state.h) / 100));

    if(altitude.size() == 2)
    {
        altitude = "0" + altitude;
    }
    else if(altitude.size() == 1)
    {
        altitude = "00" + altitude;
    }

    QString nextFix = flight->getNextFix();

    if(state.cm == BADA::Level)
    {
        shortEtiquette[19] = ' ';
        longEtiquette[19] = ' ';
    }
    else if(state.cm == BADA::Descend)
    {
        shortEtiquette[19] = QChar(0x2193);
        longEtiquette[19] = QChar(0x2193);
    }
    else
    {
        shortEtiquette[19] = QChar(0x2191);
        longEtiquette[19] = QChar(0x2191);
    }

    for(int i = 0; i < altitude.size(); i++)
    {
        shortEtiquette[i + 16] = altitude.at(i);
        longEtiquette[i + 16] = altitude.at(i);
    }

    QString targetAltitude = flight->getTargetAltitude().right(3);
    if(!flight->isGlidePath())
    {
        for(int i = 0; i < 3; i++)
        {
            shortEtiquette[i + 20] = targetAltitude.at(i);
            longEtiquette[i + 20] = targetAltitude.at(i);
        }
    }
    else
    {
        shortEtiquette[20] = ' ';
        longEtiquette[20] = ' ';

        shortEtiquette[21] = QChar(0x22A5);
        longEtiquette[21] = QChar(0x22A5);

        shortEtiquette[22] = ' ';
        longEtiquette[22] = ' ';
    }

    if(flight->getNavMode() == ATC::Nav)
    {
        if(flight->isDCT())
        {
            for(int i = 0; i < nextFix.size(); i++)
            {
                shortEtiquette[i + 24] = nextFix.at(i);
                longEtiquette[i + 24] = nextFix.at(i);
            }

            for(int i = 0; i < 3; i++)
            {
                longEtiquette[i + 39] = '-';
            }
        }
        else
        {
            for(int i = 0; i < 5; i++)
            {
                shortEtiquette[i + 24] = ' ';
                longEtiquette[i + 24] = ' ';
            }

            for(int i = 0; i < 3; i++)
            {
                longEtiquette[i + 39] = '-';
            }
        }
    }

    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

    bool shortTag = false;
    bool longTag = false;
    if(flight->getNavMode() == ATC::Nav && (flight->getTargetSpeed().isEmpty() || flight->getTargetSpeed() == "---"))
    {
        shortTag = true;
    }
    else
    {
        longTag = true;
    }

    if((flight->getFlightTag()->getTagType() == ATC::Full) || flight->getFlightTag()->getTagBox()->isHovered())
    {
        if(!shortTag || flight->getFlightTag()->getTagBox()->isHovered())
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setShort();
            flight->getFlightTag()->getTagBox()->rectLong2Short();
            flight->getFlightTag()->setTagType(ATC::Short);
        }
    }
    else if(flight->getFlightTag()->getTagType() == ATC::Short)
    {
        if(!longTag)
        {
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
            flight->getFlightTag()->getTagBox()->rectShort2Long();
            flight->getFlightTag()->setTagType(ATC::Full);
        }
    }
}

void ATCSituationalDisplay::updateRoutePrediction(ATCFlight *flight)
{
    QGraphicsPathItem *pathItem = flight->getRoutePrediction()->getPolygon();
    QPainterPath path = pathItem->path();

    QPolygonF pathPoly = path.toSubpathPolygons().at(0);
    pathPoly.replace(0, flight->getFlightTag()->getDiamondPosition());

    QPainterPath newPath;
    newPath.addPolygon(pathPoly);
    pathItem->setPath(newPath);
}

void ATCSituationalDisplay::assignTagPosition(ATCFlightTag *tag)
{
    QGraphicsLineItem *connector = tag->getConnector();

    QPointF p1 = connector->line().p1();
    QPointF p2 = connector->line().p2();

    double dx = (p2.x() - p1.x()) * currentScale;
    double dy = (p2.y() - p1.y()) * currentScale;

    tag->setDX(dx);
    tag->setDY(dy);
}

void ATCSituationalDisplay::assignTagPositions()
{
    for(int i = 0; i < visibleTags.size(); i++)
    {
        assignTagPosition(visibleTags.at(i));
    }
}

void ATCSituationalDisplay::hideAll()
{
    for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
    {
        visibleSectorsARTCCLow.at(i)->hide();
    }

    for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
    {
        visibleSectorsARTCCHigh.at(i)->hide();
    }

    for(int i = 0; i < visibleSectorsARTCC.size(); i++)
    {
        visibleSectorsARTCC.at(i)->hide();
    }

    for(int i = 0; i < visibleCentrelines.size(); i++)
    {
        visibleCentrelines.at(i)->hide();
    }

    for(int i = 0; i < visibleFixes.size(); i++)
    {
        visibleFixes.at(i)->hide();
    }

    for(int i = 0; i < visibleAirports.size(); i++)
    {
        visibleAirports.at(i)->hide();
    }

    for(int i = 0; i < visibleVORs.size(); i++)
    {
        visibleVORs.at(i)->hide();
    }

    for(int i = 0; i < visibleNDBs.size(); i++)
    {
        visibleNDBs.at(i)->hide();
    }

    for(int i = 0; i < visibleSTARs.size(); i++)
    {
        visibleSTARs.at(i)->hide();
    }

    for(int i = 0; i < visibleSIDs.size(); i++)
    {
        visibleSIDs.at(i)->hide();
    }

    for(int i = 0; i < visibleLowAirways.size(); i++)
    {
        visibleLowAirways.at(i)->hide();
    }

    for(int i = 0; i < visibleHighAirways.size(); i++)
    {
        visibleHighAirways.at(i)->hide();
    }

    visibleSectorsARTCCLow.clear();
    visibleSectorsARTCCHigh.clear();
    visibleSectorsARTCC.clear();
    visibleCentrelines.clear();
    visibleFixes.clear();
    visibleAirports.clear();
    visibleVORs.clear();
    visibleNDBs.clear();
    visibleSTARs.clear();
    visibleSIDs.clear();
    visibleLowAirways.clear();
    visibleHighAirways.clear();
}

void ATCSituationalDisplay::loadDisplay(QString path)
{
    hideAll();
    interpretDisplayFile(path);

    settings->DISPLAY_ACTIVE_PATH = path;
}

void ATCSituationalDisplay::loadInitialDisplay(QString path)
{
    interpretDisplayFile(path);

    settings->DISPLAY_ACTIVE_PATH = path;
}

void ATCSituationalDisplay::interpretDisplayFile(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " << path;
        return;
    }

    bool flagINFO = false;
    bool flagARTCCLow = false;
    bool flagARTCCHigh = false;
    bool flagARTCC = false;
    bool flagVOR = false;
    bool flagNDB = false;
    bool flagFixes = false;
    bool flagAirport = false;
    bool flagRunway = false;
    bool flagSTAR = false;
    bool flagSID = false;
    bool flagLowAirway = false;
    bool flagHighAirway = false;

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString textLine = stream.readLine();
        textLine = textLine.trimmed();

        if(!textLine.isEmpty())
        {
            if(textLine.contains("[INFO]", Qt::CaseInsensitive))
            {
                flagINFO = true;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = true;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = true;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = true;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = true;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = true;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = true;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = true;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = true;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = true;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = true;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = true;
                flagHighAirway = false;
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = true;
            }
            else if(flagINFO)
            {
                QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

                if(stringList.at(0).trimmed() == "NAME")
                {
                    settings->DISPLAY_NAME = stringList.at(1).trimmed();
                }
                else if(stringList.at(0).trimmed() == "CENTRE")
                {
                    QStringList commaStringList = stringList.at(1).trimmed().split(",", QString::SkipEmptyParts);

                    double centreX = commaStringList.at(0).trimmed().toDouble();
                    double centreY = commaStringList.at(1).trimmed().toDouble();

                    centerOn(centreX, centreY);
                }
                else if(stringList.at(0).trimmed() == "SCALE")
                {
                    QStringList commaStringList = stringList.at(1).trimmed().split(",", QString::SkipEmptyParts);

                    double scaleX = commaStringList.at(0).trimmed().toDouble();
                    double scaleY = commaStringList.at(1).trimmed().toDouble();

                    QTransform currentTransformation(transform());

                    double m11 = scaleX;
                    double m12 = currentTransformation.m12();
                    double m13 = currentTransformation.m13();
                    double m21 = currentTransformation.m21();
                    double m22 = scaleY;
                    double m23 = currentTransformation.m23();
                    double m31 = currentTransformation.m31();
                    double m32 = currentTransformation.m32();
                    double m33 = currentTransformation.m33();

                    currentTransformation.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                    setTransform(currentTransformation);

                    currentScale = m11;

                    rescaleAll();
                }

            }
            else if(flagARTCCLow)
            {
                slotShowSectorARTCCLow(textLine);
            }
            else if(flagARTCCHigh)
            {
                slotShowSectorARTCCHigh(textLine);
            }
            else if(flagARTCC)
            {
                slotShowSectorARTCC(textLine);
            }
            else if(flagVOR)
            {
                slotShowVOR(textLine);
            }
            else if(flagNDB)
            {
                slotShowNDB(textLine);
            }
            else if(flagFixes)
            {
                slotShowFix(textLine);
            }
            else if(flagAirport)
            {
                slotShowAirport(textLine);
            }
            else if(flagRunway)
            {
                slotShowCentreline(textLine);
            }
            else if(flagSTAR)
            {
                slotShowSTAR(textLine);
            }
            else if(flagSID)
            {
                slotShowSID(textLine);
            }
            else if(flagLowAirway)
            {
                slotShowAirwayLow(textLine);
            }
            else if(flagHighAirway)
            {
                slotShowAirwayHigh(textLine);
            }
        }
    }

    file.close();
}

double ATCSituationalDisplay::mercatorProjectionLon(double longitudeDeg, double referenceLongitudeDeg, double scale)
{
    return scale * (longitudeDeg - referenceLongitudeDeg);
}

double ATCSituationalDisplay::mercatorProjectionLat(double latitudeDeg, double scale)
{
    return scale * qLn(qTan(ATCConst::PI / 4 + latitudeDeg * ATCConst::DEG_2_RAD / 2)
               * qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeDeg * ATCConst::DEG_2_RAD)) /
               (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeDeg * ATCConst::DEG_2_RAD)) ,
                      ATCConst::WGS84_FIRST_ECCENTRICITY / 2)) * ATCConst::RAD_2_DEG;
}

double ATCSituationalDisplay::inverseMercatorLon(double mercatorX, double referenceLongitude, double scale)
{
    return mercatorX / scale + referenceLongitude;
}

double ATCSituationalDisplay::inverseMercatorLat(double mercatorY, double error, double scale)
{
    double t = qExp(-1 * mercatorY * ATCConst::DEG_2_RAD / scale);
    double lat = ATCConst::PI / 2 - 2 * qAtan(t);
    double lat1;

    double change = error + 1;

    while(change > error)
    {
        lat1 = ATCConst::PI/2 - 2 * qAtan(t * qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(lat)) / (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(lat)), ATCConst::WGS84_FIRST_ECCENTRICITY / 2));
        change = qFabs(lat1 - lat) / lat;

        lat = lat1;
    }

    return lat * ATCConst::RAD_2_DEG;
}

double ATCSituationalDisplay::rotateX(double coordX, double coordY, double angleDeg)
{
    return coordX * qCos(angleDeg * ATCConst::DEG_2_RAD) - coordY * qSin(angleDeg * ATCConst::DEG_2_RAD);
}

double ATCSituationalDisplay::rotateY(double coordX, double coordY, double angleDeg)
{
    return coordX * qSin(angleDeg * ATCConst::DEG_2_RAD) + coordY * qCos(angleDeg * ATCConst::DEG_2_RAD);
}

double ATCSituationalDisplay::translateToLocalX(double coordX)
{
    return (coordX - sectorCentreX) * scaleFactor;
}

double ATCSituationalDisplay::translateToLocalY(double coordY)
{
    return (-1 * (coordY - sectorCentreY) * scaleFactor);
}

double ATCSituationalDisplay::translateFromLocalX(double localX)
{
    return localX / scaleFactor + sectorCentreX;
}

double ATCSituationalDisplay::translateFromLocalY(double localY)
{
    return -1 * localY / scaleFactor + sectorCentreY;
}

QPointF ATCSituationalDisplay::geo2local(double latRad, double lonRad, double angleDeg, double scale, double refLon)
{
    double xProjected = mercatorProjectionLon(ATCMath::rad2deg(lonRad), refLon, scale);
    double yProjected = mercatorProjectionLat(ATCMath::rad2deg(latRad), scale);

    double xRotated = rotateX(xProjected, yProjected, angleDeg);
    double yRotated = rotateY(xProjected, yProjected, angleDeg);

    double xLocal = translateToLocalX(xRotated);
    double yLocal = translateToLocalY(yRotated);

    return QPointF(xLocal, yLocal);
}

QPointF ATCSituationalDisplay::local2geo(double x, double y, double angleDeg, double scale, double refLon)
{
    double xRotated = translateFromLocalX(x);
    double yRotated = translateFromLocalY(y);

    double xProjected = rotateX(xRotated, yRotated, -1 * angleDeg);
    double yProjected = rotateY(xRotated, yRotated, -1 * angleDeg);

    double lon = inverseMercatorLon(xProjected, refLon, scale);
    double lat = inverseMercatorLat(yProjected, 1E-8);

    return QPointF(lon, lat);
}

QPointF ATCSituationalDisplay::rotatePoint(QPointF pt, double angle, ATC::AngularUnits units)
{
    if(units == ATC::Deg) angle = ATCMath::deg2rad(angle);

    double x = pt.x() * qCos(angle) - pt.y() * qSin(angle);
    double y = pt.x() * qSin(angle) + pt.y() * qCos(angle);

    return(QPointF(x, y));
}

void ATCSituationalDisplay::calculateSectorParameters()
{
    double sectorXmin = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).x1;
    double sectorXmax = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).x1;
    double sectorYmin = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).y1;
    double sectorYmax = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).y1;

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    sectorCentreX = (sectorXmin + sectorXmax) / 2;
    sectorCentreY = (sectorYmin + sectorYmax) / 2;

    double spanX = sectorXmax - sectorXmin;
    double spanY = sectorYmax - sectorYmin;

    double spanXperPixel = spanX / ATCConst::SCENE_WIDTH;
    double spanYperPixel = spanY / ATCConst::SCENE_HEIGHT;

    if(spanYperPixel >= spanXperPixel)
    {
        scaleFactor = ATCConst::SCENE_HEIGHT / ATCConst::SECTOR_SHRINK_FACTOR / spanY;
    }
    else
    {
        scaleFactor = ATCConst::SCENE_WIDTH / ATCConst::SECTOR_SHRINK_FACTOR / spanX;
    }
}

void ATCSituationalDisplay::connectSlots()
{
    connect(settings, SIGNAL(signalColorARTCCLow(QColor)), this, SLOT(slotSetColorSectorARTCCLow(QColor)));
    connect(settings, SIGNAL(signalColorARTCCHigh(QColor)), this, SLOT(slotSetColorSectorARTCCHigh(QColor)));
    connect(settings, SIGNAL(signalColorARTCC(QColor)), this, SLOT(slotSetColorSectorARTCC(QColor)));
    connect(settings, SIGNAL(signalColorVOR(QColor)), this, SLOT(slotSetColorVOR(QColor)));
    connect(settings, SIGNAL(signalColorNDB(QColor)), this, SLOT(slotSetColorNDB(QColor)));
    connect(settings, SIGNAL(signalColorFix(QColor)), this, SLOT(slotSetColorFix(QColor)));
    connect(settings, SIGNAL(signalColorAirport(QColor)), this, SLOT(slotSetColorAirport(QColor)));
    connect(settings, SIGNAL(signalColorCentreline(QColor)), this, SLOT(slotSetColorCentreline(QColor)));
    connect(settings, SIGNAL(signalColorSTAR(QColor)), this, SLOT(slotSetColorSTAR(QColor)));
    connect(settings, SIGNAL(signalColorSID(QColor)), this, SLOT(slotSetColorSID(QColor)));
    connect(settings, SIGNAL(signalColorAirwayLow(QColor)), this, SLOT(slotSetColorAirwayLow(QColor)));
    connect(settings, SIGNAL(signalColorAirwayHigh(QColor)), this, SLOT(slotSetColorAirwayHigh(QColor)));
}

void ATCSituationalDisplay::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        assignTagPositions();

        QPoint numDegrees = event->angleDelta();
        qreal increment = (numDegrees.y() / 120) * scaleResolution;

        qreal newScale = baseScale + increment;
        currentScale = currentScale * newScale;

        scale(newScale, newScale);

        rescaleSectorsARTCCLow();
        rescaleSectorsARTCCHigh();
        rescaleSectorsARTCC();
        rescaleFixes();
        rescaleFixLabels();
        rescaleAirports();
        rescaleAirportLabels();
        rescaleExtendedCentrelines();
        rescaleVORs();
        rescaleVORLabels();
        rescaleNDBs();
        rescaleNDBLabels();
        rescaleSTARs();
        rescaleSIDs();
        rescaleAirwaysLow();
        rescaleAirwaysHigh();

        rescaleTags();
        rescaleRoutes();

        if(ruler != nullptr) deleteRuler();
    }

    event->accept();
}

void ATCSituationalDisplay::mousePressEvent(QMouseEvent *event)
{
    if((ruler == nullptr) && (event->button() == Qt::RightButton) && !keyPressedCTRL)
    {
        ruler = new ATCRuler(mapToScene(event->pos()), settings, currentScene, &currentScale);
        mousePressedRMB = true;
    }

    if(flagGetLocation)
    {
        emit signalShowFlightCreator();
        viewport()->setCursor(Qt::CrossCursor);

        //Map from radar screen to lat/lon
        QPointF point = mapToScene(event->pos());
        QPointF geo = local2geo(point.x(), point.y(), ATCConst::AVG_DECLINATION);

        emit signalDisplayClicked(geo.x(), geo.y());

        //Delete temp route prediction
        currentScene->removeItem(tempPrediction->getPolygon());
        for(int i = 0; i < tempPrediction->getLabels().size(); i++)
        {
            currentScene->removeItem(tempPrediction->getLabels().at(i));
        }

        visibleRoutes.remove(visibleRoutes.size() - 1);
        delete tempPrediction;
        tempPrediction = nullptr;

        //Change flag
        flagGetLocation = false;
    }

    if(dialogAltitudeCloseOnClick)
    {
        dialogAltitudeCloseOnClick = false;

        if(dialogAltitude != nullptr)
        {
            dialogAltitude->close();
            slotDialogAltitudeClosed();
        }
    }

    if(dialogSpeedCloseOnClick)
    {
        dialogSpeedCloseOnClick = false;

        if(dialogSpeed != nullptr)
        {
            dialogSpeed->close();
            slotDialogSpeedClosed();
        }
    }

    if(dialogHeadingCloseOnClick)
    {
        dialogHeadingCloseOnClick = false;

        if(dialogHeading != nullptr)
        {
            dialogHeading->close();
            slotDialogHeadingClosed();
        }
    }

    if(dialogRouteCloseOnClick)
    {
        dialogRouteCloseOnClick = false;

        if(dialogRoute != nullptr)
        {
            dialogRoute->close();
            slotDialogRouteClosed();
        }
    }

    if(dialogHandoffCloseOnClick)
    {
        dialogHandoffCloseOnClick = false;

        if(dialogHandoff != nullptr)
        {
            dialogHandoff->close();
            slotDialogHandoffClosed();
        }
    }

    QGraphicsView::mousePressEvent(event);
    event->accept();
}

void ATCSituationalDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if((ruler != nullptr) && mousePressedRMB) ruler->moveEnd(mapToScene(event->pos()));

    QGraphicsView::mouseMoveEvent(event);
    event->accept();
}

void ATCSituationalDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    if((ruler != nullptr) && (event->button() == Qt::RightButton)) deleteRuler();

    mousePressedRMB = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void ATCSituationalDisplay::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        keyPressedCTRL = true;
    }

    event->accept();
}

void ATCSituationalDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::NoDrag);
        viewport()->setCursor(Qt::CrossCursor);
        keyPressedCTRL = false;
    }

    event->accept();
}

template<class T> void ATCSituationalDisplay::removeFromVisible(T *object, QVector<T*> &vector)
{
    for(int i = vector.size() - 1; i >= 0; i--)
    {
        if(object == vector.at(i))
        {
            vector.remove(i);
        }
    }
}
