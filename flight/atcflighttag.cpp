
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

    if(leader != nullptr)
    {
        delete leader;
    }
}

QGraphicsRectItem* ATCFlightTag::getDiamond()
{
    return diamond;
}

QPointF& ATCFlightTag::getDiamondPosition()
{
    return diamondPos;
}

QGraphicsLineItem *ATCFlightTag::getLeader()
{
    return leader;
}

QPointF &ATCFlightTag::getLeaderEndPosition()
{
    return leaderEnd;
}

void ATCFlightTag::setDiamond(QGraphicsRectItem *rect)
{
    diamond = rect;
}

void ATCFlightTag::setDiamondPosition(QPointF &pos)
{
    diamondPos = pos;
}

void ATCFlightTag::setLeader(QGraphicsLineItem *line)
{
    leader = line;
}

void ATCFlightTag::setLeaderEndPosition(QPointF &pos)
{
    leaderEnd = pos;
}
