#ifndef ATCTAGRECT_H
#define ATCTAGRECT_H

#include "atcconst.h"
#include "atcsettings.h"
#include "atcflags.h"

#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

#include <QDebug>

class ATCTagRect : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit ATCTagRect(double x, double y, double width, double height, ATCSettings *settings, double *scale, ATC::TagType *type);
    ~ATCTagRect();

    QString getShortEtiquette();
    QString getLongEtiquette();

    void setShortEtiquette(QString &string);
    void setLongEtiquette(QString &string);

    void setShort();
    void setLong();

    void setConnector(QGraphicsLineItem *connector);
    void setText(QGraphicsSimpleTextItem *txt);

signals:
    void signalCreateDialogAltitude(QPoint point);

private:
    void moveLine(QPointF newPos);

    ATC::TagType *type;

    QGraphicsLineItem *line;
    QGraphicsSimpleTextItem *text;
    ATCSettings *settings;
    double *scale;

    QString shortEtiquette;
    QString longEtiquette;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // ATCTAGRECT_H
