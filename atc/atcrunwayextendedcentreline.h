#ifndef ATCRUNWAYEXTENDEDCENTRELINE_H
#define ATCRUNWAYEXTENDEDCENTRELINE_H

#include <QGraphicsItem>

class ATCRunwayExtendedCentreline
{
public:
    explicit ATCRunwayExtendedCentreline();
    explicit ATCRunwayExtendedCentreline(QGraphicsLineItem *line);
    ~ATCRunwayExtendedCentreline();

    QGraphicsLineItem* getCentreline();
    QGraphicsLineItem* getTick(int i);

    void appendTick(QGraphicsLineItem *tick);

    void setCentreline(QGraphicsLineItem *line);

private:
    QGraphicsLineItem *centreline = nullptr;
    QVector<QGraphicsLineItem*> *ticks;
};

#endif // ATCRUNWAYEXTENDEDCENTRELINE_H
