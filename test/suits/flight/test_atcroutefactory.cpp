
#include "test_atcroutefactory.h"

Test_ATCRouteFactory::Test_ATCRouteFactory()
{
    ATCPaths paths;
    path = paths.ROUTE_TEST_PATH;
}

Test_ATCRouteFactory::~Test_ATCRouteFactory()
{

}

void Test_ATCRouteFactory::test_getRoute()
{
    ATCRouteFactory factory(path);

    QStringList list;
    list << "EVINA" << "M66" << "KUKAM";


    ATCRoute *route = factory.getRoute();
    QVERIFY(route->getDeparture() == "EPWA");
    QVERIFY(route->getDestination() == "EPKK");
    QVERIFY(route->getRoute() == list);

    route = factory.getRoute(0);
    QVERIFY(route->getDeparture() == "EPWA");
    QVERIFY(route->getDestination() == "EPKK");
    QVERIFY(route->getRoute() == list);

    route = factory.getRoute("EPWA", "EPKK");
    QVERIFY(route->getDeparture() == "EPWA");
    QVERIFY(route->getDestination() == "EPKK");
    QVERIFY(route->getRoute() == list);

    route = factory.getRoute("EPPO", "EPKK");
    QVERIFY(route == nullptr);

    route = factory.getRoute("EPWA", "EPKT");
    QVERIFY(route == nullptr);
}

