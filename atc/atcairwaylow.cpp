#include "atcairwaylow.h"
#include <QDebug>

ATCAirwayLow::ATCAirwayLow(QString name) : ATCAbstractAirway(name)
{

}

ATCAirwayLow::~ATCAirwayLow()
{
    qDebug() << "Airway low: " << getName() << "deleted...";
}
