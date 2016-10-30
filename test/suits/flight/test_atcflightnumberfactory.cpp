
#include "test_atcflightnumberfactory.h"


void Test_ATCFlightNumberFactory::test_getFligthNumber()
{
    for(int i = 0; i < 1000; i++)
    {
        QString foo = ATCFlightNumberFactory::getFlightNumber();
        if(foo.size() == 1)
        {
            QVERIFY(foo.at(0).isDigit());
        }
        else if(foo.size() == 2)
        {
            QVERIFY((foo.at(0).isDigit() && foo.at(1).isDigit()) ||
                    (foo.at(0).isDigit() && foo.at(1).isUpper()));
        }
        else if(foo.size() == 3)
        {
            QVERIFY((foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isDigit()) ||
                    (foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isUpper()));
        }
        else if(foo.size() == 4)
        {
            QVERIFY((foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isDigit() && foo.at(3).isDigit()) ||
                    (foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isDigit() && foo.at(3).isUpper()) ||
                    (foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isUpper() && foo.at(3).isUpper()));
        }

        foo = ATCFlightNumberFactory::getFlightNumber(5);
        QVERIFY(foo.size() == 5);

        foo = ATCFlightNumberFactory::getFlightNumber(2, 2);
        QVERIFY(foo.size() == 4);
        QVERIFY((foo.at(0).isDigit() && foo.at(1).isDigit() && foo.at(2).isUpper() && foo.at(3).isUpper()));
    }
}
