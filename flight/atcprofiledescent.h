#ifndef ATCPROFILEDESCENT_H
#define ATCPROFILEDESCENT_H

#include "atcabstractprofile.h"

class ATCProfileDescent : public ATCAbstractProfile
{
public:
    explicit ATCProfileDescent(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type = ExType::Linear);
    ~ATCProfileDescent();
};

#endif // ATCPROFILEDESCENT_H
