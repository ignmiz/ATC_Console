
#include "test_atcbeaconndb.h"

void Test_ATCBeaconNDB::test_constructObject_correct()
{
    ATCBeaconNDB bcn("BCN", 113.00, 20, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(bcn.latitude() == 20);
    QVERIFY(bcn.longitude() == 50);
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}

void Test_ATCBeaconNDB::test_constructObject_incorrectLat()
{
    ATCBeaconNDB bcn("BCN", 113.00, -91, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);

    bcn = ATCBeaconNDB("BCN", 113.00, 91, 50);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}

void Test_ATCBeaconNDB::test_constructObject_incorrectLon()
{
    ATCBeaconNDB bcn("BCN", 113.00, 20, -181);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);

    bcn = ATCBeaconNDB("BCN", 113.00, 20, 181);
    QVERIFY(bcn.getName() == "BCN");
    QVERIFY(bcn.getFrequency() == 113.00);
    QVERIFY(qIsNaN(bcn.latitude()));
    QVERIFY(qIsNaN(bcn.longitude()));
    QVERIFY(bcn.getScenePosition() == nullptr);
    QVERIFY(bcn.getSymbol() == nullptr);
    QVERIFY(bcn.getLabel() == nullptr);
}
