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
}

QGraphicsLineItem* ATCRunwayExtendedCentreline::getCentreline()
{
    return centreline;
}

QGraphicsLineItem* ATCRunwayExtendedCentreline::getTick(int i)
{
    return ticks->at(i);
}

void ATCRunwayExtendedCentreline::appendTick(QGraphicsLineItem *tick)
{
    ticks->append(tick);
}

void ATCRunwayExtendedCentreline::setCentreline(QGraphicsLineItem *line)
{
    centreline = line;
}

