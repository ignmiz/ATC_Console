#ifndef ATCROUTEPREDICTION_H
#define ATCROUTEPREDICTION_H

#include <QGraphicsItem>
#include <QString>


class ATCRoutePrediction
{
public:
    explicit ATCRoutePrediction();
    ~ATCRoutePrediction();

    QGraphicsPolygonItem* getPolygon();
    QVector<QGraphicsSimpleTextItem*>& getLabels();

    void setPolygon(QGraphicsPolygonItem *poly);
    void appendLabel(QGraphicsSimpleTextItem *text);

private:
    QGraphicsPolygonItem *polygon = nullptr;
    QVector<QGraphicsSimpleTextItem*> labels;
};

#endif // ATCROUTEPREDICTION_H
