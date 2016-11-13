
#include "test_atcsimulation.h"

void Test_ATCSimulation::test_getFlight()
{
    ATCSimulation foo;

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

    ATCFlight *flight1 = new ATCFlight(state);
    ATCFlight *flight2 = new ATCFlight(state);
    ATCFlight *flight3 = new ATCFlight(state);

    ATCCompany company1("ABC", "", "");
    ATCCompany company2("CDE", "", "");
    ATCCompany company3("SPHMD", "", "");

    QString fn1 = "123";
    QString fn2 = "456";

    ATCFlightPlan *plan1 = new ATCFlightPlan();
    ATCFlightPlan *plan2 = new ATCFlightPlan();
    ATCFlightPlan *plan3 = new ATCFlightPlan();

    plan1->setCompany(&company1);
    plan2->setCompany(&company2);
    plan3->setCompany(&company3);

    plan1->setFlightNumber(fn1);
    plan2->setFlightNumber(fn2);

    flight1->setFlightPlan(plan1);
    flight2->setFlightPlan(plan2);
    flight3->setFlightPlan(plan3);

    foo.appendFlight(flight1);
    foo.appendFlight(flight2);
    foo.appendFlight(flight3);

    QVERIFY(foo.getFlightsVectorSize() == 3);

    QVERIFY(foo.getFlight(0) == flight1);
    QVERIFY(foo.getFlight(1) == flight2);
    QVERIFY(foo.getFlight(2) == flight3);

    QVERIFY(foo.getFlight("ABC123") == flight1);
    QVERIFY(foo.getFlight("CDE456") == flight2);
    QVERIFY(foo.getFlight("SPHMD") == flight3);
}

void Test_ATCSimulation::test_removeFlight()
{
    ATCSimulation foo;

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

    ATCFlight *flight1 = new ATCFlight(state);
    ATCFlight *flight2 = new ATCFlight(state);

    ATCCompany company1("ABC", "", "");
    ATCCompany company2("CDE", "", "");

    QString fn1 = "123";
    QString fn2 = "456";

    ATCFlightPlan *plan1 = new ATCFlightPlan();
    ATCFlightPlan *plan2 = new ATCFlightPlan();

    plan1->setCompany(&company1);
    plan2->setCompany(&company2);

    plan1->setFlightNumber(fn1);
    plan2->setFlightNumber(fn2);

    flight1->setFlightPlan(plan1);
    flight2->setFlightPlan(plan2);

    foo.appendFlight(flight1);
    foo.appendFlight(flight2);

    QVERIFY(foo.getFlightsVectorSize() == 2);

    foo.removeFlight("CDG12");
    QVERIFY(foo.getFlightsVectorSize() == 2);

    foo.removeFlight("ABC123");
    QVERIFY(foo.getFlightsVectorSize() == 1);
    QVERIFY(foo.getFlight(0) == flight2);
}
