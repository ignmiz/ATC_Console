#ifndef ATCSIMULATION_H
#define ATCSIMULATION_H

#include "atcconst.h"
#include "atcactiverunways.h"
#include "atcflight.h"
#include "atcmath.h"

#include <QElapsedTimer>
#include <QObject>
#include <QThread>
#include <QDebug>

class ATCSimulation : public QObject
{
    Q_OBJECT

public:
    explicit ATCSimulation();
    ~ATCSimulation();

    ATCActiveRunways* getActiveRunways();

    ATCFlight* getFlight(int i);
    ATCFlight* getFlight(QString callsign);
    QVector<ATCFlight*>& getFlightsVector();
    int getFlightsVectorSize();

    void appendFlight(ATCFlight *flight);

    void removeFlight(QString callsign);
    void clearFlights();

public slots:
    void slotStartSimulation();
    void slotStopSimulation();

private:
    ATCActiveRunways *activeRunways = nullptr;
    QVector<ATCFlight*> flights;

    bool simLoop = false;

    void progressState(GeographicLib::Geodesic &geo);

    ISA calculateEnvironment(ATCFlight *flight);
    void assignDiscreteState(ATCFlight *flight, ISA &isa);
    void assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo);

};

#endif // ATCSIMULATION_H
