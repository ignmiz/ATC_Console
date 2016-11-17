#ifndef ATCSETTINGS_H
#define ATCSETTINGS_H

#include "atcpaths.h"

#include <QColor>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

class ATCSettings : public QObject
{
    Q_OBJECT

public:
    explicit ATCSettings(ATCPaths *p);
    ~ATCSettings();

    void exportSymbology(QString path);
    void loadSymbology(QString path);

    bool fileExists(QString path);    

    QString SYMBOLOGY_NAME;
    QString SYMBOLOGY_ACTIVE_PATH;

    QString DISPLAY_NAME;
    QString DISPLAY_ACTIVE_PATH;

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

    double TICK_FIRST_DISTANCE;
    double TICK_SEPARATION;
    double TICK_FIRST_MAJOR_AT;
    double TICK_MAJOR_SEPARATION;
    double TICK_MINOR_LENGTH;
    double TICK_MAJOR_LENGTH;

    QColor STAR_COLOR;
    double STAR_LINE_WIDTH;

    QColor SID_COLOR;
    double SID_LINE_WIDTH;

    QColor AIRWAY_LOW_COLOR;
    double AIRWAY_LOW_LINE_WIDTH;

    QColor AIRWAY_HIGH_COLOR;
    double AIRWAY_HIGH_LINE_WIDTH;

    double TAG_DIAMOND_WIDTH;
    double TAG_DIAMOND_DIA;
    double TAG_DIAMOND_LINE;
    double TAG_LEADER_LENGTH;
    double TAG_LEADER_WIDTH;

signals:
    void signalColorARTCCLow(const QColor &color);
    void signalColorARTCCHigh(const QColor &color);
    void signalColorARTCC(const QColor &color);
    void signalColorVOR(const QColor &color);
    void signalColorNDB(const QColor &color);
    void signalColorFix(const QColor &color);
    void signalColorAirport(const QColor &color);
    void signalColorCentreline(const QColor &color);
    void signalColorSTAR(const QColor &color);
    void signalColorSID(const QColor &color);
    void signalColorAirwayLow(const QColor &color);
    void signalColorAirwayHigh(const QColor &color);

    void signalApplySettings();

private:
    ATCPaths *paths;

    QRgb colorFromString(QString string);

    void loadInitialSymbology(QString path);

    void interpretSymbologyFile(QString path);
};

#endif // ATCSETTINGS_H
