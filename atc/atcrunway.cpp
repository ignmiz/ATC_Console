#include "atcrunway.h"

#include <QDebug>

ATCRunway::ATCRunway(QString id1, QString id2, unsigned int hdg1, unsigned int hdg2,
                     double startLat, double startLon, double endLat, double endLon) :
    rwyID1(id1),
    rwyID2(id2),
    magneticHDG1(hdg1),
    magneticHDG2(hdg2),
    startPoint(startLat, startLon),
    endPoint(endLat, endLon)
{
}

ATCRunway::~ATCRunway()
{
    if(extendedCentreline != nullptr) delete extendedCentreline;
    qDebug() << rwyID1 << " " << rwyID2 << " " << magneticHDG1 << " " << magneticHDG2 << " "
             << startPoint.toString() << " " << endPoint.toString() << " deleted...";
}

QString ATCRunway::getRunwayID1()
{
    return rwyID1;
}

QString ATCRunway::getRunwayID2()
{
    return rwyID2;
}

unsigned int ATCRunway::getMagneticHDG1()
{
    return magneticHDG1;
}

unsigned int ATCRunway::getMagneticHDG2()
{
    return magneticHDG2;
}

QGeoCoordinate& ATCRunway::getStartPoint()
{
    return startPoint;
}

QGeoCoordinate& ATCRunway::getEndPoit()
{
    return endPoint;
}

ATCRunwayExtendedCentreline* ATCRunway::getExtendedCentreline()
{
    return extendedCentreline;
}

void ATCRunway::setExtendedCentreline(ATCRunwayExtendedCentreline *centreline)
{
    extendedCentreline = centreline;
}

