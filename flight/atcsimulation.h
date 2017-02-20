#ifndef ATCSIMULATION_H
#define ATCSIMULATION_H

#include "atcairspace.h"
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

    bool isPaused();
    void setPaused(bool flag);

    void setAirspace(ATCAirspace *a);
    void appendFlight(ATCFlight *flight);

    void removeFlight(QString callsign);
    void clearFlights();

    void subtractFlightTimeouts();

signals:
    void signalUpdateTags();
    void signalDisplayRoute(ATCFlight *flight);
    void signalSetSimulationStartTime();

    void signalShowFlightTag(ATCFlightTag *tag);
    void signalHideFlightTag(ATCFlightTag *tag);

public slots:
    void slotStartSimulation();
    void slotStopSimulation();

private:
    ATCAirspace *airspace;
    ATCActiveRunways *activeRunways = nullptr;
    QVector<ATCFlight*> flights;
    QVector<int> cleanupIndices;

    QElapsedTimer globalTimer;

    bool simLoop = false;
    bool paused = false;

    void preallocateTempData();
    void progressState(GeographicLib::Geodesic &geo, QElapsedTimer &globalTimer);

    ISA calculateEnvironment(ATCFlight *flight);
    void assignDiscreteState(ATCFlight *flight, ISA &isa);
    void assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo, int flightIndex);
    void flightsCleanup();

    void incrementUpdateCounter(double &counter);

};

#endif // ATCSIMULATION_H
