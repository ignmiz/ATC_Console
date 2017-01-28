
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
    alternate = other->getAlternate();
}

ATCRoute::ATCRoute(ATCRoute &other)
{
    departure = other.getDeparture();
    route = other.getRoute();
    destination = other.getDestination();
    alternate = other.getAlternate();
}

ATCRoute::~ATCRoute()
{

}

void ATCRoute::setDeparture(QString code)
{
    departure = code;
}

void ATCRoute::setDestination(QString code)
{
    destination = code;
}

void ATCRoute::setRoute(QStringList list)
{
    route = list;
}

void ATCRoute::setAlternate(QString alt)
{
    alternate = alt;
}

QString ATCRoute::getDeparture()
{
    return departure;
}

QString ATCRoute::getDestination()
{
    return destination;
}

QString ATCRoute::getAlternate()
{
    return alternate;
}

QStringList ATCRoute::getRoute()
{
    return route;
}

