
#include "test_atcproceduresid.h"

void Test_ATCProcedureSID::test_constructObject()
{
    ATCProcedureSID foo("NAME", "CODE", "01");
    QVERIFY(foo.getName() == "NAME");
    QVERIFY(foo.getAirport() == "CODE");
    QVERIFY(foo.getRunwayID() == "01");
    QVERIFY(foo.getFixListSize() == 0);
}
