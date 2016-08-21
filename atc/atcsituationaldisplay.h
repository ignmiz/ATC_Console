#ifndef ATCSITUATIONALDISPLAY_H
#define ATCSITUATIONALDISPLAY_H

#include "atcairspace.h"
#include "atcconst.h"
#include "atcsettings.h"

#include <QGraphicsView>
#include <QVector>

struct coord
{
    double x;
    double y;
};

struct sector
{
    QVector<coord> coords;
};

class ATCSituationalDisplay : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ATCSituationalDisplay(QWidget *parent = 0);
    ~ATCSituationalDisplay();

    qreal getBaseScale() const;
    void setBaseScale(qreal scale);

    ATCSettings* getSettings();
    ATCAirspace* getAirspaceData();

public slots:
    void slotSetColorSectorARTCCLow(QColor color);
    void slotSetColorSectorARTCCHigh(QColor color);
    void slotSetColorSectorARTCC(QColor color);

    void slotHideSectorARTCCLow(QString name);
    void slotHideSectorARTCCHigh(QString name);
    void slotHideSectorARTCC(QString name);
    void slotHideFix(QString name);
    void slotHideNDB(QString name);
    void slotHideVOR(QString name);
    void slotHideAirport(QString name);
    void slotHideSID(QString name);
    void slotHideSTAR(QString name);
    void slotHideAirwayLow(QString name);
    void slotHideAirwayHigh(QString name);

    void slotShowSectorARTCCLow(QString name);
    void slotShowSectorARTCCHigh(QString name);
    void slotShowSectorARTCC(QString name);
    void slotShowFix(QString name);
    void slotShowNDB(QString name);
    void slotShowVOR(QString name);
    void slotShowAirport(QString name);
    void slotShowSID(QString name);
    void slotShowSTAR(QString name);
    void slotShowAirwayLow(QString name);
    void slotShowAirwayHigh(QString name);

private:
    ATCAirspace *airspaceData = nullptr;
    ATCSettings *settings = nullptr;

    qreal baseScale = 1;
    qreal scaleResolution = 0.15;
    double scaleFactor;
    double currentScale;

    double sectorCentreX;
    double sectorCentreY;

    QGraphicsScene *scene = nullptr;

    QVector<ATCAirspaceSector*> visibleSectors;
    QVector<ATCSectorARTCCLow*> visibleSectorsARTCCLow;
    QVector<ATCSectorARTCCHigh*> visibleSectorsARTCCHigh;
    QVector<ATCSectorARTCC*> visibleSectorsARTCC;
    QVector<ATCRunwayExtendedCentreline*> visibleCentrelines;
    QVector<ATCNavFix*> visibleFixes;
    QVector<ATCAirport*> visibleAirports;
    QVector<ATCBeaconVOR*> visibleVORs;
    QVector<ATCBeaconNDB*> visibleNDBs;
    QVector<ATCProcedureSTARSymbol*> visibleSTARs;
    QVector<ATCProcedureSIDSymbol*> visibleSIDs;
    QVector<ATCAirwayLow*> visibleLowAirways;
    QVector<ATCAirwayHigh*> visibleHighAirways;

    void situationalDisplaySetup();
    void loadData(); //Temp - to be moved

    void rescaleScene();
    void rescaleSectorsARTCCLow();
    void rescaleSectorsARTCCHigh();
    void rescaleSectorsARTCC();
    void rescaleFixes();
    void rescaleFixLabels();
    void rescaleVORs();
    void rescaleVORLabels();
    void rescaleNDBs();
    void rescaleNDBLabels();
    void rescaleAirports();
    void rescaleAirportLabels();
    void rescaleExtendedCentrelines();
    void rescaleSTARs();
    void rescaleSIDs();
    void rescaleAirwayLow();
    void rescaleAirwayHigh();

    void projectSectorsARTCCLow();
    void projectSectorsARTCCHigh();
    void projectSectorsARTCC();

    void displaySectorsARTCCLow();
    void displaySectorsARTCCHigh();
    void displaySectorsARTCC();
    void displayFixes();
    void displayAirports();
    void displayExtendedCentrelines();
    void displayVORs();
    void displayNDBs();
    void displaySTARs();
    void displaySIDs();
    void displayAirwayLow();
    void displayAirwayHigh();

    double mercatorProjectionLon(double longitude, double referenceLongitude = 0, double scale = ATCConst::WGS84_RADIUS);
    double mercatorProjectionLat(double latitude, double scale = ATCConst::WGS84_RADIUS);

    double rotateX(double coordX, double coordY, double angleDeg);
    double rotateY(double coordX, double coordY, double angleDeg);

    void calculateSectorParameters();
    void connectSlots();

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ATCSITUATIONALDISPLAY_H
