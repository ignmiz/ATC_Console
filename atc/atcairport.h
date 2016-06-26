#ifndef ATCAIRPORT_H
#define ATCAIRPORT_H

#include "atcrunway.h"

#include <QGeoCoordinate>
#include <QVector>

class ATCAirport : public QGeoCoordinate
{

public:
   ATCAirport(QString airportName, double latitude, double longitude);
   ~ATCAirport();

   void deleteAllRunways();

   QString getName();
   ATCRunway* getRunway(int iterator);

   void setName(QString airportName);
   void appendRunway(ATCRunway *runway);

private:
   QString name;
   QVector<ATCRunway*> runways;
};

#endif // ATCAIRPORT_H
