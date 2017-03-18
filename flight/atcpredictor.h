#ifndef ATCPREDICTOR_H
#define ATCPREDICTOR_H

#include "atcsimulation.h"

#include <QObject>
#include <QDebug>

class ATCPredictor : public QObject
{
    Q_OBJECT

public:
    explicit ATCPredictor(ATCSimulation *sim);
    ~ATCPredictor();

public slots:
    void slotStartPredictor();
    void slotStopPredictor();

private:
    ATCSimulation *simulation;

    int flightIterator = 0;
    bool predLoop = false;
};

#endif // ATCPREDICTOR_H
