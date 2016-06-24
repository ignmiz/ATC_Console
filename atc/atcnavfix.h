#ifndef ATCNAVFIX_H
#define ATCNAVFIX_H

#include <QGeoCoordinate>

class ATCNavFix : public QGeoCoordinate
{
public:
    explicit ATCNavFix(QString fixName, double latitude, double longitude);
    ~ATCNavFix();

    QString getName();

    void setName(QString fixName);

private:
    QString name;
};

#endif // ATCNAVFIX_H
