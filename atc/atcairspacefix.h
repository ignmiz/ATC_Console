#ifndef ATCAIRSPACEFIX_H
#define ATCAIRSPACEFIX_H

#include <QGeoCoordinate>

class ATCAirspaceFix : public QGeoCoordinate
{

public:
    explicit ATCAirspaceFix(double latitude, double longitude);
    ~ATCAirspaceFix();

    void transformToLocal(double centreLat, double centreLong);

    bool getFlagLocalCoordsInitialized();
    double getLocalLatitude();
    double getLocalLongitude();

    void setFlagLocalCoordsInitialized(bool flag);
    void setLocalLatitude(double localLat);
    void setLocalLongitude(double localLon);

private:
    bool flagLocalCoordsInitialized = false;

    double localLatitude;
    double localLongitude;
};

#endif // ATCAIRSPACEFIX_H
