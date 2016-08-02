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

ATCSituationalDisplay::ATCSituationalDisplay(QWidget *parent) : QGraphicsView(parent), airspaceData(new ATCAirspace), settings(new ATCSettings)
{
    situationalDisplaySetup();
    loadData();

    rescaleScene();

    projectSectorsARTCCLow();
    projectSectorsARTCCHigh();
    projectSectorsARTCC();

    calculateSectorParameters();

    displaySectorsARTCCLow();
    displaySectorsARTCCHigh();
    displaySectorsARTCC();
    displayAirwayLow();
    displayAirwayHigh();
    displaySTARs();
    displaySIDs();
    displayExtendedCentrelines();
    displayNDBs();
    displayVORs();
    displayFixes();
    displayAirports();
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    if(airspaceData != nullptr) delete airspaceData;
    if(settings != nullptr) delete settings;
    scene->clear();
}

qreal ATCSituationalDisplay::getBaseScale() const
{
    return baseScale;
}

void ATCSituationalDisplay::setBaseScale(qreal scale)
{
    baseScale = scale;
}

ATCSettings* ATCSituationalDisplay::getSettings()
{
    return settings;
}

void ATCSituationalDisplay::slotSetColorSectorARTCCLow(QColor color)
{
    for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
    {
        visibleSectorsARTCCLow.at(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCCHigh(QColor color)
{
    for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
    {
        visibleSectorsARTCCHigh.at(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCC(QColor color)
{
    for(int i = 0; i < visibleSectorsARTCC.size(); i++)
    {
        visibleSectorsARTCC.at(i)->setColor(color);
    }
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

    scene = new QGraphicsScene(this);
    setScene(scene);
}

void ATCSituationalDisplay::loadData()
{
//Load sectorfiles from ESE
    QFile eseFile("E:/Qt/ATC_Console/ATC_Console/EPWW_175_20160428.ese");

    if(!eseFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening ese file...";
        return;
    }

    QTextStream eseStream(&eseFile);
    while(!eseStream.atEnd())
    {
        QString textLine = eseStream.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("SECTORLINE", Qt::CaseInsensitive))
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);
                QString sectorName = stringList.at(1);

                airspaceData->appendSector(new ATCAirspaceSector(sectorName));
                qDebug() << "Sector " + sectorName + " appended...";
            }
            else if(textLine.contains("COORD", Qt::CaseInsensitive))
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);
                QString latitudeString = stringList.at(1);
                QString longitudeString = stringList.at(2).left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->getLastSector()->appendAirspaceFix(new ATCAirspaceFix(latitudeDouble, longitudeDouble));
            }
        }
    }

    eseFile.close();

//Load VORs, NDBs, fixes, airports and runways from SCT
    QFile sctFile("E:/Qt/ATC_Console/ATC_Console/EPWW_175_20160428.sct");

    if(!sctFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening sct file...";
        return;
    }

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

    QTextStream sctStream(&sctFile);
    while(!sctStream.atEnd())
    {
        QString textLine = sctStream.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
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

                qDebug() << "ARTCC LOW:";
            }
            if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
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

                qDebug() << "ARTCC HIGH:";
            }
            if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
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

                qDebug() << "ARTCC:";
            }
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
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

                qDebug() << "VORs:";
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
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

                qDebug() << "NDBs:";
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
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

                qDebug() << "Fixes:";
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
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

                qDebug() << "Airports:";
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
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

                qDebug() << "Runways:";
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
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

                qDebug() << "STAR Symbols:";
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
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

                qDebug() << "SID Symbols:";
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
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

                qDebug() << "Low Airways:";
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
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

                qDebug() << "High Airways:";
            }
            else if(flagARTCCLow)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCCLow *currentObject = new ATCSectorARTCCLow(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    airspaceData->appendSectorARTCCLow(currentObject);

                    qDebug() << "Sector ARTCC Low:" << currentObject->getName() << "appended...";
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    airspaceData->getLastSectorARTCCLow()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    airspaceData->getLastSectorARTCCLow()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagARTCCHigh)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCCHigh *currentObject = new ATCSectorARTCCHigh(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    airspaceData->appendSectorARTCCHigh(currentObject);

                    qDebug() << "Sector ARTCC High:" << currentObject->getName() << "appended...";
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    airspaceData->getLastSectorARTCCHigh()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    airspaceData->getLastSectorARTCCHigh()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagARTCC)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    ATCSectorARTCC *currentObject = new ATCSectorARTCC(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    airspaceData->appendSectorARTCC(currentObject);

                    qDebug() << "Sector ARTCC:" << currentObject->getName() << "appended...";
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    airspaceData->getLastSectorARTCC()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    airspaceData->getLastSectorARTCC()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagFixes)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString fixName = stringList.at(0);
                QString latitudeString = stringList.at(1);
                QString longitudeString = stringList.at(2).left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendFix(new ATCNavFix(fixName, latitudeDouble, longitudeDouble));

                qDebug() << fixName + " appended...";
            }
            else if(flagVOR)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString vorName = stringList.at(0);
                float frequency = stringList.at(1).toFloat();
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendVOR(new ATCBeaconVOR(vorName, frequency, latitudeDouble, longitudeDouble));

                qDebug() << vorName + " appended...";
            }
            else if(flagNDB)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString ndbName = stringList.at(0);
                float frequency = stringList.at(1).toFloat();
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendNDB(new ATCBeaconNDB(ndbName, frequency, latitudeDouble, longitudeDouble));

                qDebug() << ndbName + " appended...";
            }
            else if(flagAirport)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString airportName = stringList.at(0);
                QString latitudeString = stringList.at(2);
                QString longitudeString = stringList.at(3).left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendAirport(new ATCAirport(airportName, latitudeDouble, longitudeDouble));

                qDebug() << airportName + " appended...";
            }
            else if(flagRunway)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString rwyID1 = stringList.at(0);
                QString rwyID2 = stringList.at(1);
                unsigned int magneticHDG1 = stringList.at(2).toUInt();
                unsigned int magneticHDG2 = stringList.at(3).toUInt();
                double startLat = airspaceData->coordsStringToDouble(stringList.at(4));
                double startLon = airspaceData->coordsStringToDouble(stringList.at(5));
                double endLat = airspaceData->coordsStringToDouble(stringList.at(6));
                double endLon = airspaceData->coordsStringToDouble(stringList.at(7).left(14));
                QString airportName = stringList.at(8).left(4);

                ATCAirport *desiredAirport = airspaceData->findAirport(airportName);

                if(desiredAirport != nullptr)
                {
                    desiredAirport->appendRunway(new ATCRunway(rwyID1, rwyID2, magneticHDG1, magneticHDG2, startLat, startLon, endLat, endLon));
                    qDebug() << airportName << " rwy: " << rwyID1 << rwyID2 << " appended...";
                }
            }
            else if(flagSTAR)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    ATCProcedureSTARSymbol *currentObject = new ATCProcedureSTARSymbol(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    airspaceData->appendSTARSymbol(currentObject);

                    qDebug() << currentObject->getName();
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    airspaceData->getLastSTARSymbol()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    airspaceData->getLastSTARSymbol()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagSID)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    ATCProcedureSIDSymbol *currentObject = new ATCProcedureSIDSymbol(name);
                    currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                    airspaceData->appendSIDSymbol(currentObject);

                    qDebug() << currentObject->getName();
                }
                else if((iterator == 0) && coordsFound)
                {
                    QString lat1String = stringList.at(0);
                    QString lon1String = stringList.at(1);
                    QString lat2String = stringList.at(2);
                    QString lon2String = stringList.at(3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    airspaceData->getLastSIDSymbol()->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                    airspaceData->getLastSIDSymbol()->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                }
            }
            else if(flagLowAirway)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    if(airspaceData->isAirwayLow(name))
                    {
                        ATCAirwayLow *currentObject = airspaceData->findAirwayLow(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                    }
                    else
                    {
                        ATCAirwayLow *currentObject = new ATCAirwayLow(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        airspaceData->appendAirwayLow(currentObject);

                        qDebug() << "Low airway: " << name << " appended...";
                    }
                }
            }
            else if(flagHighAirway)
            {
                textLine = textLine.split(";", QString::SkipEmptyParts).at(0);
                textLine = textLine.trimmed();

                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);
                int size = stringList.size();

                int iterator = 0;
                bool coordsFound = false;

                for(int i = 0; i < size - 3; i++)
                {
                    if((airspaceData->isValidCoordsFormat(stringList.at(i)) || airspaceData->isValidNavaid(stringList.at(i))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 1)) || airspaceData->isValidNavaid(stringList.at(i + 1))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 2)) || airspaceData->isValidNavaid(stringList.at(i + 2))) &&
                       (airspaceData->isValidCoordsFormat(stringList.at(i + 3)) || airspaceData->isValidNavaid(stringList.at(i + 3))))
                    {
                        iterator = i;
                        coordsFound = true;
                    }
                }

                if(iterator != 0 && coordsFound)
                {
                    QString name;

                    for(int i = 0; i < iterator; i++)
                    {
                        name = name + " " + stringList.at(i);
                    }

                    QString lat1String = stringList.at(iterator);
                    QString lon1String = stringList.at(iterator + 1);
                    QString lat2String = stringList.at(iterator + 2);
                    QString lon2String = stringList.at(iterator + 3);

                    double lat1;
                    double lon1;
                    double lat2;
                    double lon2;

                    if(airspaceData->isValidCoordsFormat(lat1String))
                    {
                        lat1 = airspaceData->coordsStringToDouble(lat1String);
                    }
                    else
                    {
                        lat1 = airspaceData->getNavaidLatitude(lat1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon1String))
                    {
                        lon1 = airspaceData->coordsStringToDouble(lon1String);
                    }
                    else
                    {
                        lon1 = airspaceData->getNavaidLongitude(lon1String);
                    }

                    if(airspaceData->isValidCoordsFormat(lat2String))
                    {
                        lat2 = airspaceData->coordsStringToDouble(lat2String);
                    }
                    else
                    {
                        lat2 = airspaceData->getNavaidLatitude(lat2String);
                    }

                    if(airspaceData->isValidCoordsFormat(lon2String))
                    {
                        lon2 = airspaceData->coordsStringToDouble(lon2String);
                    }
                    else
                    {
                        lon2 = airspaceData->getNavaidLongitude(lon2String);
                    }

                    if(airspaceData->isAirwayHigh(name))
                    {
                        ATCAirwayHigh *currentObject = airspaceData->findAirwayHigh(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));
                    }
                    else
                    {
                        ATCAirwayHigh *currentObject = new ATCAirwayHigh(name);
                        currentObject->appendCoords1(new ATCAirspaceFix(lat1, lon1));
                        currentObject->appendCoords2(new ATCAirspaceFix(lat2, lon2));

                        airspaceData->appendAirwayHigh(currentObject);

                        qDebug() << "High airway: " << name << " appended...";
                    }
                }
            }
        }
    }

    sctFile.close();

//Load SID and STAR procedures from ESE
    if(!eseFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening ese file...";
        return;
    }

    bool flagSidStar = false;
    bool flagAirspace = false;

    QTextStream eseStream2(&eseFile);
    while(!eseStream2.atEnd())
    {
        QString textLine = eseStream2.readLine();
        textLine = textLine.trimmed();

        if(textLine.isEmpty() || (textLine.at(0) == ';'))
        {
        }
        else
        {
            if(textLine.contains("[SIDSSTARS]", Qt::CaseInsensitive))
            {
                flagSidStar = true;
                flagAirspace = false;

                qDebug() << "SIDs & STARs:";
            }
            else if(textLine.contains("[AIRSPACE]", Qt::CaseInsensitive))
            {
                flagSidStar = false;
                flagAirspace = true;
            }
            else if(flagSidStar)
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);

                QString procedureType = stringList.at(0);
                QString airportCode = stringList.at(1);
                QString runwayID = stringList.at(2);
                QString procedureName = stringList.at(3);
                QString fixListString = stringList.at(4);

                QRegExp expression("(\\s|\\t)");
                QStringList fixList = fixListString.split(expression, QString::SkipEmptyParts);

                if(procedureType == "SID")
                {
                    ATCProcedureSID *currentProcedure = new ATCProcedureSID(procedureName, airportCode, runwayID);
                    currentProcedure->setFixList(fixList);

                    airspaceData->appendSID(currentProcedure);

                    qDebug() << "SID: " << airportCode << ":" << runwayID << ":" << procedureName << " appended...";
                }
                else if(procedureType == "STAR")
                {
                    ATCProcedureSTAR *currentProcedure = new ATCProcedureSTAR(procedureName, airportCode, runwayID);
                    currentProcedure->setFixList(fixList);

                    airspaceData->appendSTAR(currentProcedure);

                    qDebug() << "STAR: " << airportCode << ":" << runwayID << ":" << procedureName << " appended...";
                }
            }
        }
    }

    eseFile.close();
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
        currentPen.setWidthF(ATCConst::ARTCC_LOW_LINE_WIDTH / currentScale);

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
        currentPen.setWidthF(ATCConst::ARTCC_HIGH_LINE_WIDTH / currentScale);

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
        currentPen.setWidthF(ATCConst::ARTCC_LINE_WIDTH / currentScale);

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
        qreal sideLength = ATCConst::FIX_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleFixes.at(0)->getSymbol()->pen());
        currentPen.setWidthF(ATCConst::FIX_LINE_WIDTH / currentScale);

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
        textFont.setPointSizeF(ATCConst::FIX_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleFixes.size(); i++)
        {
            ATCNavFix *currentFix = visibleFixes.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentFix->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentFix->getScenePosition()->x();
            double positionY = currentFix->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::FIX_LABEL_DX / currentScale,
                                 positionY + ATCConst::FIX_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleVORs()
{
    if(!visibleVORs.empty())
    {
        qreal sideLength = ATCConst::VOR_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleVORs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(ATCConst::VOR_LINE_WIDTH / currentScale);

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
        textFont.setPointSizeF(ATCConst::VOR_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleVORs.size(); i++)
        {
            ATCBeaconVOR *currentVOR = visibleVORs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentVOR->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentVOR->getScenePosition()->x();
            double positionY = currentVOR->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::VOR_LABEL_DX / currentScale,
                                 positionY + ATCConst::VOR_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBs()
{
    if(!visibleNDBs.empty())
    {
        QPen currentPen(visibleNDBs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(ATCConst::NDB_SYMBOL_WIDTH / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = dynamic_cast<QGraphicsEllipseItem*>(visibleNDBs.at(i)->getSymbol());
            QPointF *currentPosition = visibleNDBs.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - ATCConst::NDB_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - ATCConst::NDB_SYMBOL_DIA / 2 / currentScale,
                                   ATCConst::NDB_SYMBOL_DIA / currentScale,
                                   ATCConst::NDB_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBLabels()
{
    if(!visibleNDBs.empty())
    {
        QFont textFont(visibleNDBs.at(0)->getLabel()->font());
        textFont.setPointSizeF(ATCConst::NDB_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            ATCBeaconNDB *currentNDB = visibleNDBs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentNDB->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentNDB->getScenePosition()->x();
            double positionY = currentNDB->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::AIRPORT_LABEL_DX / currentScale,
                                 positionY + ATCConst::AIRPORT_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleAirports()
{
    if(!visibleAirports.empty())
    {
        QPen currentPen(visibleAirports.at(0)->getSymbol()->pen());
        currentPen.setWidthF(ATCConst::AIRPORT_SYMBOL_WIDTH / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = visibleAirports.at(i)->getSymbol();
            QPointF *currentPosition = visibleAirports.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   ATCConst::AIRPORT_SYMBOL_DIA / currentScale,
                                   ATCConst::AIRPORT_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleAirportLabels()
{
    if(!visibleAirports.empty())
    {
        QFont textFont(visibleAirports.at(0)->getLabel()->font());
        textFont.setPointSizeF(ATCConst::AIRPORT_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            ATCAirport *currentAirport = visibleAirports.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentAirport->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentAirport->getScenePosition()->x();
            double positionY = currentAirport->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::AIRPORT_LABEL_DX / currentScale,
                                 positionY + ATCConst::AIRPORT_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleExtendedCentrelines()
{
    if(!visibleCentrelines.empty())
    {
        QPen currentPen(visibleCentrelines.at(0)->getCentreline()->pen());
        currentPen.setWidthF(ATCConst::RWY_CENTRELINE_WIDTH / currentScale);

        for(int i = 0; i < visibleCentrelines.size(); i++)
        {
            visibleCentrelines.at(i)->getCentreline()->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleSTARs()
{
    if(!visibleSTARs.empty())
    {
        QPen currentPen(visibleSTARs.at(0)->getLine(0)->pen());
        currentPen.setWidthF(ATCConst::STAR_LINE_WIDTH / currentScale);

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
        currentPen.setWidthF(ATCConst::SID_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSIDs.size(); i++)
        {
            for(int j = 0; j < visibleSIDs.at(i)->getCoordsVectorSize(); j++)
            {
                visibleSIDs.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwayLow()
{
    if(!visibleLowAirways.empty())
    {
        QPen currentPen(visibleLowAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(ATCConst::AIRWAY_LOW_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleLowAirways.size(); i++)
        {
            for(int j = 0; j < visibleLowAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleLowAirways.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwayHigh()
{
    if(!visibleHighAirways.empty())
    {
        QPen currentPen(visibleHighAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(ATCConst::AIRWAY_HIGH_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleHighAirways.size(); i++)
        {
            for(int j = 0; j < visibleHighAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleHighAirways.at(i)->getLine(j)->setPen(currentPen);
            }
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

void ATCSituationalDisplay::displaySectorsARTCCLow()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

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
    pen.setWidthF(ATCConst::ARTCC_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCLow(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        visibleSectorsARTCCLow.append(airspaceData->getSectorARTCCLow(i));
    }
}

void ATCSituationalDisplay::displaySectorsARTCCHigh()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

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
    pen.setWidthF(ATCConst::ARTCC_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCHigh(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        visibleSectorsARTCCHigh.append(airspaceData->getSectorARTCCHigh(i));
    }
}

void ATCSituationalDisplay::displaySectorsARTCC()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

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
    pen.setWidthF(ATCConst::ARTCC_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCC(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        visibleSectorsARTCC.append(airspaceData->getSectorARTCC(i));
    }
}

void ATCSituationalDisplay::displayFixes()
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
        tempFixes[i].x = (tempFixes.at(i).x - sectorCentreX) * scaleFactor;
        tempFixes[i].y = -1 * (tempFixes.at(i).y - sectorCentreY) * scaleFactor;

        airspaceData->getFix(i)->setScenePosition(new QPointF(tempFixes.at(i).x, tempFixes.at(i).y));
    }

//Build and position symbol polygons
    int vertexNumber = 3;
    qreal sideLength = ATCConst::FIX_SIDE_LENGTH / currentScale;

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
    QPen pen(Qt::green);
    pen.setWidthF(ATCConst::FIX_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol = airspaceData->getFix(i)->getSymbol();

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);

        visibleFixes.append(airspaceData->getFix(i));
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(ATCConst::FIX_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        ATCNavFix *currentFix = airspaceData->getFix(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentFix->getName());
        currentFix->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentFix->getScenePosition()->x();
        double positionY = currentFix->getScenePosition()->y();

        currentLabel->setPos(positionX + ATCConst::FIX_LABEL_DX / currentScale,
                             positionY + ATCConst::FIX_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
    }
}

void ATCSituationalDisplay::displayAirports()
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
        tempAirports[i].x = (tempAirports.at(i).x - sectorCentreX) * scaleFactor;
        tempAirports[i].y = -1 * (tempAirports.at(i).y - sectorCentreY) * scaleFactor;

        airspaceData->getAirport(i)->setScenePosition(new QPointF(tempAirports.at(i).x, tempAirports.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        airspaceData->getAirport(i)->setSymbol(new QGraphicsEllipseItem(tempAirports.at(i).x - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        tempAirports.at(i).y - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        ATCConst::AIRPORT_SYMBOL_DIA / currentScale,
                                                                        ATCConst::AIRPORT_SYMBOL_DIA / currentScale));
    }

//Display airport symbols on scene
    QPen pen(Qt::yellow);
    pen.setWidthF(ATCConst::AIRPORT_SYMBOL_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        QGraphicsEllipseItem *currentSymbol(airspaceData->getAirport(i)->getSymbol());

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);

        visibleAirports.append(airspaceData->getAirport(i));
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(ATCConst::AIRPORT_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
        {
            ATCAirport *currentAirport = airspaceData->getAirport(i);
            QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentAirport->getName());
            currentAirport->setLabel(currentLabel);

            currentLabel->setBrush(textBrush);
            currentLabel->setFont(textFont);

            double positionX = currentAirport->getScenePosition()->x();
            double positionY = currentAirport->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::AIRPORT_LABEL_DX / currentScale,
                                 positionY + ATCConst::AIRPORT_LABEL_DY / currentScale);

            scene->addItem(currentLabel);
        }
}

void ATCSituationalDisplay::displayExtendedCentrelines()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    QVector<coord> rwyCoords1;
    QVector<coord> rwyCoords2;
    QVector<coord> centrelineEnd1;
    QVector<coord> centrelineEnd2;

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
                rwy2.x = currentRunway->getEndPoit().longitude();
                rwy2.y = currentRunway->getEndPoit().latitude();

                double angle = qAcos((rwy2.y - rwy1.y) / qSqrt(qPow(rwy2.x - rwy1.x, 2) + qPow(rwy2.y - rwy1.y, 2)));

                coord centreEnd1;
                coord centreEnd2;

                centreEnd1.x = rwy1.x + ATCConst::RWY_CENTRELINE_LENGTH / 60 * qSin(angle + ATCConst::PI);
                centreEnd1.y = rwy1.y + ATCConst::RWY_CENTRELINE_LENGTH / 60 * qCos(angle + ATCConst::PI);
                centreEnd2.x = rwy2.x + ATCConst::RWY_CENTRELINE_LENGTH / 60 * qSin(angle);
                centreEnd2.y = rwy2.y + ATCConst::RWY_CENTRELINE_LENGTH / 60 * qCos(angle);

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
        rwyCoords1[i].x = mercatorProjectionLon(rwyCoords1.at(i).x);
        rwyCoords1[i].y = mercatorProjectionLat(rwyCoords1.at(i).y);

        rwyCoords2[i].x = mercatorProjectionLon(rwyCoords2.at(i).x);
        rwyCoords2[i].y = mercatorProjectionLat(rwyCoords2.at(i).y);

        centrelineEnd1[i].x = mercatorProjectionLon(centrelineEnd1.at(i).x);
        centrelineEnd1[i].y = mercatorProjectionLat(centrelineEnd1.at(i).y);

        centrelineEnd2[i].x = mercatorProjectionLon(centrelineEnd2.at(i).x);
        centrelineEnd2[i].y = mercatorProjectionLat(centrelineEnd2.at(i).y);

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
        rwyCoords1[i].x = (rwyCoords1.at(i).x - sectorCentreX) * scaleFactor;
        rwyCoords1[i].y = -1 * (rwyCoords1.at(i).y - sectorCentreY) * scaleFactor;

        rwyCoords2[i].x = (rwyCoords2.at(i).x - sectorCentreX) * scaleFactor;
        rwyCoords2[i].y = -1 * (rwyCoords2.at(i).y - sectorCentreY) * scaleFactor;

        centrelineEnd1[i].x = (centrelineEnd1.at(i).x - sectorCentreX) * scaleFactor;
        centrelineEnd1[i].y = -1 * (centrelineEnd1.at(i).y - sectorCentreY) * scaleFactor;

        centrelineEnd2[i].x = (centrelineEnd2.at(i).x - sectorCentreX) * scaleFactor;
        centrelineEnd2[i].y = -1 * (centrelineEnd2.at(i).y - sectorCentreY) * scaleFactor;
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

                k++;
            }
        }
    }

//Display extended centrelines
    QPen pen(Qt::red);
    pen.setWidthF(ATCConst::RWY_CENTRELINE_WIDTH / currentScale);

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

                scene->addItem(currentCentreline1->getCentreline());
                scene->addItem(currentCentreline2->getCentreline());

                visibleCentrelines.append(currentCentreline1);
                visibleCentrelines.append(currentCentreline2);
            }
        }
    }
}

void ATCSituationalDisplay::displayVORs()
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
        tempVORs[i].x = (tempVORs.at(i).x - sectorCentreX) * scaleFactor;
        tempVORs[i].y = -1 * (tempVORs.at(i).y - sectorCentreY) * scaleFactor;

        airspaceData->getVOR(i)->setScenePosition(new QPointF(tempVORs.at(i).x, tempVORs.at(i).y));
    }

//Build and position symbol polygons
    qreal sideLength = ATCConst::VOR_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QPointF topLeft(tempVORs.at(i).x - sideLength / 2, tempVORs.at(i).y - sideLength / 2);
        QPointF bottomRight(tempVORs.at(i).x + sideLength / 2, tempVORs.at(i).y + sideLength / 2);

        QRectF rect(topLeft, bottomRight);
        airspaceData->getVOR(i)->setSymbol(new QGraphicsRectItem(rect));
    }

//Display VOR symbols on scene
    QPen pen(Qt::cyan);
    pen.setWidthF(ATCConst::VOR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentRect(airspaceData->getVOR(i)->getSymbol());

        currentRect->setPen(pen);
        scene->addItem(currentRect);

        visibleVORs.append(airspaceData->getVOR(i));
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(ATCConst::VOR_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        ATCBeaconVOR *currentVOR = airspaceData->getVOR(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentVOR->getName());
        currentVOR->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentVOR->getScenePosition()->x();
        double positionY = currentVOR->getScenePosition()->y();

        currentLabel->setPos(positionX + ATCConst::VOR_LABEL_DX / currentScale,
                             positionY + ATCConst::VOR_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
    }
}

void ATCSituationalDisplay::displayNDBs()
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
        tempNDBs[i].x = (tempNDBs.at(i).x - sectorCentreX) * scaleFactor;
        tempNDBs[i].y = -1 * (tempNDBs.at(i).y - sectorCentreY) * scaleFactor;

        airspaceData->getNDB(i)->setScenePosition(new QPointF(tempNDBs.at(i).x, tempNDBs.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        airspaceData->getNDB(i)->setSymbol(new QGraphicsEllipseItem(tempNDBs.at(i).x - ATCConst::NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    tempNDBs.at(i).y - ATCConst::NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    ATCConst::NDB_SYMBOL_DIA / currentScale,
                                                                    ATCConst::NDB_SYMBOL_DIA / currentScale));
    }

//Display NDB symbols on scene
    QPen pen(Qt::magenta);
    pen.setWidthF(ATCConst::NDB_SYMBOL_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol(airspaceData->getNDB(i)->getSymbol());

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);

        visibleNDBs.append(airspaceData->getNDB(i));
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(ATCConst::NDB_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        ATCBeaconNDB *currentNDB = airspaceData->getNDB(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentNDB->getName());
        currentNDB->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentNDB->getScenePosition()->x();
        double positionY = currentNDB->getScenePosition()->y();

        currentLabel->setPos(positionX + ATCConst::VOR_LABEL_DX / currentScale,
                             positionY + ATCConst::VOR_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
    }
}

void ATCSituationalDisplay::displaySTARs()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

            airspaceData->getSTARSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display STAR symbol lines on scene
    QPen pen(QColor(255, 165, 0));
    pen.setWidthF(ATCConst::STAR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSTARSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        visibleSTARs.append(airspaceData->getSTARSymbol(i));
    }
}

void ATCSituationalDisplay::displaySIDs()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

            airspaceData->getSIDSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display SID symbol lines on scene
    QPen pen(QColor(0, 255, 255));
    pen.setWidthF(ATCConst::SID_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSIDSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        visibleSIDs.append(airspaceData->getSIDSymbol(i));
    }
}

void ATCSituationalDisplay::displayAirwayLow()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

            airspaceData->getAirwayLow(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display Low Airway lines on scene
    QPen pen(QColor(255, 192, 203));
    pen.setWidthF(ATCConst::AIRWAY_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayLow(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        visibleLowAirways.append(airspaceData->getAirwayLow(i));
    }
}

void ATCSituationalDisplay::displayAirwayHigh()
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

            currentCoords1.x = (currentCoords1.x - sectorCentreX) * scaleFactor;
            currentCoords1.y = -1 * (currentCoords1.y - sectorCentreY) * scaleFactor;
            currentCoords2.x = (currentCoords2.x - sectorCentreX) * scaleFactor;
            currentCoords2.y = -1 * (currentCoords2.y - sectorCentreY) * scaleFactor;

            airspaceData->getAirwayHigh(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display High Airway lines on scene
    QPen pen(QColor(123, 104, 238));
    pen.setWidthF(ATCConst::AIRWAY_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayHigh(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        visibleHighAirways.append(airspaceData->getAirwayHigh(i));
    }
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

double ATCSituationalDisplay::rotateX(double coordX, double coordY, double angleDeg)
{
    return coordX * qCos(angleDeg * ATCConst::DEG_2_RAD) - coordY * qSin(angleDeg * ATCConst::DEG_2_RAD);
}

double ATCSituationalDisplay::rotateY(double coordX, double coordY, double angleDeg)
{
    return coordX * qSin(angleDeg * ATCConst::DEG_2_RAD) + coordY * qCos(angleDeg * ATCConst::DEG_2_RAD);
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

void ATCSituationalDisplay::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
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
        rescaleAirwayLow();
        rescaleAirwayHigh();
    }

    event->accept();
}

void ATCSituationalDisplay::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void ATCSituationalDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QGraphicsView::mouseMoveEvent(event);
    }
    event->accept();
}

void ATCSituationalDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void ATCSituationalDisplay::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    event->accept();
}

void ATCSituationalDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::NoDrag);
        viewport()->setCursor(Qt::CrossCursor);        
    }
    event->accept();
}
