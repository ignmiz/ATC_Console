#ifndef ATCFLIGHTTAG_H
#define ATCFLIGHTTAG_H

#include <QGraphicsItem>

class ATCFlightTag
{
public:
    explicit ATCFlightTag();
    ~ATCFlightTag();

    QGraphicsRectItem* getDiamond();

    void setDiamond(QGraphicsRectItem *rect);

private:
    QGraphicsRectItem *diamond = nullptr;
};

#endif // ATCFLIGHTTAG_H
