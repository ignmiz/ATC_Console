
#include "test_atccompanyfactory.h"

void Test_ATCCompanyFactory::test_getCompany()
{
    ATCCompanyFactory companyFactory(path);

    for(int i = 0; i < 100; i++)
    {
        ATCCompany *company = companyFactory.getCompany();

        QVERIFY(((company->getCode() == "AAA") && (company->getCallsign() == "TSTCALL1") && (company->getName() == "Test Name 1")) ||
                ((company->getCode() == "AAB") && (company->getCallsign() == "TSTCALL2") && (company->getName() == "Test Name 2")) ||
                ((company->getCode() == "AAC") && (company->getCallsign() == "TSTCALL3") && (company->getName() == "Test Name 3")));
    }

    ATCCompany *company = companyFactory.getCompany(0);

    QVERIFY(((company->getCode() == "AAA") && (company->getCallsign() == "TSTCALL1") && (company->getName() == "Test Name 1")));
}
