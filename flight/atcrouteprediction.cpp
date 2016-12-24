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
