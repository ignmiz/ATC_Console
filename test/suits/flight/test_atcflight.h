#ifndef TEST_ATCFLIGHT_H
#define TEST_ATCFLIGHT_H

#include "atcflight.h"
#include "atcflightplanfactory.h"
#include "atcpaths.h"

#include <QObject>
#include <QTest>

class Test_ATCFlight : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_setFlightPlan();
    void test_setSquawk();
    void test_setAssignedSquawk();
    void test_setState();
    void test_setTemp();
    void test_setNavMode();
    void test_setTargetAltitude();
    void test_setTargetSpeed();
    void test_setHdgRestriction();
    void test_setNextFix();
    void test_setSimStartTime();
    void test_setTag();
    void test_setFixList();
    void test_setMainFixList();
    void test_setRoutePrediction();
    void test_setRunwayDeparture();
    void test_setRunwayDestination();
    void test_setSID();
    void test_setSTAR();
    void test_appendWaypoint();
    void test_appendProjectedWaypoint();
    void test_setWaypointIndex();
    void test_setDCT();
    void test_setCldFinalApp();
    void test_setFinalApp();
    void test_setGlidePath();
    void test_setAppTargetAltitude();
    void test_setSimulated();
    void test_setTrailingDots();
    void test_setProfileClimb();
    void test_setProfileDescent();
    void test_setProfileApproach();
    void test_setProfileSpeed();
    void test_appendLegDistance();
    void test_appendLegAngleChange();
    void test_setAccuratePredictionFlag();
    void test_setDistanceToGo();
    void test_setDistanceToNext();
    void test_setPredictionPhase();
    void test_setTOC();
    void test_setTOCposition();
    void test_setTOClevel();
    void test_setTOCtime();
    void test_setTOD();
    void test_setTODposition();
    void test_setTODlevel();
    void test_setTODtime();
    void test_setWaypointLevels();
    void test_setWaypointTimes();
    void test_setRTA();
    void test_setMeteringFixIndex();
    void test_setRTAmod();
};

#endif // TEST_ATCFLIGHT_H
