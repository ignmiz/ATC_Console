#ifndef TEST_ATCROUTEPREDICTION_H
#define TEST_ATCROUTEPREDICTION_H

#include "atcrouteprediction.h"

#include <QObject>
#include <QTest>

class Test_ATCRoutePrediction : public QObject
{
    Q_OBJECT

private slots:
    void test_getPolygon();
    void test_getLabels();
};

#endif // TEST_ATCROUTEPREDICTION_H
