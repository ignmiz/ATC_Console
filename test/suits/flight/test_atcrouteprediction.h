#ifndef TEST_ATCROUTEPREDICTION_H
#define TEST_ATCROUTEPREDICTION_H

#include "atcrouteprediction.h"

#include <QObject>
#include <QTest>

class Test_ATCRoutePrediction : public QObject
{
    Q_OBJECT

private slots:
    void test_setPredictionState();
    void test_getPolygon();
    void test_getLabels();
    void test_getTOC();
    void test_getLabelTOC();
    void test_getTOD();
    void test_getLabelTOD();
    void test_hideRoute();
    void test_showRoute();
};

#endif // TEST_ATCROUTEPREDICTION_H
