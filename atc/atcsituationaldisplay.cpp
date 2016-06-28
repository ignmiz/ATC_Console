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
    displayFixes();
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    delete airspaceData;
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
    setRenderHint(QPainter::Antialiasing);

//    setSceneRect(-50000, -50000, 100000, 100000);
    setSceneRect(-0.5 * ATCConst::SCENE_WIDTH, -0.5 * ATCConst::SCENE_HEIGHT, ATCConst::SCENE_WIDTH, ATCConst::SCENE_HEIGHT);

    viewport()->setCursor(Qt::CrossCursor);

    scene = new QGraphicsScene(this);
    setScene(scene);

    QPen penLine(Qt::white);
    penLine.setWidth(20);

    lineH = scene->addLine(-25, 0, 25, 0, penLine);
    lineV = scene->addLine(0, -25, 0, 25, penLine);

    scene->addRect(sceneRect(), penLine);
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
                QString sectorName = stringList[1];

                airspaceData->appendSector(new ATCAirspaceSector(sectorName));
                qDebug() << "Sector " + sectorName + " appended...";
            }
            else if(textLine.contains("COORD", Qt::CaseInsensitive))
            {
                QStringList stringList = textLine.split(":", QString::SkipEmptyParts);
                QString latitudeString = stringList[1];
                QString longitudeString = stringList[2].left(14);

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

                QString fixName = stringList[0];
                QString latitudeString = stringList[1];
                QString longitudeString = stringList[2].left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendFix(new ATCNavFix(fixName, latitudeDouble, longitudeDouble));

                qDebug() << fixName + " appended...";
            }
            else if(flagVOR)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString vorName = stringList[0];
                float frequency = stringList[1].toFloat();
                QString latitudeString = stringList[2];
                QString longitudeString = stringList[3].left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendVOR(new ATCBeaconVOR(vorName, frequency, latitudeDouble, longitudeDouble));

                qDebug() << vorName + " appended...";
            }
            else if(flagNDB)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString ndbName = stringList[0];
                float frequency = stringList[1].toFloat();
                QString latitudeString = stringList[2];
                QString longitudeString = stringList[3].left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendNDB(new ATCBeaconNDB(ndbName, frequency, latitudeDouble, longitudeDouble));

                qDebug() << ndbName + " appended...";
            }
            else if(flagAirport)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString airportName = stringList[0];
                QString latitudeString = stringList[2];
                QString longitudeString = stringList[3].left(14);

                double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
                double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

                airspaceData->appendAirport(new ATCAirport(airportName, latitudeDouble, longitudeDouble));

                qDebug() << airportName + " appended...";
            }
            else if(flagRunway)
            {
                QRegExp expression("(\\s|\\t)");
                QStringList stringList = textLine.split(expression, QString::SkipEmptyParts);

                QString rwyID1 = stringList[0];
                QString rwyID2 = stringList[1];
                unsigned int magneticHDG1 = stringList[2].toUInt();
                unsigned int magneticHDG2 = stringList[3].toUInt();
                double startLat = airspaceData->coordsStringToDouble(stringList[4]);
                double startLon = airspaceData->coordsStringToDouble(stringList[5]);
                double endLat = airspaceData->coordsStringToDouble(stringList[6]);
                double endLon = airspaceData->coordsStringToDouble(stringList[7].left(14));
                QString airportName = stringList[8].left(4);

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
        QPointF *currentPosition = airspaceData->getFix(i)->getScenePosiiton();

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

void ATCSituationalDisplay::displaySectors()
{
    QVector<sector> tempSectors;

    projectSectors(tempSectors, airspaceData);

    double mercatorXmin = tempSectors[0].coords[0].x;
    double mercatorXmax = tempSectors[0].coords[0].x;
    double mercatorYmin = tempSectors[0].coords[0].y;
    double mercatorYmax = tempSectors[0].coords[0].y;

    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            double currentX = tempSectors[i].coords[j].x;
            double currentY = tempSectors[i].coords[j].y;

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

//Mercator projection of fixes
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        coord currentFix;

        currentFix.x = mercatorProjectionLon(airspaceData->getFix(i)->longitude());
        currentFix.y = mercatorProjectionLat(airspaceData->getFix(i)->latitude());

        tempFixes.append(currentFix);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        tempFixes[i].x = (tempFixes[i].x - sectorCentreX) * scaleFactor;
        tempFixes[i].y = -1 * (tempFixes[i].y - sectorCentreY) * scaleFactor;
//        tempFixes[i].x = (tempFixes[i].x - sectorCentreX) * 1000;
//        tempFixes[i].y = -1 * (tempFixes[i].y - sectorCentreY) * 1000;

        airspaceData->getFix(i)->setScenePosition(new QPointF(tempFixes[i].x, tempFixes[i].y));
        qDebug() << "Fix: " << tempFixes[i].x << " : " << tempFixes[i].y;
    }

//Build and position symbol polygons
    int vertexNumber = 3;
    qreal sideLength = ATCConst::FIX_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QVector<QPointF> polygonVertex(vertexNumber + 1);

        QPointF upperVertex(tempFixes[i].x, tempFixes[i].y - sideLength * qSqrt(3) / 3);
        QPointF lowerLeftVertex(tempFixes[i].x - sideLength / 2, tempFixes[i].y + sideLength * qSqrt(3) / 6);
        QPointF lowerRightVertex(tempFixes[i].x + sideLength / 2, tempFixes[i].y + sideLength * qSqrt(3) / 6);

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

//Calculate labels - IN PROGRESS
    QGraphicsSimpleTextItem *simpleText = new QGraphicsSimpleTextItem("ABAKU");

    QBrush textBrush(Qt::red);
    simpleText->setBrush(textBrush);

    QFont textFont("Arial");
    textFont.setPointSizeF(3);
    simpleText->setFont(textFont);

    QRectF boundingRect(simpleText->boundingRect());

    scene->addItem(simpleText);
    simpleText->setPos(tempFixes[0].x, tempFixes[0].y - boundingRect.height() / 2);
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

void ATCSituationalDisplay::projectSectors(QVector<sector> &targetVector, ATCAirspace *airspace)
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
//        baseScale = ATCConst::DISPLAY_HEIGHT * scaleFactor / spanY;
    }
    else
    {
        scaleFactor = ATCConst::SCENE_WIDTH / ATCConst::SECTOR_SHRINK_FACTOR / spanX;
//        baseScale = ATCConst::DISPLAY_WIDTH * scaleFactor / spanX;
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
            qreal sceneCoordX = static_cast<qreal>((sectorVector[i].coords[j].x - centreX) * scaleFactor);
            qreal sceneCoordY = static_cast<qreal>(-1 * (sectorVector[i].coords[j].y - centreY) * scaleFactor);
//            qreal sceneCoordX = static_cast<qreal>((sectorVector[i].coords[j].x - centreX) * 1000);
//            qreal sceneCoordY = static_cast<qreal>(-1 * (sectorVector[i].coords[j].y - centreY) * 1000);

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
