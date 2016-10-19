#ifndef MOCK_ATCAIRSPACEFIX_H
#define MOCK_ATCAIRSPACEFIX_H

#include "atcabstractfix.h"

class Mock_ATCAbstractFix : public ATCAbstractFix
{
public:
    explicit Mock_ATCAbstractFix(double latitude, double longitude);
    ~Mock_ATCAbstractFix();
};

#endif // MOCK_ATCAIRSPACEFIX_H
