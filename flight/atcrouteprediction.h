#ifndef ATCROUTEPREDICTION_H
#define ATCROUTEPREDICTION_H

#include <QGraphicsItem>
#include <QString>

enum class PredictionState
{
    FixNames, FixETA, FixLevels, None
};

class ATCRoutePrediction
{
public:
    explicit ATCRoutePrediction();
    ~ATCRoutePrediction();

    PredictionState getPredictionState();
    void setPredictionState(PredictionState flag);

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
    PredictionState type = PredictionState::None;

    QGraphicsPathItem *polygon = nullptr;
    QVector<QGraphicsSimpleTextItem*> labels;

    QGraphicsEllipseItem *toc = nullptr;
    QGraphicsSimpleTextItem *tocLabel = nullptr;

    QGraphicsEllipseItem *tod = nullptr;
    QGraphicsSimpleTextItem *todLabel = nullptr;
};

#endif // ATCROUTEPREDICTION_H
