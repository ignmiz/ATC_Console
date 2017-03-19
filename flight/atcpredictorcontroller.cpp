
#include "atcpredictorcontroller.h"

ATCPredictorController::ATCPredictorController(ATCPredictor *pred) :
    predictor(pred),
    predictorThread(new QThread(this))
{
    predictor->moveToThread(predictorThread);
}

ATCPredictorController::~ATCPredictorController()
{
    if(predictorThread->isRunning())
    {
        predictor->slotStopPredictor();
        predictorThread->quit();
        predictorThread->wait();
    }

    if(predictorThread != nullptr) delete predictorThread;
}

void ATCPredictorController::start()
{
    predictorThread->start(QThread::HighPriority);
}

void ATCPredictorController::stop()
{
    predictor->slotStopPredictor();
    predictorThread->quit();
    predictorThread->wait();
}
