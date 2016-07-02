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

ATCSituationalDisplay::ATCSituationalDisplay(QWidget *parent) : QGraphicsView(parent), airspaceData(new ATCAirspace)
{
    situationalDisplaySetup();
    loadData();

    rescaleScene();

    displaySectors();
    displayRunwayCentrelines();
    displayFixes();
    displayAirports();

}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    if(airspaceData != nullptr) delete airspaceData;
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

void ATCSituationalDisplay::situationalDisplaySetup()
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
//    setRenderHint(QPainter::Antialiasing);

    setSceneRect(-0.5 * ATCConst::SCENE_WIDTH, -0.5 * ATCConst::SCENE_HEIGHT, ATCConst::SCENE_WIDTH, ATCConst::SCENE_HEIGHT);

    viewport()->setCursor(Qt::CrossCursor);

    scene = new QGraphicsScene(this);
    setScene(scene);
}

void ATCSituationalDisplay::loadData()
{    
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

    QFile sctFile("E:/Qt/ATC_Console/ATC_Console/EPWW_175_20160428.sct");

    if(!sctFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error opening sct file...";
        return;
    }

    bool flagVOR = false;
    bool flagNDB = false;
    bool flagFixes = false;
    bool flagAirport = false;
    bool flagRunway = false;

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
            if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flagVOR = true;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;

                qDebug() << "VORs:";
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flagVOR = false;
                flagNDB = true;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;

                qDebug() << "NDBs:";
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flagVOR = false;
                flagNDB = false;
                flagFixes = true;
                flagAirport = false;
                flagRunway = false;

                qDebug() << "Fixes:";
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = true;
                flagRunway = false;

                qDebug() << "Airports:";
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = true;

                qDebug() << "Runways:";
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
        }
    }

    sctFile.close();
}

void ATCSituationalDisplay::rescaleScene()
{
    qreal scaleX = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);
    qreal scaleY = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);

    scale(scaleX, scaleY);
    currentScale = scaleX;
}

void ATCSituationalDisplay::rescaleSectors()
{
    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        QGraphicsPolygonItem *currentPolygonItem = airspaceData->getSector(i)->getPolygon();
        QPen currentPen(currentPolygonItem->pen());

        currentPen.setWidthF(ATCConst::SECTORLINE_WIDTH / currentScale);
        currentPolygonItem->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleFixes()
{
    qreal sideLength = ATCConst::FIX_SIDE_LENGTH / currentScale;

    QPen currentPen(airspaceData->getFix(0)->getSymbol()->pen());
    currentPen.setWidthF(ATCConst::FIX_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QGraphicsPolygonItem *currentPolygonItem = airspaceData->getFix(i)->getSymbol();
        QPointF *currentPosition = airspaceData->getFix(i)->getScenePosition();

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

void ATCSituationalDisplay::rescaleFixLabels()
{
    QFont textFont(airspaceData->getFix(0)->getLabel()->font());
    textFont.setPointSizeF(ATCConst::FIX_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        ATCNavFix *currentFix = airspaceData->getFix(i);
        QGraphicsSimpleTextItem *currentLabel = airspaceData->getFix(i)->getLabel();

        currentLabel->setFont(textFont);

        double positionX = currentFix->getScenePosition()->x();
        double positionY = currentFix->getScenePosition()->y();

        currentLabel->setPos(positionX + ATCConst::FIX_LABEL_DX / currentScale,
                             positionY + ATCConst::FIX_LABEL_DY / currentScale);
    }
}

void ATCSituationalDisplay::rescaleAirports()
{
    QPen currentPen(airspaceData->getAirport(0)->getSymbol()->pen());
    currentPen.setWidthF(ATCConst::AIRPORT_SYMBOL_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        QGraphicsEllipseItem *currentSymbol = airspaceData->getAirport(i)->getSymbol();
        QPointF *currentPosition = airspaceData->getAirport(i)->getScenePosition();

        currentSymbol->setRect(currentPosition->x() - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                               currentPosition->y() - ATCConst::AIRPORT_SYMBOL_DIA / 2 / currentScale,
                               ATCConst::AIRPORT_SYMBOL_DIA / currentScale,
                               ATCConst::AIRPORT_SYMBOL_DIA / currentScale);
        currentSymbol->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleAirportLabels()
{
    QFont textFont(airspaceData->getAirport(0)->getLabel()->font());
    textFont.setPointSizeF(ATCConst::AIRPORT_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        ATCAirport *currentAirport = airspaceData->getAirport(i);
        QGraphicsSimpleTextItem *currentLabel = airspaceData->getAirport(i)->getLabel();

        currentLabel->setFont(textFont);

        double positionX = currentAirport->getScenePosition()->x();
        double positionY = currentAirport->getScenePosition()->y();

        currentLabel->setPos(positionX + ATCConst::AIRPORT_LABEL_DX / currentScale,
                             positionY + ATCConst::AIRPORT_LABEL_DY / currentScale);
    }
}

void ATCSituationalDisplay::displaySectors()
{
    QVector<sector> tempSectors;

    projectSectors(tempSectors, airspaceData, ATCConst::AVG_DECLINATION);

    double mercatorXmin = tempSectors.at(0).coords.at(0).x;
    double mercatorXmax = tempSectors.at(0).coords.at(0).x;
    double mercatorYmin = tempSectors.at(0).coords.at(0).y;
    double mercatorYmax = tempSectors.at(0).coords.at(0).y;

    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            double currentX = tempSectors.at(i).coords.at(j).x;
            double currentY = tempSectors.at(i).coords.at(j).y;

            if(currentX < mercatorXmin)
                mercatorXmin = currentX;
            else if(currentX > mercatorXmax)
                mercatorXmax = currentX;

            if(currentY < mercatorYmin)
                mercatorYmin = currentY;
            else if(currentY > mercatorYmax)
                mercatorYmax = currentY;
        }
    }

    sectorCentreX = (mercatorXmin + mercatorXmax) / 2;
    sectorCentreY = (mercatorYmin + mercatorYmax) / 2;

    scaleFactor = calculateScaleFactor(mercatorXmin, mercatorXmax, mercatorYmin, mercatorYmax);

    calculateSectorPolygons(tempSectors, airspaceData, sectorCentreX, sectorCentreY, scaleFactor);

    displayOnScene(airspaceData);
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

        double xRotated = currentFix.x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - currentFix.y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double yRotated = currentFix.x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + currentFix.y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

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
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QGraphicsPolygonItem *currentPolygon(airspaceData->getFix(i)->getSymbol());

        QPen pen(Qt::green);
        pen.setWidthF(ATCConst::FIX_LINE_WIDTH / currentScale);

        currentPolygon->setPen(pen);
        scene->addItem(currentPolygon);
    }

//Calculate labels
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        ATCNavFix *currentFix = airspaceData->getFix(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentFix->getName());
        currentFix->setLabel(currentLabel);

        QBrush textBrush(Qt::white);
        currentLabel->setBrush(textBrush);

        QFont textFont("Arial");
        textFont.setPointSizeF(ATCConst::FIX_LABEL_HEIGHT / currentScale);
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
    double rotationDeg = 5;

//Mercator projection of airports + rotation
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        coord currentAirport;

        currentAirport.x = mercatorProjectionLon(airspaceData->getAirport(i)->longitude());
        currentAirport.y = mercatorProjectionLat(airspaceData->getAirport(i)->latitude());

        double xRotated = currentAirport.x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - currentAirport.y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double yRotated = currentAirport.x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + currentAirport.y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

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

//Display fix symbols on scene
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        QGraphicsEllipseItem *currentSymbol(airspaceData->getAirport(i)->getSymbol());

        QPen pen(Qt::yellow);
        pen.setWidthF(ATCConst::AIRPORT_SYMBOL_WIDTH / currentScale);

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);
    }

//Calculate labels
        for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
        {
            ATCAirport *currentAirport = airspaceData->getAirport(i);
            QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentAirport->getName());
            currentAirport->setLabel(currentLabel);

            QBrush textBrush(Qt::white);
            currentLabel->setBrush(textBrush);

            QFont textFont("Arial");
            textFont.setPointSizeF(ATCConst::AIRPORT_LABEL_HEIGHT / currentScale);
            currentLabel->setFont(textFont);

            double positionX = currentAirport->getScenePosition()->x();
            double positionY = currentAirport->getScenePosition()->y();

            currentLabel->setPos(positionX + ATCConst::AIRPORT_LABEL_DX / currentScale,
                                 positionY + ATCConst::AIRPORT_LABEL_DY / currentScale);

            scene->addItem(currentLabel);
        }
}

void ATCSituationalDisplay::displayRunwayCentrelines()
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

                unsigned int hdg1 = currentRunway->getMagneticHDG1();
                unsigned int hdg2 = currentRunway->getMagneticHDG2();

                coord rwy1;
                coord rwy2;

                rwy1.x = currentRunway->getStartPoint().longitude();
                rwy1.y = currentRunway->getStartPoint().latitude();
                rwy2.x = currentRunway->getEndPoit().longitude();
                rwy2.y = currentRunway->getEndPoit().latitude();

                coord centreEnd1;
                coord centreEnd2;

                centreEnd1.x = rwy1.x +
                        ATCConst::RWY_CENTRELINE_LENGTH / 60 * qSin((hdg1 - 180 + ATCConst::AVG_DECLINATION) * ATCConst::DEG_2_RAD);
                centreEnd1.y = rwy1.y +
                        ATCConst::RWY_CENTRELINE_LENGTH / 60 * qCos((hdg1 - 180 + ATCConst::AVG_DECLINATION) * ATCConst::DEG_2_RAD);
                centreEnd2.x = rwy2.x +
                        ATCConst::RWY_CENTRELINE_LENGTH / 60 * qSin((hdg2 - 180 + ATCConst::AVG_DECLINATION) * ATCConst::DEG_2_RAD);
                centreEnd2.y = rwy2.y +
                        ATCConst::RWY_CENTRELINE_LENGTH / 60 * qCos((hdg2 - 180 + ATCConst::AVG_DECLINATION) * ATCConst::DEG_2_RAD);

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

        double rwyCoords1xRot = rwyCoords1.at(i).x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - rwyCoords1.at(i).y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double rwyCoords1yRot = rwyCoords1.at(i).x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + rwyCoords1.at(i).y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

        double rwyCoords2xRot = rwyCoords2.at(i).x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - rwyCoords2.at(i).y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double rwyCoords2yRot = rwyCoords2.at(i).x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + rwyCoords2.at(i).y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

        double centrelineEnd1xRot = centrelineEnd1.at(i).x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - centrelineEnd1.at(i).y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double centrelineEnd1yRot = centrelineEnd1.at(i).x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + centrelineEnd1.at(i).y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

        double centrelineEnd2xRot = centrelineEnd2.at(i).x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - centrelineEnd2.at(i).y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
        double centrelineEnd2yRot = centrelineEnd2.at(i).x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + centrelineEnd2.at(i).y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

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

        QPen pen(Qt::red);
        pen.setWidthF(ATCConst::RWY_CENTRELINE_WIDTH);

        scene->addLine(rwyCoords1.at(i).x, rwyCoords1.at(i).y, centrelineEnd1.at(i).x, centrelineEnd1.at(i).y, pen);
        scene->addLine(rwyCoords2.at(i).x, rwyCoords2.at(i).y, centrelineEnd2.at(i).x, centrelineEnd2.at(i).y, pen);

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

void ATCSituationalDisplay::projectSectors(QVector<sector> &targetVector, ATCAirspace *airspace, double rotationDeg)
{
    for(int i = 0; i < airspace->getSectorVectorSize(); i++)
    {
        sector tempSector;

        for(int j = 0; j < airspace->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            ATCAirspaceFix* currentAirspaceFix = airspace->getSector(i)->getCoordinates(j);

            coord tempCoord;
            tempCoord.x = mercatorProjectionLon(currentAirspaceFix->longitude());
            tempCoord.y = mercatorProjectionLat(currentAirspaceFix->latitude());

            double xRotated = tempCoord.x * qCos(rotationDeg * ATCConst::DEG_2_RAD) - tempCoord.y * qSin(rotationDeg * ATCConst::DEG_2_RAD);
            double yRotated = tempCoord.x * qSin(rotationDeg * ATCConst::DEG_2_RAD) + tempCoord.y * qCos(rotationDeg * ATCConst::DEG_2_RAD);

            tempCoord.x = xRotated;
            tempCoord.y = yRotated;

            tempSector.coords.append(tempCoord);
        }

        targetVector.append(tempSector);
    }
}

double ATCSituationalDisplay::calculateScaleFactor(double mercatorXmin, double mercatorXmax, double mercatorYmin, double mercatorYmax)
{
    double spanX = mercatorXmax - mercatorXmin;
    double spanY = mercatorYmax - mercatorYmin;

    double spanXperPixel = spanX / ATCConst::SCENE_WIDTH;
    double spanYperPixel = spanY / ATCConst::SCENE_HEIGHT;

    double scaleFactor = 0;
    if(spanYperPixel >= spanXperPixel)
    {
        scaleFactor = ATCConst::SCENE_HEIGHT / ATCConst::SECTOR_SHRINK_FACTOR / spanY;
    }
    else
    {
        scaleFactor = ATCConst::SCENE_WIDTH / ATCConst::SECTOR_SHRINK_FACTOR / spanX;
    }

    return scaleFactor;
}

void ATCSituationalDisplay::calculateSectorPolygons(QVector<sector> &sectorVector, ATCAirspace *airspace, double centreX, double centreY, double scaleFactor)
{
    for(int i = 0; i < airspace->getSectorVectorSize(); i++)
    {
        QVector<QPointF> polygonVertex(airspace->getSector(i)->getCoordinatesVectorSize());

        for(int j = 0; j < airspace->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            qreal sceneCoordX = static_cast<qreal>((sectorVector.at(i).coords.at(j).x - centreX) * scaleFactor);
            qreal sceneCoordY = static_cast<qreal>(-1 * (sectorVector.at(i).coords.at(j).y - centreY) * scaleFactor);

            QPointF vertex(sceneCoordX, sceneCoordY);
            polygonVertex[j] = vertex;
        }

        polygonVertex.append(polygonVertex[0]);

        QPolygonF sectorPolygon(polygonVertex);
        airspace->getSector(i)->setPolygon(new QGraphicsPolygonItem(sectorPolygon));
    }
}

void ATCSituationalDisplay::displayOnScene(ATCAirspace *airspace)
{
    for(int i = 0; i < airspace->getSectorVectorSize(); i++)
    {
            QGraphicsPolygonItem *currentPolygon(airspace->getSector(i)->getPolygon());

            QPen pen(Qt::gray);
            pen.setWidthF(ATCConst::SECTORLINE_WIDTH / currentScale);

            currentPolygon->setPen(pen);
            scene->addItem(currentPolygon);
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
        rescaleSectors();
        rescaleFixes();
        rescaleFixLabels();
        rescaleAirports();
        rescaleAirportLabels();
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
