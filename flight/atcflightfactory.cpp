
#include "atcflightfactory.h"

ATCFlightFactory::ATCFlightFactory(ATCPaths paths) : flightPlanFactory(ATCFlightPlanFactory(paths))
{

}

ATCFlightFactory::~ATCFlightFactory()
{

}

ATCFlight *ATCFlightFactory::newFlight(State s)
{
    return new ATCFlight(s);
}

ATCFlight *ATCFlightFactory::newFlight(State s, ATC::FlightRules fr, QString adep, QString ades, int altitude, QTime dep)
{
    ATCFlightPlan* plan = flightPlanFactory.newFlightPlan(fr, adep, ades, altitude, dep);

    return new ATCFlight(s, plan);
}

ATCFlight *ATCFlightFactory::newFlight(State s, ATC::FlightRules fr, QString adep, QString ades, int altitude, QTime dep, QString sq)
{
    ATCFlightPlan* plan = flightPlanFactory.newFlightPlan(fr, adep, ades, altitude, dep);

    return new ATCFlight(s, plan, sq);
}
