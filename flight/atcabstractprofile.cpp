
#include "atcabstractprofile.h"

ATCAbstractProfile::ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    levels(levels),
    time(time),
    distance(distance),
    timeInterpolator(levels, time, type),
    distanceInterpolator(levels, distance, type)
{

}

ATCAbstractProfile::~ATCAbstractProfile()
{

}

void ATCAbstractProfile::setTimeExtrapolationType(ExType t)
{
    timeInterpolator.setExtrapolationType(t);
}

ExType ATCAbstractProfile::getTimeExtrapolationType()
{
    return timeInterpolator.getExtrapolationType();
}

void ATCAbstractProfile::setDistanceExtrapolationType(ExType t)
{
    distanceInterpolator.setExtrapolationType(t);
}

ExType ATCAbstractProfile::getDistanceExtrapolationType()
{
    return distanceInterpolator.getExtrapolationType();
}

double ATCAbstractProfile::timeInterval(double lvlFrom, double lvlTo)
{
    return interval(timeInterpolator, lvlFrom, lvlTo);
}

double ATCAbstractProfile::distanceInterval(double lvlFrom, double lvlTo)
{
    return interval(distanceInterpolator, lvlFrom, lvlTo);
}

double ATCAbstractProfile::interval(ATCInterpolator &interpolator, double lvlFrom, double lvlTo)
{
    QVector<double> sites;
    sites << lvlFrom << lvlTo;

    QVector<double> results(2);

    interpolator.interpolate(sites, results);

    return qFabs(results.at(1) - results.at(0)); //to - from
}
