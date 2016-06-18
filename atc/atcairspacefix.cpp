#include "atcairspacefix.h"
#include <QDebug>

ATCAirspaceFix::ATCAirspaceFix(double latitude, double longitude) : QGeoCoordinate(latitude, longitude)
{

}

ATCAirspaceFix::~ATCAirspaceFix()
{
    qDebug() << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) + " deleted...";
}
