#ifndef ATCRULER_H
#define ATCRULER_H

#include "atcmath.h"
#include "atcsettings.h"

#include <QGraphicsItem>
#include <QGraphicsScene>

class ATCRuler
{
public:
    explicit ATCRuler(QPointF p1, ATCSettings *settings, QGraphicsScene *scene, double currentScale, double sctCentreX, double sctCentreY, double scaleFactor);
    ~ATCRuler();

    void moveEnd(QPointF end);

private:
    ATCSettings *settings;
    QGraphicsScene *scene;
    double currentScale;
    double sectorCentreX;
    double sectorCentreY;
    double scaleFactor;

    QPointF p1;
    QPointF p1geo;

    QPointF p2;
    QPointF p2geo;

    QString etiquetteTemplate;

    QGraphicsLineItem *line = nullptr;
    QGraphicsEllipseItem *circ1 = nullptr;
    QGraphicsEllipseItem *circ2 = nullptr;
    QGraphicsSimpleTextItem *text = nullptr;

    GeographicLib::Geodesic ellipsoidGeodesic;
    GeographicLib::Rhumb ellipsoidRhumb;

    void constructRuler();
    void calculateParameters();

    double azimuth();
    double rhumbDistance();
    double geodesicDistance();
};

#endif // ATCRULER_H
