
#include "atcabstractprofile.h"

ATCAbstractProfile::ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type) :
    levels(levels),
    time(time),
    distance(distance),
    timeInterpolator(levels, time, type),
    distanceInterpolator(levels, distance, type)
{
    //Inverse interpolators need to be implemented in derived classes due breakpoints sequence limitation
}

void ATCAbstractProfile::initializeInverseInterpolators(ATCInterpolator &invTime, ATCInterpolator &invDst)
{
    inverseTimeInterpolator = invTime;
    inverseDistanceInterpolator = invDst;
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

double ATCAbstractProfile::mixedTimeInterval(double lvlFrom, double interval)
{
    return mixedInterval(timeInterpolator, inverseTimeInterpolator, lvlFrom, interval);
}

QVector<double> ATCAbstractProfile::mixedTimeInterval(double lvlFrom, QVector<double> &intervals)
{
    return mixedInterval(timeInterpolator, inverseTimeInterpolator, lvlFrom, intervals);
}

double ATCAbstractProfile::mixedDistanceInterval(double lvlFrom, double interval)
{
    return mixedInterval(distanceInterpolator, inverseDistanceInterpolator, lvlFrom, interval);
}

QVector<double> ATCAbstractProfile::mixedDistanceInterval(double lvlFrom, QVector<double> &intervals)
{
    return mixedInterval(distanceInterpolator, inverseDistanceInterpolator, lvlFrom, intervals);
}

double ATCAbstractProfile::interval(ATCInterpolator &interpolator, double lvlFrom, double lvlTo)
{
    QVector<double> sites;
    sites << lvlFrom << lvlTo;

    QVector<double> results(2);

    interpolator.interpolate(sites, results);

    return qFabs(results.at(1) - results.at(0)); //to - from
}

double ATCAbstractProfile::mixedInterval(ATCInterpolator &interpolator, ATCInterpolator &inverseInterpolator, double lvlFrom, double interval)
{
    //Calculate quantity X from initial level using standard interpolator
    QVector<double> site(1, lvlFrom);
    QVector<double> result(1);
    interpolator.interpolate(site, result);

    //Update quantity X for final level using input interval
    double finalX = interval + result.at(0);

    //Calculate final level using updated quantity X
    QVector<double> finalSite(1, finalX);
    QVector<double> finalResult(1);
    inverseInterpolator.interpolate(finalSite, finalResult);

    //Return calculated level
    return finalResult.at(0);
}

QVector<double> ATCAbstractProfile::mixedInterval(ATCInterpolator &interpolator, ATCInterpolator &inverseInterpolator, double lvlFrom, QVector<double> &intervals)
{
    //Calculate quantity X from initial level using standard interpolator
    QVector<double> site(1, lvlFrom);
    QVector<double> result(1);
    interpolator.interpolate(site, result);

    //Update quantity X for final level using input interval
    for(int i = 0; i < intervals.size(); i++)
    {
        intervals.replace(i, intervals.at(i) + result.at(0));
    }

    //Calculate final level using updated quantity X
    QVector<double> finalResults(intervals.size());
    inverseInterpolator.interpolate(intervals, finalResults);

    //Return calculated level
    return finalResults;
}
