#ifndef ATCAIRWAYHIGH_H
#define ATCAIRWAYHIGH_H

#include "atcabstractairway.h"

class ATCAirwayHigh : public ATCAbstractAirway
{
public:
    explicit ATCAirwayHigh(QString name);
    ~ATCAirwayHigh();
};

#endif // ATCAIRWAYHIGH_H
