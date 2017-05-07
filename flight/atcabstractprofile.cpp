
#include "atcabstractprofile.h"

ATCAbstractProfile::ATCAbstractProfile()
{

}

ATCAbstractProfile::~ATCAbstractProfile()
{

}

ATCInterpolator2D *ATCAbstractProfile::getTimeInterpolator()
{
    return &timeInterpolator;
}

ATCInterpolator2D *ATCAbstractProfile::getDistanceInterpolator()
{
    return &distanceInterpolator;
}

ATCInterpolator2D *ATCAbstractProfile::getInverseTimeInterpolator()
{
    return &inverseTimeInterpolator;
}

ATCInterpolator2D *ATCAbstractProfile::getInverseDistanceInterpolator()
{
    return &inverseDistanceInterpolator;
}

double ATCAbstractProfile::timeInterval(double key, double lvlFrom, double lvlTo)
{
    return interval(timeInterpolator, key, lvlFrom, lvlTo);
}

double ATCAbstractProfile::distanceInterval(double key, double lvlFrom, double lvlTo)
{
    return interval(distanceInterpolator, key, lvlFrom, lvlTo);
}

double ATCAbstractProfile::mixedTimeInterval(double key, double lvlFrom, double interval)
{
    return mixedInterval(timeInterpolator, inverseTimeInterpolator, key, lvlFrom, interval);
}

QVector<double> ATCAbstractProfile::mixedTimeInterval(double key, double lvlFrom, QVector<double> &intervals)
{
    return mixedInterval(timeInterpolator, inverseTimeInterpolator, key, lvlFrom, intervals);
}

double ATCAbstractProfile::mixedDistanceInterval(double key, double lvlFrom, double interval)
{
    return mixedInterval(distanceInterpolator, inverseDistanceInterpolator, key, lvlFrom, interval);
}

QVector<double> ATCAbstractProfile::mixedDistanceInterval(double key, double lvlFrom, QVector<double> &intervals)
{
    return mixedInterval(distanceInterpolator, inverseDistanceInterpolator, key, lvlFrom, intervals);
}

double ATCAbstractProfile::interval(ATCInterpolator2D &interpolator, double key, double lvlFrom, double lvlTo)
{
    QVector<double> sites;
    sites << lvlFrom << lvlTo;

    QVector<double> results(2);

    interpolator.interpolate(key, sites, results);

    return qFabs(results.at(1) - results.at(0)); //to - from
}

double ATCAbstractProfile::mixedInterval(ATCInterpolator2D &interpolator, ATCInterpolator2D &inverseInterpolator, double key, double lvlFrom, double interval)
{
    //Calculate quantity X from initial level using standard interpolator
    QVector<double> site(1, lvlFrom);
    QVector<double> result(1);
    interpolator.interpolate(key, site, result);

    //Update quantity X for final level using input interval
    double finalX = interval + result.at(0);

    //Calculate final level using updated quantity X
    QVector<double> finalSite(1, finalX);
    QVector<double> finalResult(1);
    inverseInterpolator.interpolate(key, finalSite, finalResult);

    //Return calculated level
    return finalResult.at(0);
}

QVector<double> ATCAbstractProfile::mixedInterval(ATCInterpolator2D &interpolator, ATCInterpolator2D &inverseInterpolator, double key, double lvlFrom, QVector<double> &intervals)
{
    //Calculate quantity X from initial level using standard interpolator
    QVector<double> site(1, lvlFrom);
    QVector<double> result(1);
    interpolator.interpolate(key, site, result);

    //Update quantity X for final level using input interval
    for(int i = 0; i < intervals.size(); i++)
    {
        intervals.replace(i, intervals.at(i) + result.at(0));
    }

    //Calculate final level using updated quantity X
    QVector<double> finalResults(intervals.size());
    inverseInterpolator.interpolate(key, intervals, finalResults);

    //Return calculated level
    return finalResults;
}
