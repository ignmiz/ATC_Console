#ifndef MOCK_ATCABSTRACTSECTOR_H
#define MOCK_ATCABSTRACTSECTOR_H

#include "atcabstractsector.h"

class Mock_ATCAbstractSector : public ATCAbstractSector
{
public:
    explicit Mock_ATCAbstractSector(QString name);
    ~Mock_ATCAbstractSector();
};

#endif // MOCK_ATCABSTRACTSECTOR_H
