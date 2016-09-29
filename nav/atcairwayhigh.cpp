#include "atcairwayhigh.h"
#include <QDebug>

ATCAirwayHigh::ATCAirwayHigh(QString name) : ATCAbstractAirway(name)
{

}

ATCAirwayHigh::~ATCAirwayHigh()
{
    qDebug() << "Airway high: " << getName() << "deleted...";
}
