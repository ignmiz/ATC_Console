
#include "test_atcroute.h"

void Test_ATCRoute::test_constructObject()
{
    QStringList route;
    route << "EVINA" << "M985" << "POBOK";

    ATCRoute foo("EPWA", route, "EPKK");

    QVERIFY(foo.getDeparture() == "EPWA");
    QVERIFY(foo.getDestination() == "EPKK");
    QVERIFY(foo.getRoute() == route);
}
