
#include "test_atcflighttag.h"


void Test_ATCFlightTag::test_setDiamond()
{
    ATCFlightTag foo;
    QVERIFY(foo.getDiamond() == nullptr);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    foo.setDiamond(rect);
    QVERIFY(foo.getDiamond() == rect);
}
