
#include "test_atcsectorartcc.h"

void Test_ATCSectorARTCC::test_constructObject()
{
    ATCSectorARTCC foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}
