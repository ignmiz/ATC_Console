
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
    state.cm = BADA::Climb;
    state.fp = BADA::Approach;
    state.rpm = BADA::Off;
    state.shm = BADA::CAS;
    state.trm = BADA::High;
    state.am = BADA::Accelerate;

    ATCFlight *flight = factory.newFlight(state);
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == BADA::Climb);
    QVERIFY(flight->getState().fp == BADA::Approach);
    QVERIFY(flight->getState().rpm == BADA::Off);
    QVERIFY(flight->getState().shm == BADA::CAS);
    QVERIFY(flight->getState().trm == BADA::High);
    QVERIFY(flight->getState().am == BADA::Accelerate);

    delete flight;

    flight = factory.newFlight(state, ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0));
    QVERIFY(flight->getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(flight->getFlightPlan()->getRoute().getDestination() == "EPKK");
    QVERIFY(flight->getFlightPlan()->getAltitude() == "F350");
    QVERIFY(flight->getFlightPlan()->getDepartureTime() == QTime(12, 30, 0, 0));
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == BADA::Climb);
    QVERIFY(flight->getState().fp == BADA::Approach);
    QVERIFY(flight->getState().rpm == BADA::Off);
    QVERIFY(flight->getState().shm == BADA::CAS);
    QVERIFY(flight->getState().trm == BADA::High);
    QVERIFY(flight->getState().am == BADA::Accelerate);

    delete flight;

    flight = factory.newFlight(state, ATC::IFR, "EPWA", "EPKK", "F350", QTime(12, 30, 0, 0), "1234");
    QVERIFY(flight->getFlightPlan()->getRoute().getDeparture() == "EPWA");
    QVERIFY(flight->getFlightPlan()->getRoute().getDestination() == "EPKK");
    QVERIFY(flight->getFlightPlan()->getAltitude() == "F350");
    QVERIFY(flight->getFlightPlan()->getDepartureTime() == QTime(12, 30, 0, 0));
    QVERIFY(flight->getAssignedSquawk() == "1234");
    QVERIFY(flight->getState().x == 1);
    QVERIFY(flight->getState().y == 2);
    QVERIFY(flight->getState().h == 3);
    QVERIFY(flight->getState().v == 4);
    QVERIFY(flight->getState().hdg == 5);
    QVERIFY(flight->getState().cm == BADA::Climb);
    QVERIFY(flight->getState().fp == BADA::Approach);
    QVERIFY(flight->getState().rpm == BADA::Off);
    QVERIFY(flight->getState().shm == BADA::CAS);
    QVERIFY(flight->getState().trm == BADA::High);
    QVERIFY(flight->getState().am == BADA::Accelerate);

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
