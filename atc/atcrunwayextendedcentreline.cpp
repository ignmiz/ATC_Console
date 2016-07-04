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

void ATCRunwayExtendedCentreline::setCentreline(QGraphicsLineItem *line)
{
    centreline = line;
}

