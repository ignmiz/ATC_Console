#include "test_atcrouteprediction.h"

void Test_ATCRoutePrediction::test_setPredictionState()
{
    ATCRoutePrediction foo;
    QVERIFY(foo.getPredictionState() == PredictionState::None);

    foo.setPredictionState(PredictionState::FixETA);
    QVERIFY(foo.getPredictionState() == PredictionState::FixETA);
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

void Test_ATCRoutePrediction::test_getTOC()
{
    ATCRoutePrediction foo;

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem();
    foo.setTOC(el);

    QVERIFY(foo.getTOC() == el);
}

void Test_ATCRoutePrediction::test_getLabelTOC()
{
    ATCRoutePrediction foo;

    QGraphicsSimpleTextItem *t = new QGraphicsSimpleTextItem();
    foo.setLabelTOC(t);

    QVERIFY(foo.getLabelTOC() == t);
}

void Test_ATCRoutePrediction::test_getTOD()
{
    ATCRoutePrediction foo;

    QGraphicsEllipseItem *el = new QGraphicsEllipseItem();
    foo.setTOD(el);

    QVERIFY(foo.getTOD() == el);
}

void Test_ATCRoutePrediction::test_getLabelTOD()
{
    ATCRoutePrediction foo;

    QGraphicsSimpleTextItem *t = new QGraphicsSimpleTextItem();
    foo.setLabelTOD(t);

    QVERIFY(foo.getLabelTOD() == t);
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

    QGraphicsEllipseItem *el1 = new QGraphicsEllipseItem();
    QGraphicsEllipseItem *el2 = new QGraphicsEllipseItem();
    QGraphicsSimpleTextItem *t1 = new QGraphicsSimpleTextItem();
    QGraphicsSimpleTextItem *t2 = new QGraphicsSimpleTextItem();

    foo.setTOC(el1);
    foo.setLabelTOC(t1);
    foo.setTOD(el2);
    foo.setLabelTOD(t2);

    QVERIFY(foo.getPolygon()->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getTOC()->isVisible() == true);
    QVERIFY(foo.getLabelTOC()->isVisible() == true);
    QVERIFY(foo.getTOD()->isVisible() == true);
    QVERIFY(foo.getLabelTOD()->isVisible() == true);

    foo.hideRoute();
    QVERIFY(foo.getPolygon()->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getTOC()->isVisible() == false);
    QVERIFY(foo.getLabelTOC()->isVisible() == false);
    QVERIFY(foo.getTOD()->isVisible() == false);
    QVERIFY(foo.getLabelTOD()->isVisible() == false);
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

    QGraphicsEllipseItem *el1 = new QGraphicsEllipseItem();
    QGraphicsEllipseItem *el2 = new QGraphicsEllipseItem();
    QGraphicsSimpleTextItem *t1 = new QGraphicsSimpleTextItem();
    QGraphicsSimpleTextItem *t2 = new QGraphicsSimpleTextItem();

    foo.setTOC(el1);
    foo.setLabelTOC(t1);
    foo.setTOD(el2);
    foo.setLabelTOD(t2);

    foo.hideRoute();
    QVERIFY(foo.getPolygon()->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getLabels().at(0)->isVisible() == false);
    QVERIFY(foo.getTOC()->isVisible() == false);
    QVERIFY(foo.getLabelTOC()->isVisible() == false);
    QVERIFY(foo.getTOD()->isVisible() == false);
    QVERIFY(foo.getLabelTOD()->isVisible() == false);

    foo.showRoute();
    QVERIFY(foo.getPolygon()->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getLabels().at(0)->isVisible() == true);
    QVERIFY(foo.getTOC()->isVisible() == true);
    QVERIFY(foo.getLabelTOC()->isVisible() == true);
    QVERIFY(foo.getTOD()->isVisible() == true);
    QVERIFY(foo.getLabelTOD()->isVisible() == true);
}
