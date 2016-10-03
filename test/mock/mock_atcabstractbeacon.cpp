
#include "mock_atcabstractbeacon.h"

Mock_ATCAbstractBeacon::Mock_ATCAbstractBeacon(QString beaconName, float beaconFrequency, double latitude, double longitude) :
    ATCAbstractBeacon(beaconName, beaconFrequency, latitude, longitude)
{

}

Mock_ATCAbstractBeacon::~Mock_ATCAbstractBeacon()
{

}
