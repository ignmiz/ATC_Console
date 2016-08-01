#ifndef ATCSETTINGS_H
#define ATCSETTINGS_H

#include <QColor>

class ATCSettings
{
public:
    explicit ATCSettings();
    ~ATCSettings();

    QColor ARTCC_LOW_COLOR;
    QColor ARTCC_HIGH_COLOR;
    QColor ARTCC_COLOR;
};

#endif // ATCSETTINGS_H
