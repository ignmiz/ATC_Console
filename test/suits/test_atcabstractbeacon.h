#ifndef TEST_ATCABSTRACTBEACON_H
#define TEST_ATCABSTRACTBEACON_H

#include "mock_atcabstractbeacon.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractBeacon : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();

    void test_getName();
    void test_setName();
    void test_getFrequency();
    void test_setFrequency();
    void test_setGetSymbol();
    void test_setGetLabel();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCABSTRACTBEACON_H
