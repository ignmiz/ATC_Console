#ifndef TEST_ATCABSTRACTPROCEDURESYMBOL_H
#define TEST_ATCABSTRACTPROCEDURESYMBOL_H

#include "mock_atcabstractproceduresymbol.h"

#include <QTest>
#include <QObject>

class Test_ATCAbstractProcedureSymbol : public QObject
{
    Q_OBJECT

private slots:
    void test_constructObject();
    void test_getName();
    void test_setName();
    void test_appendGetCoords1();
    void test_appendGetCoords2();
    void test_appendGetLine();
    void test_getCoordsVectorSize();
    void test_setColor();
    void test_show();
    void test_hide();
    void test_isVisible();
};

#endif // TEST_ATCABSTRACTPROCEDURESYMBOL_H
