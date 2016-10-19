
#include "test_atcsectorartcclow.h"

void Test_ATCSectorARTCCLow::test_constructObject()
{
    ATCSectorARTCCLow foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}
