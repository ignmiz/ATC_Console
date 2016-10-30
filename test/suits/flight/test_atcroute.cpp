
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
