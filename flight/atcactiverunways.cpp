
#include "atcactiverunways.h"

ATCActiveRunways::ATCActiveRunways()
{

}

ATCActiveRunways::~ATCActiveRunways()
{

}

QVector<ActiveAirport> &ATCActiveRunways::getActiveAirports()
{
    return activeAirports;
}

ActiveAirport ATCActiveRunways::getActiveAirport(int i)
{
    return activeAirports.at(i);
}

void ATCActiveRunways::appendActiveAirport(ActiveAirport &airport)
{
    activeAirports.append(airport);
}

void ATCActiveRunways::clearActiveAirports()
{
    activeAirports.clear();
}

