
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

bool ATCSimulation::isPaused()
{
    return paused;
}

void ATCSimulation::setPaused(bool flag)
{
    paused = flag;
}

void ATCSimulation::setDataLogged(bool flag, QString path)
{
    dataLogged = flag;
    dataLoggedPath = path;
}

void ATCSimulation::setAirspace(ATCAirspace *a)
{
    airspace = a;
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

void ATCSimulation::subtractFlightTimeouts()
{
    qint64 elapsedTimeMS = globalTimer.elapsed();

    for(int i = 0; i < flights.size(); i++)
    {
        ATCFlight *flight = flights.at(i);

        if(flight->getSimStartTime() != QTime(0, 0, 0))
        {
            qint64 simStartTimeMS = QTime(0, 0, 0).msecsTo(flight->getSimStartTime());

            if(simStartTimeMS >= elapsedTimeMS)
            {
                flight->setSimStartTime(flight->getSimStartTime().addMSecs(-elapsedTimeMS));
            }
            else
            {
                flight->setSimStartTime(QTime(0, 0, 0));
            }
        }
    }
}

void ATCSimulation::slotStartSimulation()
{
    GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();
    qint64 dt = ATCConst::DT * 1e9;

    QElapsedTimer timer;

    qint64 elapsedTime;
    qint64 diff;
    double counter = 0;

    preallocateTempData();
    simLoop = true;

    if(!paused) emit signalSetSimulationStartTime();
    setPaused(false);

    globalTimer.start();
    while(simLoop)
    {
        timer.start();

        progressState(geo, globalTimer);
        flightsCleanup();
        incrementUpdateCounter(counter);

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

void ATCSimulation::createDataLogs()
{
    for(int i = 0; i < flights.size(); i++)
    {
        //create QFiles here
    }
}

void ATCSimulation::preallocateTempData()
{
    if(!paused)
    {
        for(int i = 0; i < flights.size(); i++)
        {
            //Assign temporaries
            ATCFlight *flight = flights.at(i);
            ATCAircraftType *type = flight->getFlightPlan()->getType();

            Temp temp;

            temp.m = ATCMath::randomMass(qFloor(1.2 * type->getMass().min * 1000), qFloor(type->getMass().ref * 1000));

            if(type->getAcType().engineType == ATC::Jet)
            {
                temp.Cpowred = ATCMath::recalculateReductionFactor(0.15, temp.m, type->getMass().min * 1000, type->getMass().max * 1000);
            }
            else if(type->getAcType().engineType == ATC::Turboprop)
            {
                temp.Cpowred = ATCMath::recalculateReductionFactor(0.25, temp.m, type->getMass().min * 1000, type->getMass().max * 1000);
            }
            else //Piston
            {
                temp.Cpowred = ATCMath::recalculateReductionFactor(0, temp.m, type->getMass().min * 1000, type->getMass().max * 1000);
            }

            temp.vStallCR = ATCMath::recalculateSpeed(type->getAeroCR().V_stall, temp.m, type->getMass().ref * 1000);
            temp.vStallIC = ATCMath::recalculateSpeed(type->getAeroIC().V_stall, temp.m, type->getMass().ref * 1000);
            temp.vStallTO = ATCMath::recalculateSpeed(type->getAeroTO().V_stall, temp.m, type->getMass().ref * 1000);
            temp.vStallAP = ATCMath::recalculateSpeed(type->getAeroAP().V_stall, temp.m, type->getMass().ref * 1000);
            temp.vStallLD = ATCMath::recalculateSpeed(type->getAeroLD().V_stall, temp.m, type->getMass().ref * 1000);

            temp.xoverAltClbM = ATCMath::crossoverAltitude(ATCMath::kt2mps(type->getVelocity().V_CL2_AV), type->getVelocity().M_CL_AV);
            temp.xoverAltCrsM = ATCMath::crossoverAltitude(ATCMath::kt2mps(type->getVelocity().V_CR2_AV), type->getVelocity().M_CR_AV);
            temp.xoverAltDesM = ATCMath::crossoverAltitude(ATCMath::kt2mps(type->getVelocity().V_DS2_AV), type->getVelocity().M_DS_AV);

            if(!flight->getRunwayDestination().isEmpty())
            {
                ATCRunway *runway = airspace->findRunway(flight->getFlightPlan()->getRoute().getDestination(), flight->getRunwayDestination());

                if(runway != nullptr)
                {
                    double thrLat;
                    double thrLon;
                    double azimuth;

                    if(flight->getRunwayDestination().left(2).toInt() <= 18)
                    {
                        thrLat = runway->getStartPoint().latitude();
                        thrLon = runway->getStartPoint().longitude();
                        azimuth = runway->getAzimuth();
                    }
                    else
                    {
                        thrLat = runway->getEndPoint().latitude();
                        thrLon = runway->getEndPoint().longitude();
                        azimuth = ATCMath::normalizeAngle(runway->getAzimuth() + ATCConst::PI, ATC::Deg);
                    }

                    temp.rwyDesThr = QPair<double, double>(thrLat, thrLon);
                    temp.rwyDesAzimuth = azimuth;

                    double rangeLat;
                    double rangeLon;
                    GeographicLib::Rhumb rhumb = GeographicLib::Rhumb::WGS84();
                    rhumb.Direct(thrLat, thrLon, ATCMath::rad2deg(ATCMath::normalizeAngle(azimuth + ATCConst::PI, ATC::Deg)), ATCConst::APP_RANGE, rangeLat, rangeLon);
                    temp.rwyDesAppRange = QPair<double, double>(rangeLat, rangeLon);
                }
            }

            flight->setTemp(temp);

            //Assign waypoint coords & index
            QStringList fixList = flight->getFixList();
            for(int i = 0; i < fixList.size(); i++)
            {
                ATCNavFix *fix = nullptr;
                ATCBeaconVOR *vor = nullptr;
                ATCAirport *airport = nullptr;
                ATCBeaconNDB *ndb = nullptr;

                double lat;
                double lon;

                double x;
                double y;

                if((fix = airspace->findFix(fixList.at(i))) != nullptr)
                {
                    lat = fix->latitude();
                    lon = fix->longitude();
                    x = fix->getScenePosition()->x();
                    y = fix->getScenePosition()->y();
                }
                else if((airport = airspace->findAirport(fixList.at(i))) != nullptr)
                {
                    lat = airport->latitude();
                    lon = airport->longitude();
                    x = airport->getScenePosition()->x();
                    y = airport->getScenePosition()->y();
                }
                else if((vor = airspace->findVOR(fixList.at(i))) != nullptr)
                {
                    lat = vor->latitude();
                    lon = vor->longitude();
                    x = vor->getScenePosition()->x();
                    y = vor->getScenePosition()->y();
                }
                else if((ndb = airspace->findNDB(fixList.at(i))) != nullptr)
                {
                    lat = ndb->latitude();
                    lon = ndb->longitude();
                    x = ndb->getScenePosition()->x();
                    y = ndb->getScenePosition()->y();
                }

                flight->appendWaypoint(QPair<double, double>(lat, lon));
                flight->appendProjectedWaypoint(QPair<double, double>(x, y));
                if(fixList.at(i) == flight->getNextFix())
                {
                    flight->setWaypointIndex(i);
                    if(!flight->isFinalApp()) flight->setDCT(true);
                }
            }

            //Exclude flights that will happen in future
            if(flight->getSimStartTime() != QTime(0, 0, 0))
            {
                emit signalHideFlightTag(flight->getFlightTag());
                if(flight->getRoutePrediction() != nullptr) emit signalDisplayRoute(flight);
                flight->setSimulated(false);
            }
        }
    }
}

void ATCSimulation::progressState(GeographicLib::Geodesic &geo, QElapsedTimer &globalTimer)
{
    for(int i = 0; i < flights.size(); i++)
    {
        ATCFlight *flight = flights.at(i);

        if(flight->isSimulated())
        {
            ISA isa = calculateEnvironment(flight);
            assignDiscreteState(flight, isa);
            assignContinuousState(flight, isa, geo, i);
        }
        else
        {
            if(globalTimer.hasExpired(static_cast<qint64>(QTime(0, 0, 0).msecsTo(flight->getSimStartTime()))))
            {
                if(!flight->getFlightTag()->getDiamond()->isVisible()) emit signalShowFlightTag(flight->getFlightTag());
                flight->setSimulated(true);
                flight->setSimStartTime(QTime(0, 0, 0));
            }
        }
    }
}

ISA ATCSimulation::calculateEnvironment(ATCFlight *flight)
{
    return ATCMath::atmosISA(flight->getState().h);
}

void ATCSimulation::assignDiscreteState(ATCFlight *flight, ISA &isa)
{
    State state = flight->getState();
    Temp temp = flight->getTemp();

    double targetAltitude;
    if(!flight->isGlidePath())
    {
        targetAltitude = ATCMath::ft2m(flight->getTargetAltitude().mid(1, -1).toDouble()) * 100;
    }
    else
    {
        targetAltitude = flight->getAppTargetAltitude();
    }

    ATCAircraftType *type = flight->getFlightPlan()->getType();

    state.cm = ATCMath::assignCM(state.h, targetAltitude);
    state.fp = ATCMath::assignFP(state.h, ATCMath::ft2m(type->getThrust().h_pdes), state.v, type->getVelocity().V_DS1_AV, isa.p, isa.rho);
    state.rpm = ATCMath::assignRPM(state.h, type->getEnvelope().h_max_MTOW);
    state.shm = ATCMath::assignSHM(state.h, state.cm, temp.xoverAltClbM, temp.xoverAltCrsM, temp.xoverAltDesM);

    double Vnom;
    QString targetSpeed = flight->getTargetSpeed();
    if(targetSpeed.isEmpty() || (targetSpeed == "---"))
    {
        switch(state.cm)
        {
            case BADA::Level:
                Vnom = ATCMath::nominalSpeedCR(state.h, state.shm, type->getAcType().engineType, type->getVelocity().V_CR1_AV, type->getVelocity().V_CR2_AV, type->getVelocity().M_CR_AV, isa.a, isa.rho, isa.p);
                break;

            case BADA::Descend:
                Vnom = ATCMath::nominalSpeedDS(state.h, state.shm, type->getAcType().engineType, temp.vStallTO, type->getVelocity().V_DS1_AV, type->getVelocity().V_DS2_AV, type->getVelocity().M_DS_AV, isa.a, isa.rho, isa.p);
                break;

            case BADA::Climb:
                Vnom = ATCMath::nominalSpeedCL(state.h, state.shm, type->getAcType().engineType, temp.vStallTO, type->getVelocity().V_CL1_AV, type->getVelocity().V_CL2_AV, type->getVelocity().M_CL_AV, isa.a, isa.rho, isa.p);
                break;
        }
    }
    else
    {
        if(targetSpeed.at(1) == ".")
        {
            Vnom = isa.a * targetSpeed.toDouble();
        }
        else
        {
            Vnom = ATCMath::cas2tas(ATCMath::kt2mps(targetSpeed.toDouble()), isa.p, isa.rho);
        }
    }

    double vMin;
    if((state.fp == BADA::UpperDescent) || (state.fp == BADA::LowerDescent))
    {
        vMin = ATCConst::C_V_MIN * ATCMath::kt2mps(temp.vStallCR);
    }
    else if(state.fp == BADA::Approach)
    {
        vMin = ATCConst::C_V_MIN * ATCMath::kt2mps(temp.vStallAP);
    }
    else
    {
        vMin = ATCConst::C_V_MIN * ATCMath::kt2mps(temp.vStallLD);
    }
    if(Vnom < vMin) Vnom = vMin;
    state.am = ATCMath::assignAM(state.v, Vnom);

    flight->setState(state);
//    qDebug() << QString::number(ATCMath::rad2deg(state.x), 'g', 10) << "\t" << QString::number(ATCMath::rad2deg(state.y), 'g', 10) << "\t" << ATCMath::m2ft(state.h) << "\t" << ATCMath::m2ft(targetAltitude) << "\t" << ATCMath::mps2kt(state.v) << "\t" << ATCMath::mps2kt(Vnom) << "\t" << ATCMath::rad2deg(state.hdg);
//    qDebug() << state.cm << state.fp << state.rpm << state.trm << state.shm << state.am;
}

void ATCSimulation::assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo, int flightIndex)
{
    State state = flight->getState();
    Temp temp = flight->getTemp();
    ATCAircraftType *type = flight->getFlightPlan()->getType();

    double m = temp.m;

    double bankAngle;
    if(flight->getNavMode() == ATC::Nav)
    {
        int waypointIndex = flight->getWaypointIndex();

        double xtrackError;
        double headingError;
        double dstToNext;

        if(flight->isDCT())
        {
            double fix1lat = ATCMath::rad2deg(state.y);
            double fix1lon = ATCMath::rad2deg(state.x);

            QPair<double, double> waypointDCT = flight->getWaypoint(waypointIndex);
            ATCMath::projectAcftPosOnPath(geo, fix1lat, fix1lon, waypointDCT.first, waypointDCT.second, state.y, state.x, state.hdg, xtrackError, headingError, dstToNext);
        }
        else
        {
            if(flight->getWaypointIndex() >= 1)
            {
                if(!flight->isFinalApp())
                {
                    QPair<double, double> previous = flight->getWaypoint(waypointIndex - 1);
                    QPair<double, double> current = flight->getWaypoint(waypointIndex);
                    ATCMath::projectAcftPosOnPath(geo, previous.first, previous.second, current.first, current.second, state.y, state.x, state.hdg, xtrackError, headingError, dstToNext);
                }
                else
                {
                    ATCMath::projectAcftPosOnPath(geo, temp.rwyDesAppRange.first, temp.rwyDesAppRange.second, temp.rwyDesThr.first, temp.rwyDesThr.second, state.y, state.x, state.hdg, xtrackError, headingError, dstToNext);

                    double glidePathAltitude = dstToNext * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                    flight->setAppTargetAltitude(glidePathAltitude);

                    if(!flight->isGlidePath() && (state.h >= glidePathAltitude) && (state.h <= 1.05 * glidePathAltitude)) flight->setGlidePath(true);
                }
            }
            else
            {
                flight->setDCT(true);

                double fix1lat = ATCMath::rad2deg(state.y);
                double fix1lon = ATCMath::rad2deg(state.x);

                QPair<double, double> waypointDCT = flight->getWaypoint(0);
                ATCMath::projectAcftPosOnPath(geo, fix1lat, fix1lon, waypointDCT.first, waypointDCT.second, state.y, state.x, state.hdg, xtrackError, headingError, dstToNext);
            }
        }

        bool alreadyOnAppPath = false;
        double dstThrToIntersect;
        if(flight->isCldFinalApp())
        {
            double appXtrackError;
            double appHeadingError;
            double appDstToNext;
            ATCMath::projectAcftPosOnPath(geo, temp.rwyDesAppRange.first, temp.rwyDesAppRange.second, temp.rwyDesThr.first, temp.rwyDesThr.second, state.y, state.x, state.hdg, appXtrackError, appHeadingError, appDstToNext);
            ATCMath::sphericalRhumbIntersection(geo, temp.rwyDesThr.first, temp.rwyDesThr.second, temp.rwyDesAzimuth, state.y, state.x, state.hdg, dstToNext, dstThrToIntersect);

            if((qFabs(appXtrackError) < ATCConst::APP_ALLOWED_XTRACK_INTERCEPT) && (qFabs(appHeadingError) < ATCMath::deg2rad(ATCConst::APP_ALLOWED_HDG_INTERCEPT)))
            {
                alreadyOnAppPath = true;
                dstToNext = 0;
                xtrackError = appXtrackError;
                headingError = appHeadingError;
            }
        }

        bankAngle = ATCMath::bankAngle(ATCConst::k1, ATCConst::k2, xtrackError, headingError, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE));
//        qDebug() << xtrackError << ATCMath::rad2deg(headingError) << dstToNext;

        if((!flight->isFinalApp() && (waypointIndex < flight->getWaypointsVectorSize() - 1)) || alreadyOnAppPath)
        {
            double legAngleCurrent;
            double legAngleNext;
            double dHdg;

            if(!flight->isCldFinalApp())
            {
                if(flight->isDCT())
                {
                    QPointF diamond = flight->getFlightTag()->getDiamondPosition();
                    QPair<double, double> current = flight->getProjectedWaypoint(waypointIndex);
                    QPair<double, double> next = flight->getProjectedWaypoint(waypointIndex + 1);

                    legAngleCurrent = qAtan2(current.first - diamond.x(), current.second - diamond.y());
                    legAngleNext = qAtan2(next.first - current.first, next.second - current.second);
                }
                else
                {
                    QPair<double, double> previous = flight->getProjectedWaypoint(waypointIndex - 1);
                    QPair<double, double> current = flight->getProjectedWaypoint(waypointIndex);
                    QPair<double, double> next = flight->getProjectedWaypoint(waypointIndex + 1);

                    legAngleCurrent = qAtan2(current.first - previous.first, current.second - previous.second);
                    legAngleNext = qAtan2(next.first - current.first, next.second - current.second);
                }
            }

            if(!flight->isCldFinalApp())
            {
                dHdg = ATCMath::normalizeHdgChange(legAngleNext - legAngleCurrent);
            }
            else
            {
                dHdg = ATCMath::normalizeHdgChange(temp.rwyDesAzimuth - state.hdg);
            }

            double DTA = ATCMath::DTA(state.v, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE), dHdg, ATCConst::FLY_OVER_DST);

//            qDebug() << ATCMath::rad2deg(dHdg) << DTA << dstToNext - DTA;
            if(dstToNext - DTA <= 0)
            {
                double wpt = waypointIndex;

                if(!flight->isCldFinalApp())
                {
                    wpt = waypointIndex + 1;
                }
                else if(flight->isCldFinalApp() && ((dstThrToIntersect <= ATCConst::APP_RANGE) || alreadyOnAppPath))
                {
                    if(flight->getFlightPlan()->getRoute().getAlternate().isEmpty())
                    {
                        wpt = flight->getWaypointsVectorSize() - 1;
                    }
                    else
                    {
                        wpt = flight->getWaypointsVectorSize() - 2;
                    }

                    flight->setCldFinalApp(false);
                    flight->setFinalApp(true);
                }

                flight->setWaypointIndex(wpt);
                flight->setNextFix(flight->getFixList().at(wpt));
                flight->setDCT(false);

                if(flight->getRoutePrediction() != nullptr)
                {
                    emit signalDisplayRoute(flight);
                    emit signalDisplayRoute(flight);
                }
            }
        }
    }
    else
    {
        double targetHdg = flight->getHdgRestriction();
        double headingError = ATCMath::normalizeHdgChange(state.hdg - ATCMath::deg2rad(targetHdg + ATCConst::AVG_DECLINATION));

        if(flight->isCldFinalApp())
        {
            bool alreadyOnAppPath = false;
            double dstThrToIntersect;
            double dstToNext;

            double appXtrackError;
            double appHeadingError;
            double appDstToNext;
            ATCMath::projectAcftPosOnPath(geo, temp.rwyDesAppRange.first, temp.rwyDesAppRange.second, temp.rwyDesThr.first, temp.rwyDesThr.second, state.y, state.x, state.hdg, appXtrackError, appHeadingError, appDstToNext);
            ATCMath::sphericalRhumbIntersection(geo, temp.rwyDesThr.first, temp.rwyDesThr.second, temp.rwyDesAzimuth, state.y, state.x, state.hdg, dstToNext, dstThrToIntersect);

            if((qFabs(appXtrackError) < ATCConst::APP_ALLOWED_XTRACK_INTERCEPT) && (qFabs(appHeadingError) < ATCMath::deg2rad(ATCConst::APP_ALLOWED_HDG_INTERCEPT)))
            {
                alreadyOnAppPath = true;
            }

            double dHdg = ATCMath::normalizeHdgChange(temp.rwyDesAzimuth - state.hdg);
            double DTA = ATCMath::DTA(state.v, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE), dHdg, ATCConst::FLY_OVER_DST);

            if(((dstToNext - DTA <= 0) && (dstThrToIntersect <= ATCConst::APP_RANGE)) || alreadyOnAppPath)
            {
                int wpt;
                if(flight->getFlightPlan()->getRoute().getAlternate().isEmpty())
                {
                    wpt = flight->getWaypointsVectorSize() - 1;
                }
                else
                {
                    wpt = flight->getWaypointsVectorSize() - 2;
                }

                flight->setCldFinalApp(false);
                flight->setFinalApp(true);

                flight->setWaypointIndex(wpt);
                flight->setNextFix(flight->getFixList().at(wpt));
                flight->setDCT(false);
                flight->setNavMode(ATC::Nav);

                if(flight->getRoutePrediction() != nullptr)
                {
                    emit signalDisplayRoute(flight);
                    emit signalDisplayRoute(flight);
                }
            }
        }

        bankAngle = ATCMath::bankAngle(0, ATCConst::k2, 0, headingError, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE));
    }

    double CL = ATCMath::liftCoefficient(m, isa.rho, state.v, type->getSurface(), bankAngle);
    double lift = ATCMath::lift(isa.rho, state.v, type->getSurface(), CL);

    double CD;

    if((state.fp == BADA::UpperDescent) || state.fp == (BADA::LowerDescent))
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroCR().CD0, type->getAeroCR().CD2, 0);
    }
    else if(state.fp == BADA::Approach)
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroAP().CD0, type->getAeroAP().CD2, 0);
    }
    else
    {
        CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, type->getCDldg());
    }

    double drag = ATCMath::drag(isa.rho, state.v, type->getSurface(), CD);

    double thrust;
    if(state.fp == BADA::UpperDescent)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_high, temp.Cpowred);
    }
    else if(state.fp == BADA::LowerDescent)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_low, temp.Cpowred);
    }
    else if(state.fp == BADA::Approach)
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_app, temp.Cpowred);
    }
    else
    {
        thrust = ATCMath::thrust(state.v, state.h, drag, state.cm, state.am, state.rpm, type->getAcType().engineType, type->getThrust().C_Tc1, type->getThrust().C_Tc2, type->getThrust().C_Tc3, type->getThrust().C_Tdes_ldg, temp.Cpowred);
    }

    double Mach = state.v / isa.a;
    double ESF = ATCMath::ESF(state.cm, state.am, state.shm, state.trm, Mach, isa.T, 0);

    double pathAngle;
    if(!flight->isGlidePath())
    {
        pathAngle = ATCMath::pathAngle(thrust, drag, ESF, m);
    }
    else if(flight->isGlidePath() && (state.cm == BADA::Descend))
    {
        pathAngle = ATCMath::deg2rad(-1 * ATCConst::APP_PATH_ANGLE);
    }
    else
    {
        pathAngle = 0;
    }

    double radius = ATCMath::ellipsoidRadius(state.h, state.y);

    double xNext = state.x + ATCMath::stateLonDot(state.v, state.hdg, pathAngle, state.y, radius) * ATCConst::DT;
    double yNext = state.y + ATCMath::stateLatDot(state.v, state.hdg, pathAngle, radius) * ATCConst::DT;
    double hNext = state.h + ATCMath::stateHDot(state.v, pathAngle) * ATCConst::DT;
    double vNext = state.v + ATCMath::stateVDot(thrust, drag, m, ESF) * ATCConst::DT;
    double hdgNext = ATCMath::normalizeAngle(state.hdg + ATCMath::stateHdgDot(lift, m, state.v, bankAngle) * ATCConst::DT, ATC::Rad);

    state.x = xNext;
    state.y = yNext;
    state.h = hNext;
    state.v = vNext;
    state.hdg = hdgNext;

    if((state.h <= ATCConst::APP_ALT_FLT_TERMINATED) && flight->isFinalApp())
    {
        cleanupIndices.append(flightIndex);
    }

    flight->setState(state);
}

void ATCSimulation::flightsCleanup()
{
    if(!cleanupIndices.isEmpty())
    {
        for(int i = cleanupIndices.size() - 1; i >= 0; i--)
        {
            delete flights.at(cleanupIndices.at(i));
            flights.remove(cleanupIndices.at(i));
        }

        cleanupIndices.clear();
    }
}

void ATCSimulation::incrementUpdateCounter(double &counter)
{
    counter += ATCConst::DT;
    if(counter >= ATCConst::REFRESH_INTERVAL)
    {
        counter = 0;
        emit signalUpdateTags();
    }
}
