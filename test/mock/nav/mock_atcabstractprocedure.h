#ifndef MOCK_ATCABSTRACTPROCEDURE_H
#define MOCK_ATCABSTRACTPROCEDURE_H

#include "atcabstractprocedure.h"

class Mock_ATCAbstractProcedure : public ATCAbstractProcedure
{
public:
    explicit Mock_ATCAbstractProcedure(QString procedureName, QString airportCode, QString runway);
    ~Mock_ATCAbstractProcedure();
};

#endif // MOCK_ATCABSTRACTPROCEDURE_H
