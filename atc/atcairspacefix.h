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
    double getMercatorX();
    double getMercatorY();

    void setFlagLocalCoordsInitialized(bool flag);
    void setLocalLatitude(double localLat);
    void setLocalLongitude(double localLon);
    void setMercatorX(double mercX);
    void setMercatorY(double mercY);

private:
    bool flagLocalCoordsInitialized = false;

    double localLatitude;
    double localLongitude;

    double mercatorX;
    double mercatorY;
};

#endif // ATCAIRSPACEFIX_H
