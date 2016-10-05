
#include "test_atcprocedurestar.h"

void Test_ATCProcedureSTAR::test_constructObject()
{
    ATCProcedureSTAR foo("NAME", "CODE", "01");
    QVERIFY(foo.getName() == "NAME");
    QVERIFY(foo.getAirport() == "CODE");
    QVERIFY(foo.getRunwayID() == "01");
    QVERIFY(foo.getFixListSize() == 0);
}
