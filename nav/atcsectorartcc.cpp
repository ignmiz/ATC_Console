#include "atcsectorartcc.h"
#include <QDebug>

ATCSectorARTCC::ATCSectorARTCC(QString name) : ATCAbstractSector(name)
{

}

ATCSectorARTCC::~ATCSectorARTCC()
{
    qDebug() << "Sector ARTCC: " << getName() << " deleted...";
}
