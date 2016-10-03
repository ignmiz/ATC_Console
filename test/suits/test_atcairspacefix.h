#ifndef TEST_ATCAIRSPACEFIX_H
#define TEST_ATCAIRSPACEFIX_H

#include "atcairspacefix.h"

#include <QTest>
#include <QObject>

class Test_ATCAirspaceFix : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject_correct();
    void test_constructObject_incorrectLat();
    void test_constructObject_incorrectLon();
};

#endif // TEST_ATCAIRSPACEFIX_H
