#include "atcairspacefix.h"
#include <QDebug>

ATCAirspaceFix::ATCAirspaceFix(double latitude, double longitude) : QGeoCoordinate(latitude, longitude)
{

}

ATCAirspaceFix::~ATCAirspaceFix()
{
    qDebug() << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) + " deleted...";
}

void ATCAirspaceFix::transformToLocal(double centreLat, double centreLong)
{
    localLatitude = latitude() - centreLat;
    localLongitude = longitude() - centreLong;

    flagLocalCoordsInitialized = true;
}

bool ATCAirspaceFix::getFlagLocalCoordsInitialized()
{
    return flagLocalCoordsInitialized;
}

double ATCAirspaceFix::getLocalLatitude()
{
    return localLatitude;
}

double ATCAirspaceFix::getLocalLongitude()
{
    return localLongitude;
}

void ATCAirspaceFix::setFlagLocalCoordsInitialized(bool flag)
{
    flagLocalCoordsInitialized = flag;
}

void ATCAirspaceFix::setLocalLatitude(double localLat)
{
    localLatitude = localLat;
}

void ATCAirspaceFix::setLocalLongitude(double localLon)
{
    localLongitude = localLon;
}
