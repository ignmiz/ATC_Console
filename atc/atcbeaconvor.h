#ifndef ATCBEACONVOR_H
#define ATCBEACONVOR_H

#include "atcabstractbeacon.h"

#include <QGraphicsItem>

class ATCBeaconVOR : public ATCAbstractBeacon
{

public:
    explicit ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconVOR();
};

#endif // ATCBEACONVOR_H
