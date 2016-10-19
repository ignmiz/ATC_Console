#ifndef TEST_ATCABSTRACTFIX_H
#define TEST_ATCABSTRACTFIX_H

#include "mock_atcabstractfix.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractFix : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();
};

#endif // TEST_ATCABSTRACTFIX_H
