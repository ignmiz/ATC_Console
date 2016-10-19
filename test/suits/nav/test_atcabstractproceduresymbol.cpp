
#include "test_atcabstractproceduresymbol.h"

void Test_ATCAbstractProcedureSymbol::test_constructObject()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    QVERIFY(foo.getName() == "TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);
}

void Test_ATCAbstractProcedureSymbol::test_getName()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    QVERIFY(foo.getName() == "TEST");
}

void Test_ATCAbstractProcedureSymbol::test_setName()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    foo.setName("NEW");
    QVERIFY(foo.getName() == "NEW");
}

void Test_ATCAbstractProcedureSymbol::test_appendGetCoords1()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    foo.appendCoords1(fix);

    QVERIFY(foo.getCoords1(0) == fix);
}

void Test_ATCAbstractProcedureSymbol::test_appendGetCoords2()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");

    ATCAirspaceFix *fix = new ATCAirspaceFix(10, 20);
    foo.appendCoords2(fix);

    QVERIFY(foo.getCoords2(0) == fix);
}

void Test_ATCAbstractProcedureSymbol::test_appendGetLine()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");

    QGraphicsLineItem *line = new QGraphicsLineItem();
    foo.appendLine(line);

    QVERIFY(foo.getLine(0) == line);
}

void Test_ATCAbstractProcedureSymbol::test_getCoordsVectorSize()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    QVERIFY(foo.getCoordsVectorSize() == 0);

    ATCAirspaceFix *fix1 = new ATCAirspaceFix(10, 20);
    foo.appendCoords1(fix1);
    QVERIFY(foo.getCoordsVectorSize() == 1);

    ATCAirspaceFix *fix2 = new ATCAirspaceFix(10, 20);
    foo.appendCoords2(fix2);
    QVERIFY(foo.getCoordsVectorSize() == 1);
}

void Test_ATCAbstractProcedureSymbol::test_setColor()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");

    QGraphicsLineItem *line = new QGraphicsLineItem();
    foo.appendLine(line);
    foo.setColor(Qt::red);

    QVERIFY(foo.getLine(0)->pen().color() == Qt::red);
}

void Test_ATCAbstractProcedureSymbol::test_show()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    QVERIFY(!foo.isVisible());

    QGraphicsLineItem *line = new QGraphicsLineItem();
    foo.appendLine(line);

    foo.show();

    QVERIFY(foo.isVisible());
    QVERIFY(foo.getLine(0)->isVisible());
}

void Test_ATCAbstractProcedureSymbol::test_hide()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");
    QVERIFY(!foo.isVisible());

    QGraphicsLineItem *line = new QGraphicsLineItem();
    foo.appendLine(line);

    foo.hide();

    QVERIFY(!foo.isVisible());
    QVERIFY(!foo.getLine(0)->isVisible());
}

void Test_ATCAbstractProcedureSymbol::test_isVisible()
{
    Mock_ATCAbstractProcedureSymbol foo("TEST");

    foo.hide();
    QVERIFY(!foo.isVisible());

    foo.show();
    QVERIFY(foo.isVisible());
}
