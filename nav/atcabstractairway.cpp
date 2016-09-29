#include "atcabstractairway.h"

ATCAbstractAirway::ATCAbstractAirway(QString name) : airwayName(name)
{
}

ATCAbstractAirway::~ATCAbstractAirway()
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

QString ATCAbstractAirway::getName()
{
    return airwayName;
}

ATCAirspaceFix *ATCAbstractAirway::getCoords1(int iterator)
{
    return coords1.at(iterator);
}

ATCAirspaceFix *ATCAbstractAirway::getCoords2(int iterator)
{
    return coords2.at(iterator);
}

QGraphicsLineItem *ATCAbstractAirway::getLine(int iterator)
{
    return lines.at(iterator);
}

int ATCAbstractAirway::getCoordsVectorSize()
{
    return coords1.size();
}

void ATCAbstractAirway::setName(QString name)
{
    airwayName = name;
}

void ATCAbstractAirway::setColor(QColor color)
{
    for(int i = 0; i < lines.size(); i++)
    {
        QPen newPen(lines.at(i)->pen());
        newPen.setColor(color);

        lines.at(i)->setPen(newPen);
    }
}

void ATCAbstractAirway::appendCoords1(ATCAirspaceFix *coord)
{
    coords1.append(coord);
}

void ATCAbstractAirway::appendCoords2(ATCAirspaceFix *coord)
{
    coords2.append(coord);
}

void ATCAbstractAirway::appendLine(QGraphicsLineItem *line)
{
    lines.append(line);
}

void ATCAbstractAirway::show()
{
    for(int i = 0; i < lines.size(); i++)
    {
        lines.at(i)->show();
    }

    visible = true;
}

void ATCAbstractAirway::hide()
{
    for(int i = 0; i < lines.size(); i++)
    {
        lines.at(i)->hide();
    }

    visible = false;
}

bool ATCAbstractAirway::isVisible()
{
    return visible;
}

void ATCAbstractAirway::setFlagVisible(bool flag)
{
    visible = flag;
}
