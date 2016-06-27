#ifndef ATCNAVFIX_H
#define ATCNAVFIX_H

#include <QGeoCoordinate>
#include <QGraphicsItem>

class ATCNavFix : public QGeoCoordinate
{
public:
    explicit ATCNavFix(QString fixName, double latitude, double longitude);
    ~ATCNavFix();

    QString getName();
    QGraphicsPolygonItem* getSymbol();

    void setName(QString fixName);
    void setSymbol(QGraphicsPolygonItem *fixSymbol);

private:
    QString name;
    QGraphicsPolygonItem *symbol;
};

#endif // ATCNAVFIX_H
