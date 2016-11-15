#ifndef ATCFLIGHTFACTORY_H
#define ATCFLIGHTFACTORY_H

#include "atcflight.h"
#include "atcflightplanfactory.h"
#include "atcpaths.h"

class ATCFlightFactory
{
public:
    explicit ATCFlightFactory(ATCPaths paths);
    ~ATCFlightFactory();

    ATCFlight* newFlight(State s);
    ATCFlight* newFlight(State s, ATC::FlightRules fr, QString adep, QString ades, QString altitude, QTime dep);
    ATCFlight* newFlight(State s, ATC::FlightRules fr, QString adep, QString ades, QString altitude, QTime dep, QString sq);

    ATCFlightPlanFactory& getFlightPlanFactory();
    ATCAircraftTypeFactory& getAircraftTypeFactory();
    ATCCompanyFactory& getCompanyFactory();
    ATCRouteFactory& getRouteFactory();

    static QString generateSquawk();

private:
    ATCFlightPlanFactory flightPlanFactory;
};


#endif // ATCFLIGHTFACTORY_H
