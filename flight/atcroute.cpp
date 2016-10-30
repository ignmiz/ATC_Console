
#include "atcroute.h"

ATCRoute::ATCRoute()
{

}

ATCRoute::ATCRoute(QString dep, QStringList route, QString des) : departure(dep), route(route), destination(des)
{

}

ATCRoute::ATCRoute(ATCRoute *other)
{
    departure = other->getDeparture();
    route = other->getRoute();
    destination = other->getDestination();
}

ATCRoute::ATCRoute(ATCRoute &other)
{
    departure = other.getDeparture();
    route = other.getRoute();
    destination = other.getDestination();
}

ATCRoute::~ATCRoute()
{

}

QString ATCRoute::getDeparture()
{
    return departure;
}

QString ATCRoute::getDestination()
{
    return destination;
}

QStringList ATCRoute::getRoute()
{
    return route;
}

