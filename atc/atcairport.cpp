#include "atcairport.h"

#include <QDebug>

ATCAirport::ATCAirport(QString airportName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(airportName)
{
}

ATCAirport::~ATCAirport()
{
    deleteAllRunways();
    qDebug() << "Airport: " << name << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << "deleted...";
}

void ATCAirport::deleteAllRunways()
{
    if(!runways.empty())
    {
        for(int i = 0; i < runways.size(); i++)
        {
            delete runways[i];
        }
    }
    else
    {
        qDebug() << "Vector of runways empty...";
    }
}

QString ATCAirport::getName()
{
    return name;
}

ATCRunway *ATCAirport::getRunway(int iterator)
{
    return runways[iterator];
}

void ATCAirport::setName(QString airportName)
{
    name = airportName;
}

void ATCAirport::appendRunway(ATCRunway *runway)
{
    runways.append(runway);
}
