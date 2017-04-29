
#include "atcsimulation.h"

ATCSimulation::ATCSimulation() : activeRunways(new ATCActiveRunways()), timeLog(0)
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

int ATCSimulation::getActiveCount()
{
    return activeCount;
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

void ATCSimulation::setTimeHandle(QTime *t)
{
    timeHandle = t;
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

void ATCSimulation::setMeteringFix(QString &fix)
{
    meteringFix = fix;
}

QString &ATCSimulation::getMeteringFix()
{
    return meteringFix;
}

void ATCSimulation::slotStartSimulation()
{
    GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();
    qint64 dt = ATCConst::DT * 1e9;

    QElapsedTimer timer;

    qint64 elapsedTime;
    qint64 diff;

    preallocateTempData();

    setActiveFlightsCount();
    calculatePredictorInterval();
    calculatePredictorCycles();

    if(dataLogged) createDataLogs();

    emit signalUpdateFlightList();
    if(!paused) emit signalSetSimulationStartTime();

    setPaused(false);

    simLoop = true;

    globalTimer.start();
    while(simLoop)
    {
        timer.start();

        progressState(geo);
        flightsCleanup();

        incrementSweepCounter();
        incrementPredictorCounter();

        elapsedTime = timer.nsecsElapsed();
        diff = dt - elapsedTime;

//        qDebug() << "Elapsed: " << elapsedTime << "ns\t|\tDiff: " << diff << "ns\t|\tError: " << dt - (elapsedTime + qFloor(diff/1000) * 1000) << "ns";
        QThread::usleep(qFloor(diff / 1000));
    }

    if(dataLogged) closeDataLogs();
}

void ATCSimulation::slotStopSimulation()
{
    simLoop = false;
}

void ATCSimulation::setActiveFlightsCount()
{
    activeCount = 0;

    for(int i = 0; i < flights.size(); i++)
    {
        if(flights.at(i)->isSimulated())
        {
            activeCount++;
        }
    }
}

void ATCSimulation::calculatePredictorInterval()
{
    predictorInterval = qFloor((ATCConst::TOTAL_PREDICTION_INTERVAL / activeCount) / ATCConst::DT) * ATCConst::DT;
}

void ATCSimulation::calculatePredictorCycles()
{
    int maxPredictorVolume = qFloor(ATCConst::TOTAL_PREDICTION_INTERVAL / ATCConst::DT);
    predictorCycles = qFloor(activeCount / maxPredictorVolume) + 1;
}

void ATCSimulation::createDataLogs()
{
    for(int i = 0; i < flights.size(); i++)
    {
        QString fileName = flights.at(i)->getFlightPlan()->getCompany()->getCode() + flights.at(i)->getFlightPlan()->getFlightNumber();
        flights.at(i)->setDataLog(dataLoggedPath + "/" + fileName + ".txt");
    }
}

void ATCSimulation::closeDataLogs()
{
    for(int i = 0; i < flights.size(); i++)
    {
        flights.at(i)->closeDataLog();
    }

    dataLogged = false;
}

void ATCSimulation::preallocateTempData()
{
    for(int i = 0; i < flights.size(); i++)
    {
        ATCFlight *flight = flights.at(i);

        if(!flight->hasTemp())
        {
            preallocateTempData(flight);
            createClimbProfile(flight);
            createDescentProfile(flight);
            createApproachProfile(flight);
            createSpeedProfile(flight);

            flight->setTempFlag(true);
        }
    }
}

void ATCSimulation::preallocateTempData(ATCFlight *flight)
{
    //Assign temporaries
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
            double x;
            double y;

            if(flight->getRunwayDestination().left(2).toInt() <= 18)
            {
                thrLat = runway->getStartPoint().latitude();
                thrLon = runway->getStartPoint().longitude();

                azimuth = runway->getAzimuth();

                x = runway->getExtendedCentreline1()->getCentreline()->line().x1();
                y = runway->getExtendedCentreline1()->getCentreline()->line().y1();
            }
            else
            {
                thrLat = runway->getEndPoint().latitude();
                thrLon = runway->getEndPoint().longitude();

                azimuth = ATCMath::normalizeAngle(runway->getAzimuth() + ATCConst::PI, ATC::Deg);

                x = runway->getExtendedCentreline2()->getCentreline()->line().x1();
                y = runway->getExtendedCentreline2()->getCentreline()->line().y1();
            }

            temp.rwyDesThr = QPair<double, double>(thrLat, thrLon);
            temp.rwyDesThrProjected = QPair<double, double>(x, y);
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

    //Assign leg distances and angle changes
    bool alternateExists = (!flight->getFlightPlan()->getRoute().getAlternate().isEmpty()) ? true : false;
    int iterationLimit = alternateExists ? flight->getWaypointsVectorSize() - 2 : flight->getWaypointsVectorSize() - 1;

    double previousForwardAzimuth;

    for(int i = 0; i < iterationLimit; i++)
    {
        GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();

        bool substituteRwyThr = ((i == iterationLimit - 1) && !flight->getRunwayDestination().isEmpty()) ? true : false;

        QPair<double, double> fix1 = flight->getWaypoint(i);
        QPair<double, double> fix2 = substituteRwyThr ? flight->getTemp().rwyDesThr : flight->getWaypoint(i + 1);

        double distance;
        double azimuth1to2;
        double azimuth2to1;
        geo.Inverse(fix1.first, fix1.second, fix2.first, fix2.second, distance, azimuth1to2, azimuth2to1);

        if(i != 0)
        {
            double hdgChange = ATCMath::normalizeHdgChange(ATCMath::deg2rad(azimuth1to2 - previousForwardAzimuth));
            flight->appendLegAngleChange(hdgChange);
        }

        previousForwardAzimuth = azimuth2to1;
        flight->appendLegDistance(distance);
    }

    //Assign route distance
    double routeDst = 0;
    for(int i = 0; i < flight->getLegDistanceVectorSize(); i++)
    {
        routeDst = routeDst + flight->getLegDistance(i);
    }

    temp.routeDistance = routeDst;
    flight->setTemp(temp);

    //Exclude flights that will happen in future
    if(flight->getSimStartTime() != QTime(0, 0, 0))
    {
        emit signalHideFlightTag(flight->getFlightTag());
        if(flight->getRoutePrediction() != nullptr) emit signalClearRoute(flight);
        flight->setSimulated(false);
    }
}

void ATCSimulation::createClimbProfile(ATCFlight *flight)
{
    //Create mockup buffer
    QString unusedBuffer;

    //Create dummy flight
    ATCFlight _flight;
    _flight.setFlightPlan(new ATCFlightPlan());

    ATCAircraftType *type = flight->getFlightPlan()->getType();
    _flight.getFlightPlan()->setType(type);

    //Copy temporaries
    Temp temp;

    temp.m = flight->getTemp().m;
    temp.Cpowred = flight->getTemp().Cpowred;

    temp.vStallCR = flight->getTemp().vStallCR;
    temp.vStallIC = flight->getTemp().vStallIC;
    temp.vStallTO = flight->getTemp().vStallTO;
    temp.vStallAP = flight->getTemp().vStallAP;
    temp.vStallLD = flight->getTemp().vStallLD;

    temp.xoverAltClbM = flight->getTemp().xoverAltClbM;
    temp.xoverAltCrsM = flight->getTemp().xoverAltCrsM;
    temp.xoverAltDesM = flight->getTemp().xoverAltDesM;

    _flight.setTemp(temp);

    //Initialize dummy state
    State state;
    state.x = 0;
    state.y = 0;
    state.h = 0;
    state.v = ATCConst::C_V_MIN_TO * ATCMath::kt2mps(flight->getTemp().vStallTO);
    state.hdg = ATCMath::deg2rad(90);

    _flight.setState(state);

    //Initialize target altitude
    double targetAlt = type->getEnvelope().h_max;
    _flight.setTargetAltitude("F" + QString::number(qRound(targetAlt / 100)));

    targetAlt = ATCMath::ft2m(targetAlt);

    //Declare vectors for values
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    //Initialize profile simulation parameters
    double t = 0;
    bool maxAlt = false;
    double altitudeCheckpoint = 0;

    //Perform simplified integration of vertical component
    while(!maxAlt && (_flight.getState().h <= targetAlt))
    {
        //Append data points
        if(_flight.getState().h >= altitudeCheckpoint)
        {
            levels.append(_flight.getState().h);
            time.append(t);
            distance.append(_flight.getState().x);

            altitudeCheckpoint += ATCConst::PROFILE_ALT_INTERVAL;
        }

        //Calculate profile
        ISA isa = calculateEnvironment(&_flight, unusedBuffer);
        assignDiscreteState(&_flight, isa, unusedBuffer);
        assignVerticalProfile(&_flight, isa, maxAlt);

        //Increment time
        t += ATCConst::DT_COARSE;
    }

    //Append last data points
    levels.append(_flight.getState().h);
    time.append(t);
    distance.append(_flight.getState().x);

    //Create ATCProfile, ATCFlight takes ownership
    ATCProfileClimb *profile = new ATCProfileClimb(levels, time, distance);
    flight->setProfileClimb(profile);
}

void ATCSimulation::createDescentProfile(ATCFlight *flight)
{
    //Create mockup buffer
    QString unusedBuffer;

    //Create dummy flight
    ATCFlight _flight;
    _flight.setFlightPlan(new ATCFlightPlan());

    ATCAircraftType *type = flight->getFlightPlan()->getType();
    _flight.getFlightPlan()->setType(type);

    //Copy temporaries
    Temp temp;

    temp.m = flight->getTemp().m;
    temp.Cpowred = flight->getTemp().Cpowred;

    temp.vStallCR = flight->getTemp().vStallCR;
    temp.vStallIC = flight->getTemp().vStallIC;
    temp.vStallTO = flight->getTemp().vStallTO;
    temp.vStallAP = flight->getTemp().vStallAP;
    temp.vStallLD = flight->getTemp().vStallLD;

    temp.xoverAltClbM = flight->getTemp().xoverAltClbM;
    temp.xoverAltCrsM = flight->getTemp().xoverAltCrsM;
    temp.xoverAltDesM = flight->getTemp().xoverAltDesM;

    _flight.setTemp(temp);

    //Calculate TAS at theoretical ceiling
    ISA isa = ATCMath::atmosISA(ATCMath::ft2m(type->getEnvelope().h_max));

    double mach = type->getVelocity().M_CR_AV;
    double cas = type->getVelocity().V_CR2_AV;
    double tas;

    if(ATCMath::ft2m(type->getEnvelope().h_max) >= temp.xoverAltCrsM)
    {
        tas = qFabs(ATCMath::mach2tas(mach, isa.a));
    }
    else
    {
        tas = qFabs(ATCMath::cas2tas(ATCMath::kt2mps(cas), isa.p, isa.rho));
    }

    //Initialize dummy state
    State state;
    state.x = 0;
    state.y = 0;
    state.h = ATCMath::ft2m(type->getEnvelope().h_max);
    state.v = tas;
    state.hdg = ATCMath::deg2rad(90);

    _flight.setState(state);

    //Initialize target altitude
    double targetAlt = 0;
    _flight.setTargetAltitude("F000");

    targetAlt = ATCMath::ft2m(targetAlt);

    //Declare temp vectors for values
    QVector<double> t_levels;
    QVector<double> t_time;
    QVector<double> t_distance;

    //Initialize profile simulation parameters
    double t = 0;
    bool minAlt = false; //This most likely is irrelevant, since there is sufficient thrust at low levels. Left for unification.
    double altitudeCheckpoint = state.h;

    //Perform simplified integration of vertical component
    while(!minAlt && (_flight.getState().h >= targetAlt))
    {
        //Append data points
        if(_flight.getState().h <= altitudeCheckpoint)
        {
            t_levels.append(_flight.getState().h);
            t_time.append(t);
            t_distance.append(_flight.getState().x);

            altitudeCheckpoint -= ATCConst::PROFILE_ALT_INTERVAL;
        }

        //Calculate profile
        ISA isa = calculateEnvironment(&_flight, unusedBuffer);
        assignDiscreteState(&_flight, isa, unusedBuffer);
        assignVerticalProfile(&_flight, isa, minAlt);

        //Increment time
        t += ATCConst::DT_COARSE;
    }

    //Append last data points
    t_levels.append(_flight.getState().h);
    t_time.append(t);
    t_distance.append(_flight.getState().x);

    //Declare final vectors for valuse
    QVector<double> levels(t_levels.size());
    QVector<double> time(t_time.size());
    QVector<double> distance(t_distance.size());

    //Reverse order of data to comply with ATCInterpolator restrictions
    for(int j = 0; j < levels.size(); j++)
    {
        levels[j] = t_levels.at(t_levels.size() - 1 - j);
        time[j] = t_time.at(time.size() - 1 - j);
        distance[j] = t_distance.at(distance.size() - 1 - j);
    }

    //Create ATCProfile, ATCFlight takes ownership
    ATCProfileDescent *profile = new ATCProfileDescent(levels, time, distance);
    flight->setProfileDescent(profile);
}

void ATCSimulation::createApproachProfile(ATCFlight *flight)
{
    //Create mockup buffer
    QString unusedBuffer;

    //Create dummy flight
    ATCFlight _flight;
    _flight.setFlightPlan(new ATCFlightPlan());

    ATCAircraftType *type = flight->getFlightPlan()->getType();
    _flight.getFlightPlan()->setType(type);

    //Copy temporaries
    Temp temp;

    temp.m = flight->getTemp().m;
    temp.Cpowred = flight->getTemp().Cpowred;

    temp.vStallCR = flight->getTemp().vStallCR;
    temp.vStallIC = flight->getTemp().vStallIC;
    temp.vStallTO = flight->getTemp().vStallTO;
    temp.vStallAP = flight->getTemp().vStallAP;
    temp.vStallLD = flight->getTemp().vStallLD;

    temp.xoverAltClbM = flight->getTemp().xoverAltClbM;
    temp.xoverAltCrsM = flight->getTemp().xoverAltCrsM;
    temp.xoverAltDesM = flight->getTemp().xoverAltDesM;

    _flight.setTemp(temp);

    //Calculate TAS at theoretical ceiling
    ISA isa = ATCMath::atmosISA(ATCMath::ft2m(type->getEnvelope().h_max));

    double mach = type->getVelocity().M_CR_AV;
    double cas = type->getVelocity().V_CR2_AV;
    double tas;

    if(ATCMath::ft2m(type->getEnvelope().h_max) >= temp.xoverAltCrsM)
    {
        tas = qFabs(ATCMath::mach2tas(mach, isa.a));
    }
    else
    {
        tas = qFabs(ATCMath::cas2tas(ATCMath::kt2mps(cas), isa.p, isa.rho));
    }

    //Initialize dummy state
    State state;
    state.x = 0;
    state.y = 0;
    state.h = ATCConst::APP_RANGE * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
    state.v = tas;
    state.hdg = ATCMath::deg2rad(90);

    _flight.setState(state);

    //Initialize target altitude
    double targetAlt = 0;
    _flight.setTargetAltitude("F000");

    targetAlt = ATCMath::ft2m(targetAlt);

    //Declare temp vectors for values
    QVector<double> t_levels;
    QVector<double> t_time;

    //Initialize profile simulation parameters
    double t = 0;
    bool minAlt = false; //This most likely is irrelevant, since there is sufficient thrust at low levels. Left for unification.
    double altitudeCheckpoint = state.h;

    //Perform simplified integration of vertical component
    while(!minAlt && (_flight.getState().h >= targetAlt))
    {
        //Append data points
        if(_flight.getState().h <= altitudeCheckpoint)
        {
            t_levels.append(_flight.getState().h);
            t_time.append(t);

            altitudeCheckpoint -= ATCConst::PROFILE_ALT_INTERVAL;
        }

        //Calculate profile
        ISA isa = calculateEnvironment(&_flight, unusedBuffer);
        assignDiscreteState(&_flight, isa, unusedBuffer);
        assignApproachProfile(&_flight, isa);

        //Increment time
        t += ATCConst::DT_COARSE;
    }

    //Append last data points
    t_levels.append(_flight.getState().h);
    t_time.append(t);

    //Declare final vectors for valuse
    QVector<double> levels(t_levels.size());
    QVector<double> time(t_time.size());

    //Reverse order of data to comply with ATCInterpolator restrictions
    for(int j = 0; j < levels.size(); j++)
    {
        levels[j] = t_levels.at(t_levels.size() - 1 - j);
        time[j] = t_time.at(time.size() - 1 - j);
    }

    //Create ATCProfile, ATCFlight takes ownership
    ATCProfileApproach *profile = new ATCProfileApproach(levels, time);
    flight->setProfileApproach(profile);
}

void ATCSimulation::createSpeedProfile(ATCFlight *flight)
{
    Temp temp = flight->getTemp();
    ATCAircraftType *type = flight->getFlightPlan()->getType();

    double ceiling = ATCMath::ft2m(type->getEnvelope().h_max);
    double interval = ATCConst::PROFILE_ALT_INTERVAL;
    double limit = ceiling + interval;

    QVector<double> levels;
    QVector<double> clbSpds;
    QVector<double> crsSpds;
    QVector<double> dscSpds;

    double currentLvl = 0;

    while(currentLvl <= limit)
    {
        ISA isa = ATCMath::atmosISA(currentLvl);

        BADA::SpeedHoldMode shmCL = (currentLvl < temp.xoverAltClbM) ? BADA::CAS : BADA::Mach;
        BADA::SpeedHoldMode shmCR = (currentLvl < temp.xoverAltCrsM) ? BADA::CAS : BADA::Mach;
        BADA::SpeedHoldMode shmDS = (currentLvl < temp.xoverAltDesM) ? BADA::CAS : BADA::Mach;

        levels.append(currentLvl);
        clbSpds.append(ATCMath::nominalSpeedCL(currentLvl, shmCL, type->getAcType().engineType, temp.vStallTO, type->getVelocity().V_CL1_AV, type->getVelocity().V_CL2_AV, type->getVelocity().M_CL_AV, isa.a, isa.rho, isa.p));
        crsSpds.append(ATCMath::nominalSpeedCR(currentLvl, shmCR, type->getAcType().engineType, type->getVelocity().V_CR1_AV, type->getVelocity().V_CR2_AV, type->getVelocity().M_CR_AV, isa.a, isa.rho, isa.p));
        dscSpds.append(ATCMath::nominalSpeedDS(currentLvl, shmDS, type->getAcType().engineType, temp.vStallTO, type->getVelocity().V_DS1_AV, type->getVelocity().V_DS2_AV, type->getVelocity().M_DS_AV, isa.a, isa.rho, isa.p));

        currentLvl += interval;
    }

    flight->setProfileSpeed(new ATCProfileSpeed(levels, clbSpds, crsSpds, dscSpds));
}

void ATCSimulation::progressState(GeographicLib::Geodesic &geo)
{
    bool updateList = false;

    for(int i = 0; i < flights.size(); i++)
    {
        ATCFlight *flight = flights.at(i);
        QString buffer;

        if(flight->isSimulated())
        {
            if(dataLogged) appendToLogBuffer(buffer, QString::number(timeLog, 'f', 2).rightJustified(8, '0'));

            ISA isa = calculateEnvironment(flight, buffer);
            assignDiscreteState(flight, isa, buffer);
            assignContinuousState(flight, isa, geo, i, buffer);
        }
        else
        {
            if(globalTimer.hasExpired(static_cast<qint64>(QTime(0, 0, 0).msecsTo(flight->getSimStartTime()))))
            {
                if(!flight->getFlightTag()->getDiamond()->isVisible()) emit signalShowFlightTag(flight->getFlightTag());
                flight->setSimulated(true);
                flight->setSimStartTime(QTime(0, 0, 0));

                updateList = true;

                activeCount++;
                calculatePredictorInterval();
                calculatePredictorCycles();
            }
        }

        if(updateList) emit signalUpdateFlightList();

        if(dataLogged)
        {
            flight->logData(buffer);
            buffer.clear();
        }
    }

    timeLog += ATCConst::DT;
}

ISA ATCSimulation::calculateEnvironment(ATCFlight *flight, QString &buffer)
{
    ISA isa = ATCMath::atmosISA(flight->getState().h);

    if(dataLogged)
    {
        appendToLogBuffer(buffer, QString::number(isa.a, 'f', 2).rightJustified(6, '0'));
        appendToLogBuffer(buffer, QString::number(isa.rho, 'f', 4).rightJustified(6, '0'));
        appendToLogBuffer(buffer, QString::number(isa.p, 'f', 1).rightJustified(8, '0'));
        appendToLogBuffer(buffer, QString::number(isa.T, 'f', 2).rightJustified(6, '0'));
    }

    return isa;
}

void ATCSimulation::assignDiscreteState(ATCFlight *flight, ISA &isa, QString &buffer)
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

    //Assign prediction phase
    double CFL = ATCMath::ft2m(flight->getTargetAltitude().right(3).toDouble() * 100);
    double RFL = ATCMath::ft2m(flight->getFlightPlan()->getAltitude().right(3).toDouble() * 100);

    flight->setPredictionPhase(ATCMath::assignPredictionPhase(CFL, RFL, state.cm, flight->getPredictionPhase()));

    //Log data to buffer
    if(dataLogged)
    {
        appendToLogBuffer(buffer, QString::number(state.cm));
        appendToLogBuffer(buffer, QString::number(state.fp));
        appendToLogBuffer(buffer, QString::number(state.rpm));
        appendToLogBuffer(buffer, QString::number(state.shm));
        appendToLogBuffer(buffer, QString::number(state.trm));
        appendToLogBuffer(buffer, QString::number(state.am));
        appendToLogBuffer(buffer, QString::number(ATCMath::m2ft(targetAltitude)).rightJustified(9, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::mps2kt(Vnom)).rightJustified(7, '0'));
    }
}

void ATCSimulation::assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo, int flightIndex, QString &buffer)
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

        //Calculate distance to go
        double distanceToGo = dstToNext;
        if((waypointIndex < flight->getLegDistanceVectorSize()) && (waypointIndex != -1))
        {
            for(int i = waypointIndex; i < flight->getLegDistanceVectorSize(); i++)
            {
                distanceToGo = distanceToGo + flight->getLegDistance(i);
            }
        }

        flight->setDistanceToGo(distanceToGo);

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

        flight->setDistanceToNext(dstToNext);

        if(dataLogged)
        {
            appendToLogBuffer(buffer, QString::number(ATCMath::m2nm(xtrackError), 'f', 3).rightJustified(7, '0'));
            appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(headingError), 'f', 2).rightJustified(6, '0'));
        }

        bankAngle = ATCMath::bankAngle(ATCConst::k1, ATCConst::k2, xtrackError, headingError, ATCMath::deg2rad(ATCConst::NOM_BANK_ANGLE));

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
                    emit signalUpdateRoute(flight);
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
                    emit signalUpdateRoute(flight);
                }
            }
        }

        if(dataLogged)
        {
            appendToLogBuffer(buffer, QString::number(0, 'f', 3).rightJustified(7, '0'));
            appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(headingError), 'f', 2).rightJustified(6, '0'));
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
        if(state.cm == BADA::Descend)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, type->getCDldg());
        }
        else if(state.cm == BADA::Climb)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroAP().CD0, type->getAeroAP().CD2, 0);
        }
        else
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, 0);
        }
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

    if(dataLogged)
    {
        appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(state.x), 'f', 8).rightJustified(12, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(state.y), 'f', 8).rightJustified(11, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::m2ft(state.h), 'f', 2).rightJustified(9, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::mps2kt(state.v), 'f', 2).rightJustified(7, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(state.hdg), 'f', 1).rightJustified(5, '0'));

        appendToLogBuffer(buffer, QString::number(Mach, 'f', 4).rightJustified(6, '0'));

        appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(bankAngle), 'f', 2).rightJustified(5, '0'));
        appendToLogBuffer(buffer, QString::number(ATCMath::rad2deg(pathAngle), 'f', 2).rightJustified(5, '0'));
        appendToLogBuffer(buffer, QString::number(ESF, 'f', 2).rightJustified(5, '0'));

        appendToLogBuffer(buffer, QString::number(CL, 'f', 6).rightJustified(8, '0'));
        appendToLogBuffer(buffer, QString::number(CD, 'f', 6).rightJustified(8, '0'));

        appendToLogBuffer(buffer, QString::number(lift, 'f', 0).rightJustified(10 , '0'));
        appendToLogBuffer(buffer, QString::number(drag, 'f', 0).rightJustified(10, '0'));
        appendToLogBuffer(buffer, QString::number(thrust, 'f', 0).rightJustified(10, '0'));

        appendToLogBuffer(buffer, QString::number(flight->getNavMode()));

        appendToLogBuffer(buffer, flight->isCldFinalApp() ? "1" : "0");
        appendToLogBuffer(buffer, flight->isFinalApp() ? "1" : "0");
        appendToLogBuffer(buffer, flight->isGlidePath() ? "1" : "0");
    }
}

void ATCSimulation::assignVerticalProfile(ATCFlight *flight, ISA &isa, bool &maxAlt)
{
    State state = flight->getState();
    Temp temp = flight->getTemp();
    ATCAircraftType *type = flight->getFlightPlan()->getType();

    double m = temp.m;

    double bankAngle = 0;
    double CL = ATCMath::liftCoefficient(m, isa.rho, state.v, type->getSurface(), bankAngle);

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
        if(state.cm == BADA::Descend)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, type->getCDldg());
        }
        else if(state.cm == BADA::Climb)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroAP().CD0, type->getAeroAP().CD2, 0);
        }
        else
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, 0);
        }
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

    double pathAngle = ATCMath::pathAngle(thrust, drag, ESF, m);

    double xNext = state.x + state.v * qCos(pathAngle) * ATCConst::DT_COARSE;
    double hNext = state.h + ATCMath::stateHDot(state.v, pathAngle) * ATCConst::DT_COARSE;
    double vNext = state.v + ATCMath::stateVDot(thrust, drag, m, ESF) * ATCConst::DT_COARSE;

    double relativeAltitudeDiff = (qFabs(hNext - state.h) / state.h);
    if(relativeAltitudeDiff < ATCConst::PROFILE_CEILING_REL_DIFF) maxAlt = true;

    state.x = xNext;
    state.h = hNext;
    state.v = vNext;

    flight->setState(state);
}

void ATCSimulation::assignApproachProfile(ATCFlight *flight, ISA &isa)
{
    State state = flight->getState();
    Temp temp = flight->getTemp();
    ATCAircraftType *type = flight->getFlightPlan()->getType();

    double m = temp.m;

    double bankAngle = 0;
    double CL = ATCMath::liftCoefficient(m, isa.rho, state.v, type->getSurface(), bankAngle);

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
        if(state.cm == BADA::Descend)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, type->getCDldg());
        }
        else if(state.cm == BADA::Climb)
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroAP().CD0, type->getAeroAP().CD2, 0);
        }
        else
        {
            CD = ATCMath::dragCoefficient(CL, type->getAeroLD().CD0, type->getAeroLD().CD2, 0);
        }
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

    double pathAngle = ATCMath::deg2rad(-1 * ATCConst::APP_PATH_ANGLE);

    double xNext = state.x + state.v * qCos(pathAngle) * ATCConst::DT_COARSE;
    double hNext = state.h + ATCMath::stateHDot(state.v, pathAngle) * ATCConst::DT_COARSE;
    double vNext = state.v + ATCMath::stateVDot(thrust, drag, m, ESF) * ATCConst::DT_COARSE;

    state.x = xNext;
    state.h = hNext;
    state.v = vNext;

    flight->setState(state);
}

double ATCSimulation::distanceTOC(ATCFlight *flight, double AFL, double targetFL)
{
    ATCProfileClimb *profile = flight->getProfileClimb();

    return profile->distanceInterval(AFL, targetFL);
}

double ATCSimulation::distanceTOD(ATCFlight *flight, double fromLvl)
{
    //Function calculating Top of Descent as distanceToGo, at which ToD is predicted to be located.

    //Assign aircraft data
    ATCProfileDescent *profile = flight->getProfileDescent();

    //Calculate total distance from runway threshold (assumed to be at 0ft) to RFL
    //Default ILS interception assumed to be at 3000ft, with no level buffer
    double appLvl = (fromLvl < ATCConst::APP_TYPICAL_INTERCEPT_ALT) ? fromLvl : ATCConst::APP_TYPICAL_INTERCEPT_ALT;
    double appDistance = appLvl / qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));

    if(fromLvl <= ATCConst::APP_TYPICAL_INTERCEPT_ALT) return appDistance;

    double dstToIntercept = profile->distanceInterval(fromLvl, ATCConst::APP_TYPICAL_INTERCEPT_ALT);
    return appDistance + dstToIntercept;
}

QTime ATCSimulation::timeTOC(ATCFlight *flight, double AFL, double targetFL)
{
    return timeHandle->addMSecs(qRound(flight->getProfileClimb()->timeInterval(AFL, targetFL) * 1000));
}

QTime ATCSimulation::timeTOD(ATCFlight *flight, double AFL, double fromLvl, double dstToTOD)
{
    ATCProfileClimb *profileClimb = flight->getProfileClimb();

    double climbMSecs = profileClimb->timeInterval(AFL, fromLvl) * 1000;

    double cruiseSpeed = flight->getProfileSpeed()->nominalCruiseSpeed(fromLvl);
    double cruiseDistance = dstToTOD - profileClimb->distanceInterval(AFL, fromLvl);
    double cruiseMSecs = cruiseDistance / cruiseSpeed * 1000;

    return timeHandle->addMSecs(qRound(climbMSecs + cruiseMSecs));
}

void ATCSimulation::assignTOCandTOD(ATCFlight *flight)
{
    //Assign altitudes and distanceToGo;
    double AFL = flight->getState().h;
    double CFL = ATCMath::ft2m(flight->getTargetAltitude().right(3).toDouble() * 100);
    double RFL = ATCMath::ft2m(flight->getFlightPlan()->getAltitude().right(3).toDouble() * 100);
    double distanceToGo = flight->getDistanceToGo();

    PredictionPhase phase = flight->getPredictionPhase();
    BADA::ClimbMode cm = flight->getState().cm;

    //Declare ToC & ToD
    double ToC;
    double ToD;

    QTime ToCtime;
    QTime ToDtime;

    double ToClevel;
    double ToDlevel;

    //Assign ToC & ToD based on prediction phase and flight parameters
    switch(phase)
    {
        case PredictionPhase::Climb:
            //TOC prediction @ CFL and TOD prediction @ CFL or RFL, whichever is higher
            //if in level flight below RFL: TOD @ RFL
            if(cm == BADA::Climb)
            {
                ToC = distanceToGo - distanceTOC(flight, AFL, CFL);
                ToD = distanceTOD(flight, (CFL <= RFL) ? RFL : CFL);

                ToCtime = timeTOC(flight, AFL, CFL);
                ToDtime = timeTOD(flight, AFL, (CFL <= RFL) ? RFL : CFL, distanceToGo - ToD);

                ToClevel = CFL;
                ToDlevel = (CFL <= RFL) ? RFL : CFL;
            }
            else if((cm == BADA::Level) && (AFL < RFL))
            {
                ToC = 0;
                ToD = distanceTOD(flight, RFL);

                ToCtime = QTime();
                ToDtime = timeTOD(flight, AFL, RFL, distanceToGo - ToD);

                ToClevel = 0;
                ToDlevel = RFL;
            }

            break;

        case PredictionPhase::Cruise:
            //Only TOD prediction @ CFL
            ToC = 0;
            ToD = distanceTOD(flight, CFL);

            ToCtime = QTime();
            ToDtime = timeTOD(flight, CFL, CFL, distanceToGo - ToD);

            ToClevel = 0;
            ToDlevel = CFL;

            break;

        case PredictionPhase::Descent:
            //TOD prediction displayed only if descent base (as distanceToGo) is more than distanceTOD from CFL
            double descentBase = distanceToGo - flight->getProfileDescent()->distanceInterval(AFL, CFL);
            double TODfromCFL = distanceTOD(flight, CFL);

            QTime descentBaseTime = timeHandle->addMSecs(qRound(flight->getProfileDescent()->timeInterval(AFL, CFL) * 1000));
            double levelDst = (distanceToGo - TODfromCFL) - (distanceToGo - descentBase);
            double levelSpd = flight->getProfileSpeed()->nominalCruiseSpeed(CFL);

            ToC = 0;
            ToD = (descentBase > TODfromCFL) ? TODfromCFL : 0;

            ToCtime = QTime();
            ToDtime = descentBaseTime.addMSecs(qRound(levelDst / levelSpd * 1000));

            ToClevel = 0;
            ToDlevel = (descentBase > TODfromCFL) ? CFL : 0;

            break;
    }

    //Set values to flight
    flight->setTOC(ToC);
    flight->setTOD(ToD);

    flight->setTOCtime(ToCtime);
    flight->setTODtime(ToDtime);

    flight->setTOClevel(ToClevel);
    flight->setTODlevel(ToDlevel);
}

void ATCSimulation::calculateTOCposition(ATCFlight *flight)
{
    double ToC = flight->getTOC();

    //Determine route leg at which ToC exists
    double distanceToGo = flight->getDistanceToGo();
    double distanceToNext = flight->getDistanceToNext();
    double distanceCounter = distanceToGo;

    int index = -1;
    int waypointIndex = flight->getWaypointIndex();

    if(distanceCounter > ToC)
    {
        distanceCounter -= distanceToNext;

        for(int i = waypointIndex; i < flight->getWaypointsVectorSize(); i++)
        {
            if(distanceCounter > ToC)
            {
                if(i != flight->getWaypointsVectorSize() - 1) distanceCounter -= flight->getLegDistance(i);
            }
            else
            {
                index = i;
                break;
            }
        }
    }

    //Determine ToC (lat, lon) coordinates
    if(index != -1)
    {
        GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();

        QPair<double, double> WPfirstProj;
        if(index == waypointIndex)
        {
            QPointF diamondPos = flight->getFlightTag()->getDiamondPosition();
            WPfirstProj = QPair<double, double>(diamondPos.x(), diamondPos.y());
        }
        else WPfirstProj = flight->getProjectedWaypoint(index - 1);

        QPair<double, double> WPsecondProj = flight->getProjectedWaypoint(index);

        double azimuth = qAtan2(WPfirstProj.first - WPsecondProj.first, WPsecondProj.second - WPfirstProj.second);
        azimuth = ATCMath::normalizeAngle(ATCMath::rad2deg(azimuth) + ATCConst::AVG_DECLINATION, ATC::Deg);

        QPair<double, double> WPsecond = flight->getWaypoint(index);
        QPair<double, double> TOCpos;

        geo.Direct(WPsecond.first, WPsecond.second, azimuth, ToC - distanceCounter, TOCpos.first, TOCpos.second);
        flight->setTOCposition(TOCpos);
    }
}

void ATCSimulation::calculateTODposition(ATCFlight *flight)
{
    double ToD = flight->getTOD();

    //Determine route leg at which ToD exists
    double distanceToGo = flight->getDistanceToGo();
    double distanceToNext = flight->getDistanceToNext();
    double distanceCounter = distanceToGo;

    int index = -1;
    int waypointIndex = flight->getWaypointIndex();

    if(distanceCounter > ToD)
    {
        distanceCounter -= distanceToNext;

        for(int i = waypointIndex; i < flight->getWaypointsVectorSize(); i++)
        {
            if(distanceCounter > ToD)
            {
                if(i != flight->getWaypointsVectorSize() - 1) distanceCounter -= flight->getLegDistance(i);
            }
            else
            {
                index = i;
                break;
            }
        }
    }

    //Determine ToD (lat, lon) coordinates
    if(index != -1)
    {
        GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();

        QPair<double, double> WPfirstProj;
        if(index == waypointIndex)
        {
            QPointF diamondPos = flight->getFlightTag()->getDiamondPosition();
            WPfirstProj = QPair<double, double>(diamondPos.x(), diamondPos.y());
        }
        else WPfirstProj = flight->getProjectedWaypoint(index - 1);

        QPair<double, double> WPsecond;
        QPair<double, double> WPsecondProj;

        if((waypointIndex != flight->getLegDistanceVectorSize()) || flight->getRunwayDestination().isEmpty()) //Any waypoint but the destination airfield or no destination runway assigned
        {
            WPsecond = flight->getWaypoint(index);
            WPsecondProj = flight->getProjectedWaypoint(index);
        }
        else //Threshold of destination airfield chosen runway
        {
            WPsecond = flight->getTemp().rwyDesThr;
            WPsecondProj = flight->getTemp().rwyDesThrProjected;
        }

        double azimuth = qAtan2(WPfirstProj.first - WPsecondProj.first, WPsecondProj.second - WPfirstProj.second);
        azimuth = ATCMath::normalizeAngle(ATCMath::rad2deg(azimuth) + ATCConst::AVG_DECLINATION, ATC::Deg);

        QPair<double, double> TODpos;

        geo.Direct(WPsecond.first, WPsecond.second, azimuth, ToD - distanceCounter, TODpos.first, TODpos.second);
        flight->setTODposition(TODpos);
    }
}

void ATCSimulation::calculateWaypointTraits(ATCFlight *flight)
{
    State state = flight->getState();

    double AFL = state.h;
    double CFL = ATCMath::ft2m(flight->getTargetAltitude().right(3).toDouble() * 100);
    double RFL = ATCMath::ft2m(flight->getFlightPlan()->getAltitude().right(3).toDouble() * 100);

    double distanceToGo = flight->getDistanceToGo();

    PredictionPhase phase = flight->getPredictionPhase();

    ATCProfileClimb *climbProfile = flight->getProfileClimb();
    ATCProfileDescent *descentProfile = flight->getProfileDescent();
    ATCProfileApproach *approachProfile = flight->getProfileApproach();
    ATCProfileSpeed *speedProfile = flight->getProfileSpeed();

    //Declare container for fix levels
    int fixListSize = flight->getFixList().size();

    //Calculate top level
    double topLevel = (CFL <= RFL) ? RFL : CFL;

    //Calculate ToC & ToD distances and times
    double ToC = distanceToGo - distanceTOC(flight, AFL, topLevel);
    double ToD = flight->getTOD();

    QTime ToCtime = timeTOC(flight, AFL, topLevel);
    QTime ToDtime = timeTOD(flight, AFL, topLevel, distanceToGo - ToD);

    //Get current waypoint index
    int waypointIndex = flight->getWaypointIndex();

    //Initialize counters
    double distanceCounter = distanceToGo - flight->getDistanceToNext();
    int climbCounter = 0;
    int cruiseCounter = 0;
    int descentCounter = 0;

    //Count waypoints in every phase segment
    for(int i = waypointIndex; i < fixListSize; i++)
    {
        if(distanceCounter > ToC)
        {
            climbCounter++;
        }
        else if(distanceCounter >= ToD)
        {
            cruiseCounter++;
        }
        else
        {
            descentCounter++;
        }

        if(i != fixListSize - 1) distanceCounter -= flight->getLegDistance(i);
    }

    //Reset distance counter for intervals calculation
    distanceCounter = flight->getDistanceToNext();

    //Initialize range limits
    int climbLimit = waypointIndex + climbCounter;
    int cruiseLimit = climbLimit + cruiseCounter;

    //Initialize containers
    QVector<QString> labels(fixListSize, "---");
    QVector<QTime> times(fixListSize, QTime());

    //Predict waypoint levels
    switch(phase)
    {
        case PredictionPhase::Climb:
            {
                //Interpolate levels for climb
                for(int i = waypointIndex; i < climbLimit; i++)
                {
                    double level = climbProfile->mixedDistanceInterval(AFL, distanceCounter);
                    labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                    QTime time = timeHandle->addMSecs(qRound(climbProfile->timeInterval(AFL, level) * 1000));
                    times.replace(i, time);

                    if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                }

                //Assign levels for cruise
                double crsSpd = speedProfile->nominalCruiseSpeed(topLevel);

                for(int i = climbLimit; i < cruiseLimit; i++)
                {
                    labels.replace(i, QString::number(qRound(ATCMath::m2ft(topLevel) / 100), 'f', 0).rightJustified(3, '0'));

                    double crsDst = distanceCounter - (distanceToGo - ToC);
                    QTime time = ToCtime.addMSecs(qRound(crsDst / crsSpd * 1000));
                    times.replace(i, time);

                    if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                }

                //Prepare counter for descent interpolation
                distanceCounter = (ToC >= ToD) ? ToD : ToC;
                for(int i = cruiseLimit; i < flight->getLegDistanceVectorSize(); i++) distanceCounter -= flight->getLegDistance(i);

                double ilsDst = ToD - ATCConst::APP_TYPICAL_INTERCEPT_ALT / qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                QTime ilsTime = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, ATCConst::APP_TYPICAL_INTERCEPT_ALT) * 1000));

                //Interpolate levels for descent
                for(int i = cruiseLimit; i < fixListSize; i++)
                {
                    if(ToC >= ToD) //When cruising level is planned correctly
                    {
                        if(distanceCounter < ilsDst) //On descent
                        {
                            double level = descentProfile->mixedDistanceInterval(topLevel, distanceCounter);
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, level) * 1000));
                            times.replace(i, time);
                        }
                        else //On approach
                        {
                            double level = (ToD - distanceCounter) * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = ilsTime.addMSecs(qRound(approachProfile->timeInterval(ATCConst::APP_TYPICAL_INTERCEPT_ALT, level) * 1000));
                            times.replace(i, time);
                        }
                    }
                    else //When cruising level is too high to descend in time
                    {
                        double level = descentProfile->mixedDistanceInterval(topLevel, distanceCounter);
                        labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                        QTime time = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, level) * 1000));
                        times.replace(i, time);
                    }

                    if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                }
            }

            break;

        case PredictionPhase::Cruise:
            {
                if(distanceToGo >= ToD)
                {
                    //Assign levels for cruise
                    distanceCounter = flight->getDistanceToNext();
                    double crsSpd = state.v;

                    for(int i = climbLimit; i < cruiseLimit; i++)
                    {
                        labels.replace(i, QString::number(qRound(ATCMath::m2ft(topLevel) / 100), 'f', 0).rightJustified(3, '0'));

                        double crsDst = distanceCounter;
                        QTime time = timeHandle->addMSecs(qRound(crsDst / crsSpd * 1000));
                        times.replace(i, time);

                        if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                    }
                }

                //Prepare counter for descent interpolation
                distanceCounter = (ToC >= ToD) ? ToD : ToC;
                for(int i = cruiseLimit; i < flight->getLegDistanceVectorSize(); i++) distanceCounter -= flight->getLegDistance(i);

                double ilsDst = ToD - ATCConst::APP_TYPICAL_INTERCEPT_ALT / qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                QTime ilsTime = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, ATCConst::APP_TYPICAL_INTERCEPT_ALT) * 1000));

                //Interpolate levels for descent
                for(int i = cruiseLimit; i < fixListSize; i++)
                {
                    if(distanceToGo >= ToD)
                    {
                        if(distanceCounter < ilsDst) //On descent
                        {
                            double level = descentProfile->mixedDistanceInterval(topLevel, distanceCounter);
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, level) * 1000));
                            times.replace(i, time);
                        }
                        else //On approach
                        {
                            double level = (ToD - distanceCounter) * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = ilsTime.addMSecs(qRound(approachProfile->timeInterval(ATCConst::APP_TYPICAL_INTERCEPT_ALT, level) * 1000));
                            times.replace(i, time);
                        }
                    }
                    else
                    {
                        double level = descentProfile->mixedDistanceInterval(topLevel, distanceCounter);
                        labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                        QTime time = ToDtime.addMSecs(qRound(descentProfile->timeInterval(topLevel, level) * 1000));
                        times.replace(i, time);
                    }

                    if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                }
            }

            break;

        case PredictionPhase::Descent:
            {
                //Prepare counter for descent interpolation
                distanceCounter = flight->getDistanceToNext();

                bool belowStdAppAlt = (CFL < ATCConst::APP_TYPICAL_INTERCEPT_ALT) ? true : false;
                double appLvl = belowStdAppAlt ? CFL : ATCConst::APP_TYPICAL_INTERCEPT_ALT;

                double ilsDst = distanceToGo - appLvl / qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));

                //Determine descent base and ToD@CFL
                double descentBase = descentProfile->distanceInterval(AFL, CFL);
                double TODfromCFL = distanceToGo - distanceTOD(flight, CFL);

                QTime descentBaseTime = timeHandle->addMSecs(qRound(descentProfile->timeInterval(AFL, CFL) * 1000));

                double lvlSpd;
                if(state.cm == BADA::Level) lvlSpd = state.v;
                else lvlSpd = speedProfile->nominalCruiseSpeed(CFL);

                QTime TODfromCFLtime = descentBaseTime.addMSecs(qRound((TODfromCFL - descentBase) / lvlSpd * 1000));
                QTime ilsTime = TODfromCFLtime.addMSecs(qRound(descentProfile->timeInterval(CFL, appLvl) * 1000));

                //Interpolate levels for descent
                for(int i = waypointIndex; i < fixListSize; i++)
                {
                    if(ToD != 0) //Descent phase before ToD
                    {
                        if(distanceCounter < descentBase)
                        {
                            double level = descentProfile->mixedDistanceInterval(AFL, distanceCounter);
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = timeHandle->addMSecs(qRound(descentProfile->timeInterval(AFL, level) * 1000));
                            times.replace(i, time);
                        }
                        else if(distanceCounter <= TODfromCFL)
                        {
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(CFL) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = descentBaseTime.addMSecs(qRound((distanceCounter - descentBase)/ lvlSpd * 1000));
                            times.replace(i, time);
                        }
                        else if(distanceCounter < ilsDst)
                        {
                            double level = descentProfile->mixedDistanceInterval(CFL, distanceCounter - TODfromCFL);
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = TODfromCFLtime.addMSecs(qRound(descentProfile->timeInterval(CFL, level) * 1000));
                            times.replace(i, time);
                        }
                        else
                        {
                            double level = (distanceToGo - distanceCounter) * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                            labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                            QTime time = ilsTime.addMSecs(qRound(approachProfile->timeInterval(appLvl, level) * 1000));
                            times.replace(i, time);
                        }
                    }
                    else if(flight->isGlidePath()) //On ILS GP
                    {
                        double level = (distanceToGo - distanceCounter) * qTan(ATCMath::deg2rad(ATCConst::APP_PATH_ANGLE));
                        labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                        QTime time = timeHandle->addMSecs(qRound(approachProfile->timeInterval(AFL, level) * 1000));
                        times.replace(i, time);
                    }
                    else //Level flight past ToD or descent past ToD, not on approach
                    {
                        double level = descentProfile->mixedDistanceInterval(AFL, distanceCounter);
                        labels.replace(i, QString::number(qRound(ATCMath::m2ft(level) / 100), 'f', 0).rightJustified(3, '0'));

                        QTime time = timeHandle->addMSecs(qRound(descentProfile->timeInterval(AFL, level) * 1000));
                        times.replace(i, time);
                    }

                    if(i != fixListSize - 1) distanceCounter += flight->getLegDistance(i);
                }
            }

            break;
    }

    //Pass container to flight
    flight->setWaypointLevels(labels);
    flight->setWaypointTimes(times);
}

void ATCSimulation::findMeteringFixIndex(ATCFlight *flight)
{
    if(!meteringFix.isEmpty())
    {
        //Check if flight has changed the nav mode to HDG
        if(flight->getNavMode() == ATC::Hdg)
        {
            if(flight->getMeteringFixIndex() != -1)
            {
                emit signalMeteringFixLost(flight);
                flight->setMeteringFixIndex(-1);
            }

            return;
        }

        //Assign fix lists properties
        QStringList fixList = flight->getFixList();
        int fixListSize = fixList.size();

        //Search for metering fix
        bool found = false;
        for(int i = flight->getWaypointIndex(); i < fixListSize; i++)
        {
            if(fixList.at(i) == meteringFix)
            {
                flight->setMeteringFixIndex(i);
                found = true;

                emit signalMeteringFixFound(flight);
                return;
            }
        }

        //Check if flight has passed the metering fix or was rerouted
        if(!found && (flight->getMeteringFixIndex() != -1))
        {
            if(fixList.at(flight->getMeteringFixIndex()) == meteringFix) emit signalMeteringFixLost(flight);
            flight->setMeteringFixIndex(-1);
        }
    }
}

void ATCSimulation::predictTrajectories()
{
    for(int i = 0; i < predictorCycles; i++)
    {
        while(true)
        {
            if(predictorIterator < flights.size())
            {
                ATCFlight *flight = flights.at(predictorIterator);

                if(flight->isSimulated())
                {
                    //Predict trajectory
                    if(flight->getNavMode() == ATC::Nav)
                    {
                        assignTOCandTOD(flight);
                        calculateTOCposition(flight);
                        calculateTODposition(flight);

                        calculateWaypointTraits(flight);

                        flight->setAccuratePredictionFlag(true);
                        if(flight->getRoutePrediction() != nullptr) emit signalUpdateRoute(flight);
                    }

                    //Check metering fix state
                    findMeteringFixIndex(flight);

//                    qDebug() << "Iterator: " << predictorIterator << ", flight: " << flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber() << " ToC: " << ATCMath::m2nm(flight->getTOC()) << " TOClevel: " << ATCMath::m2ft(flight->getTOClevel()) << " ToD: " << ATCMath::m2nm(flight->getTOD()) << " TODlevel: " << ATCMath::m2ft(flight->getTODlevel());

                    predictorIterator++;
                    break;
                }
                else
                {
                    predictorIterator++;
                }
            }
            else
            {
                predictorIterator = 0;
            }
        }
    }
}

void ATCSimulation::flightsCleanup()
{
    if(!cleanupIndices.isEmpty())
    {
        for(int i = cleanupIndices.size() - 1; i >= 0; i--)
        {
            delete flights.at(cleanupIndices.at(i));
            flights.remove(cleanupIndices.at(i));

            activeCount--;
            if(cleanupIndices.at(i) < predictorIterator) predictorIterator--;
        }

        if(activeCount != 0)
        {
            calculatePredictorInterval();
            calculatePredictorCycles();
        }

        cleanupIndices.clear();
        emit signalUpdateFlightList();
    }
}

void ATCSimulation::incrementSweepCounter()
{
    sweepCounter += ATCConst::DT;
    if(sweepCounter >= ATCConst::SWEEP_INTERVAL)
    {
        sweepCounter = 0;
        emit signalUpdateTags();
    }
}

void ATCSimulation::incrementPredictorCounter()
{
    if(activeCount != 0)
    {
        predictorCounter += ATCConst::DT;
        if(predictorCounter >= predictorInterval)
        {
            predictorCounter = 0;
            predictTrajectories();
        }
    }
    else
    {
        predictorCounter = 0;
    }

}

void ATCSimulation::appendToLogBuffer(QString &buffer, QString data)
{
    if(buffer.isEmpty())
    {
        buffer = data;
    }
    else
    {
        buffer += "   ";
        buffer += data;
    }
}
