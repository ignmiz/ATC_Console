
#include "atcflightplan.h"


ATCFlightPlan::ATCFlightPlan()
{

}

ATCFlightPlan::ATCFlightPlan(ATC::FlightRules fr, ATCCompany *company, QString fltNo, ATCAircraftType *type, ATCRoute *route, int tas, QString altitude, QTime depTime, QTime enrTime, QTime fuelTime) :
    rules(fr),
    company(company),
    flightNumber(fltNo),
    type(type),
    route(ATCRoute(route)),
    TAS(tas),
    altitude(altitude),
    departureTime(depTime),
    enrouteTime(enrTime),
    fuelTime(fuelTime)
{

}

ATCFlightPlan::~ATCFlightPlan()
{

}

ATC::FlightRules ATCFlightPlan::getFlightRules()
{
    return rules;
}

ATCCompany *ATCFlightPlan::getCompany()
{
    return company;
}

QString ATCFlightPlan::getFlightNumber()
{
    return flightNumber;
}

ATCAircraftType *ATCFlightPlan::getType()
{
    return type;
}

ATCRoute& ATCFlightPlan::getRoute()
{
    return route;
}

int ATCFlightPlan::getTAS()
{
    return TAS;
}

QString ATCFlightPlan::getAltitude()
{
    return altitude;
}

QTime ATCFlightPlan::getDepartureTime()
{
    return departureTime;
}

QTime ATCFlightPlan::getEnrouteTime()
{
    return enrouteTime;
}

QTime ATCFlightPlan::getFuelTime()
{
    return fuelTime;
}

void ATCFlightPlan::setFlightRules(ATC::FlightRules newRules)
{
    rules = newRules;
}

void ATCFlightPlan::setCompany(ATCCompany *newCompany)
{
    company = newCompany;
}

void ATCFlightPlan::setFlightNumber(QString newNo)
{
    flightNumber = newNo;
}

void ATCFlightPlan::setType(ATCAircraftType *newType)
{
    type = newType;
}

void ATCFlightPlan::setRoute(ATCRoute &newRoute)
{
    route = ATCRoute(newRoute);
}

void ATCFlightPlan::setTAS(int tas)
{
    TAS = tas;
}

void ATCFlightPlan::setAltitude(QString alt)
{
    altitude = alt;
}

void ATCFlightPlan::setDepartureTime(QTime newDepTime)
{
    departureTime = newDepTime;
}

void ATCFlightPlan::setEnrouteTime(QTime newEnrTime)
{
    enrouteTime = newEnrTime;
}

void ATCFlightPlan::setFuelTime(QTime newFuelTime)
{
    fuelTime = newFuelTime;
}
