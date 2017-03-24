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

    void setTOC(QGraphicsEllipseItem *t);
    QGraphicsEllipseItem* getTOC();

    void setLabelTOC(QGraphicsSimpleTextItem *t);
    QGraphicsSimpleTextItem* getLabelTOC();

    void setTOD(QGraphicsEllipseItem *t);
    QGraphicsEllipseItem* getTOD();

    void setLabelTOD(QGraphicsSimpleTextItem *t);
    QGraphicsSimpleTextItem* getLabelTOD();

    void hideRoute();
    void showRoute();

private:
    PredictionType type = PredictionType::FixNames;

    QGraphicsPathItem *polygon = nullptr;
    QVector<QGraphicsSimpleTextItem*> labels;

    QGraphicsEllipseItem *toc = nullptr;
    QGraphicsSimpleTextItem *tocLabel = nullptr;

    QGraphicsEllipseItem *tod = nullptr;
    QGraphicsSimpleTextItem *todLabel = nullptr;
};

#endif // ATCROUTEPREDICTION_H
