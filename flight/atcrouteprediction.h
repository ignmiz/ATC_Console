#ifndef ATCROUTEPREDICTION_H
#define ATCROUTEPREDICTION_H

#include <QGraphicsItem>
#include <QString>

enum class PredictionType
{
    FixNames, FixETA, FixLevels
};

class ATCRoutePrediction
{
public:
    explicit ATCRoutePrediction();
    ~ATCRoutePrediction();

    PredictionType getPredictionType();
    void setPredictionType(PredictionType flag);

    QGraphicsPathItem* getPolygon();
    QVector<QGraphicsSimpleTextItem*>& getLabels();

    void setPolygon(QGraphicsPathItem *poly);
    void appendLabel(QGraphicsSimpleTextItem *text);

    void hideRoute();
    void showRoute();

private:
    PredictionType type = PredictionType::FixNames;

    QGraphicsPathItem *polygon = nullptr;
    QVector<QGraphicsSimpleTextItem*> labels;


};

#endif // ATCROUTEPREDICTION_H
