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

PredictionState ATCRoutePrediction::getPredictionState()
{
    return type;
}

void ATCRoutePrediction::setPredictionState(PredictionState flag)
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

void ATCRoutePrediction::setTOC(QGraphicsEllipseItem *t)
{
    toc = t;
}

QGraphicsEllipseItem *ATCRoutePrediction::getTOC()
{
    return toc;
}

void ATCRoutePrediction::setLabelTOC(QGraphicsSimpleTextItem *t)
{
    tocLabel = t;
}

QGraphicsSimpleTextItem *ATCRoutePrediction::getLabelTOC()
{
    return tocLabel;
}

void ATCRoutePrediction::setTOD(QGraphicsEllipseItem *t)
{
    tod = t;
}

QGraphicsEllipseItem *ATCRoutePrediction::getTOD()
{
    return tod;
}

void ATCRoutePrediction::setLabelTOD(QGraphicsSimpleTextItem *t)
{
    todLabel = t;
}

QGraphicsSimpleTextItem *ATCRoutePrediction::getLabelTOD()
{
    return todLabel;
}

void ATCRoutePrediction::hideRoute()
{
    if(polygon != nullptr) polygon->hide();
    for(int i = 0; i < labels.size(); i++)
    {
        labels.at(i)->hide();
    }

    if(toc != nullptr) toc->hide();
    if(tocLabel != nullptr) tocLabel->hide();

    if(tod != nullptr) tod->hide();
    if(todLabel != nullptr) todLabel->hide();
}

void ATCRoutePrediction::showRoute()
{
    if(polygon != nullptr) polygon->show();
    for(int i = 0; i < labels.size(); i++)
    {
        labels.at(i)->show();
    }

    if(toc != nullptr) toc->show();
    if(tocLabel != nullptr) tocLabel->show();

    if(tod != nullptr) tod->show();
    if(todLabel != nullptr) todLabel->show();
}
