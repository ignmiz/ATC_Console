#ifndef ATCSITUATIONALDISPLAY_H
#define ATCSITUATIONALDISPLAY_H

#include "atcairspace.h"
#include <QGraphicsView>

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
    QWidget *parentWindow;

    ATCAirspace *airspaceData;

    qreal baseScale = 1;
    qreal scaleResolution = 0.1;

    QGraphicsScene *scene;
    QGraphicsRectItem *rect1, *rect2, *rect3, *rect4;
    QGraphicsLineItem *lineH, *lineV;

    void situationalDisplaySetup();
    void loadData(); //Temp - to be moved

    void displayData();
    double mercatorProjectionLong(double longitude, double referenceLongitude = 0, double scale = 1);
    double mercatorProjectionLat(double latitude, double scale = 1);
    void projectSectors(QVector<sector> &targetVector, ATCAirspace *airspace);
    double calculateScaleFactor(double mercatorXmin, double mercatorXmax, double mercatorYmin, double mercatorYmax);
    void displaySectors(QVector<sector> &sectorVector, ATCAirspace *airspace, double centreX, double centreY, double scaleFactor);

protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // ATCSITUATIONALDISPLAY_H
