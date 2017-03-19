
#include "atcpredictor.h"

ATCPredictor::ATCPredictor(ATCSimulation *sim) : simulation(sim)
{

}

ATCPredictor::~ATCPredictor()
{

}

void ATCPredictor::setMutex(QSharedPointer<QMutex> m)
{
    mutex = m;
}

void ATCPredictor::slotSimulationStatus(int total, int active)
{
    totalFlights = total;
    activeFlights = active;
}

void ATCPredictor::slotStartPredictor()
{
//    QThread::msleep(7500);

    mutex->lock();
    QElapsedTimer timer;
    predLoop = true;

    bool updateCounter = true;
    mutex->unlock();

    while(predLoop)
    {
        mutex->tryLock();
        if(updateCounter) timer.start();

        if(flightIterator < totalFlights)
        {
            ATCFlight *flight = simulation->getFlight(flightIterator);
            qDebug() << flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber();

            if(flight->isSimulated())
            {
                for(int ii = 0; ii < 50000; ii++); //Random dummy task

                flightIterator++;
                updateCounter = true;

                qint64 dt = qRound(ATCConst::TOTAL_PREDICTION_INTERVAL / 2 * 1e9); //Problems with simulation->getActiveCount
                qint64 elapsedTime = timer.nsecsElapsed();
                qint64 diff = dt - elapsedTime;

                qDebug() << dt;
                qDebug() << elapsedTime;
                qDebug() << qFloor(diff / 1000);

                qDebug() << "Iterator: " << flightIterator - 1 << "\t|\tElapsed: " << elapsedTime << "ns\t|\tDiff: " << diff << "ns\t|\tError: " << dt - (elapsedTime + qFloor(diff/1000) * 1000) << "ns";

                mutex->unlock();
                QThread::usleep(qFloor(diff / 1000));
            }
            else
            {
                flightIterator++;
                updateCounter = false;
            }
        }
        else
        {
            flightIterator = 0;
            updateCounter = false;
        }
    }
}

void ATCPredictor::slotStopPredictor()
{
    predLoop = false;
}
