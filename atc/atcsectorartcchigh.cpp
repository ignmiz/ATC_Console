#include "atcsectorartcchigh.h"
#include <QDebug>

ATCSectorARTCCHigh::ATCSectorARTCCHigh(QString name) : ATCAbstractSector(name)
{

}

ATCSectorARTCCHigh::~ATCSectorARTCCHigh()
{
    qDebug() << "Sector ARTCC High: " << getName() << " deleted...";
}
