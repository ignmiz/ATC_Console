
#include "test_atcflight.h"

void Test_ATCFlight::test_constructObject()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = BADA::Climb;
    state.fp = BADA::Approach;
    state.rpm = BADA::Off;
    state.shm = BADA::CAS;
    state.trm = BADA::High;
    state.am = BADA::Accelerate;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.newFlightPlan(ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getAssignedSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == BADA::Climb);
    QVERIFY(foo.getState().fp == BADA::Approach);
    QVERIFY(foo.getState().rpm == BADA::Off);
    QVERIFY(foo.getState().shm == BADA::CAS);
    QVERIFY(foo.getState().trm == BADA::High);
    QVERIFY(foo.getState().am == BADA::Accelerate);
}

void Test_ATCFlight::test_setFlightPlan()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = BADA::Climb;
    state.fp = BADA::Approach;
    state.rpm = BADA::Off;
    state.shm = BADA::CAS;
    state.trm = BADA::High;
    state.am = BADA::Accelerate;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.newFlightPlan(ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getAssignedSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == BADA::Climb);
    QVERIFY(foo.getState().fp == BADA::Approach);
    QVERIFY(foo.getState().rpm == BADA::Off);
    QVERIFY(foo.getState().shm == BADA::CAS);
    QVERIFY(foo.getState().trm == BADA::High);
    QVERIFY(foo.getState().am == BADA::Accelerate);

    ATCFlightPlan *newFpl = factory.newFlightPlan(ATC::IFR, "EPWA", "EPPO", "F350", QTime(12, 0, 0, 0));
    foo.setFlightPlan(newFpl);
    QVERIFY(foo.getFlightPlan() == newFpl);
}

void Test_ATCFlight::test_setSquawk()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = BADA::Climb;
    state.fp = BADA::Approach;
    state.rpm = BADA::Off;
    state.shm = BADA::CAS;
    state.trm = BADA::High;
    state.am = BADA::Accelerate;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.newFlightPlan(ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getAssignedSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == BADA::Climb);
    QVERIFY(foo.getState().fp == BADA::Approach);
    QVERIFY(foo.getState().rpm == BADA::Off);
    QVERIFY(foo.getState().shm == BADA::CAS);
    QVERIFY(foo.getState().trm == BADA::High);
    QVERIFY(foo.getState().am == BADA::Accelerate);

    QString newSq = "3456";
    foo.setSquawk(newSq);
    QVERIFY(foo.getSquawk() == newSq);
}

void Test_ATCFlight::test_setAssignedSquawk()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setAssignedSquawk("1234");
    QVERIFY(foo.getAssignedSquawk() == "1234");
}

void Test_ATCFlight::test_setState()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = BADA::Climb;
    state.fp = BADA::Approach;
    state.rpm = BADA::Off;
    state.shm = BADA::CAS;
    state.trm = BADA::High;
    state.am = BADA::Accelerate;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.newFlightPlan(ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getAssignedSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == BADA::Climb);
    QVERIFY(foo.getState().fp == BADA::Approach);
    QVERIFY(foo.getState().rpm == BADA::Off);
    QVERIFY(foo.getState().shm == BADA::CAS);
    QVERIFY(foo.getState().trm == BADA::High);
    QVERIFY(foo.getState().am == BADA::Accelerate);

    State newState;
    newState.x = 10;
    newState.y = 20;
    newState.h = 30;
    newState.v = 40;
    newState.hdg = 50;
    newState.cm = BADA::Descend;
    newState.fp = BADA::Landing;
    newState.rpm = BADA::On;
    newState.shm = BADA::Mach;
    newState.trm = BADA::Low;
    newState.am = BADA::Constant;

    foo.setState(newState);
    QVERIFY(foo.getState().x == 10);
    QVERIFY(foo.getState().y == 20);
    QVERIFY(foo.getState().h == 30);
    QVERIFY(foo.getState().v == 40);
    QVERIFY(foo.getState().hdg == 50);
    QVERIFY(foo.getState().cm == BADA::Descend);
    QVERIFY(foo.getState().fp == BADA::Landing);
    QVERIFY(foo.getState().rpm == BADA::On);
    QVERIFY(foo.getState().shm == BADA::Mach);
    QVERIFY(foo.getState().trm == BADA::Low);
    QVERIFY(foo.getState().am == BADA::Constant);
}

void Test_ATCFlight::test_setTemp()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    Temp temp;
    temp.m = 100;
    temp.Cpowred = 0.5;
    temp.vStallAP = 1;
    temp.vStallCR = 2;
    temp.vStallIC = 3;
    temp.vStallLD = 4;
    temp.vStallTO = 5;
    temp.xoverAltClbM = 50;
    temp.xoverAltCrsM = 60;
    temp.xoverAltDesM = 70;
    temp.rwyDesThr = QPair<double, double>(0.2, 0.3);
    temp.rwyDesAppRange = QPair<double, double>(0.4, 0.5);
    temp.rwyDesAzimuth = 0.75;

    foo.setTemp(temp);
    QVERIFY(foo.getTemp().m == 100);
    QVERIFY(foo.getTemp().Cpowred == 0.5);
    QVERIFY(foo.getTemp().vStallAP == 1);
    QVERIFY(foo.getTemp().vStallCR == 2);
    QVERIFY(foo.getTemp().vStallIC == 3);
    QVERIFY(foo.getTemp().vStallLD == 4);
    QVERIFY(foo.getTemp().vStallTO == 5);
    QVERIFY(foo.getTemp().xoverAltClbM == 50);
    QVERIFY(foo.getTemp().xoverAltCrsM == 60);
    QVERIFY(foo.getTemp().xoverAltDesM == 70);
    QVERIFY(foo.getTemp().rwyDesThr == (QPair<double, double>(0.2, 0.3)));
    QVERIFY(foo.getTemp().rwyDesAppRange == (QPair<double, double>(0.4, 0.5)));
    QVERIFY(foo.getTemp().rwyDesAzimuth == 0.75);
}

void Test_ATCFlight::test_setNavMode()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setNavMode(ATC::Nav);
    QVERIFY(foo.getNavMode() == ATC::Nav);
}

void Test_ATCFlight::test_setTargetAltitude()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setTargetAltitude("F350");
    QVERIFY(foo.getTargetAltitude() == "F350");
}

void Test_ATCFlight::test_setTargetSpeed()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setTargetSpeed("447");
    QVERIFY(foo.getTargetSpeed() == "447");
}

void Test_ATCFlight::test_setHdgRestriction()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setHdgRestriction(182);
    QVERIFY(foo.getHdgRestriction() == 182);
}

void Test_ATCFlight::test_setNextFix()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setNextFix("EVINA");
    QVERIFY(foo.getNextFix() == "EVINA");
}

void Test_ATCFlight::test_setSimStartTime()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setSimStartTime(QTime(0, 1, 20, 0));
    QVERIFY(foo.getSimStartTime() == QTime(0, 1, 20, 0));
}

void Test_ATCFlight::test_setTag()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    ATCFlightTag *tag = new ATCFlightTag();

    QVERIFY(foo.getFlightTag() == nullptr);

    foo.setFlightTag(tag);
    QVERIFY(foo.getFlightTag() == tag);
}

void Test_ATCFlight::test_setFixList()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QStringList list;
    list << "TEST1" << "TEST2";
    foo.setFixList(list);

    QVERIFY(foo.getFixList() == list);
}

void Test_ATCFlight::test_setMainFixList()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QStringList list;
    list << "TEST1" << "TEST2";
    foo.setMainFixList(list);

    QVERIFY(foo.getMainFixList() == list);
}

void Test_ATCFlight::test_setRoutePrediction()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    ATCRoutePrediction *prediction1 = new ATCRoutePrediction();
    foo.setRoutePrediction(prediction1);
    QVERIFY(foo.getRoutePrediction() == prediction1);

    ATCRoutePrediction *prediction2 = new ATCRoutePrediction();
    foo.setRoutePrediction(prediction2);
    QVERIFY(foo.getRoutePrediction() == prediction2);
}

void Test_ATCFlight::test_setRunwayDeparture()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QString rwy = "15";
    foo.setRunwayDeparture(rwy);
    QVERIFY(foo.getRunwayDeparture() == rwy);
}

void Test_ATCFlight::test_setRunwayDestination()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QString rwy = "15";
    foo.setRunwayDestination(rwy);
    QVERIFY(foo.getRunwayDestination() == rwy);
}

void Test_ATCFlight::test_setSID()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QString procedure = "OLILA5G";
    foo.setSID(procedure);
    QVERIFY(foo.getSID() == procedure);
}

void Test_ATCFlight::test_setSTAR()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    QString procedure = "OLILA5G";
    foo.setSTAR(procedure);
    QVERIFY(foo.getSTAR() == procedure);
}

void Test_ATCFlight::test_appendWaypoint()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.appendWaypoint(QPair<double, double>(0.5, 0.8));
    foo.appendWaypoint(QPair<double, double>(0.6, 0.9));
    QVERIFY(foo.getWaypointsVectorSize() == 2);
    QVERIFY(foo.getWaypoint(0) == (QPair<double, double>(0.5, 0.8)));
    QVERIFY(foo.getWaypoint(1) == (QPair<double, double>(0.6, 0.9)));

    QVector<QPair<double, double>> waypoints = foo.getWaypoints();
    QVERIFY(waypoints.size() == 2);
    QVERIFY(waypoints.at(0) == (QPair<double, double>(0.5, 0.8)));
    QVERIFY(waypoints.at(1) == (QPair<double, double>(0.6, 0.9)));

    foo.clearWaypoints();
    QVERIFY(foo.getWaypointsVectorSize() == 0);
}

void Test_ATCFlight::test_appendProjectedWaypoint()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.appendProjectedWaypoint(QPair<double, double>(0.5, 0.8));
    foo.appendProjectedWaypoint(QPair<double, double>(0.6, 0.9));
    QVERIFY(foo.getProjectedWaypointsVectorSize() == 2);
    QVERIFY(foo.getProjectedWaypoint(0) == (QPair<double, double>(0.5, 0.8)));
    QVERIFY(foo.getProjectedWaypoint(1) == (QPair<double, double>(0.6, 0.9)));

    QVector<QPair<double, double>> waypoints = foo.getProjectedWaypoints();
    QVERIFY(waypoints.size() == 2);
    QVERIFY(waypoints.at(0) == (QPair<double, double>(0.5, 0.8)));
    QVERIFY(waypoints.at(1) == (QPair<double, double>(0.6, 0.9)));

    foo.clearWaypoints();
    QVERIFY(foo.getProjectedWaypointsVectorSize() == 0);
}

void Test_ATCFlight::test_setWaypointIndex()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);

    foo.setWaypointIndex(1);
    QVERIFY(foo.getWaypointIndex() == 1);
}

void Test_ATCFlight::test_setDCT()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.isDCT() == false);

    foo.setDCT(true);
    QVERIFY(foo.isDCT() == true);
}

void Test_ATCFlight::test_setCldFinalApp()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.isCldFinalApp() == false);

    foo.setCldFinalApp(true);
    QVERIFY(foo.isCldFinalApp() == true);
}

void Test_ATCFlight::test_setFinalApp()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.isFinalApp() == false);

    foo.setFinalApp(true);
    QVERIFY(foo.isFinalApp() == true);
}

void Test_ATCFlight::test_setGlidePath()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.isGlidePath() == false);

    foo.setGlidePath(true);
    QVERIFY(foo.isGlidePath() == true);
}

void Test_ATCFlight::test_setAppTargetAltitude()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    foo.setAppTargetAltitude(1234.56);
    QVERIFY(foo.getAppTargetAltitude() == 1234.56);
}

void Test_ATCFlight::test_setSimulated()
{
    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.isSimulated() == true);

    foo.setSimulated(false);
    QVERIFY(foo.isSimulated() == false);
}

void Test_ATCFlight::test_setTrailingDots()
{
    ATCPaths paths;
    ATCSettings settings(&paths);
    double currentScale = 1;

    State state;
    state.x = 1;

    ATCFlight foo(state);
    QVERIFY(foo.getTrailingDotsVectorSize() == 0);

    QVector<ATCTrailingDot*> vec;
    ATCTrailingDot *dot0 = new ATCTrailingDot(1, 2, &currentScale, &settings);
    ATCTrailingDot *dot1 = new ATCTrailingDot(4, 5, &currentScale, &settings);
    vec.append(dot0);
    vec.append(dot1);

    foo.setTrailingDots(vec);
    QVERIFY(foo.getTrailingDotsVectorSize() == 2);
    QVERIFY(foo.getTrailingDot(0) == dot0);
    QVERIFY(foo.getTrailingDot(1) == dot1);

    ATCTrailingDot *dot2 = new ATCTrailingDot(6, 7, &currentScale, &settings);
    foo.appendTrailingDot(dot2);
    QVERIFY(foo.getTrailingDotsVectorSize() == 3);
    QVERIFY(foo.getTrailingDot(2) == dot2);

    vec.append(dot2);
    QVERIFY(foo.getTrailingDots() == vec);

    foo.removeOldestDot();
    vec.removeFirst();
    QVERIFY(foo.getTrailingDots() == vec);
}

void Test_ATCFlight::test_setProfileClimb()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1;
    time << 0 << 1;
    distance << 0 << 2;

    ATCProfileClimb *p = new ATCProfileClimb(levels, time, distance);

    ATCFlight foo;
    QVERIFY(foo.getProfileClimb() == nullptr);

    foo.setProfileClimb(p);
    QVERIFY(foo.getProfileClimb() == p);
}

void Test_ATCFlight::test_setProfileDescent()
{
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    levels << 0 << 1;
    time << 0 << 1;
    distance << 0 << 2;

    ATCProfileDescent *p = new ATCProfileDescent(levels, time, distance);

    ATCFlight foo;
    QVERIFY(foo.getProfileDescent() == nullptr);

    foo.setProfileDescent(p);
    QVERIFY(foo.getProfileDescent() == p);
}

void Test_ATCFlight::test_appendLegDistance()
{
    ATCFlight foo;
    QVERIFY(foo.getLegDistanceVectorSize() == 0);

    foo.appendLegDistance(0.52);
    foo.appendLegDistance(0.13);
    QVERIFY(foo.getLegDistanceVectorSize() == 2);
    QVERIFY(foo.getLegDistance(0) == 0.52);
    QVERIFY(foo.getLegDistance(1) == 0.13);

    foo.clearLegDistances();
    QVERIFY(foo.getLegDistanceVectorSize() == 0);
}

void Test_ATCFlight::test_appendLegAngleChange()
{
    ATCFlight foo;
    QVERIFY(foo.getLegAngleChangeVectorSize() == 0);

    foo.appendLegAngleChange(0.52);
    foo.appendLegAngleChange(0.13);
    QVERIFY(foo.getLegAngleChangeVectorSize() == 2);
    QVERIFY(foo.getLegAngleChange(0) == 0.52);
    QVERIFY(foo.getLegAngleChange(1) == 0.13);

    foo.clearLegAngleChanges();
    QVERIFY(foo.getLegAngleChangeVectorSize() == 0);
}

void Test_ATCFlight::test_setDistanceToGo()
{
    ATCFlight foo;

    foo.setDistanceToGo(0.25);
    QVERIFY(foo.getDistanceToGo() == 0.25);
}

void Test_ATCFlight::test_setTOC()
{
    ATCFlight foo;

    foo.setTOC(0.25);
    QVERIFY(foo.getTOC() == 0.25);
}

void Test_ATCFlight::test_setTOD()
{
    ATCFlight foo;

    foo.setTOD(0.25);
    QVERIFY(foo.getTOD() == 0.25);
}

void Test_ATCFlight::test_setTopLevel()
{
    ATCFlight foo;

    foo.setTopLevel(300);
    QVERIFY(foo.getTopLevel() == 300);
}

