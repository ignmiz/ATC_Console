#ifndef ATCAMANFLIGHTLABEL_H
#define ATCAMANFLIGHTLABEL_H

#include "atcflight.h"

#include <QPen>
#include <QPolygonF>
#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>

enum class SelectStatus
{
    Inactive, Active
};

class ATCAmanFlightLabel : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    explicit ATCAmanFlightLabel(ATCFlight *flight, QPointF arrowPos);
    ~ATCAmanFlightLabel();

    void addToScene(QGraphicsScene *scene);
    void moveByInterval(double dx, double dy);

    void createTimeRangeBar(QGraphicsScene *scene);
    void createSelector(QGraphicsScene *scene);

    void select();
    void deselect();

    void showLabel();
    void hideLabel();

    bool isSelected();
    bool isHovered();
    bool isLabelVisible();

    QGraphicsLineItem* getTimeArrow();
    QGraphicsLineItem* getConnector();
    QGraphicsSimpleTextItem* getText();
    QGraphicsRectItem* getRangeBar();
    QGraphicsLineItem* getSelector();

signals:
    void signalFlightLabelSelected(ATCAmanFlightLabel *label);
    void signalLabelHovered(bool flag);

private slots:
    void slotValueChanged(int value);

private:
    ATCFlight *flight;

    QGraphicsLineItem *timeArrow = nullptr;
    QGraphicsLineItem *connector = nullptr;
    QGraphicsSimpleTextItem *text = nullptr;

    QGraphicsRectItem *rangeBar = nullptr;
    QGraphicsLineItem *selector = nullptr;

    bool selected = false;
    bool hovered = false;
    bool labelVisible = true;

    void createLabelItems(QPointF arrowPos);

    void moveLine(QPointF newPos);

    void swapLine(QGraphicsLineItem *line);
    void swapPolygon(QGraphicsPolygonItem *polygon);
    void swapText(QGraphicsSimpleTextItem *text);
    void swapRect(QGraphicsRectItem *rect);
    void swapSide();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // ATCAMANFLIGHTLABEL_H
