#ifndef ATCPROFILEAPPROACH_H
#define ATCPROFILEAPPROACH_H

#include "atcinterpolator.h"
#include "atcmath.h"

#include <QVector>

class ATCProfileApproach
{
public:
    explicit ATCProfileApproach(QVector<double> &lvls, QVector<double> time, ExType type = ExType::Tangent);
    ~ATCProfileApproach();

    double timeInterval(double lvlFrom, double lvlTo);

private:
    QVector<double> levels;
    QVector<double> time;

    ATCInterpolator timeInterpolator;
};

#endif // ATCPROFILEAPPROACH_H
