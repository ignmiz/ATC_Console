#ifndef ATCROUTEPREDICTION_H
#define ATCROUTEPREDICTION_H

#include <QGraphicsItem>
#include <QString>


class ATCRoutePrediction
{
public:
    explicit ATCRoutePrediction();
    ~ATCRoutePrediction();

    QGraphicsPathItem* getPolygon();
    QVector<QGraphicsSimpleTextItem*>& getLabels();

    void setPolygon(QGraphicsPathItem *poly);
    void appendLabel(QGraphicsSimpleTextItem *text);

    void hideRoute();
    void showRoute();

private:
    QGraphicsPathItem *polygon = nullptr;
    QVector<QGraphicsSimpleTextItem*> labels;
};

#endif // ATCROUTEPREDICTION_H
