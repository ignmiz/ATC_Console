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
    double ARTCC_LOW_LINE_WIDTH;

    QColor ARTCC_HIGH_COLOR;
    double ARTCC_HIGH_LINE_WIDTH;

    QColor ARTCC_COLOR;
    double ARTCC_LINE_WIDTH;

    QColor VOR_COLOR;
    double VOR_LINE_WIDTH;
    double VOR_SIDE_LENGTH;
    double VOR_LABEL_HEIGHT;
    double VOR_LABEL_DX;
    double VOR_LABEL_DY;

    QColor NDB_COLOR;
    double NDB_LINE_WIDTH;
    double NDB_SYMBOL_DIA;
    double NDB_LABEL_HEIGHT;
    double NDB_LABEL_DX;
    double NDB_LABEL_DY;

    QColor FIX_COLOR;
    double FIX_LINE_WIDTH;
    double FIX_SIDE_LENGTH;
    double FIX_LABEL_HEIGHT;
    double FIX_LABEL_DX;
    double FIX_LABEL_DY;

    QColor AIRPORT_COLOR;
    double AIRPORT_LINE_WIDTH;
    double AIRPORT_SYMBOL_DIA;
    double AIRPORT_LABEL_HEIGHT;
    double AIRPORT_LABEL_DX;
    double AIRPORT_LABEL_DY;

    QColor RUNWAY_COLOR;
    double RUNWAY_LINE_WIDTH;
    double RUNWAY_CENTELINE_LENGTH;

    QColor STAR_COLOR;
    double STAR_LINE_WIDTH;

    QColor SID_COLOR;
    double SID_LINE_WITH;

    QColor AIRWAY_LOW_COLOR;
    double AIRWAY_LOW_LINE_WIDTH;

    QColor AIRWAY_HIGH_COLOR;
    double AIRWAY_HIGH_LINE_WIDTH;

signals:
    void signalColorARTCCLow(const QColor &color);
    void signalColorARTCCHigh(const QColor &color);
    void signalColorARTCC(const QColor &color);
    void signalColorVOR(const QColor &color);
    void signalColorNDB(const QColor &color);
    void signalColorFix(const QColor &color);
    void signalColorAirport(const QColor &color);
    void signalColorRunway(const QColor &color);
    void signalColorSTAR(const QColor &color);
    void signalColorSID(const QColor &color);
    void signalColorAirwayLow(const QColor &color);
    void signalColorAirwayHigh(const QColor &color);

private:
    QRgb colorFromString(QString string);

    void assignPaths();
    void loadInitialSettings(QString path);

    void interpretSettingsFile(QString path);
};

#endif // ATCSETTINGS_H
