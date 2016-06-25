#ifndef ATCBEACONNDB_H
#define ATCBEACONNDB_H

#include <QGeoCoordinate>

class ATCBeaconNDB : QGeoCoordinate
{

public:
    explicit ATCBeaconNDB(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconNDB();

    QString getName();
    float getFrequency();

    void setName(QString beaconName);
    void setFrequency(float beaconFrequency);

private:
    QString name;
    float frequency;

};

#endif // ATCBEACONNDB_H
