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
//Part calculating Mercator projection
//    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
//    {
//        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
//        {
//            ATCAirspaceFix* currentAirspaceFix = airspaceData->getSector(i)->getCoordinates(j);

//            double mercatorX = currentAirspaceFix->longitude();
//            double mercatorY = qLn(qTan(ATCConst::PI / 4 + currentAirspaceFix->latitude() * ATCConst::DEG_2_RAD / 2)
//                               * qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(currentAirspaceFix->latitude() * ATCConst::DEG_2_RAD)) /
//                               (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(currentAirspaceFix->latitude() * ATCConst::DEG_2_RAD)) ,
//                               ATCConst::WGS84_FIRST_ECCENTRICITY / 2)) * ATCConst::RAD_2_DEG;

//            currentAirspaceFix->setMercatorX(mercatorX);
//            currentAirspaceFix->setMercatorY(mercatorY);
//        }
//    }

//Part calculating global lat & lon extrema
    double latitudeMin = airspaceData->getSector(0)->getCoordinates(0)->latitude();
    double latitudeMax = airspaceData->getSector(0)->getCoordinates(0)->latitude();
    double longitudeMin = airspaceData->getSector(0)->getCoordinates(0)->longitude();
    double longitudeMax = airspaceData->getSector(0)->getCoordinates(0)->longitude();

    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            double latitudeCurrent = airspaceData->getSector(i)->getCoordinates(j)->latitude();
            double longitudeCurrent = airspaceData->getSector(i)->getCoordinates(j)->longitude();

            if(latitudeCurrent < latitudeMin)
                latitudeMin = latitudeCurrent;
            else if(latitudeCurrent > latitudeMax)
                latitudeMax = latitudeCurrent;

            if(longitudeCurrent < longitudeMin)
                longitudeMin = longitudeCurrent;
            else if(longitudeCurrent > longitudeMax)
                longitudeMax = longitudeCurrent;
        }
    }

    qDebug() << "Lat min: " << latitudeMin;
    qDebug() << "Lat max: " << latitudeMax;
    qDebug() << "Long min: " << longitudeMin;
    qDebug() << "Long max: " << longitudeMax;

//Part calculating loaded sector centre
    double sectorCentreLatitude = (latitudeMin + latitudeMax) / 2;
    double sectorCentreLongitude = (longitudeMin + longitudeMax) / 2;

    qDebug() << "Centre lat: " << sectorCentreLatitude;
    qDebug() << "Centre long: " << sectorCentreLongitude;

//Part calculating local lat & lon by transforming from global to sector centre
    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        qDebug() << airspaceData->getSector(i)->getSectorName();

        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            ATCAirspaceFix* currentAirspaceFix = airspaceData->getSector(i)->getCoordinates(j);

            currentAirspaceFix->transformToLocal(sectorCentreLatitude, sectorCentreLongitude);
            if(currentAirspaceFix->getFlagLocalCoordsInitialized())
                qDebug() << currentAirspaceFix->getLocalLatitude() << "   " << currentAirspaceFix->getLocalLongitude();
        }
    }

//Part calculating scale factor
    double latitudeSpan = latitudeMax - latitudeMin;
    double longitudeSpan = longitudeMax - longitudeMin;

    qDebug() << "Lat span: " << latitudeSpan;
    qDebug() << "Long span: " << longitudeSpan;

    double latitudeSpanPerPixel = latitudeSpan / 1020;
    double longitudeSpanPerPixel = longitudeSpan / 1920;

    qDebug() << "Lat span per pixel: " << latitudeSpanPerPixel;
    qDebug() << "Long span per pixel: " << longitudeSpanPerPixel;

    double scaleFactor = 0;
    if(latitudeSpanPerPixel >= longitudeSpanPerPixel)
    {
        scaleFactor = 1020 / latitudeSpan;
    }
    else
    {
        scaleFactor = 1920 / longitudeSpan;
    }

    qDebug() << "Scale factor: " << scaleFactor;

//Part translating from local lat & lon to scene QPointF coordinates and add them to scene
    for(int i = 0; i < airspaceData->getSectorVectorSize(); i++)
    {
        qDebug() << airspaceData->getSector(i)->getSectorName();

        QVector<QPointF> polygonVertex(airspaceData->getSector(i)->getCoordinatesVectorSize());

        for(int j = 0; j < airspaceData->getSector(i)->getCoordinatesVectorSize(); j++)
        {
            ATCAirspaceFix* currentAirspaceFix = airspaceData->getSector(i)->getCoordinates(j);

            double currentLocalLatitude = currentAirspaceFix->getLocalLatitude();
            double currentLocalLongitude = currentAirspaceFix->getLocalLongitude();

            qreal sceneCoordX = static_cast<qreal>(currentLocalLongitude * scaleFactor);
            qreal sceneCoordY = static_cast<qreal>(-1 * currentLocalLatitude * scaleFactor);

            qDebug() << "( " << sceneCoordX << " , " << sceneCoordY << ")";

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
