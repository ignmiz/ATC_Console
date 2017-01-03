#ifndef ATCACTIVERUNWAYS_H
#define ATCACTIVERUNWAYS_H

#include <QString>
#include <QVector>

struct ActiveAirport
{
    QString airportCode;
    bool dep;
    bool arr;
    QStringList depRwys;
    QStringList arrRwys;
};

class ATCActiveRunways
{
public:
    explicit ATCActiveRunways();
    ~ATCActiveRunways();

    QVector<ActiveAirport>& getActiveAirports();
    ActiveAirport getActiveAirport(int i);

    void appendActiveAirport(ActiveAirport &airport);

    void clearActiveAirports();

private:
    QVector<ActiveAirport> activeAirports;
};

#endif // ATCACTIVERUNWAYS_H
