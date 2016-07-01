#include "atcairport.h"

#include <QDebug>

ATCAirport::ATCAirport(QString airportName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(airportName)
{
}

ATCAirport::~ATCAirport()
{
    deleteAllRunways();
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label = nullptr) delete label;
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

QPointF *ATCAirport::getScenePosition()
{
    return scenePosition;
}

QGraphicsEllipseItem *ATCAirport::getSymbol()
{
    return symbol;
}

QGraphicsSimpleTextItem *ATCAirport::getLabel()
{
    return label;
}

ATCRunway *ATCAirport::getRunway(int iterator)
{
    return runways[iterator];
}

void ATCAirport::setName(QString airportName)
{
    name = airportName;
}

void ATCAirport::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCAirport::setSymbol(QGraphicsEllipseItem *ellipse)
{
    symbol = ellipse;
}

void ATCAirport::setLabel(QGraphicsSimpleTextItem *text)
{
    label = text;
}

void ATCAirport::appendRunway(ATCRunway *runway)
{
    runways.append(runway);
}