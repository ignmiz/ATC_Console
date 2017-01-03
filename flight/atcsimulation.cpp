
#include "atcsimulation.h"

ATCSimulation::ATCSimulation() : activeRunways(new ATCActiveRunways())
{

}

ATCSimulation::~ATCSimulation()
{
    if(activeRunways != nullptr) delete activeRunways;

    if(!flights.empty())
    {
        for(int i = 0; i < flights.size(); i++)
        {
            delete flights.at(i);
        }
    }

    flights.clear();
}

ATCActiveRunways *ATCSimulation::getActiveRunways()
{
    return activeRunways;
}

ATCFlight* ATCSimulation::getFlight(int i)
{
    return flights.at(i);
}

ATCFlight* ATCSimulation::getFlight(QString callsign)
{
    ATCFlight *flight = nullptr;

    for(int i = 0; i < flights.size(); i++)
    {
        if((callsign.left(3) == flights.at(i)->getFlightPlan()->getCompany()->getCode()) &&
           (callsign.right(callsign.size() - 3) == flights.at(i)->getFlightPlan()->getFlightNumber()))
        {
            flight = flights.at(i);
        }
        else if(callsign == flights.at(i)->getFlightPlan()->getCompany()->getCode())
        {
            flight = flights.at(i);
        }
    }

    return flight;
}

QVector<ATCFlight *> &ATCSimulation::getFlightsVector()
{
    return flights;
}

int ATCSimulation::getFlightsVectorSize()
{
    return flights.size();
}

void ATCSimulation::appendFlight(ATCFlight *flight)
{
    flights.append(flight);
}

void ATCSimulation::removeFlight(QString callsign)
{
    ATCFlight *flight = getFlight(callsign);

    if(flight != nullptr)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            if(flights.at(i) == flight)
            {
                delete flights.at(i);
                flights.remove(i);
            }
        }
    }
}

void ATCSimulation::clearFlights()
{
    flights.clear();
}
