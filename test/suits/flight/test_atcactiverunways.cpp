
#include "test_atcactiverunways.h"


void Test_ATCActiveRunways::test_getActiveAirports()
{
    ATCActiveRunways foo;

    ActiveAirport airport;
    airport.airportCode = "EPWA";
    airport.dep = true;
    airport.arr = false;
    airport.depRwys << "09" << "27";
    airport.arrRwys << "18";

    foo.appendActiveAirport(airport);
    foo.appendActiveAirport(airport);

    QVERIFY(foo.getActiveAirports().size() == 2);

    QVERIFY(foo.getActiveAirports().at(0).airportCode == "EPWA");
    QVERIFY(foo.getActiveAirports().at(0).dep == true);
    QVERIFY(foo.getActiveAirports().at(0).arr == false);
    QVERIFY(foo.getActiveAirports().at(0).depRwys.at(0) == "09");
    QVERIFY(foo.getActiveAirports().at(0).depRwys.at(1) == "27");
    QVERIFY(foo.getActiveAirports().at(0).arrRwys.at(0) == "18");

    QVERIFY(foo.getActiveAirports().at(1).airportCode == "EPWA");
    QVERIFY(foo.getActiveAirports().at(1).dep == true);
    QVERIFY(foo.getActiveAirports().at(1).arr == false);
    QVERIFY(foo.getActiveAirports().at(1).depRwys.at(0) == "09");
    QVERIFY(foo.getActiveAirports().at(1).depRwys.at(1) == "27");
    QVERIFY(foo.getActiveAirports().at(1).arrRwys.at(0) == "18");
}

void Test_ATCActiveRunways::test_getActiveAirport()
{
    ATCActiveRunways foo;

    ActiveAirport airport;
    airport.airportCode = "EPWA";
    airport.dep = true;
    airport.arr = false;
    airport.depRwys << "09" << "27";
    airport.arrRwys << "18";

    foo.appendActiveAirport(airport);

    QVERIFY(foo.getActiveAirports().at(0).airportCode == "EPWA");
    QVERIFY(foo.getActiveAirports().at(0).dep == true);
    QVERIFY(foo.getActiveAirports().at(0).arr == false);
    QVERIFY(foo.getActiveAirports().at(0).depRwys.at(0) == "09");
    QVERIFY(foo.getActiveAirports().at(0).depRwys.at(1) == "27");
    QVERIFY(foo.getActiveAirports().at(0).arrRwys.at(0) == "18");
}

void Test_ATCActiveRunways::test_clearActiveAirports()
{
    ATCActiveRunways foo;

    ActiveAirport airport;
    airport.airportCode = "EPWA";
    airport.dep = true;
    airport.arr = false;
    airport.depRwys << "09" << "27";
    airport.arrRwys << "18";

    foo.appendActiveAirport(airport);
    foo.appendActiveAirport(airport);

    QVERIFY(foo.getActiveAirports().size() == 2);

    foo.clearActiveAirports();
    QVERIFY(foo.getActiveAirports().size() == 0);
}
