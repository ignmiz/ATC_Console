#include "atcabstractsector.h"

ATCAbstractSector::ATCAbstractSector(QString name) : sectorName(name)
{
}

ATCAbstractSector::~ATCAbstractSector()
{
    if(!lines.empty())
    {
        for(int i = 0; i < lines.size(); i++)
        {
            delete lines.at(i);
        }
    }

    if(!coords1.empty())
    {
        for(int i = 0; i < coords1.size(); i++)
        {
            delete coords1.at(i);
        }
    }

    if(!coords2.empty())
    {
        for(int i = 0; i < coords2.size(); i++)
        {
            delete coords2.at(i);
        }
    }

    if(!polygons.empty())
    {
        for(int i = 0; i < polygons.size(); i++)
        {
            delete polygons.at(i);
        }
    }
}

QString ATCAbstractSector::getName()
{
    return sectorName;
}

ATCAirspaceFix *ATCAbstractSector::getCoords1(int iterator)
{
    return coords1.at(iterator);
}

ATCAirspaceFix *ATCAbstractSector::getCoords2(int iterator)
{
    return coords2.at(iterator);
}

coordsPair& ATCAbstractSector::getCoordsPair(int iterator)
{
    return projectedCoords[iterator];
}

QGraphicsLineItem *ATCAbstractSector::getLine(int iterator)
{
    return lines.at(iterator);
}

QGraphicsPolygonItem *ATCAbstractSector::getPolygon(int iterator)
{
    return polygons.at(iterator);
}

QColor ATCAbstractSector::getColor()
{
    return polygons.at(0)->pen().color();
}

int ATCAbstractSector::getCoordsVectorSize()
{
    return coords1.size();
}

int ATCAbstractSector::getPolygonsVectorSize()
{
    return polygons.size();
}

void ATCAbstractSector::setName(QString name)
{
    sectorName = name;
}

void ATCAbstractSector::setColor(QColor &color)
{
    for(int i = 0; i < polygons.size(); i++)
    {
        QPen newPen(polygons.at(i)->pen());
        newPen.setColor(color);

        polygons.at(i)->setPen(newPen);
    }
}

void ATCAbstractSector::appendCoords1(ATCAirspaceFix *coord)
{
    coords1.append(coord);
}

void ATCAbstractSector::appendCoords2(ATCAirspaceFix *coord)
{
    coords2.append(coord);
}

void ATCAbstractSector::appendCoordsPair(coordsPair &pair)
{
    projectedCoords.append(pair);
}

void ATCAbstractSector::appendLine(QGraphicsLineItem *line)
{
    lines.append(line);
}

void ATCAbstractSector::appendPolygon(QGraphicsPolygonItem *polygon)
{
    polygons.append(polygon);
}

void ATCAbstractSector::show() //TO BE CHANGED
{
    visible = true;
}

void ATCAbstractSector::hide() //TO BE CHANGED
{
    visible = false;
}

bool ATCAbstractSector::isVisible()
{
    return visible;
}

void ATCAbstractSector::setFlagVisible(bool flag)
{
    visible = flag;
}
