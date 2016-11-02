
#include "atcflightplanfactory.h"

ATCFlightPlanFactory::ATCFlightPlanFactory(ATCPaths &paths) :
    companyFactory(ATCCompanyFactory(paths.COMPANY_PATH)),
    typeFactory(ATCAircraftTypeFactory(paths.BADA_PATH)),
    routeFactory(ATCRouteFactory(paths.ROUTE_PATH))
{
//    companyFactory = ATCCompanyFactory(paths.COMPANY_PATH);
//    typeFactory = ATCAircraftTypeFactory(paths.BADA_PATH);
//    routeFactory = ATCRouteFactory(paths.ROUTE_PATH);
}

ATCFlightPlanFactory::~ATCFlightPlanFactory()
{

}

ATCFlightPlan *ATCFlightPlanFactory::getFlightPlan()
{
    return new ATCFlightPlan();
}

ATCFlightPlan *ATCFlightPlanFactory::getFlightPlan(ATC::FlightRules fr, QString adep, QString ades, int altitude, QTime dep)
{
    ATCCompany *company = companyFactory.getCompany();
    QString fltNo = ATCFlightNumberFactory::getFlightNumber();
    ATCAircraftType *type = typeFactory.getType();
    ATCRoute *route = routeFactory.getRoute(adep, ades);
    int tas = type->getVelocity().V_CR2_AV; //DUMMY! CONVERSION IAS/CAS/MACH + ISA ATMOSPHERE NEEDS TO BE IMPLEMENTED FIRST!
    QTime enr(0, 0, 0, 0); //DUMMY! HERE ENROUTE DISTANCE & TIME CALCULATOR IS NEEDED

    if(route == nullptr)
    {
        return nullptr;
    }
    else
    {
        return new ATCFlightPlan(fr, company, fltNo, type, route, tas, altitude, dep, enr);
    }
}

