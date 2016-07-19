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

int ATCAbstractSector::getCoordsVectorSize()
{
    return coords1.size();
}

void ATCAbstractSector::setName(QString name)
{
    sectorName = name;
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
