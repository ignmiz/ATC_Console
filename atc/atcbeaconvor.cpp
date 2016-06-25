#include "atcbeaconvor.h"
#include <QDebug>

ATCBeaconVOR::ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    name(beaconName),
    frequency(beaconFrequency),
    QGeoCoordinate(latitude, longitude)
{
}

ATCBeaconVOR::~ATCBeaconVOR()
{
    qDebug() << "VOR: " << name << " : " << frequency << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCBeaconVOR::getName()
{
    return name;
}

float ATCBeaconVOR::getFrequency()
{
    return frequency;
}

void ATCBeaconVOR::setName(QString beaconName)
{
    name = beaconName;
}

void ATCBeaconVOR::setFrequency(float beaconFrequency)
{
    frequency = beaconFrequency;
}
