#ifndef ATCPREDICTORCONTROLLER_H
#define ATCPREDICTORCONTROLLER_H

#include "atcpredictor.h"

#include <QThread>
#include <QObject>

class ATCPredictorController : public QObject
{
    Q_OBJECT

public:
    explicit ATCPredictorController(ATCPredictor *pred);
    ~ATCPredictorController();

    void start();
    void stop();

private:
    QThread *predictorThread = nullptr;
    ATCPredictor *predictor;
};

#endif // ATCPREDICTORCONTROLLER_H
