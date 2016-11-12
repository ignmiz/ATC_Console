
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

ATCFlightPlanFactory &ATCFlightFactory::getFlightPlanFactory()
{
    return flightPlanFactory;
}

ATCAircraftTypeFactory &ATCFlightFactory::getAircraftTypeFactory()
{
    return flightPlanFactory.getTypeFactory();
}

ATCCompanyFactory &ATCFlightFactory::getCompanyFactory()
{
    return flightPlanFactory.getCompanyFactory();
}

ATCRouteFactory &ATCFlightFactory::getRouteFactory()
{
    return flightPlanFactory.getRouteFactory();
}

QString ATCFlightFactory::generateSquawk()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QString squawk;

    for(int i = 0; i < 4; i++)
    {
        int asciiNumMin = 48;
        int asciiNumMax = 55;

        char ch = asciiNumMin + qrand() % (asciiNumMax - asciiNumMin + 1);


        squawk.append(ch);
    }

    return squawk;
}
