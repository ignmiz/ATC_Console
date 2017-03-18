#ifndef ATCPREDICTOR_H
#define ATCPREDICTOR_H

#include <QObject>

class ATCPredictor : public QObject
{
    Q_OBJECT

public:
    explicit ATCPredictor();
    ~ATCPredictor();
};

#endif // ATCPREDICTOR_H
