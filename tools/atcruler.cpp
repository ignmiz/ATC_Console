
#include "atcruler.h"

//GENERAL PROCEDURE:

//create points from scene coords in constructor
//converse points from local to geo
//calculate rhumb distance between geo coords
//calculate geodesic distance between geo cords
//calculate rhumb azimuth based on local coords

//create line
//create end circles
//create text

//append elements to scene

ATCRuler::ATCRuler(QPointF p1, ATCSettings *settings, QGraphicsScene *scene, double *currentScale) :
    p1(p1),
    settings(settings),
    scene(scene),
    currentScale(currentScale)
{
    constructRuler();
}

ATCRuler::~ATCRuler()
{
    if(line != nullptr) delete line;
    if(circ1 != nullptr) delete circ1;
    if(circ2 != nullptr) delete circ2;
    if(text != nullptr) delete text;
}

void ATCRuler::moveEnd(QPointF p2)
{
    double diameter = 8 / *currentScale;      //mockup

    line->setLine(QLineF(p1, p2));
    circ2->setRect(p2.x() - diameter/2, p2.y() - diameter/2, diameter, diameter);
}

void ATCRuler::constructRuler()
{
    double diameter = 8 / *currentScale;       //mockup
    double circleWidth = 1 / *currentScale;    //mockup
    double lineWidth = 1 / *currentScale;      //mockup

    line = new QGraphicsLineItem(p1.x(), p1.y(), p1.x(), p1.y());
    circ1 = new QGraphicsEllipseItem(p1.x() - diameter/2, p1.y() - diameter/2, diameter, diameter);
    circ2 = new QGraphicsEllipseItem(p1.x() - diameter/2, p1.y() - diameter/2, diameter, diameter);

    QPen linePen(Qt::yellow);
    linePen.setWidthF(lineWidth);

    QPen circlePen(Qt::cyan);
    circlePen.setWidthF(circleWidth);

    line->setPen(linePen);
    circ1->setPen(circlePen);
    circ2->setPen(circlePen);

    scene->addItem(line);
    scene->addItem(circ1);
    scene->addItem(circ2);
}
