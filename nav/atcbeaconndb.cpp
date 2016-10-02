#include "atcbeaconndb.h"
#include <QDebug>

ATCBeaconNDB::ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    ATCAbstractBeacon(beaconName, beaconFrequency, latitude, longitude)
{
}

ATCBeaconNDB::~ATCBeaconNDB()
{
}
