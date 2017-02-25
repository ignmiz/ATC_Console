
#include "atcruler.h"

ATCRuler::ATCRuler(QPointF p1, ATCSettings *settings, QGraphicsScene *scene, double currentScale, double sctCentreX, double sctCentreY, double scaleFactor) :
    p1(p1),
    settings(settings),
    scene(scene),
    currentScale(currentScale),
    sectorCentreX(sctCentreX),
    sectorCentreY(sctCentreY),
    scaleFactor(scaleFactor),
    ellipsoidGeodesic(GeographicLib::Geodesic::WGS84()),
    ellipsoidRhumb(GeographicLib::Rhumb::WGS84())
{
    constructRuler();
}

ATCRuler::~ATCRuler()
{
    if(line != nullptr) delete line;
    if(circ1 != nullptr) delete circ1;
    if(circ2 != nullptr) delete circ2;
    if(text != nullptr) delete text;
}

void ATCRuler::moveEnd(QPointF end)
{
    double diameter = settings->RULER_END_DIA / currentScale;

    line->setLine(QLineF(p1, end));
    circ2->setRect(end.x() - diameter/2, end.y() - diameter/2, diameter, diameter);

    p2 = end;
    p2geo = ATCMath::local2geo(p2.x(), p2.y(), ATCConst::AVG_DECLINATION, sectorCentreX, sectorCentreY, scaleFactor);

    calculateParameters();
}

void ATCRuler::constructRuler()
{
    p1geo = ATCMath::local2geo(p1.x(), p1.y(), ATCConst::AVG_DECLINATION, sectorCentreX, sectorCentreY, scaleFactor);

    double diameter = settings->RULER_END_DIA / currentScale;
    double circleWidth = settings->RULER_END_LINE_WIDTH / currentScale;
    double lineWidth = settings->RULER_LINE_WIDTH / currentScale;
    double fontHeight = settings->RULER_LABEL_HEIGHT / currentScale;

    etiquetteTemplate = "            |            \n"
                        "      <     |     >      ";

    line = new QGraphicsLineItem(p1.x(), p1.y(), p1.x(), p1.y());
    circ1 = new QGraphicsEllipseItem(p1.x() - diameter/2, p1.y() - diameter/2, diameter, diameter);
    circ2 = new QGraphicsEllipseItem(p1.x() - diameter/2, p1.y() - diameter/2, diameter, diameter);
    text = new QGraphicsSimpleTextItem();

    QPen linePen(settings->RULER_LINE_COLOR);
    linePen.setWidthF(lineWidth);

    QPen circlePen(settings->RULER_END_COLOR);
    circlePen.setWidthF(circleWidth);

    line->setPen(linePen);
    circ1->setPen(circlePen);
    circ2->setPen(circlePen);

    QFont font("Consolas");
    font.setPointSizeF(fontHeight);

    text->setFont(font);
    text->setBrush(QBrush(settings->RULER_LABEL_COLOR));
    text->setPos(p1);

    scene->addItem(line);
    scene->addItem(circ1);
    scene->addItem(circ2);
    scene->addItem(text);
}

void ATCRuler::calculateParameters()
{
    double azi = qRound(ATCMath::rad2deg(ATCMath::normalizeAngle(azimuth(), ATC::Rad)));
    double geodesicDst = ATCMath::m2nm(geodesicDistance());
    double rhumbDst = ATCMath::m2nm(rhumbDistance());

    QString etiquette = etiquetteTemplate;

    QString geodesicStr = "G: " + QString::number(geodesicDst, 'f', 2);
    QString rhumbStr = "R: " + QString::number(rhumbDst, 'f', 2);

    for(int i = 0; i < geodesicStr.size(); i++)
    {
        etiquette[i + 14] = geodesicStr.at(i);
    }

    for(int i = 0; i < rhumbStr.size(); i++)
    {
        etiquette[10 - i] = rhumbStr.at(rhumbStr.size() - 1 - i);
    }

    if(azi <= 180)
    {
        text->setRotation(azi - 90);

        QString aziStr1 = QString::number(azi + 180);
        QString aziStr2 = QString::number(azi);

        if(azi < 10)
        {
            aziStr2 = "00" + aziStr2;
        }
        else if(azi < 100)
        {
            aziStr2 = "0" + aziStr2;
        }

        for(int i = 0; i < 3; i++)
        {
            etiquette[i + 34] = aziStr1.at(i);
            etiquette[i + 40] = aziStr2.at(i);
        }
    }
    else
    {
        text->setRotation(azi + 90);

        QString aziStr1 = QString::number(azi);
        QString aziStr2 = QString::number(azi - 180);

        if(azi - 180 < 10)
        {
            aziStr2 = "00" + aziStr2;
        }
        else if(azi - 180 < 100)
        {
            aziStr2 = "0" + aziStr2;
        }

        for(int i = 0; i < 3; i++)
        {
            etiquette[i + 34] = aziStr1.at(i);
            etiquette[i + 40] = aziStr2.at(i);
        }
    }

    text->setText(etiquette);

    QRectF rect = text->boundingRect();
    double centreX = (p1.x() + p2.x()) / 2;
    double centreY = (p1.y() + p2.y()) / 2;

    QPointF rectCentre(centreX - rect.width()/2, centreY - rect.height()/2);

    text->setPos(rectCentre);
    text->setTransformOriginPoint(rect.width()/2, rect.height()/2);
}

double ATCRuler::azimuth()
{
    return qAtan2(p2.x() - p1.x(), p1.y() - p2.y());
}

double ATCRuler::rhumbDistance()
{
    double dst;
    double trueAzimuth; //not used
    ellipsoidRhumb.Inverse(p1geo.y(), p1geo.x(), p2geo.y(), p2geo.x(), dst, trueAzimuth);

    return dst;
}

double ATCRuler::geodesicDistance()
{
    double dst;
    ellipsoidGeodesic.Inverse(p1geo.y(), p1geo.x(), p2geo.y(), p2geo.x(), dst);

    return dst;
}
