#ifndef MOCK_ATCABSTRACTBEACON_H
#define MOCK_ATCABSTRACTBEACON_H

#include "atcabstractbeacon.h"

class Mock_ATCAbstractBeacon : public ATCAbstractBeacon
{
public:
    explicit Mock_ATCAbstractBeacon(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~Mock_ATCAbstractBeacon();
};

#endif // MOCK_ATCABSTRACTBEACON_H
