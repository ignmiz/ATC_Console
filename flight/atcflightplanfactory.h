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

    ATCFlightPlan* getFlightPlan();
    ATCFlightPlan* getFlightPlan(ATC::FlightRules fr, QString adep, QString ades, int altitude, QTime dep);

private:
    ATCCompanyFactory companyFactory;
    ATCAircraftTypeFactory typeFactory;
    ATCRouteFactory routeFactory;
};

#endif // ATCFLIGHTPLANFACTORY_H
