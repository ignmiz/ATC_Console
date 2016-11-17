
#include "atcflighttag.h"

ATCFlightTag::ATCFlightTag()
{

}

ATCFlightTag::~ATCFlightTag()
{
    if(diamond != nullptr)
    {
        delete diamond;
    }
}

QGraphicsRectItem *ATCFlightTag::getDiamond()
{
    return diamond;
}

void ATCFlightTag::setDiamond(QGraphicsRectItem *rect)
{
    diamond = rect;
}
