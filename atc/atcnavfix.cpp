#include "atcnavfix.h"
#include <QDebug>

ATCNavFix::ATCNavFix(QString fixName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(fixName)
{
}

ATCNavFix::~ATCNavFix()
{
    qDebug() << name << ": " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCNavFix::getName()
{
    return name;
}

void ATCNavFix::setName(QString fixName)
{
    name = fixName;
}
