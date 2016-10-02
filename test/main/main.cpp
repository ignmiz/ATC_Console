#include "test_atcairport.h"
#include "test_atcairspace.h"

int main(int argc, char *argv[])
{
//Test suites are declared here
    Test_ATCAirport test_ATCAirport;
    Test_ATCAirspace test_ATCAirspace;

//Test suites are run here
    QTest::qExec(&test_ATCAirport, argc, argv);
    QTest::qExec(&test_ATCAirspace, argc, argv);

    return 0;
}
