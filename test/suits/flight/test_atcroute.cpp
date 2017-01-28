
#include "test_atcroute.h"

void Test_ATCRoute::test_constructObject()
{
    QStringList route;
    route << "EVINA" << "M985" << "POBOK";

    ATCRoute foo("EPWA", route, "EPKK");

    QVERIFY(foo.getDeparture() == "EPWA");
    QVERIFY(foo.getDestination() == "EPKK");
    QVERIFY(foo.getRoute() == route);

    ATCRoute *foo2 = new ATCRoute("EPWA", route, "EPKK");
    foo = ATCRoute(foo2);

    QVERIFY(foo.getDeparture() == "EPWA");
    QVERIFY(foo.getDestination() == "EPKK");
    QVERIFY(foo.getRoute() == route);

    delete foo2;

    ATCRoute foo3("EPWA", route, "EPKK");
    foo = ATCRoute(foo3);

    QVERIFY(foo.getDeparture() == "EPWA");
    QVERIFY(foo.getDestination() == "EPKK");
    QVERIFY(foo.getRoute() == route);
}

void Test_ATCRoute::test_setGetAlternate()
{
    QStringList route;
    route << "EVINA" << "M985" << "POBOK";

    ATCRoute foo("EPWA", route, "EPKK");

    foo.setAlternate("EPKT");
    QVERIFY(foo.getAlternate() == "EPKT");
}

void Test_ATCRoute::test_setDeparture()
{
    ATCRoute foo;
    foo.setDeparture("EPWA");
    QVERIFY(foo.getDeparture() == "EPWA");
}

void Test_ATCRoute::test_setDestination()
{
    ATCRoute foo;
    foo.setDestination("EPWA");
    QVERIFY(foo.getDestination() == "EPWA");
}

void Test_ATCRoute::test_setRoute()
{
    QStringList route;
    route << "EVINA" << "M985" << "POBOK";

    ATCRoute foo;
    foo.setRoute(route);
    QVERIFY(foo.getRoute() == route);
}
