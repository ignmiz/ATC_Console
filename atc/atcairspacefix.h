#ifndef ATCAIRSPACEFIX_H
#define ATCAIRSPACEFIX_H

#include <QGeoCoordinate>

class ATCAirspaceFix : public QGeoCoordinate
{

public:
    explicit ATCAirspaceFix(double latitude, double longitude);
    ~ATCAirspaceFix();    
};

#endif // ATCAIRSPACEFIX_H
