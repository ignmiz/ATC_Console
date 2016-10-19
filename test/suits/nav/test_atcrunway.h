#ifndef TEST_ATCRUNWAY_H
#define TEST_ATCRUNWAY_H

#include "atcrunway.h"

#include <QTest>
#include <QObject>

class Test_ATCRunway : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();

    void test_getRunway1();
    void test_getRunway2();
    void test_getMagneticHDG1();
    void test_getMagneticHDG2();
    void test_getStartPoint();
    void test_getEndPoint();
    void test_setGetExtendedCentreline1();
    void test_setGetExtendedCentreline2();
};

#endif // TEST_ATCRUNWAY_H
