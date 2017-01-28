#ifndef ATCROUTE_H
#define ATCROUTE_H

#include <QString>
#include <QStringList>

class ATCRoute
{
public:
    explicit ATCRoute();
    explicit ATCRoute(QString dep, QStringList route, QString des);
    explicit ATCRoute(ATCRoute *other);
    explicit ATCRoute(ATCRoute &other);
    ~ATCRoute();

    void setDeparture(QString code);
    void setDestination(QString code);
    void setRoute(QStringList list);
    void setAlternate(QString alt);


    QString getDeparture();
    QString getDestination();
    QString getAlternate();
    QStringList getRoute();

private:
    QString departure;
    QStringList route;
    QString destination;
    QString alternate;

};

#endif // ATCROUTE_H
