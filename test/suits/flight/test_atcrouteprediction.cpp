#include "test_atcrouteprediction.h"

void Test_ATCRoutePrediction::test_setPredictionType()
{
    ATCRoutePrediction foo;
    QVERIFY(foo.getPredictionType() == PredictionType::FixNames);

    foo.setPredictionType(PredictionType::FixETA);
    QVERIFY(foo.getPredictionType() == PredictionType::FixETA);
}

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

void Test_ATCRoutePrediction::test_hideRoute()
{
    ATCRoutePrediction foo;

    QGraphicsPathItem *polygon = new QGraphicsPathItem();
    foo.setPolygon(polygon);

    QGraphicsSimpleTextItem *text1 = new QGraphicsSimpleTextItem();
    QGraphicsSimpleTextItem *text2 = new QGraphicsSimpleTextItem();

    foo.appendLabel(text1);
    foo.appendLabel(text2);

    QVERIFY(foo.getPolygon()->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);

    foo.hideRoute();
    QVERIFY(foo.getPolygon()->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
}

void Test_ATCRoutePrediction::test_showRoute()
{
    ATCRoutePrediction foo;

    QGraphicsPathItem *polygon = new QGraphicsPathItem();
    foo.setPolygon(polygon);

    QGraphicsSimpleTextItem *text1 = new QGraphicsSimpleTextItem();
    QGraphicsSimpleTextItem *text2 = new QGraphicsSimpleTextItem();

    foo.appendLabel(text1);
    foo.appendLabel(text2);

    foo.hideRoute();
    QVERIFY(foo.getPolygon()->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);

    foo.showRoute();
    QVERIFY(foo.getPolygon()->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
}
