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
    displayData();
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    scene->clear();
    delete airspaceData;
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
    setSceneRect(-50000, -50000, 100000, 100000);

    viewport()->setCursor(Qt::CrossCursor);

    scene = new QGraphicsScene(this);
    setScene(scene);

    QBrush brush(Qt::gray);

    QPen pen(Qt::green);
    pen.setWidth(3);

    QPen penLine(Qt::white);
    penLine.setWidth(5);

    rect1 = scene->addRect(-250, -250, 100, 100, pen, brush);
    rect2 = scene->addRect(-250, 150, 100, 100, pen, brush);
    rect3 = scene->addRect(150, 150, 100, 100, pen, brush);
    rect4 = scene->addRect(150, -250, 100, 100, pen, brush);

    lineH = scene->addLine(-25, 0, 25, 0, penLine);
    lineV = scene->addLine(0, -25, 0, 25, penLine);
}

void ATCSituationalDisplay::loadData()
{    
    QFile sectorFile("E:/Qt/ATC_Console/ATC_Console/EPWA_TMA.txt");

    if(!sectorFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening data file...";
        return;
    }

    QTextStream inStream(&sectorFile);
    while(!inStream.atEnd())
    {
        QString textLine = inStream.readLine();
        textLine = textLine.trimmed();

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
            QString longitudeString = stringList[2];

            double latitudeDouble = airspaceData->coordsStringToDouble(latitudeString);
            double longitudeDouble = airspaceData->coordsStringToDouble(longitudeString);

            airspaceData->getLastSector()->appendAirspaceFix(new ATCAirspaceFix(latitudeDouble, longitudeDouble));
        }
    }

    sectorFile.close();
}

void ATCSituationalDisplay::displayData()
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

    double sectorCentreX = (mercatorXmin + mercatorXmax) / 2;
    double sectorCentreY = (mercatorYmin + mercatorYmax) / 2;

    double scaleFactor = calculateScaleFactor(mercatorXmin, mercatorXmax, mercatorYmin, mercatorYmax);

    displaySectors(tempSectors, airspaceData, sectorCentreX, sectorCentreY, scaleFactor);
}

double ATCSituationalDisplay::mercatorProjectionLong(double longitudeDeg, double referenceLongitudeDeg, double scale)
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
            tempCoord.x = mercatorProjectionLong(currentAirspaceFix->longitude());
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

    double spanXperPixel = spanX / 1920;
    double spanYperPixel = spanY / 1020;

    double scaleFactor = 0;
    if(spanYperPixel >= spanXperPixel)
    {
        scaleFactor = 1020 / spanY;
    }
    else
    {
        scaleFactor = 1920 / spanX;
    }

    return scaleFactor;
}

void ATCSituationalDisplay::displaySectors(QVector<sector> &sectorVector, ATCAirspace *airspace, double centreX, double centreY, double scaleFactor)
{
    for(int i = 0; i < airspace->getSectorVectorSize(); i++)
    {
        QVector<QPointF> polygonVertex(airspace->getSector(i)->getCoordinatesVectorSize());

        for(int j = 0; j < airspace->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            qreal sceneCoordX = static_cast<qreal>((sectorVector[i].coords[j].x - centreX) * scaleFactor);
            qreal sceneCoordY = static_cast<qreal>(-1 * (sectorVector[i].coords[j].y - centreY) * scaleFactor);

            QPointF vertex(sceneCoordX, sceneCoordY);
            polygonVertex[j] = vertex;
        }

        polygonVertex.append(polygonVertex[0]);

        QPolygonF sectorPolygon(polygonVertex);
        QPen pen(Qt::blue);
        pen.setWidth(3);

        scene->addPolygon(sectorPolygon, pen);
    }
}

void ATCSituationalDisplay::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QPoint numDegrees = event->angleDelta();
        qreal newScale = baseScale + (numDegrees.y() / 120) * scaleResolution;

        scale(newScale, newScale);
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
