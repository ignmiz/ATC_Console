#include "test_atcabstractairway.h"
#include "test_atcabstractbeacon.h"
#include "test_atcabstractfix.h"
#include "test_atcabstractprocedure.h"
#include "test_atcabstractproceduresymbol.h"
#include "test_atcabstractsector.h"

#include "test_atcairport.h"
#include "test_atcairspace.h"
#include "test_atcairspacefix.h"
#include "test_atcairspacesector.h"
#include "test_atcairwayhigh.h"
#include "test_atcairwaylow.h"
#include "test_atcbeaconndb.h"
#include "test_atcbeaconvor.h"
#include "test_atcnavfix.h"

#include <QThread>

int main(int argc, char *argv[])
{
    bool failed;

//Test suites are declared here
    Test_ATCAbstractAirway test_ATCAbstractAirway;
    Test_ATCAbstractBeacon test_ATCAbstractBeacon;
    Test_ATCAbstractFix test_ATCAbstractFix;
    Test_ATCAbstractProcedure test_ATCAbstractProcedure;
    Test_ATCAbstractProcedureSymbol test_ATCAbstractProcedureSymbol;
    Test_ATCAbstractSector test_ATCAbstractSector;

    Test_ATCAirport test_ATCAirport;
    Test_ATCAirspace test_ATCAirspace;
    Test_ATCAirspaceFix test_ATCAirspaceFix;
    Test_ATCAirspaceSector test_ATCAirspaceSector;
    Test_ATCAirwayHigh test_ATCAirwayHigh;
    Test_ATCAirwayLow test_ATCAirwayLow;
    Test_ATCBeaconNDB test_ATCBeaconNDB;
    Test_ATCBeaconVOR test_ATCBeaconVOR;
    Test_ATCNavFix test_ATCNavFix;

//Test suites are declared here
    failed = QTest::qExec(&test_ATCAbstractAirway, argc, argv);
    failed = QTest::qExec(&test_ATCAbstractBeacon, argc, argv);
    failed = QTest::qExec(&test_ATCAbstractFix, argc, argv);
    failed = QTest::qExec(&test_ATCAbstractProcedure, argc, argv);
    failed = QTest::qExec(&test_ATCAbstractProcedureSymbol, argc, argv);
    failed = QTest::qExec(&test_ATCAbstractSector, argc, argv);

    failed = QTest::qExec(&test_ATCAirport, argc, argv);
    failed = QTest::qExec(&test_ATCAirspace, argc, argv);
    failed = QTest::qExec(&test_ATCAirspaceFix, argc, argv);
    failed = QTest::qExec(&test_ATCAirspaceSector, argc, argv);
    failed = QTest::qExec(&test_ATCAirwayHigh, argc, argv);
    failed = QTest::qExec(&test_ATCAirwayLow, argc, argv);
    failed = QTest::qExec(&test_ATCBeaconNDB, argc, argv);
    failed = QTest::qExec(&test_ATCBeaconVOR, argc, argv);
    failed = QTest::qExec(&test_ATCNavFix, argc, argv);

    QThread::msleep(1000);

    if(failed)
    {
        qDebug() << "At least one test FAILED!";
    }
    else
    {
        qDebug() << "All tests PASSED!";
    }

    return 0;
}
