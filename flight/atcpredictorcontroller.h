#ifndef ATCPREDICTORCONTROLLER_H
#define ATCPREDICTORCONTROLLER_H

#include "atcpredictor.h"
#include <QObject>

class ATCPredictorController : public QObject
{
    Q_OBJECT

public:
    explicit ATCPredictorController();
    ~ATCPredictorController();
};

#endif // ATCPREDICTORCONTROLLER_H
