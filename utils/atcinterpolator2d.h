#ifndef ATCINTERPOLATOR2D_H
#define ATCINTERPOLATOR2D_H

#include "atcinterpolator.h"
#include "atcmath.h"

#include <QMap>
#include <QList>
#include <QDebug>

class ATCInterpolator2D
{
public:
    explicit ATCInterpolator2D();
    ~ATCInterpolator2D();

    void interpolate(double key, QVector<double> &sites, QVector<double> &results);

    ATCInterpolator* getInterpolator(double key);
    void insertInterpolator(double key, ATCInterpolator *value);
    void removeInterpolator(double key);

private:
    QMap<double, ATCInterpolator*> interpolators;
    QList<double> keys;
};

#endif // ATCINTERPOLATOR2D_H
