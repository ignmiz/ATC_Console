#ifndef ATCSIMULATION_H
#define ATCSIMULATION_H

#include "atcflight.h"

#include <QObject>

class ATCSimulation : public QObject
{
    Q_OBJECT

public:
    explicit ATCSimulation();
    ~ATCSimulation();

    ATCFlight* getFlight(int i);
    ATCFlight* getFlight(QString callsign);
    QVector<ATCFlight*>& getFlightsVector();
    int getFlightsVectorSize();

    void appendFlight(ATCFlight *flight);

    void removeFlight(QString callsign);
    void clearFlights();

private:
    QVector<ATCFlight*> flights;
};

#endif // ATCSIMULATION_H
