
#include "atcflighttag.h"

ATCFlightTag::ATCFlightTag()
{

}

ATCFlightTag::~ATCFlightTag()
{
    if(diamond != nullptr) delete diamond;
    if(leader != nullptr) delete leader;
    if(tagBox != nullptr) delete tagBox;
    if(connector != nullptr) delete connector;
}

ATC::TagType& ATCFlightTag::getTagType()
{
    return type;
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

ATCTagRect *ATCFlightTag::getTagBox()
{
    return tagBox;
}

QGraphicsLineItem *ATCFlightTag::getConnector()
{
    return connector;
}

double ATCFlightTag::getDX()
{
    return dx;
}

double ATCFlightTag::getDY()
{
    return dy;
}

QGraphicsSimpleTextItem *ATCFlightTag::getText()
{
    return text;
}

void ATCFlightTag::setTagType(ATC::TagType t)
{
    type = t;
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

void ATCFlightTag::setTagBox(ATCTagRect *rect)
{
    tagBox = rect;
}

void ATCFlightTag::setConnector(QGraphicsLineItem *line)
{
    connector = line;
}

void ATCFlightTag::setDX(double deltaX)
{
    dx = deltaX;
}

void ATCFlightTag::setDY(double deltaY)
{
    dy = deltaY;
}

void ATCFlightTag::setText(QGraphicsSimpleTextItem *t)
{
    text = t;
}

void ATCFlightTag::hideTag()
{
    diamond->hide();
    leader->hide();
    tagBox->hide();
    connector->hide();
    text->hide();
}

void ATCFlightTag::showTag()
{
    diamond->show();
    leader->show();
    tagBox->show();
    connector->show();
    text->show();
}
