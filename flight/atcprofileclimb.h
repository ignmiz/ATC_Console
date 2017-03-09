#ifndef ATCPROFILECLIMB_H
#define ATCPROFILECLIMB_H

#include "atcabstractprofile.h"

class ATCProfileClimb : public ATCAbstractProfile
{
public:
    explicit ATCProfileClimb(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type = ExType::Linear);
    ~ATCProfileClimb();
};

#endif // ATCPROFILECLIMB_H
