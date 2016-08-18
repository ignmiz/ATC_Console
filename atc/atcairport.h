#ifndef ATCAIRPORT_H
#define ATCAIRPORT_H

#include "atcrunway.h"

#include <QGeoCoordinate>
#include <QGraphicsItem>
#include <QVector>

class ATCAirport : public QGeoCoordinate
{

public:
   ATCAirport(QString airportName, double latitude, double longitude);
   ~ATCAirport();

   void deleteAllRunways();

   QString getName();
   QPointF* getScenePosition();
   QGraphicsEllipseItem* getSymbol();
   QGraphicsSimpleTextItem* getLabel();
   ATCRunway* getRunway(int iterator);
   int getRunwayVectorSize();

   void setName(QString airportName);
   void setScenePosition(QPointF *position);
   void setSymbol(QGraphicsEllipseItem *ellipse);
   void setLabel(QGraphicsSimpleTextItem *text);
   void appendRunway(ATCRunway *runway);

   bool isRunwayVectorEmpty();

   void show();
   void hide();
   bool isVisible();
   void setFlagVisible(bool flag); //TEMPORARY

private:
   QString name;
   QPointF *scenePosition = nullptr;
   QGraphicsEllipseItem *symbol = nullptr;
   QGraphicsSimpleTextItem *label = nullptr;
   QVector<ATCRunway*> runways;

   bool visible = false;
};

#endif // ATCAIRPORT_H
