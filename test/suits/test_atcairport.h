#ifndef TEST_ATCAIRPORT_H
#define TEST_ATCAIRPORT_H

#include "atcairport.h"

#include <QTest>
#include <QObject>

class Test_ATCAirport : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();

    void test_deleteAllRunways();
    void test_getName();
    void test_setName();
    void test_setGetScenePosition();
    void test_setGetSymbol();
    void test_setColor();
    void test_isRunwayVectorEmpty();
    void test_appendGetRunway_correct();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCAIRPORT_H
