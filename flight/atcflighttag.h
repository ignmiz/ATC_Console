#ifndef ATCFLIGHTTAG_H
#define ATCFLIGHTTAG_H

#include <QGraphicsItem>

class ATCFlightTag
{
public:
    explicit ATCFlightTag();
    ~ATCFlightTag();

    QGraphicsRectItem* getDiamond();
    QPointF& getDiamondPosition();
    QGraphicsLineItem* getLeader();
    QPointF& getLeaderEndPosition();

    void setDiamond(QGraphicsRectItem *rect);
    void setDiamondPosition(QPointF &pos);
    void setLeader(QGraphicsLineItem *line);
    void setLeaderEndPosition(QPointF &pos);

private:
    QGraphicsRectItem *diamond = nullptr;
    QPointF diamondPos;

    QGraphicsLineItem *leader = nullptr;
    QPointF leaderEnd;
};

#endif // ATCFLIGHTTAG_H
