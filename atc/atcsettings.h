#ifndef ATCSETTINGS_H
#define ATCSETTINGS_H

#include <QColor>
#include <QFile>
#include <QTextStream>

class ATCSettings : public QObject
{
    Q_OBJECT

public:
    explicit ATCSettings();
    ~ATCSettings();

    void setDefaultPath(QString newPath);
    void exportSettings(QString path);
    void loadSettings(QString path);

    bool fileExists(QString path);

    QString SETTINGS_NAME;

    QString SETTINGS_PATHS_FILE = "../../ATC_Console/ATC_Console/config/paths.txt";
    QString SETTINGS_DFLT_PATH;
    QString SETTINGS_EXPORT_PATH;
    QString SETTINGS_ACTIVE_PATH;

    QColor ARTCC_LOW_COLOR;
    QColor ARTCC_HIGH_COLOR;
    QColor ARTCC_COLOR;

signals:
    void signalColorARTCCLow(const QColor &color);
    void signalColorARTCCHigh(const QColor &color);
    void signalColorARTCC(const QColor &color);

private:
    QRgb colorFromString(QString string);

    void assignPaths();
    void loadInitialSettings(QString path);

    void interpretSettingsFile(QString path);
};

#endif // ATCSETTINGS_H
