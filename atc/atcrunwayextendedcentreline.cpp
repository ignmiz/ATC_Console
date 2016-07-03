#include "atcrunwayextendedcentreline.h"

ATCRunwayExtendedCentreline::ATCRunwayExtendedCentreline()
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

