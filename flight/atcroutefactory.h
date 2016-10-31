#ifndef ATCROUTEFACTORY_H
#define ATCROUTEFACTORY_H

#include "atcroute.h"

#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QVector>

class ATCRouteFactory
{
public:
    explicit ATCRouteFactory(QString path);
    ~ATCRouteFactory();

    ATCRoute* getRoute();
    ATCRoute* getRoute(int i);
    ATCRoute* getRoute(QString adep, QString ades);

private:
    QVector<ATCRoute*> routes;

};

#endif // ATCROUTEFACTORY_H
