#ifndef ATCFLIGHTNUMBERFACTORY_H
#define ATCFLIGHTNUMBERFACTORY_H

#include <QString>

class ATCFlightNumberFactory
{
public:
    explicit ATCFlightNumberFactory();
    ~ATCFlightNumberFactory();

    static QString getFlightNumber();
    static QString getFlightNumber(int charNo);
    static QString getFlightNumber(int digits, int letters);
};

#endif // ATCFLIGHTNUMBERFACTORY_H
