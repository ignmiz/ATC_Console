
#include "atcflight.h"

ATCFlight::ATCFlight(State s) : state(s)
{

}

ATCFlight::ATCFlight(State s, ATCFlightPlan *p) : state(s), plan(p)
{

}

ATCFlight::ATCFlight(State s, ATCFlightPlan *p, QString sq) : state(s), plan(p), squawk(sq)
{

}

ATCFlight::~ATCFlight()
{
    if(plan != nullptr)
    {
        delete plan;
    }
}

ATCFlightPlan *ATCFlight::getFlightPlan()
{
    return plan;
}

QString ATCFlight::getSquawk()
{
    return squawk;
}

State& ATCFlight::getState()
{
    return state;
}

void ATCFlight::setFlightPlan(ATCFlightPlan *fpl)
{
    plan = fpl;
}

void ATCFlight::setSquawk(QString sq)
{
    squawk = sq;
}

void ATCFlight::setState(State st)
{
    state = st;
}
