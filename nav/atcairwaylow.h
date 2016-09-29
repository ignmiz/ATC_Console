#ifndef ATCAIRWAYLOW_H
#define ATCAIRWAYLOW_H

#include "atcabstractairway.h"

class ATCAirwayLow : public ATCAbstractAirway
{
public:
    explicit ATCAirwayLow(QString name);
    ~ATCAirwayLow();
};

#endif // ATCAIRWAYLOW_H
