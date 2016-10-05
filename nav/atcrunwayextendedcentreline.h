#ifndef ATCRUNWAYEXTENDEDCENTRELINE_H
#define ATCRUNWAYEXTENDEDCENTRELINE_H

#include <QGraphicsItem>
#include <QPen>

class ATCRunwayExtendedCentreline
{
public:
    explicit ATCRunwayExtendedCentreline(QGraphicsLineItem *line);
    ~ATCRunwayExtendedCentreline();

    QGraphicsLineItem* getCentreline();
    QVector<QGraphicsLineItem*>& getTicksVector();

    void appendTick(QGraphicsLineItem *tick);

    void setCentreline(QGraphicsLineItem *line);
    void setColor(QColor color);

    void show();
    void hide();
    bool isVisible();

private:
    QGraphicsLineItem *centreline = nullptr;
    QVector<QGraphicsLineItem*> ticks;

    bool visible = false;
};

#endif // ATCRUNWAYEXTENDEDCENTRELINE_H
