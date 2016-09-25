#include "atcrunwayextendedcentreline.h"

ATCRunwayExtendedCentreline::ATCRunwayExtendedCentreline()
{

}

ATCRunwayExtendedCentreline::ATCRunwayExtendedCentreline(QGraphicsLineItem *line) : centreline(line)
{

}

ATCRunwayExtendedCentreline::~ATCRunwayExtendedCentreline()
{
    if(centreline != nullptr) delete centreline;

    for(int i = 0; i < ticks.size(); i++)
    {
        if(ticks.at(i) != nullptr) delete ticks.at(i);
    }
}

QGraphicsLineItem* ATCRunwayExtendedCentreline::getCentreline()
{
    return centreline;
}

QVector<QGraphicsLineItem*>& ATCRunwayExtendedCentreline::getTicksVector()
{
    return ticks;
}

void ATCRunwayExtendedCentreline::appendTick(QGraphicsLineItem *tick)
{
    ticks.append(tick);
}

void ATCRunwayExtendedCentreline::setCentreline(QGraphicsLineItem *line)
{
    centreline = line;
}

void ATCRunwayExtendedCentreline::setColor(QColor color)
{
    QPen newPen(centreline->pen());
    newPen.setColor(color);

    centreline->setPen(newPen);

    for(int i = 0; i < ticks.size(); i++)
    {
        ticks.at(i)->setPen(newPen);
    }
}

void ATCRunwayExtendedCentreline::show()
{
    centreline->show();

    for(int i = 0; i < ticks.size(); i++)
    {
        ticks.at(i)->show();
    }

    visible = true;
}

void ATCRunwayExtendedCentreline::hide()
{
    centreline->hide();

    for(int i = 0; i < ticks.size(); i++)
    {
        ticks.at(i)->hide();
    }

    visible = false;
}

bool ATCRunwayExtendedCentreline::isVisible()
{
    return visible;
}

