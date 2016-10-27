
#include "test_atcaircrafttypefactory.h"

Test_ATCAircraftTypeFactory::Test_ATCAircraftTypeFactory()
{
    ATCPaths paths;
    DBpath = paths.BADA_TEST_PATH;
}

Test_ATCAircraftTypeFactory::~Test_ATCAircraftTypeFactory()
{

}

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

