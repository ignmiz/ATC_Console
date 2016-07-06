#ifndef ATCABSTRACTFIX_H
#define ATCABSTRACTFIX_H

#include <QGeoCoordinate>

class ATCAbstractFix : public QGeoCoordinate
{
public:
    explicit ATCAbstractFix(double latitude, double longitude);
    virtual ~ATCAbstractFix() = 0;
};

#endif // ATCABSTRACTFIX_H
