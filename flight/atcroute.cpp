
#include "atcroute.h"

ATCRoute::ATCRoute(QString dep, QStringList route, QString des) : departure(dep), route(route), destination(des)
{

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

