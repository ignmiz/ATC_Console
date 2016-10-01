#include "atcairport.h"

#include <QDebug>

ATCAirport::ATCAirport(QString airportName, double latitude, double longitude) : QGeoCoordinate(latitude, longitude), name(airportName)
{
}

ATCAirport::~ATCAirport()
{
    deleteAllRunways();
    if(scenePosition != nullptr) delete scenePosition;
    if(symbol != nullptr) delete symbol;
    if(label = nullptr) delete label;
}

void ATCAirport::deleteAllRunways()
{
    if(!runways.empty())
    {
        for(int i = 0; i < runways.size(); i++)
        {
            delete runways.at(i);
        }
    }

    runways.clear();
}

QString ATCAirport::getName()
{
    return name;
}

QPointF *ATCAirport::getScenePosition()
{
    return scenePosition;
}

QGraphicsEllipseItem *ATCAirport::getSymbol()
{
    return symbol;
}

QGraphicsSimpleTextItem *ATCAirport::getLabel()
{
    return label;
}

ATCRunway *ATCAirport::getRunway(int iterator)
{
    return runways.at(iterator);
}

int ATCAirport::getRunwayVectorSize()
{
    return runways.size();
}

void ATCAirport::setName(QString airportName)
{
    name = airportName;
}

void ATCAirport::setScenePosition(QPointF *position)
{
    scenePosition = position;
}

void ATCAirport::setSymbol(QGraphicsEllipseItem *ellipse)
{
    symbol = ellipse;
}

void ATCAirport::setLabel(QGraphicsSimpleTextItem *text)
{
    label = text;
}

void ATCAirport::setColor(QColor color)
{
    QPen newPen(symbol->pen());
    newPen.setColor(color);

    symbol->setPen(newPen);
}

void ATCAirport::appendRunway(ATCRunway *runway)
{
    runways.append(runway);
}

bool ATCAirport::isRunwayVectorEmpty()
{
    if(runways.isEmpty())
        return true;
    else
        return false;
}

void ATCAirport::show()
{
    if(symbol != nullptr) symbol->show();
    if(label != nullptr) label->show();

    visible = true;
}

void ATCAirport::hide()
{
    if(symbol != nullptr) symbol->hide();
    if(label != nullptr) label->hide();

    visible = false;
}

bool ATCAirport::isVisible()
{
    return visible;
}
