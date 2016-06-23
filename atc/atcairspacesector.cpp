#include "atcairspacesector.h"
#include <QDebug>

ATCAirspaceSector::ATCAirspaceSector()
{

}

ATCAirspaceSector::ATCAirspaceSector(QString name)
{
    sectorName = name;
}

ATCAirspaceSector::~ATCAirspaceSector()
{
    if(!sectorName.isEmpty())
    {
        qDebug() << "Sector " + sectorName + " deleted...";
    }
    else
    {
        qDebug() << "Sector with no name deleted...";
    }

    deleteAllAirspaceFixes();
}

QString ATCAirspaceSector::getSectorName()
{
    return sectorName;
}

ATCAirspaceFix* ATCAirspaceSector::getCoordinates(int iterator)
{
    return coordinates[iterator];
}

int ATCAirspaceSector::getCoordinatesVectorSize()
{
    return coordinates.size();
}

QGraphicsPolygonItem* ATCAirspaceSector::getPolygon()
{
    return polygon;
}

void ATCAirspaceSector::setSectorName(QString name)
{
    sectorName = name;
}

void ATCAirspaceSector::setPolygon(QGraphicsPolygonItem *sectorPolygon)
{
    polygon = sectorPolygon;
}

void ATCAirspaceSector::appendAirspaceFix(ATCAirspaceFix *airspaceFix)
{
    coordinates.append(airspaceFix);
}

void ATCAirspaceSector::deleteAllAirspaceFixes()
{
    if(!coordinates.empty())
    {
        for(int i = 0; i < coordinates.size(); i++)
        {
            delete coordinates[i];
        }
    }
    else
    {
        qDebug() << "Empty vector of coordinates...";
    }
}


