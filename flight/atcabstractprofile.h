#ifndef ATCABSTRACTPROFILE_H
#define ATCABSTRACTPROFILE_H

#include "atcinterpolator.h"
#include "atcmath.h"

#include <QVector>

class ATCAbstractProfile
{
public:
    explicit ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type = ExType::Linear);
    virtual ~ATCAbstractProfile() = 0;

    void setTimeExtrapolationType(ExType t);
    ExType getTimeExtrapolationType();

    void setDistanceExtrapolationType(ExType t);
    ExType getDistanceExtrapolationType();

    double timeInterval(double lvlFrom, double lvlTo);
    double distanceInterval(double lvlFrom, double lvlTo);

private:
    QVector<double> levels;
    QVector<double> time;
    QVector<double> distance;

    ATCInterpolator timeInterpolator;
    ATCInterpolator distanceInterpolator;

    double interval(ATCInterpolator &interpolator, double lvlFrom, double lvlTo);
};

#endif // ATCABSTRACTPROFILE_H
