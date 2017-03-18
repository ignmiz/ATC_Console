
#include "atcpredictor.h"

ATCPredictor::ATCPredictor(ATCSimulation *sim) : simulation(sim)
{

}

ATCPredictor::~ATCPredictor()
{

}

void ATCPredictor::slotStartPredictor()
{
    QElapsedTimer timer;
    predLoop = true;

    while(predLoop)
    {
        timer.start();
        int flightsVectorSize = simulation->getFlightsVectorSize();

        if(flightIterator < flightsVectorSize)
        {
            for(int ii = 0; ii < 50000; ii++); //Random dummy task

            flightIterator++;

            qint64 dt = qRound(ATCConst::PREDICTION_INTERVAL / flightsVectorSize * 1e9);
            qint64 elapsedTime = timer.nsecsElapsed();
            qint64 diff = dt - elapsedTime;

            qDebug() << "Iterator: " << flightIterator - 1 << "\t|\tElapsed: " << elapsedTime << "ns\t|\tDiff: " << diff << "ns\t|\tError: " << dt - (elapsedTime + qFloor(diff/1000) * 1000) << "ns";
            QThread::usleep(qFloor(diff / 1000));
        }
        else
        {
            flightIterator = 0;
        }
    }
}

void ATCPredictor::slotStopPredictor()
{
    predLoop = false;
}
