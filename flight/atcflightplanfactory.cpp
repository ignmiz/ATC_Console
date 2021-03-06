
#include "atcflightplanfactory.h"

ATCFlightPlanFactory::ATCFlightPlanFactory(ATCPaths &paths) :
    companyFactory(ATCCompanyFactory(paths.COMPANY_PATH)),
    typeFactory(ATCAircraftTypeFactory(paths.BADA_PATH)),
    routeFactory(ATCRouteFactory(paths.ROUTE_PATH))
{

}

ATCFlightPlanFactory::~ATCFlightPlanFactory()
{

}

ATCFlightPlan *ATCFlightPlanFactory::newFlightPlan()
{
    return new ATCFlightPlan();
}

ATCFlightPlan *ATCFlightPlanFactory::newFlightPlan(ATC::FlightRules fr, QString adep, QString ades, QString altitude, QTime dep)
{
    ATCCompany *company = companyFactory.getCompany();
    QString fltNo = ATCFlightNumberFactory::getFlightNumber();
    ATCAircraftType *type = typeFactory.getType();
    ATCRoute *route = routeFactory.getRoute(adep, ades);

    ISA isa = ATCMath::atmosISA(ATCMath::ft2m(altitude.right(3).toInt() * 100));
    int tas = qFloor(ATCMath::mps2kt(ATCMath::mach2tas(type->getVelocity().M_CR_AV, isa.a)));

    QTime enr(0, 0, 0, 0); //DUMMY! HERE ENROUTE DISTANCE & TIME CALCULATOR IS NEEDED
    QTime fuel(0, 0, 0, 0); //DUMMY! HERE FUEL TIME SHOULD BE CALCULATED

    if(route == nullptr)
    {
        return nullptr;
    }
    else
    {
        return new ATCFlightPlan(fr, company, fltNo, type, route, tas, altitude, dep, enr, fuel);
    }
}

ATCCompanyFactory &ATCFlightPlanFactory::getCompanyFactory()
{
    return companyFactory;
}

ATCAircraftTypeFactory &ATCFlightPlanFactory::getTypeFactory()
{
    return typeFactory;
}

ATCRouteFactory &ATCFlightPlanFactory::getRouteFactory()
{
    return routeFactory;
}

