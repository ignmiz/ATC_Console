#ifndef ATCSITUATIONALDISPLAY_H
#define ATCSITUATIONALDISPLAY_H

#include "atcairspace.h"
#include "atcconst.h"

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

private:
    ATCAirspace *airspaceData = nullptr;

    qreal baseScale = 1;
    qreal scaleResolution = 0.05;
    double scaleFactor;
    double currentScale;

    double sectorCentreX;
    double sectorCentreY;

    QGraphicsScene *scene = nullptr;

    QVector<ATCAirspaceSector*> visibleSectors;
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
    void rescaleSectors();
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

    void displaySectors();
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

    void projectSectors(QVector<sector> &targetVector, ATCAirspace *airspace, double rotationDeg);
    double calculateScaleFactor(double mercatorXmin, double mercatorXmax, double mercatorYmin, double mercatorYmax);
    void calculateSectorPolygons(QVector<sector> &sectorVector, ATCAirspace *airspace, double centreX, double centreY, double scaleFactor);
    void displayOnScene(ATCAirspace *airspace);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ATCSITUATIONALDISPLAY_H
