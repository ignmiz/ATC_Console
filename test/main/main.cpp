#include "test_atcairport.h"

int main(int argc, char *argv[])
{
//Test suites are declared here
    Test_ATCAirport test_ATCAirport;

//Test suites are run here
    QTest::qExec(&test_ATCAirport, argc, argv);

    return 0;
}
