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

    QString getDeparture();
    QString getDestination();
    QStringList getRoute();

private:
    QString departure;
    QStringList route;
    QString destination;

};

#endif // ATCROUTE_H
