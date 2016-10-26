
#include "test_atcaircrafttypefactory.h"

void Test_ATCAircraftTypeFactory::test_getType()
{
    ATCAircraftTypeFactory factory(DBpath);

    ATCAircraftType *type = factory.getType();
    QVERIFY(type->getAcType().ICAOcode == "B738");

    type = factory.getType(0);
    QVERIFY(type->getAcType().ICAOcode == "B738");

    type = factory.getType("B738");
    QVERIFY(type->getAcType().ICAOcode == "B738");

    type = factory.getType("B747");
    QVERIFY(type == nullptr);
}

