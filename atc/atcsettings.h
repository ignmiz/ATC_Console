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

    void assignPaths();
    void setDefaultPath(QString newPath);

    void loadInitialSettings(QString path);
    void exportSettings(QString path);

    bool fileExists(QString path);

    QString SETTINGS_NAME;

    QString SETTINGS_PATHS_FILE = "../../ATC_Console/ATC_Console/config/paths.txt";
    QString SETTINGS_DFLT_PATH;
    QString SETTINGS_EXPORT_PATH;
    QString SETTINGS_ACTIVE_PATH;

    QColor ARTCC_LOW_COLOR;
    QColor ARTCC_HIGH_COLOR;
    QColor ARTCC_COLOR;

private:
    QRgb colorFromString(QString string);

};

#endif // ATCSETTINGS_H
