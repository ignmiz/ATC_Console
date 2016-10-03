#include "atcairspacesector.h"
#include <QDebug>

ATCAirspaceSector::ATCAirspaceSector(QString name)
{
    sectorName = name;
}

ATCAirspaceSector::~ATCAirspaceSector()
{
    deleteAllAirspaceFixes();
    if(polygon != nullptr) delete polygon;
}

QString ATCAirspaceSector::getName()
{
    return sectorName;
}

ATCAirspaceFix* ATCAirspaceSector::getCoordinates(int iterator)
{
    return coordinates.at(iterator);
}

int ATCAirspaceSector::getCoordinatesVectorSize()
{
    return coordinates.size();
}

QGraphicsPolygonItem* ATCAirspaceSector::getPolygon()
{
    return polygon;
}

void ATCAirspaceSector::setName(QString name)
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
            delete coordinates.at(i);
        }
    }

    coordinates.clear();
}
