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

    int getActiveCount();

    bool isPaused();
    void setPaused(bool flag);

    void setDataLogged(bool flag, QString path);

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

    void signalUpdateFlightList();

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

    double timeLog;
    bool dataLogged = false;
    QString dataLoggedPath;

    double sweepCounter = 0;

    int activeCount = 0;
    int predictorIterator = 0;
    int predictorCycles = 1;
    double predictorCounter = 0;
    double predictorInterval;

    void setActiveFlightsCount();
    void calculatePredictorInterval();
    void calculatePredictorCycles();

    void createDataLogs();
    void closeDataLogs();

    void preallocateTempData();
    void preallocateTempData(ATCFlight *flight);

    void createClimbProfile(ATCFlight *flight);
    void createDescentProfile(ATCFlight *flight);

    void progressState(GeographicLib::Geodesic &geo);

    ISA calculateEnvironment(ATCFlight *flight, QString &buffer);
    void assignDiscreteState(ATCFlight *flight, ISA &isa, QString &buffer);
    void assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo, int flightIndex, QString &buffer);
    void assignVerticalProfile(ATCFlight *flight, ISA &isa, bool &maxAlt);

    double distanceTOC(ATCFlight *flight, double AFL, double CFL, double RFL);
    double distanceTOD(ATCFlight *flight, double RFL);
    void assignTOCandTOD(ATCFlight *flight);

    void predictTrajectories();

    void flightsCleanup();

    void incrementSweepCounter();
    void incrementPredictorCounter();

    void appendToLogBuffer(QString &buffer, QString data);
};

#endif // ATCSIMULATION_H
