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
#include <QList>
#include <QSet>
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

    void setTimeHandle(QTime *t);
    void subtractFlightTimeouts();

    void setMeteringFix(QString &fix);
    QString& getMeteringFix();

    QSet<ATCFlight*>& getConcernedFlights();
    void clearConcernedFlights();

signals:
    void signalUpdateTags();
    void signalDisplayRoute(ATCFlight *flight);
    void signalUpdateRoute(ATCFlight *flight);
    void signalClearRoute(ATCFlight *flight);

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

    QTime *timeHandle = nullptr;

    QElapsedTimer globalTimer;

    bool simLoop = false;
    bool paused = false;

    double timeLog;
    bool dataLogged = false;
    QString dataLoggedPath;

    double sweepCounter = 0;

    QString meteringFix;
    QSet<ATCFlight*> concernedFlights;

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
    void createApproachProfile(ATCFlight *flight);
    void createSpeedProfile(ATCFlight *flight);

    void progressState(GeographicLib::Geodesic &geo);

    ISA calculateEnvironment(ATCFlight *flight, QString &buffer);
    void assignDiscreteState(ATCFlight *flight, ISA &isa, QString &buffer);
    void assignContinuousState(ATCFlight *flight, ISA &isa, GeographicLib::Geodesic &geo, int flightIndex, QString &buffer);
    void assignVerticalProfile(ATCFlight *flight, ISA &isa, bool &maxAlt);
    void assignApproachProfile(ATCFlight *flight, ISA &isa);

    double distanceTOC(ATCFlight *flight, double AFL, double targetFL);
    double distanceTOD(ATCFlight *flight, double fromLvl);
    QTime timeTOC(ATCFlight *flight, double AFL, double targetFL);
    QTime timeTOD(ATCFlight *flight, double AFL, double fromLvl, double dstToTOD);

    void assignTOCandTOD(ATCFlight *flight);
    void calculateTOCposition(ATCFlight *flight);
    void calculateTODposition(ATCFlight *flight);

    void calculateWaypointTraits(ATCFlight *flight);
    void findMeteringFixIndex(ATCFlight *flight);

    void predictTrajectories();

    void flightsCleanup();

    void incrementSweepCounter();
    void incrementPredictorCounter();

    void appendToLogBuffer(QString &buffer, QString data);
};

#endif // ATCSIMULATION_H
