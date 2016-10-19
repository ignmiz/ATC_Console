
#include "test_atcprocedurestarsymbol.h"

void Test_ATCProcedureSTARSymbol::test_constructObject()
{
    ATCProcedureSTARSymbol foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}
