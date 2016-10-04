#ifndef TEST_ATCNAVFIX_H
#define TEST_ATCNAVFIX_H

#include "atcnavfix.h"

#include <QTest>
#include <QObject>

class Test_ATCNavFix : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();

    void test_getName();
    void test_setName();
    void test_setGetScenePosition();
    void test_setGetSymbol();
    void test_setGetLabel();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCNAVFIX_H
