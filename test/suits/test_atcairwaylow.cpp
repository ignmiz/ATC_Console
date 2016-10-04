
#include "test_atcairwaylow.h"

void Test_ATCAirwayLow::test_constructObject()
{
    ATCAirwayLow airway("TEST");
    QVERIFY(airway.getName() == "TEST");
    QVERIFY(airway.getCoordsVectorSize() == 0);
}
