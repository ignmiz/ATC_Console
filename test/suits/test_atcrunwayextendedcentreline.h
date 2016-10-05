#ifndef TEST_ATCRUNWAYEXTENDEDCENTRELINE_H
#define TEST_ATCRUNWAYEXTENDEDCENTRELINE_H

#include "atcrunwayextendedcentreline.h"

#include <QTest>
#include <QObject>

class Test_ATCRunwayExtendedCentreline : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_setGetCentreline();
    void test_appendTick();
    void test_getTicksVector();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCRUNWAYEXTENDEDCENTRELINE_H
