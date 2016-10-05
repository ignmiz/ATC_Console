
#include "test_atcproceduresidsymbol.h"

void Test_ATCProcedureSIDSymbol::test_constructObject()
{
    ATCProcedureSIDSymbol foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}
