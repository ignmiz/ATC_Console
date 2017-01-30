
#include "atcsimulationcontroller.h"

ATCSimulationController::ATCSimulationController(ATCSimulation *sim) :
    simulation(sim),
    simulationThread(new QThread(this))
{
    simulation->moveToThread(simulationThread);
    connect(simulationThread, SIGNAL(started()), simulation, SLOT(slotStartSimulation()));
}

ATCSimulationController::~ATCSimulationController()
{
    if(simulationThread->isRunning())
    {
        simulation->slotStopSimulation();
        simulationThread->quit();
        simulationThread->wait();
    }

    if(simulationThread != nullptr) delete simulationThread;
}

void ATCSimulationController::start()
{
    simulationThread->start(QThread::HighestPriority);
}

void ATCSimulationController::stop()
{
    simulation->slotStopSimulation();
    simulationThread->quit();
    simulationThread->wait();
}
