
#include "atcsimulation.h"

ATCSimulation::ATCSimulation() : activeRunways(new ATCActiveRunways())
{

}

ATCSimulation::~ATCSimulation()
{
    if(activeRunways != nullptr) delete activeRunways;

    if(!flights.empty())
    {
        for(int i = 0; i < flights.size(); i++)
        {
            delete flights.at(i);
        }
    }

    flights.clear();
}

ATCActiveRunways *ATCSimulation::getActiveRunways()
{
    return activeRunways;
}

ATCFlight* ATCSimulation::getFlight(int i)
{
    return flights.at(i);
}

ATCFlight* ATCSimulation::getFlight(QString callsign)
{
    ATCFlight *flight = nullptr;

    for(int i = 0; i < flights.size(); i++)
    {
        if((callsign.left(3) == flights.at(i)->getFlightPlan()->getCompany()->getCode()) &&
           (callsign.right(callsign.size() - 3) == flights.at(i)->getFlightPlan()->getFlightNumber()))
        {
            flight = flights.at(i);
        }
        else if(callsign == flights.at(i)->getFlightPlan()->getCompany()->getCode())
        {
            flight = flights.at(i);
        }
    }

    return flight;
}

QVector<ATCFlight *> &ATCSimulation::getFlightsVector()
{
    return flights;
}

int ATCSimulation::getFlightsVectorSize()
{
    return flights.size();
}

void ATCSimulation::appendFlight(ATCFlight *flight)
{
    flights.append(flight);
}

void ATCSimulation::removeFlight(QString callsign)
{
    ATCFlight *flight = getFlight(callsign);

    if(flight != nullptr)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            if(flights.at(i) == flight)
            {
                delete flights.at(i);
                flights.remove(i);
            }
        }
    }
}

void ATCSimulation::clearFlights()
{
    flights.clear();
}

void ATCSimulation::slotStartSimulation()
{
    GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();
    qint64 dt = ATCConst::DT;

    QElapsedTimer timer;
    qint64 elapsedTime;
    qint64 diff;

    simLoop = true;

    while(simLoop)
    {
        timer.start();
        progressState(geo);
        elapsedTime = timer.nsecsElapsed();

        diff = dt - elapsedTime;

        qDebug() << "Elapsed: " << elapsedTime << "ns\t|\tDiff: " << diff << "ns\t|\tError: " << dt - (elapsedTime + qFloor(diff/1000) * 1000) << "ns";
        QThread::usleep(qFloor(diff / 1000));
    }
}

void ATCSimulation::slotStopSimulation()
{
    simLoop = false;
}

void ATCSimulation::progressState(GeographicLib::Geodesic &geo)
{
    for(int i = 0; i < flights.size(); i++)
    {
        ATCFlight *flight = flights.at(i);

        ISA isa = calculateEnvironment(flight);
        assignDiscreteState(flight, isa);
        assignContinuousState(flight, isa, geo);
    }
}

ISA ATCSimulation::calculateEnvironment(ATCFlight *flight)
{
    return ATCMath::atmosISA(flight->getState().h);
}

void ATCSimulation::assignDiscreteState(ATCFlight *flight, ISA &isa)
{
    State state = flight->getState();
    double targetAltitude = ATCMath::ft2m(flight->getTargetAltitude().mid(1, -1).toDouble());

    ATCAircraftType *type = flight->getFlightPlan()->getType();

    state.cm = ATCMath::assignCM(state.h, targetAltitude);
    state.fp = ATCMath::assignFP(state.h, targetAltitude, state.v, type->getVelocity().V_DS1_AV, isa.p, isa.rho);
    state.rpm = ATCMath::assignRPM(state.h, type->getEnvelope().h_max_MTOW);
    state.shm = ATCMath::assignSHM(state.h, state.cm, 10000, 10000, 10000); //TEMP, NEED TO INCLUDE CROSSOVER ALTITUDES!
    state.trm = ATCMath::assignTRM(state.h);

    //Cpowred ?? YES, BUT NOT NECESSARY HERE

    //Vstall recalculation?

    double Vnom;

    switch(state.cm)
    {
        case BADA::Level:
            Vnom = ATCMath::nominalSpeedCR(state.h, state.shm, type->getAcType().engineType, type->getVelocity().V_CR1_AV, type->getVelocity().V_CR2_AV, type->getVelocity().M_CR_AV, isa.a, isa.rho, isa.p);
            break;

        case BADA::Descend:
            Vnom = ATCMath::nominalSpeedDS(state.h, state.shm, type->getAcType().engineType, type->getAeroTO().V_stall, type->getVelocity().V_DS1_AV, type->getVelocity().V_DS2_AV, type->getVelocity().M_DS_AV, isa.a, isa.rho, isa.p);
            break;

        case BADA::Climb:
            Vnom = ATCMath::nominalSpeedCL(state.h, state.shm, type->getAcType().engineType, type->getAeroTO().V_stall, type->getVelocity().V_CL1_AV, type->getVelocity().V_CL2_AV, type->getVelocity().M_CL_AV, isa.a, isa.rho, isa.p);
            break;
    }

    state.am = ATCMath::assignAM(state.v, Vnom);

    flight->setState(state);
}

void ATCSimulation::assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo)
{
    //What to do with Hdg navigation?
    //Route needs to be preprocessed before the main loop

    State state = flight->getState();
    ATCAircraftType *type = flight->getFlightPlan()->getType();

    double m = 40000; //TEMP    HOW TO HANDLE MASS?
    double dHdg = ATCMath::deg2rad(90); //TEMP

    double DTA = ATCMath::DTA(state.v, ATCConst::NOM_BANK_ANGLE, dHdg, ATCConst::FLY_OVER_DST);

    double fix1lat = ATCMath::rad2deg(state.y) - 5;
    double fix1lon = ATCMath::rad2deg(state.x) - 5;

    double fix2lat = ATCMath::rad2deg(state.y) + 5;
    double fix2lon = ATCMath::rad2deg(state.x) + 5;

    double xtrackError;
    double headingError;
    double dstToNext;
    ATCMath::projectAcftPosOnPath(geo, fix1lat, fix1lon, fix2lat, fix2lon, state.y, state.x, state.hdg, xtrackError, headingError,  dstToNext);

    //SWITCH WAYPOINT CONDITION? here or one step earlier?

    double bankAngle = ATCMath::bankAngle(ATCConst::k1, ATCConst::k2, xtrackError, headingError, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE));

    double CL = ATCMath::liftCoefficient(m, isa.rho, state.v, type->getSurface(), bankAngle);
    double lift = ATCMath::lift(isa.rho, state.v, type->getSurface(), CL);

    double CD;
    if(state.fp == BADA::Approach) //revise order of cases?
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroAP().CD0, type->getAeroAP().CD2, 0);
    }
    else if(state.fp == BADA::Landing)
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, type->getCDldg());
    }
    else
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroCR().CD0, type->getAeroCR().CD2, type->getCDldg());
    }

    double drag = ATCMath::drag(isa.rho, state.v, type->getSurface(), CD);

    double Cpowred = 0.3; //Cpowred calculation here?

    double thrust;
    if(state.fp == BADA::UpperDescent)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_high, Cpowred);
    }
    else if(state.fp == BADA::LowerDescent)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_low, Cpowred);
    }
    else if(state.fp == BADA::Approach)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_app, Cpowred);
    }
    else
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_ldg, Cpowred);
    }

    double Mach = state.v / isa.a;
    double ESF = ATCMath::ESF(state.cm, state.am, state.shm, state.trm, Mach, isa.T, 0);
    double pathAngle = ATCMath::pathAngle(thrust, drag, ESF, m);

    double radius = ATCMath::ellipsoidRadius(state.h, state.y);

    double xNext = state.x + ATCMath::stateLonDot(state.v, state.hdg, pathAngle, state.y, radius) * ATCConst::DT;
    double yNext = state.y + ATCMath::stateLatDot(state.v, state.hdg, pathAngle, radius) * ATCConst::DT;
    double hNext = state.h + ATCMath::stateHDot(state.v, pathAngle) * ATCConst::DT;
    double vNext = state.v + ATCMath::stateVDot(thrust, drag, m, ESF) * ATCConst::DT;
    double hdgNext = state.hdg + ATCMath::stateHdgDot(lift, m, state.v, bankAngle) * ATCConst::DT;

//    state.x = xNext;
//    state.y = yNext;
//    state.h = hNext;
//    state.v = vNext;
//    state.hdg = hdgNext;

    State temp = state;
    temp.x = xNext;
    temp.y = yNext;
    temp.h = hNext;
    temp.v = vNext;
    temp.hdg = hdgNext;

    flight->setState(temp);
}
