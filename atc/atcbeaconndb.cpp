#include "atcbeaconndb.h"
#include <QDebug>

ATCBeaconNDB::ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    name(beaconName),
    frequency(beaconFrequency),
    QGeoCoordinate(latitude, longitude)
{
}

ATCBeaconNDB::~ATCBeaconNDB()
{
    qDebug() << "NDB: " << name << " : " << frequency << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}

QString ATCBeaconNDB::getName()
{
    return name;
}

float ATCBeaconNDB::getFrequency()
{
    return frequency;
}

void ATCBeaconNDB::setName(QString beaconName)
{
    name = beaconName;
}

void ATCBeaconNDB::setFrequency(float beaconFrequency)
{
    frequency = beaconFrequency;
}
