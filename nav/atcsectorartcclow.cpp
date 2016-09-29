#include "atcsectorartcclow.h"
#include <QDebug>

ATCSectorARTCCLow::ATCSectorARTCCLow(QString name) : ATCAbstractSector(name)
{

}

ATCSectorARTCCLow::~ATCSectorARTCCLow()
{
    qDebug() << "Sector ARTCC Low: " << getName() << " deleted...";
}
