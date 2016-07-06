#include "atcbeaconvor.h"
#include <QDebug>

ATCBeaconVOR::ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    ATCAbstractBeacon(beaconName, beaconFrequency, latitude, longitude)
{
}

ATCBeaconVOR::~ATCBeaconVOR()
{
    qDebug() << "VOR: " << getName() << " : " << getFrequency() << " : " << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) << " deleted...";
}
