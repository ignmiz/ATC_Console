
#include "atcflight.h"

ATCFlight::ATCFlight(State s) : state(s)
{
}

ATCFlight::ATCFlight(State s, ATCFlightPlan *p) : state(s), plan(p)
{
}

ATCFlight::ATCFlight(State s, ATCFlightPlan *p, QString sq) : state(s), plan(p), assignedSquawk(sq)
{
}

ATCFlight::~ATCFlight()
{
    disconnect(tag->getTagBox(), SIGNAL(signalCreateDialogAltitude(QPoint)), this, SLOT(slotCreateDialogAltitude(QPoint)));

    if(plan != nullptr)
    {
        delete plan;
    }

    if(tag != nullptr)
    {
        delete tag;
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

QString ATCFlight::getAssignedSquawk()
{
    return assignedSquawk;
}

State& ATCFlight::getState()
{
    return state;
}

ATC::NavMode ATCFlight::getNavMode()
{
    return mode;
}

QString ATCFlight::getTargetAltitude()
{
    return targetAltitude;
}

QString ATCFlight::getTargetSpeed()
{
    return targetSpeed;
}

int ATCFlight::getHdgRestriction()
{
    return hdgRestriction;
}

QString ATCFlight::getNextFix()
{
    return nextFix;
}

QTime ATCFlight::getSimStartTime()
{
    return simStart;
}

ATCFlightTag *ATCFlight::getFlightTag()
{
    return tag;
}

void ATCFlight::setFlightPlan(ATCFlightPlan *fpl)
{
    delete plan;
    plan = fpl;
}

void ATCFlight::setSquawk(QString sq)
{
    squawk = sq;
}

void ATCFlight::setAssignedSquawk(QString sq)
{
    assignedSquawk = sq;
}

void ATCFlight::setState(State st)
{
    state = st;
}

void ATCFlight::setNavMode(ATC::NavMode m)
{
    mode = m;
}

void ATCFlight::setTargetAltitude(QString altitude)
{
    targetAltitude = altitude;
}

void ATCFlight::setTargetSpeed(QString speed)
{
    targetSpeed = speed;
}

void ATCFlight::setHdgRestriction(int hdg)
{
    hdgRestriction = hdg;
}

void ATCFlight::setNextFix(QString fix)
{
    nextFix = fix;
}

void ATCFlight::setSimStartTime(QTime time)
{
    simStart = time;
}

void ATCFlight::setFlightTag(ATCFlightTag *t)
{
    tag = t;
}

void ATCFlight::slotCreateDialogAltitude(QPoint point)
{
    emit signalCreateDialogAltitude(this, point);
}
