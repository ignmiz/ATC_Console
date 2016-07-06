#include "atcairspacefix.h"
#include <QDebug>

ATCAirspaceFix::ATCAirspaceFix(double latitude, double longitude) : ATCAbstractFix(latitude, longitude)
{

}

ATCAirspaceFix::~ATCAirspaceFix()
{
    qDebug() << toString(QGeoCoordinate::DegreesMinutesSecondsWithHemisphere) + " deleted...";
}
