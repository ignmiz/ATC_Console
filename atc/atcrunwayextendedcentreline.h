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

    void setCentreline(QGraphicsLineItem *line);

private:
    QGraphicsLineItem *centreline = nullptr;
};

#endif // ATCRUNWAYEXTENDEDCENTRELINE_H
