#ifndef MOCK_ATCABSTRACTPROCEDURESYMBOL_H
#define MOCK_ATCABSTRACTPROCEDURESYMBOL_H

#include "atcabstractproceduresymbol.h"

class Mock_ATCAbstractProcedureSymbol : public ATCAbstractProcedureSymbol
{
public:
    explicit Mock_ATCAbstractProcedureSymbol(QString name);
    ~Mock_ATCAbstractProcedureSymbol();
};

#endif // MOCK_ATCABSTRACTPROCEDURESYMBOL_H
