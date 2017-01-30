#ifndef ATCSIMULATIONCONTROLLER_H
#define ATCSIMULATIONCONTROLLER_H

#include "atcsimulation.h"

#include <QObject>
#include <QThread>

class ATCSimulationController : public QObject
{
    Q_OBJECT

public:
    explicit ATCSimulationController(ATCSimulation *sim);
    ~ATCSimulationController();

    void start();
    void stop();

private:
    QThread *simulationThread;
    ATCSimulation *simulation;
};

#endif // ATCSIMULATIONCONTROLLER_H
