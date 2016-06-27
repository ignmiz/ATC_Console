#include "atcnavfix.h"
#include <QDebug>

ATCNavFix::ATCNavFix(QString fixName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(fixName)
{
}

ATCNavFix::~ATCNavFix()
{
    delete symbol;
    qDebug() << name << ": " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCNavFix::getName()
{
    return name;
}

QGraphicsPolygonItem* ATCNavFix::getSymbol()
{
    return symbol;
}

void ATCNavFix::setName(QString fixName)
{
    name = fixName;
}

void ATCNavFix::setSymbol(QGraphicsPolygonItem *fixSymbol)
{
    symbol = fixSymbol;
}
