#ifndef ATCPREDICTOR_H
#define ATCPREDICTOR_H

#include "atcsimulation.h"

#include <QMutex>
#include <QMutexLocker>

#include <QObject>
#include <QDebug>

class ATCPredictor : public QObject
{
    Q_OBJECT

public:
    explicit ATCPredictor(ATCSimulation *sim);
    ~ATCPredictor();

    void setMutex(QSharedPointer<QMutex> m);

public slots:
    void slotSimulationStatus(int total, int active);

    void slotStartPredictor();
    void slotStopPredictor();

private:
    QSharedPointer<QMutex> mutex;
    ATCSimulation *simulation;

    int totalFlights;
    int activeFlights;

    int flightIterator = 0;
    bool predLoop = false;
};

#endif // ATCPREDICTOR_H
