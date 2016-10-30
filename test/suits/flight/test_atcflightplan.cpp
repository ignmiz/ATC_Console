
#include "test_atcflightplan.h"

Test_ATCFlightPlan::Test_ATCFlightPlan()
{
    ATCPaths paths;
    opf = paths.OPF_TEST_PATH;
    apf = paths.APF_TEST_PATH;
}

Test_ATCFlightPlan::~Test_ATCFlightPlan()
{

}

void Test_ATCFlightPlan::test_constructObject()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);

    QVERIFY(foo.getFlightRules() == ATC::IFR);
    QVERIFY(foo.getCompany() == &company);
    QVERIFY(foo.getFlightNumber() == "123X");
    QVERIFY(foo.getType() == &aircraftType);
    QVERIFY(foo.getRoute().getDeparture() == route.getDeparture());
    QVERIFY(foo.getRoute().getDestination() == route.getDestination());
    QVERIFY(foo.getRoute().getRoute() == route.getRoute());
    QVERIFY(foo.getTAS() == tas);
    QVERIFY(foo.getAltitude() == altitude);
    QVERIFY(foo.getDepartureTime() == depTime);
    QVERIFY(foo.getEnrouteTime() == enrTime);
}

void Test_ATCFlightPlan::test_setFlightRules()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getFlightRules() == ATC::IFR);

    foo.setFlightRules(ATC::VFR);
    QVERIFY(foo.getFlightRules() == ATC::VFR);

    foo.setFlightRules(ATC::SVFR);
    QVERIFY(foo.getFlightRules() == ATC::SVFR);
}

void Test_ATCFlightPlan::test_setCompany()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getCompany() == &company);

    ATCCompany newCompany("TET", "TESTING", "New Test");
    foo.setCompany(&newCompany);
    QVERIFY(foo.getCompany() == &newCompany);
}

void Test_ATCFlightPlan::test_setType()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getType() == &aircraftType);

    ATCAircraftType newType(opf, apf);
    foo.setType(&newType);
    QVERIFY(foo.getType() == &newType);
}

void Test_ATCFlightPlan::test_setRoute()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getRoute().getDeparture() == route.getDeparture());
    QVERIFY(foo.getRoute().getDestination() == route.getDestination());
    QVERIFY(foo.getRoute().getRoute() == route.getRoute());

    QStringList newList;
    newList << "EVINA" << "M66" << "KUKAM";
    ATCRoute newRoute("EPMO", newList, "EPKK");
    foo.setRoute(newRoute);
    QVERIFY(foo.getRoute().getDeparture() == newRoute.getDeparture());
    QVERIFY(foo.getRoute().getDestination() == newRoute.getDestination());
    QVERIFY(foo.getRoute().getRoute() == newRoute.getRoute());
}

void Test_ATCFlightPlan::test_setTAS()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getTAS() == tas);

    int newTAS = 425;
    foo.setTAS(newTAS);
    QVERIFY(foo.getTAS() == newTAS);
}

void Test_ATCFlightPlan::test_setAltitude()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getAltitude() == altitude);

    altitude = 33000;
    foo.setAltitude(altitude);
    QVERIFY(foo.getAltitude() == altitude);
}

void Test_ATCFlightPlan::test_setDepartureTime()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getDepartureTime() == depTime);

    depTime = QTime(11, 15, 0, 0);
    foo.setDepartureTime(depTime);
    QVERIFY(foo.getDepartureTime() == depTime);
}

void Test_ATCFlightPlan::test_setEnrouteTime()
{
    ATCCompany company("TST", "TEST", "Test Company");
    QString flightNo("123X");
    ATCAircraftType aircraftType(opf, apf);

    QStringList list;
    list << "EVINA" << "M985" << "POBOK";
    ATCRoute route("EPWA", list, "EPKK");

    int tas = 450;
    int altitude = 35000;
    QTime depTime(12, 30, 0, 0);
    QTime enrTime(4, 50, 0, 0);

    ATCFlightPlan foo(ATC::IFR, &company, flightNo, &aircraftType, &route, tas, altitude, depTime, enrTime);
    QVERIFY(foo.getEnrouteTime() == enrTime);

    enrTime = QTime(2, 35, 0, 0);
    foo.setEnrouteTime(enrTime);
    QVERIFY(foo.getEnrouteTime() == enrTime);
}

