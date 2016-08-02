#ifndef ATCSETTINGS_H
#define ATCSETTINGS_H

#include <QColor>
#include <QFile>
#include <QTextStream>

class ATCSettings
{
public:
    explicit ATCSettings();
    ~ATCSettings();

    void assignDefaultPath();

    QString SETTINGS_DFLT_PATH;
    QString SETTINGS_ACTIVE_PATH;

    QColor ARTCC_LOW_COLOR;
    QColor ARTCC_HIGH_COLOR;
    QColor ARTCC_COLOR;

};

#endif // ATCSETTINGS_H
