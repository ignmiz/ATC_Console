#ifndef ATCBEACONNDB_H
#define ATCBEACONNDB_H

#include "atcabstractbeacon.h"

#include <QGraphicsItem>

class ATCBeaconNDB : public ATCAbstractBeacon
{

public:
    explicit ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconNDB();
};

#endif // ATCBEACONNDB_H
