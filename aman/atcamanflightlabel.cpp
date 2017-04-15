
#include "atcamanflightlabel.h"

ATCAmanFlightLabel::ATCAmanFlightLabel(QPointF arrowPos)
{

}

ATCAmanFlightLabel::~ATCAmanFlightLabel()
{
    if(timeArrow != nullptr) delete timeArrow;
    if(connector != nullptr) delete connector;
    if(polygon != nullptr) delete polygon;
    if(text != nullptr) delete text;
}
