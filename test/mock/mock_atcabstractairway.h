#ifndef MOCK_ATCABSTRACTAIRWAY_H
#define MOCK_ATCABSTRACTAIRWAY_H

#include "atcabstractairway.h"

class Mock_ATCAbstractAirway : public ATCAbstractAirway
{
public:
    explicit Mock_ATCAbstractAirway(QString name);
    ~Mock_ATCAbstractAirway();
};

#endif // MOCK_ATCABSTRACTAIRWAY_H
