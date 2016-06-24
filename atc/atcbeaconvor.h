#ifndef ATCBEACONVOR_H
#define ATCBEACONVOR_H

#include <QGeoCoordinate>

class ATCBeaconVOR : public QGeoCoordinate
{

public:
    explicit ATCBeaconVOR(QString beaconName, float beaconFrequency, double latitude, double longitude);
    ~ATCBeaconVOR();

    QString getName();
    float getFrequency();

    void setName(QString beaconName);
    void setFrequency(float beaconFrequency);

private:
    QString name;
    float frequency;

};

#endif // ATCBEACONVOR_H
