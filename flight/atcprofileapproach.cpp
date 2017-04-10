
#include "atcprofileapproach.h"


ATCProfileApproach::ATCProfileApproach(QVector<double> &lvls, QVector<double> time, ExType type) :
    levels(lvls),
    time(time),
    timeInterpolator(lvls, time, type)
{

}

ATCProfileApproach::~ATCProfileApproach()
{

}

double ATCProfileApproach::timeInterval(double lvlFrom, double lvlTo)
{
    QVector<double> sites;
    sites << lvlFrom << lvlTo;

    QVector<double> results(2);

    timeInterpolator.interpolate(sites, results);

    return qFabs(results.at(1) - results.at(0)); //to - from
}
