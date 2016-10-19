
#include "test_atcsectorartcchigh.h"

void Test_ATCSectorARTCCHigh::test_constructObject()
{
    ATCSectorARTCCHigh foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}
