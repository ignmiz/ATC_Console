
#include "test_atcflight.h"

void Test_ATCFlight::test_constructObject()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = CM_C;
    state.fp = FP_AP;
    state.rpm = RPM_OFF;
    state.shm = SHM_C;
    state.trm = TRM_H;
    state.am = AM_A;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.getFlightPlan(ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == CM_C);
    QVERIFY(foo.getState().fp == FP_AP);
    QVERIFY(foo.getState().rpm == RPM_OFF);
    QVERIFY(foo.getState().shm == SHM_C);
    QVERIFY(foo.getState().trm == TRM_H);
    QVERIFY(foo.getState().am == AM_A);
}

void Test_ATCFlight::test_setFlightPlan()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = CM_C;
    state.fp = FP_AP;
    state.rpm = RPM_OFF;
    state.shm = SHM_C;
    state.trm = TRM_H;
    state.am = AM_A;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.getFlightPlan(ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == CM_C);
    QVERIFY(foo.getState().fp == FP_AP);
    QVERIFY(foo.getState().rpm == RPM_OFF);
    QVERIFY(foo.getState().shm == SHM_C);
    QVERIFY(foo.getState().trm == TRM_H);
    QVERIFY(foo.getState().am == AM_A);

    ATCFlightPlan *newFpl = factory.getFlightPlan(ATC::IFR, "EPWA", "EPPO", 35000, QTime(12, 0, 0, 0));
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
    state.cm = CM_C;
    state.fp = FP_AP;
    state.rpm = RPM_OFF;
    state.shm = SHM_C;
    state.trm = TRM_H;
    state.am = AM_A;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.getFlightPlan(ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == CM_C);
    QVERIFY(foo.getState().fp == FP_AP);
    QVERIFY(foo.getState().rpm == RPM_OFF);
    QVERIFY(foo.getState().shm == SHM_C);
    QVERIFY(foo.getState().trm == TRM_H);
    QVERIFY(foo.getState().am == AM_A);

    QString newSq = "3456";
    foo.setSquawk(newSq);
    QVERIFY(foo.getSquawk() == newSq);
}

void Test_ATCFlight::test_setState()
{
    State state;
    state.x = 1;
    state.y = 2;
    state.h = 3;
    state.v = 4;
    state.hdg = 5;
    state.cm = CM_C;
    state.fp = FP_AP;
    state.rpm = RPM_OFF;
    state.shm = SHM_C;
    state.trm = TRM_H;
    state.am = AM_A;

    QString sq = "1234";

    ATCPaths paths;
    ATCFlightPlanFactory factory(paths);
    ATCFlightPlan *fpl = factory.getFlightPlan(ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0));

    ATCFlight foo(state, fpl, sq);

    QVERIFY(foo.getFlightPlan() == fpl);
    QVERIFY(foo.getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(foo.getSquawk() == "1234");
    QVERIFY(foo.getState().x == 1);
    QVERIFY(foo.getState().y == 2);
    QVERIFY(foo.getState().h == 3);
    QVERIFY(foo.getState().v == 4);
    QVERIFY(foo.getState().hdg == 5);
    QVERIFY(foo.getState().cm == CM_C);
    QVERIFY(foo.getState().fp == FP_AP);
    QVERIFY(foo.getState().rpm == RPM_OFF);
    QVERIFY(foo.getState().shm == SHM_C);
    QVERIFY(foo.getState().trm == TRM_H);
    QVERIFY(foo.getState().am == AM_A);

    State newState;
    newState.x = 10;
    newState.y = 20;
    newState.h = 30;
    newState.v = 40;
    newState.hdg = 50;
    state.cm = CM_D;
    state.fp = FP_LA;
    state.rpm = RPM_ON;
    state.shm = SHM_M;
    state.trm = TRM_L;
    state.am = AM_C;

    foo.setState(newState);
    QVERIFY(foo.getState().x == 10);
    QVERIFY(foo.getState().y == 20);
    QVERIFY(foo.getState().h == 30);
    QVERIFY(foo.getState().v == 40);
    QVERIFY(foo.getState().hdg == 50);
    QVERIFY(foo.getState().cm == CM_D);
    QVERIFY(foo.getState().fp == FP_LA);
    QVERIFY(foo.getState().rpm == RPM_ON);
    QVERIFY(foo.getState().shm == SHM_M);
    QVERIFY(foo.getState().trm == TRM_L);
    QVERIFY(foo.getState().am == AM_C);
}

