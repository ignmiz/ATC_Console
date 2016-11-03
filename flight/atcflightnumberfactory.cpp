
#include "atcflightnumberfactory.h"

ATCFlightNumberFactory::ATCFlightNumberFactory()
{

}

ATCFlightNumberFactory::~ATCFlightNumberFactory()
{

}

QString ATCFlightNumberFactory::getFlightNumber()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    int minChars = 1;
    int maxChars = 4;

    int chars = minChars + qrand() % (maxChars - minChars + 1);

    int letters = 0;

    if((chars == 2) || (chars == 3))
    {
        int minLetters = 0;
        int maxLetters = 1;

        letters = minLetters + qrand() % (maxLetters - minLetters + 1);
    }
    else if(chars == 4)
    {
        int minLetters = 0;
        int maxLetters = 2;

        letters = minLetters + qrand() % (maxLetters - minLetters + 1);
    }

    QString flightNumber;

    for(int i = 0; i < chars - letters; i++)
    {
        int asciiNumMin = 48;
        int asciiNumMax = 57;

        char ch = asciiNumMin + qrand() % (asciiNumMax - asciiNumMin + 1);

        flightNumber.append(ch);
    }

    for(int i = 0; i < letters; i++)
    {
        int asciiCharMin = 65;
        int asciiCharMax = 90;

        char ch = asciiCharMin + qrand() % (asciiCharMax - asciiCharMin + 1);

        flightNumber.append(ch);
    }

    return flightNumber;
}

QString ATCFlightNumberFactory::getFlightNumber(int charNo)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    int minChars = charNo;
    int maxChars = charNo;

    int chars = minChars + qrand() % (maxChars - minChars + 1);

    int letters = 0;

    if((chars == 2) || (chars == 3))
    {
        int minLetters = 0;
        int maxLetters = 1;

        letters = minLetters + qrand() % (maxLetters - minLetters + 1);
    }
    else if(chars == 4)
    {
        int minLetters = 0;
        int maxLetters = 2;

        letters = minLetters + qrand() % (maxLetters - minLetters + 1);
    }

    QString flightNumber;

    for(int i = 0; i < chars - letters; i++)
    {
        int asciiNumMin = 48;
        int asciiNumMax = 57;

        char ch = asciiNumMin + qrand() % (asciiNumMax - asciiNumMin + 1);

        flightNumber.append(ch);
    }

    for(int i = 0; i < letters; i++)
    {
        int asciiCharMin = 65;
        int asciiCharMax = 90;

        char ch = asciiCharMin + qrand() % (asciiCharMax - asciiCharMin + 1);

        flightNumber.append(ch);
    }

    return flightNumber;
}

QString ATCFlightNumberFactory::getFlightNumber(int digits, int letters)
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QString flightNumber;

    for(int i = 0; i < digits; i++)
    {
        int asciiNumMin = 48;
        int asciiNumMax = 57;

        char ch = asciiNumMin + qrand() % (asciiNumMax - asciiNumMin + 1);

        flightNumber.append(ch);
    }

    for(int i = 0; i < letters; i++)
    {
        int asciiCharMin = 65;
        int asciiCharMax = 90;

        char ch = asciiCharMin + qrand() % (asciiCharMax - asciiCharMin + 1);

        flightNumber.append(ch);
    }

    return flightNumber;
}

