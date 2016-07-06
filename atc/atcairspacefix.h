#ifndef ATCAIRSPACEFIX_H
#define ATCAIRSPACEFIX_H

#include "atcabstractfix.h"

class ATCAirspaceFix : public ATCAbstractFix
{
public:
    explicit ATCAirspaceFix(double latitude, double longitude);
    ~ATCAirspaceFix();
};

#endif // ATCAIRSPACEFIX_H
