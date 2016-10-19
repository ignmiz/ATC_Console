
#include "test_atcairwayhigh.h"

void Test_ATCAirwayHigh::test_constructObject()
{
    ATCAirwayHigh airway("TEST");
    QVERIFY(airway.getName() == "TEST");
    QVERIFY(airway.getCoordsVectorSize() == 0);
}
