
#include "atcamanflightlabel.h"

ATCAmanFlightLabel::ATCAmanFlightLabel(ATCFlight *flight, QPointF arrowPos) :
    flight(flight)
{
    createLabelItems(arrowPos);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
}

ATCAmanFlightLabel::~ATCAmanFlightLabel()
{
    if(timeArrow != nullptr) delete timeArrow;
    if(connector != nullptr) delete connector;
    if(text != nullptr) delete text;

    if(rangeBar != nullptr) delete rangeBar;
    if(selector != nullptr) delete selector;
}

void ATCAmanFlightLabel::addToScene(QGraphicsScene *scene)
{
    scene->addItem(timeArrow);
    scene->addItem(connector);
    scene->addItem(this);
}

void ATCAmanFlightLabel::moveByInterval(double dx, double dy)
{
    timeArrow->moveBy(dx, dy);
    this->moveBy(dx, dy);
    if(rangeBar != nullptr) rangeBar->moveBy(dx, dy);
    if(selector != nullptr) selector->moveBy(dx, dy);
}

void ATCAmanFlightLabel::createTimeRangeBar(QGraphicsScene *scene)
{
    QPointF arrow = timeArrow->mapToScene(timeArrow->line().p1());
    int direction = ATCMath::sgn(arrow.x());
    int multiplier = arrow.x() > 0 ? 0 : -1;

    //Here a proper calculation of height should be made based on acft performance

    double oneSecondInterval = ATCConst::AMAN_DISPLAY_HEIGHT / 13 / 5 / 60;

    double lower = 60;
    double upper = 180;

    double height = (lower + upper) * oneSecondInterval; //Lower part + upper part
    double width = 15;

    QPointF topLeft(direction * 15 + multiplier * width, arrow.y() - upper * oneSecondInterval);

    rangeBar = new QGraphicsRectItem(topLeft.x(), topLeft.y(), width, height);
    rangeBar->setBrush(QBrush(QColor(0, 75, 0)));
    rangeBar->setPen(QPen(QColor(0, 75, 0), 2));
    rangeBar->setZValue(-1);

    scene->addItem(rangeBar);
}

void ATCAmanFlightLabel::createSelector(QGraphicsScene *scene)
{
    QPointF arrow = timeArrow->mapToScene(timeArrow->line().p1());
    int direction = ATCMath::sgn(arrow.x());

    //Here an actual value of RTA assignment should be done. If no RTA, then assign current ETA value

    selector = new QGraphicsLineItem(direction * 15, arrow.y(), direction * 30, arrow.y());
    selector->setPen(QPen(QColor(255, 0, 0) , 2));
    selector->setZValue(1);

    scene->addItem(selector);
}

void ATCAmanFlightLabel::select()
{
    setBrush(QColor(45, 45, 45));
    selected = true;
}

void ATCAmanFlightLabel::deselect()
{
    setBrush(QColor(0, 0, 0));

    if(rangeBar != nullptr)
    {
        delete rangeBar;
        rangeBar = nullptr;
    }

    if(selector != nullptr)
    {
        delete selector;
        selector = nullptr;
    }

    selected = false;
}

void ATCAmanFlightLabel::showLabel()
{
    timeArrow->show();
    connector->show();
    this->show();

    labelVisible = true;
}

void ATCAmanFlightLabel::hideLabel()
{
    timeArrow->hide();
    connector->hide();
    this->hide();

    labelVisible = false;
}

bool ATCAmanFlightLabel::isSelected()
{
    return selected;
}

bool ATCAmanFlightLabel::isHovered()
{
    return hovered;
}

bool ATCAmanFlightLabel::isLabelVisible()
{
    return labelVisible;
}

QGraphicsLineItem *ATCAmanFlightLabel::getTimeArrow()
{
    return timeArrow;
}

QGraphicsLineItem *ATCAmanFlightLabel::getConnector()
{
    return connector;
}

QGraphicsSimpleTextItem *ATCAmanFlightLabel::getText()
{
    return text;
}

QGraphicsRectItem *ATCAmanFlightLabel::getRangeBar()
{
    return rangeBar;
}

QGraphicsLineItem *ATCAmanFlightLabel::getSelector()
{
    return selector;
}

void ATCAmanFlightLabel::createLabelItems(QPointF arrowPos)
{
    //Initialize pens & brushes
    QPen linePen(Qt::green);
    linePen.setWidthF(2);

    QPen connectorPen(Qt::gray);
    connectorPen.setWidthF(1);

    QPen labelPen(Qt::green);
    labelPen.setWidthF(1.5);

    QBrush labelBrush(Qt::black);

    //Initialize characteristic lengths
    double timeArrowWidth = 5;
    double connectorWidth = 20;

    //Create connector
    connector = new QGraphicsLineItem(arrowPos.x() + timeArrowWidth, arrowPos.y(), arrowPos.x() + timeArrowWidth + connectorWidth, arrowPos.y());
    connector->setPen(connectorPen);

    //Create time arrow
    timeArrow = new QGraphicsLineItem(arrowPos.x(), arrowPos.y(), arrowPos.x() + timeArrowWidth, arrowPos.y());
    timeArrow->setPen(linePen);

    //Create polygon
    double width = 280;
    double height = 20;
    double labelArrowWidth = 5;

    QVector<QPointF> vertices;

    QPointF pointLabelArrow(arrowPos.x() + timeArrowWidth + connectorWidth, arrowPos.y());
    QPointF pointBottomLeft(pointLabelArrow.x() + labelArrowWidth, pointLabelArrow.y() - height / 2);
    QPointF pointBottomRight(pointBottomLeft.x() + width, pointBottomLeft.y());
    QPointF pointTopRight(pointBottomRight.x(), pointBottomRight.y() + height);
    QPointF pointTopLeft(pointTopRight.x() - width, pointTopRight.y());

    vertices << pointLabelArrow << pointBottomLeft << pointBottomRight << pointTopRight << pointTopLeft;

    setPolygon(QPolygonF(vertices));
    setPen(labelPen);
    setBrush(labelBrush);

    //Create text
    int rtaIndex = 5; //TEMP, HERE STH LIKE flight->getRTAindex();

    QString callsign = (flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber()).left(9).leftJustified(9, ' ');
    QString level;
    QString eta;

    if(flight->hasAccuratePrediction())
    {
        level = flight->getWaypointLevel(rtaIndex);
        eta = flight->getWaypointTime(rtaIndex).isValid() ? flight->getWaypointTime(rtaIndex).toString("HH:mm:ss") : "--:--:--";
    }
    else
    {
        level = "---";
        eta = "--:--:--";
    }

    //CHECK IF IT HAS RTA?
    QString rta = "12:30:40"; //TEMP

    QString label = callsign + "  " + level + "  E" + eta + "  R" + rta;

    text = new QGraphicsSimpleTextItem(label, this);

    QBrush textBrush(Qt::white);
    QFont textFont("Consolas");
    textFont.setPointSizeF(9);

    text->setBrush(textBrush);
    text->setFont(textFont);

    double w = text->boundingRect().width();
    double h = text->boundingRect().height();

    text->setPos(pointLabelArrow.x() + labelArrowWidth + (width - w)/2, pointLabelArrow.y() - h/2);
}

void ATCAmanFlightLabel::moveLine(QPointF newPos)
{
    QPointF timeArrowEnd = timeArrow->line().p2();

    QPointF p1 = timeArrow->mapToScene(timeArrowEnd);
    QPointF p2(connector->line().p2().x(), timeArrowEnd.y() + newPos.y());

    connector->setLine(QLineF(p1, p2));
}

void ATCAmanFlightLabel::swapLine(QGraphicsLineItem *line)
{
    QLineF ln = line->line();

    QPointF newP1(- ln.p1().x(), ln.p1().y());
    QPointF newP2(- ln.p2().x(), ln.p2().y());

    line->setLine(QLineF(newP1, newP2));
}

void ATCAmanFlightLabel::swapPolygon(QGraphicsPolygonItem *polygon)
{
    QPolygonF poly(polygon->polygon());

    for(int i = 0; i < poly.size(); i++)
    {
        QPointF oldPt(poly.at(i));
        QPointF newPt(- oldPt.x(), oldPt.y());

        poly.replace(i, newPt);
    }

    polygon->setPolygon(poly);
}

void ATCAmanFlightLabel::swapText(QGraphicsSimpleTextItem *text)
{
    double width = text->boundingRect().width();
    double direction = ATCMath::sgn(timeArrow->mapToScene(timeArrow->line().p1()).x());
    double value = 2 * qFabs(text->scenePos().x() + width/2);

    text->moveBy(direction * value, 0);
}

void ATCAmanFlightLabel::swapRect(QGraphicsRectItem *rect)
{
    QRectF boundary = rect->rect();

    QPointF newTopLeft(- boundary.topRight().x(), boundary.topRight().y());
    QPointF newBottomRight(- boundary.bottomLeft().x(), boundary.bottomLeft().y());

    rect->setRect(QRectF(newTopLeft, newBottomRight));
}

void ATCAmanFlightLabel::swapSide()
{
    swapLine(timeArrow);
    swapLine(connector);
    swapPolygon(this);
    swapText(text);

    if(rangeBar != nullptr) swapRect(rangeBar);
    if(selector != nullptr) swapLine(selector);
}

QVariant ATCAmanFlightLabel::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange)
    {
        //Assign initial y value
        double yValue = value.toPointF().y();

        //Assign levels of time arrow and polygon
        double timeArrowEndY = timeArrow->line().p2().y();
        double polygonArrowY = timeArrowEndY + yValue;

        //Assign scene limits
        double limit = ATCConst::AMAN_DISPLAY_HEIGHT / 2;

        //Check saturation
        if(polygonArrowY > limit)
        {
            yValue = limit - timeArrowEndY;
        }
        else if(polygonArrowY < - limit)
        {
            yValue = - limit - timeArrowEndY;
        }

        //Move connector and flight label
        moveLine(QPointF(pos().x(), yValue));
        return QPointF(pos().x(), yValue);
    }

    return QGraphicsItem::itemChange(change, value);
}

void ATCAmanFlightLabel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton) swapSide();
    event->accept();
}

void ATCAmanFlightLabel::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(!selected)
        {
            select();
            emit signalFlightLabelSelected(this);
        }
    }

    event->accept();
}

void ATCAmanFlightLabel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hovered = true;
    emit signalLabelHovered(hovered);
    event->accept();
}

void ATCAmanFlightLabel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hovered = false;
    emit signalLabelHovered(hovered);
    event->accept();
}
