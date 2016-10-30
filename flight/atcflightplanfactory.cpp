
#include "atcflightplanfactory.h"

ATCFlightPlanFactory::ATCFlightPlanFactory() :
    companyFactory(ATCCompanyFactory(paths.COMPANY_PATH)),
    typeFactory(ATCAircraftTypeFactory(paths.BADA_PATH)),
    routeFactory(ATCRouteFactory(paths.ROUTE_PATH))
{

}

ATCFlightPlanFactory::~ATCFlightPlanFactory()
{

}

ATCFlightPlan *ATCFlightPlanFactory::getFlightPlan()
{
    return new ATCFlightPlan();
}

ATCFlightPlan *ATCFlightPlanFactory::getFlightPlan(ATC::FlightRules fr, QString adep, QString ades, int altitude)
{
//    ATCCompany* company = companyFactory.getCompany();

    return new ATCFlightPlan();
}

