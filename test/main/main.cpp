#include "test_atcabstractairway.h"
#include "test_atcabstractbeacon.h"
#include "test_atcabstractfix.h"
#include "test_atcabstractprocedure.h"

#include "test_atcairport.h"
#include "test_atcairspace.h"
#include "test_atcairspacefix.h"
#include "test_atcairspacesector.h"
#include "test_atcairwayhigh.h"

int main(int argc, char *argv[])
{
//Test suites are declared here
    Test_ATCAbstractAirway test_ATCAbstractAirway;
    Test_ATCAbstractBeacon test_ATCAbstractBeacon;
    Test_ATCAbstractFix test_ATCAbstractFix;
    Test_ATCAbstractProcedure test_ATCAbstractProcedure;

    Test_ATCAirport test_ATCAirport;
    Test_ATCAirspace test_ATCAirspace;
    Test_ATCAirspaceFix test_ATCAirspaceFix;
    Test_ATCAirspaceSector test_ATCAirspaceSector;
    Test_ATCAirwayHigh test_ATCAirwayHigh;

//Test suites are declared here
    QTest::qExec(&test_ATCAbstractAirway, argc, argv);
    QTest::qExec(&test_ATCAbstractBeacon, argc, argv);
    QTest::qExec(&test_ATCAbstractFix, argc, argv);
    QTest::qExec(&test_ATCAbstractProcedure, argc, argv);

    QTest::qExec(&test_ATCAirport, argc, argv);
    QTest::qExec(&test_ATCAirspace, argc, argv);
    QTest::qExec(&test_ATCAirspaceFix, argc, argv);
    QTest::qExec(&test_ATCAirspaceSector, argc, argv);
    QTest::qExec(&test_ATCAirwayHigh, argc, argv);

    return 0;
}
