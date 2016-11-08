#ifndef ATCFLIGHTPLANFACTORY_H
#define ATCFLIGHTPLANFACTORY_H

#include "atcflightplan.h"
#include "atccompanyfactory.h"
#include "atcaircrafttypefactory.h"
#include "atcroutefactory.h"
#include "atcflightnumberfactory.h"
#include "atcpaths.h"
#include "atcmath.h"

class ATCFlightPlanFactory
{
public:
    explicit ATCFlightPlanFactory(ATCPaths &paths);
    ~ATCFlightPlanFactory();

    ATCFlightPlan* newFlightPlan();
    ATCFlightPlan* newFlightPlan(ATC::FlightRules fr, QString adep, QString ades, int altitude, QTime dep);

    ATCCompanyFactory& getCompanyFactory();
    ATCAircraftTypeFactory& getTypeFactory();
    ATCRouteFactory& getRouteFactory();

private:
    ATCCompanyFactory companyFactory;
    ATCAircraftTypeFactory typeFactory;
    ATCRouteFactory routeFactory;
};

#endif // ATCFLIGHTPLANFACTORY_H
