
#include "test_atcflightfactory.h"

void Test_ATCFlightFactory::test_newFlight()
{
    ATCPaths paths;
    ATCFlightFactory factory(paths);

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

    ATCFlight *flight = factory.newFlight(state);
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == CM_C);
    QVERIFY(flight->getState().fp == FP_AP);
    QVERIFY(flight->getState().rpm == RPM_OFF);
    QVERIFY(flight->getState().shm == SHM_C);
    QVERIFY(flight->getState().trm == TRM_H);
    QVERIFY(flight->getState().am == AM_A);

    delete flight;

    flight = factory.newFlight(state, ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0));
    QVERIFY(flight->getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(flight->getFlightPlan()->getRoute().getDestination() == "EPKK");
    QVERIFY(flight->getFlightPlan()->getAltitude() == 35000);
    QVERIFY(flight->getFlightPlan()->getDepartureTime() == QTime(12, 30, 0, 0));
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == CM_C);
    QVERIFY(flight->getState().fp == FP_AP);
    QVERIFY(flight->getState().rpm == RPM_OFF);
    QVERIFY(flight->getState().shm == SHM_C);
    QVERIFY(flight->getState().trm == TRM_H);
    QVERIFY(flight->getState().am == AM_A);

    delete flight;

    flight = factory.newFlight(state, ATC::IFR, "EPWA", "EPKK", 35000, QTime(12, 30, 0, 0), "1234");
    QVERIFY(flight->getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(flight->getFlightPlan()->getRoute().getDestination() == "EPKK");
    QVERIFY(flight->getFlightPlan()->getAltitude() == 35000);
    QVERIFY(flight->getFlightPlan()->getDepartureTime() == QTime(12, 30, 0, 0));
    QVERIFY(flight->getSquawk() == "1234");
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == CM_C);
    QVERIFY(flight->getState().fp == FP_AP);
    QVERIFY(flight->getState().rpm == RPM_OFF);
    QVERIFY(flight->getState().shm == SHM_C);
    QVERIFY(flight->getState().trm == TRM_H);
    QVERIFY(flight->getState().am == AM_A);

    delete flight;
}

void Test_ATCFlightFactory::test_generateSquawk()
{
    for(int i = 0; i < 1000; i++)
    {
        QString squawk = ATCFlightFactory::generateSquawk();

        for(int j = 0; j < 4; j++)
        {
            QVERIFY(squawk.at(j).digitValue() >= 0);
            QVERIFY(squawk.at(j).digitValue() <= 7);
        }
    }
}
