#include "atcrouteprediction.h"

ATCRoutePrediction::ATCRoutePrediction()
{

}

ATCRoutePrediction::~ATCRoutePrediction()
{
    if(polygon != nullptr) delete polygon;

    for(int i = 0; i < labels.size(); i++)
    {
       delete labels.at(i);
    }
    labels.clear();
}

PredictionType ATCRoutePrediction::getPredictionType()
{
    return type;
}

void ATCRoutePrediction::setPredictionType(PredictionType flag)
{
    type = flag;
}

QGraphicsPathItem *ATCRoutePrediction::getPolygon()
{
    return polygon;
}

QVector<QGraphicsSimpleTextItem *> &ATCRoutePrediction::getLabels()
{
    return labels;
}

void ATCRoutePrediction::setPolygon(QGraphicsPathItem *poly)
{
    polygon = poly;
}

void ATCRoutePrediction::appendLabel(QGraphicsSimpleTextItem *text)
{
    labels.append(text);
}

void ATCRoutePrediction::hideRoute()
{
    if(polygon != nullptr) polygon->hide();
    for(int i = 0; i < labels.size(); i++)
    {
        labels.at(i)->hide();
    }
}

void ATCRoutePrediction::showRoute()
{
    if(polygon != nullptr) polygon->show();
    for(int i = 0; i < labels.size(); i++)
    {
        labels.at(i)->show();
    }
}
