#include "test_atcrouteprediction.h"

void Test_ATCRoutePrediction::test_getPolygon()
{
    ATCRoutePrediction foo;
    QGraphicsPathItem *polygon = new QGraphicsPathItem();
    foo.setPolygon(polygon);

    QVERIFY(foo.getPolygon() == polygon);
}

void Test_ATCRoutePrediction::test_getLabels()
{
    ATCRoutePrediction foo;

    QGraphicsSimpleTextItem *text1 = new QGraphicsSimpleTextItem();
    QGraphicsSimpleTextItem *text2 = new QGraphicsSimpleTextItem();

    foo.appendLabel(text1);
    foo.appendLabel(text2);

    QVERIFY(foo.getLabels().size() == 2);
    QVERIFY(foo.getLabels().at(0) == text1);
    QVERIFY(foo.getLabels().at(1) == text2);
}
