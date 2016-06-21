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

double ATCAirspaceFix::getMercatorX()
{
    return mercatorX;
}

double ATCAirspaceFix::getMercatorY()
{
    return mercatorY;
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

void ATCAirspaceFix::setMercatorX(double mercX)
{
    mercatorX = mercX;
}

void ATCAirspaceFix::setMercatorY(double mercY)
{
    mercatorY = mercY;
}
