
#include "test_atcflighttag.h"


void Test_ATCFlightTag::test_setDiamond()
{
    ATCFlightTag foo;
    QVERIFY(foo.getDiamond() == nullptr);

    QGraphicsRectItem *rect = new QGraphicsRectItem();
    foo.setDiamond(rect);
    QVERIFY(foo.getDiamond() == rect);
}

void Test_ATCFlightTag::test_setDiamondPosition()
{
    ATCFlightTag foo;

    QPointF point(20, 30);
    foo.setDiamondPosition(point);
    QVERIFY(foo.getDiamondPosition() == point);
}

void Test_ATCFlightTag::test_setLeader()
{
    ATCFlightTag foo;
    QVERIFY(foo.getLeader() == nullptr);

    QGraphicsLineItem *line = new QGraphicsLineItem();
    foo.setLeader(line);
    QVERIFY(foo.getLeader() == line);
}

void Test_ATCFlightTag::test_setLeaderEndPosition()
{
    ATCFlightTag foo;

    QPointF point(20, 30);
    foo.setLeaderEndPosition(point);
    QVERIFY(foo.getLeaderEndPosition() == point);
}
