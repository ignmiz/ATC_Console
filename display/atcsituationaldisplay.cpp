#include "atcsituationaldisplay.h"
#include "mainwindow.h"
#include "atcconst.h"

#include <QtMath>
#include <QWheelEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

ATCSituationalDisplay::ATCSituationalDisplay(QWidget *parent) :
    QGraphicsView(parent),
    airspaceData(new ATCAirspace("E:/Qt/ATC_Console/ATC_Console/config/EPWW_175_20160428.sct", "E:/Qt/ATC_Console/ATC_Console/config/EPWW_175_20160428.ese")),
    settings(new ATCSettings)
{
    situationalDisplaySetup();
    connectSlots();

    rescaleScene();

    projectSectorsARTCCLow();
    projectSectorsARTCCHigh();
    projectSectorsARTCC();

    calculateSectorParameters();

    calculateSectorsARTCCLow();
    calculateSectorsARTCCHigh();
    calculateSectorsARTCC();
    calculateAirwayLow();
    calculateAirwayHigh();
    calculateSTARs();
    calculateSIDs();
    calculateExtendedCentrelines();
    calculateCentrelineTicks();
    calculateNDBs();
    calculateVORs();
    calculateFixes();
    calculateAirports();

    loadInitialDisplay(settings->DISPLAY_DFLT_PATH);
}

ATCSituationalDisplay::~ATCSituationalDisplay()
{
    if(airspaceData != nullptr) delete airspaceData;
    if(settings != nullptr) delete settings;
    scene->clear();
}

qreal ATCSituationalDisplay::getBaseScale() const
{
    return baseScale;
}

void ATCSituationalDisplay::setBaseScale(qreal scale)
{
    baseScale = scale;
}

ATCSettings* ATCSituationalDisplay::getSettings()
{
    return settings;
}

ATCAirspace *ATCSituationalDisplay::getAirspaceData()
{
    return airspaceData;
}

void ATCSituationalDisplay::exportDisplay(QString path)
{
    QStringList pathElements = path.split("/", QString::KeepEmptyParts);
    QString fileName = pathElements.at(pathElements.size() - 1).trimmed();
    QString nameWithoutExtension = fileName.split(".", QString::KeepEmptyParts).at(0).trimmed();

    QPointF viewportCentre = mapToScene(viewport()->rect().center());

    QTransform viewportTransformation(transform());
    double scaleX = viewportTransformation.m11();
    double scaleY = viewportTransformation.m22();

    QFile file(path);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " + path;
        return;
    }

    QTextStream out(&file);

    out << "[INFO]" << endl;
    out << "NAME = " << nameWithoutExtension << endl;
    out << "CENTRE = " << viewportCentre.x() << ", " << viewportCentre.y() << endl;
    out << "SCALE = " << scaleX << ", " << scaleY << endl;
    out << endl;

    out << "[ARTCC LOW]" << endl;
    for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
    {
        out << visibleSectorsARTCCLow.at(i)->getName() << endl;
    }
    out << endl;

    out << "[ARTCC HIGH]" << endl;
    for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
    {
        out << visibleSectorsARTCCHigh.at(i)->getName() << endl;
    }
    out << endl;

    out << "[ARTCC]" << endl;
    for(int i = 0; i < visibleSectorsARTCC.size(); i++)
    {
        out << visibleSectorsARTCC.at(i)->getName() << endl;
    }
    out << endl;

    out << "[VOR]" << endl;
    for(int i = 0; i < visibleVORs.size(); i++)
    {
        out << visibleVORs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[NDB]" << endl;
    for(int i = 0; i < visibleNDBs.size(); i++)
    {
        out << visibleNDBs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[FIXES]" << endl;
    for(int i = 0; i < visibleFixes.size(); i++)
    {
        out << visibleFixes.at(i)->getName() << endl;
    }
    out << endl;

    out << "[AIRPORT]" << endl;
    for(int i = 0; i < visibleAirports.size(); i++)
    {
        out << visibleAirports.at(i)->getName() << endl;
    }
    out << endl;

    out << "[RUNWAY]" << endl;
    for(int i = 0; i < visibleCentrelines.size(); i++)
    {
        for(int j = 0; j < airspaceData->getAirportsVectorSize(); j++)
        {
            for(int k = 0; k < airspaceData->getAirport(j)->getRunwayVectorSize(); k++)
            {
                if(airspaceData->getAirport(j)->getRunway(k)->getExtendedCentreline1() == visibleCentrelines.at(i))
                {
                    out << airspaceData->getAirport(j)->getName() << " " << airspaceData->getAirport(j)->getRunway(k)->getRunwayID1() << endl;
                }
                else if(airspaceData->getAirport(j)->getRunway(k)->getExtendedCentreline2() == visibleCentrelines.at(i))
                {
                    out << airspaceData->getAirport(j)->getName() << " " << airspaceData->getAirport(j)->getRunway(k)->getRunwayID2() << endl;
                }
            }
        }
    }
    out << endl;

    out << "[STAR]" << endl;
    for(int i = 0; i < visibleSTARs.size(); i++)
    {
        out << visibleSTARs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[SID]" << endl;
    for(int i = 0; i < visibleSIDs.size(); i++)
    {
        out << visibleSIDs.at(i)->getName() << endl;
    }
    out << endl;

    out << "[LOW AIRWAY]" << endl;
    for(int i = 0; i < visibleLowAirways.size(); i++)
    {
        out << visibleLowAirways.at(i)->getName() << endl;
    }
    out << endl;

    out << "[HIGH AIRWAY]" << endl;
    for(int i = 0; i < visibleHighAirways.size(); i++)
    {
        out << visibleHighAirways.at(i)->getName() << endl;
    }

    file.close();
}

void ATCSituationalDisplay::slotSetColorSectorARTCCLow(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        airspaceData->getSectorARTCCLow(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCCHigh(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        airspaceData->getSectorARTCCHigh(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSectorARTCC(QColor color)
{
    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        airspaceData->getSectorARTCC(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorVOR(QColor color)
{
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        airspaceData->getVOR(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorNDB(QColor color)
{
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        airspaceData->getNDB(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorFix(QColor color)
{
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        airspaceData->getFix(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirport(QColor color)
{
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        airspaceData->getAirport(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorCentreline(QColor color)
{
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
        {
            airspaceData->getAirport(i)->getRunway(j)->getExtendedCentreline1()->setColor(color);
            airspaceData->getAirport(i)->getRunway(j)->getExtendedCentreline2()->setColor(color);
        }
    }
}

void ATCSituationalDisplay::slotSetColorSTAR(QColor color)
{
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        airspaceData->getSTARSymbol(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorSID(QColor color)
{
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        airspaceData->getSIDSymbol(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirwayLow(QColor color)
{
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        airspaceData->getAirwayLow(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotSetColorAirwayHigh(QColor color)
{
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        airspaceData->getAirwayHigh(i)->setColor(color);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCCLow(QString name)
{
    ATCSectorARTCCLow *current = airspaceData->findSectorARTCCLow(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCCLow);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCCHigh(QString name)
{
    ATCSectorARTCCHigh *current = airspaceData->findSectorARTCCHigh(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCCHigh);
    }
}

void ATCSituationalDisplay::slotHideSectorARTCC(QString name)
{
    ATCSectorARTCC *current = airspaceData->findSectorARTCC(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSectorsARTCC);
    }
}

void ATCSituationalDisplay::slotHideFix(QString name)
{
    ATCNavFix *current = airspaceData->findFix(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleFixes);
    }
}

void ATCSituationalDisplay::slotHideNDB(QString name)
{
    ATCBeaconNDB *current = airspaceData->findNDB(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleNDBs);
    }
}

void ATCSituationalDisplay::slotHideVOR(QString name)
{
    ATCBeaconVOR *current = airspaceData->findVOR(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleVORs);
    }
}

void ATCSituationalDisplay::slotHideAirport(QString name)
{
    ATCAirport *current = airspaceData->findAirport(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleAirports);
    }
}

void ATCSituationalDisplay::slotHideCentreline(QString name)
{
    QStringList stringList = name.split(" ", QString::SkipEmptyParts);

    QString airportName = stringList.at(0);
    QString rwyID = stringList.at(1);

    ATCRunwayExtendedCentreline *current = airspaceData->findCentreline(airportName, rwyID);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleCentrelines);
    }
}

void ATCSituationalDisplay::slotHideSID(QString name)
{
    ATCProcedureSIDSymbol *current = airspaceData->findSIDSymbol(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSIDs);
    }
}

void ATCSituationalDisplay::slotHideSTAR(QString name)
{
    ATCProcedureSTARSymbol *current = airspaceData->findSTARSymbol(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleSTARs);
    }
}

void ATCSituationalDisplay::slotHideAirwayLow(QString name)
{
    ATCAirwayLow *current = airspaceData->findAirwayLow(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleLowAirways);
    }
}

void ATCSituationalDisplay::slotHideAirwayHigh(QString name)
{
    ATCAirwayHigh *current = airspaceData->findAirwayHigh(name);

    if(current != nullptr)
    {
        current->hide();
        removeFromVisible(current, visibleHighAirways);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCCLow(QString name)
{
    ATCSectorARTCCLow *current = airspaceData->findSectorARTCCLow(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCCLow.append(current);
        rescaleSectorARTCCLow(current);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCCHigh(QString name)
{
    ATCSectorARTCCHigh *current = airspaceData->findSectorARTCCHigh(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCCHigh.append(current);
        rescaleSectorARTCCHigh(current);
    }
}

void ATCSituationalDisplay::slotShowSectorARTCC(QString name)
{
    ATCSectorARTCC *current = airspaceData->findSectorARTCC(name);

    if(current != nullptr)
    {
        current->show();
        visibleSectorsARTCC.append(current);
        rescaleSectorARTCC(current);
    }
}

void ATCSituationalDisplay::slotShowFix(QString name)
{
    ATCNavFix *current = airspaceData->findFix(name);

    if(current != nullptr)
    {
        current->show();
        visibleFixes.append(current);
        rescaleFix(current);
    }
}

void ATCSituationalDisplay::slotShowNDB(QString name)
{
    ATCBeaconNDB *current = airspaceData->findNDB(name);

    if(current != nullptr)
    {
        current->show();
        visibleNDBs.append(current);
        rescaleNDB(current);
    }
}

void ATCSituationalDisplay::slotShowVOR(QString name)
{
    ATCBeaconVOR *current = airspaceData->findVOR(name);

    if(current != nullptr)
    {
        current->show();
        visibleVORs.append(current);
        rescaleVOR(current);
    }
}

void ATCSituationalDisplay::slotShowAirport(QString name)
{
    ATCAirport *current = airspaceData->findAirport(name);

    if(current != nullptr)
    {
        current->show();
        visibleAirports.append(current);
        rescaleAirport(current);
    }
}

void ATCSituationalDisplay::slotShowCentreline(QString name)
{
    QStringList stringList = name.split(" ", QString::SkipEmptyParts);

    QString airportName = stringList.at(0);
    QString rwyID = stringList.at(1);

    ATCRunwayExtendedCentreline *current = airspaceData->findCentreline(airportName, rwyID);

    if(current != nullptr)
    {
        current->show();
        visibleCentrelines.append(current);
        rescaleExtendedCentreline(current);
    }
}

void ATCSituationalDisplay::slotShowSID(QString name)
{
    ATCProcedureSIDSymbol *current = airspaceData->findSIDSymbol(name);

    if(current != nullptr)
    {
        current->show();
        visibleSIDs.append(current);
        rescaleSID(current);
    }
}

void ATCSituationalDisplay::slotShowSTAR(QString name)
{
    ATCProcedureSTARSymbol *current = airspaceData->findSTARSymbol(name);

    if(current != nullptr)
    {
        current->show();
        visibleSTARs.append(current);
        rescaleSTAR(current);
    }
}

void ATCSituationalDisplay::slotShowAirwayLow(QString name)
{
    ATCAirwayLow *current = airspaceData->findAirwayLow(name);

    if(current != nullptr)
    {
        current->show();
        visibleLowAirways.append(current);
        rescaleAirwayLow(current);
    }
}

void ATCSituationalDisplay::slotShowAirwayHigh(QString name)
{
    ATCAirwayHigh *current = airspaceData->findAirwayHigh(name);

    if(current != nullptr)
    {
        current->show();
        visibleHighAirways.append(current);
        rescaleAirwayHigh(current);
    }
}

void ATCSituationalDisplay::slotApplySettings()
{
    rescaleSectorsARTCCLow();
    rescaleSectorsARTCCHigh();
    rescaleSectorsARTCC();
    rescaleFixes();
    rescaleFixLabels();
    rescaleVORs();
    rescaleVORLabels();
    rescaleNDBs();
    rescaleNDBLabels();
    rescaleAirports();
    rescaleAirportLabels();
    rescaleExtendedCentrelines();
    rescaleSTARs();
    rescaleSIDs();
    rescaleAirwaysLow();
    rescaleAirwaysHigh();
}

void ATCSituationalDisplay::situationalDisplaySetup()
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::NoDrag);
    setRenderHint(QPainter::Antialiasing);

    setSceneRect(-0.5 * ATCConst::SCENE_WIDTH, -0.5 * ATCConst::SCENE_HEIGHT, ATCConst::SCENE_WIDTH, ATCConst::SCENE_HEIGHT);

    viewport()->setCursor(Qt::CrossCursor);

    scene = new QGraphicsScene(this);
    setScene(scene);
}

void ATCSituationalDisplay::rescaleAll()
{
    rescaleSectorsARTCCLow();
    rescaleSectorsARTCCHigh();
    rescaleSectorsARTCC();
    rescaleFixes();
    rescaleFixLabels();
    rescaleVORs();
    rescaleVORLabels();
    rescaleNDBs();
    rescaleNDBLabels();
    rescaleAirports();
    rescaleAirportLabels();
    rescaleExtendedCentrelines();
    rescaleSTARs();
    rescaleSIDs();
    rescaleAirwaysLow();
    rescaleAirwaysHigh();
}

void ATCSituationalDisplay::rescaleScene()
{
    qreal scaleX = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);
    qreal scaleY = static_cast<qreal>(ATCConst::SECTOR_SHRINK_FACTOR / ATCConst::SCENE_FACTOR);

    scale(scaleX, scaleY);
    currentScale = scaleX;
}

void ATCSituationalDisplay::rescaleSectorsARTCCLow()
{
    if(!visibleSectorsARTCCLow.empty())
    {
        QPen currentPen(visibleSectorsARTCCLow.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCCLow.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCCLow.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorsARTCCHigh()
{
    if(!visibleSectorsARTCCHigh.empty())
    {
        QPen currentPen(visibleSectorsARTCCHigh.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCCHigh.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCCHigh.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorsARTCC()
{
    if(!visibleSectorsARTCC.empty())
    {
        QPen currentPen(visibleSectorsARTCC.at(0)->getPolygon(0)->pen());
        currentPen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSectorsARTCC.size(); i++)
        {
            for(int j = 0; j < visibleSectorsARTCC.at(i)->getPolygonsVectorSize(); j++)
            {
                visibleSectorsARTCC.at(i)->getPolygon(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleFixes()
{
    if(!visibleFixes.empty())
    {
        qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleFixes.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleFixes.size(); i++)
        {
            QGraphicsPolygonItem *currentPolygonItem = dynamic_cast<QGraphicsPolygonItem*>(visibleFixes.at(i)->getSymbol());
            QPointF *currentPosition = visibleFixes.at(i)->getScenePosition();

            QVector<QPointF> polygonVertex(4);

            QPointF upperVertex(currentPosition->x(), currentPosition->y() - sideLength * qSqrt(3) / 3);
            QPointF lowerLeftVertex(currentPosition->x() - sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);
            QPointF lowerRightVertex(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);

            polygonVertex[0] = upperVertex;
            polygonVertex[1] = lowerLeftVertex;
            polygonVertex[2] = lowerRightVertex;
            polygonVertex[3] = upperVertex;

            QPolygonF symbolPolygon(polygonVertex);

            currentPolygonItem->setPolygon(symbolPolygon);
            currentPolygonItem->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleFixLabels()
{
    if(!visibleFixes.empty())
    {
        QFont textFont(visibleFixes.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleFixes.size(); i++)
        {
            ATCNavFix *currentFix = visibleFixes.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentFix->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentFix->getScenePosition()->x();
            double positionY = currentFix->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                                 positionY + settings->FIX_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleVORs()
{
    if(!visibleVORs.empty())
    {
        qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

        QPen currentPen(visibleVORs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleVORs.size(); i++)
        {
            QGraphicsRectItem *currentRectItem = dynamic_cast<QGraphicsRectItem*>(visibleVORs.at(i)->getSymbol());
            QPointF *currentPosition = visibleVORs.at(i)->getScenePosition();

            QPointF topLeft(currentPosition->x() - sideLength / 2, currentPosition->y() - sideLength / 2);
            QPointF bottomRight(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength / 2);

            QRectF rect(topLeft, bottomRight);

            currentRectItem->setRect(rect);
            currentRectItem->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleVORLabels()
{
    if(!visibleVORs.empty())
    {
        QFont textFont(visibleVORs.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleVORs.size(); i++)
        {
            ATCBeaconVOR *currentVOR = visibleVORs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentVOR->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentVOR->getScenePosition()->x();
            double positionY = currentVOR->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                                 positionY + settings->VOR_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBs()
{
    if(!visibleNDBs.empty())
    {
        QPen currentPen(visibleNDBs.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = dynamic_cast<QGraphicsEllipseItem*>(visibleNDBs.at(i)->getSymbol());
            QPointF *currentPosition = visibleNDBs.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                   settings->NDB_SYMBOL_DIA / currentScale,
                                   settings->NDB_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleNDBLabels()
{
    if(!visibleNDBs.empty())
    {
        QFont textFont(visibleNDBs.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleNDBs.size(); i++)
        {
            ATCBeaconNDB *currentNDB = visibleNDBs.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentNDB->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentNDB->getScenePosition()->x();
            double positionY = currentNDB->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                                 positionY + settings->NDB_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleAirports()
{
    if(!visibleAirports.empty())
    {
        QPen currentPen(visibleAirports.at(0)->getSymbol()->pen());
        currentPen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            QGraphicsEllipseItem *currentSymbol = visibleAirports.at(i)->getSymbol();
            QPointF *currentPosition = visibleAirports.at(i)->getScenePosition();

            currentSymbol->setRect(currentPosition->x() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   currentPosition->y() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                   settings->AIRPORT_SYMBOL_DIA / currentScale,
                                   settings->AIRPORT_SYMBOL_DIA / currentScale);
            currentSymbol->setPen(currentPen);
        }
    }
}

void ATCSituationalDisplay::rescaleAirportLabels()
{
    if(!visibleAirports.empty())
    {
        QFont textFont(visibleAirports.at(0)->getLabel()->font());
        textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

        for(int i = 0; i < visibleAirports.size(); i++)
        {
            ATCAirport *currentAirport = visibleAirports.at(i);
            QGraphicsSimpleTextItem *currentLabel = currentAirport->getLabel();

            currentLabel->setFont(textFont);

            double positionX = currentAirport->getScenePosition()->x();
            double positionY = currentAirport->getScenePosition()->y();

            currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                                 positionY + settings->AIRPORT_LABEL_DY / currentScale);
        }
    }
}

void ATCSituationalDisplay::rescaleExtendedCentrelines()
{
    if(!visibleCentrelines.empty())
    {
        QPen currentPen(visibleCentrelines.at(0)->getCentreline()->pen());
        currentPen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleCentrelines.size(); i++)
        {
            ATCRunwayExtendedCentreline *current(visibleCentrelines.at(i));
            current->getCentreline()->setPen(currentPen);

            QVector<QGraphicsLineItem*> ticks(current->getTicksVector());
            for(int i = 0; i < ticks.size(); i++)
            {
                ticks.at(i)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSTARs()
{
    if(!visibleSTARs.empty())
    {
        QPen currentPen(visibleSTARs.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSTARs.size(); i++)
        {
            for(int j = 0; j < visibleSTARs.at(i)->getCoordsVectorSize(); j++)
            {
                visibleSTARs.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSIDs()
{
    if(!visibleSIDs.empty())
    {
        QPen currentPen(visibleSIDs.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleSIDs.size(); i++)
        {
            for(int j = 0; j < visibleSIDs.at(i)->getCoordsVectorSize(); j++)
            {
                visibleSIDs.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwaysLow()
{
    if(!visibleLowAirways.empty())
    {
        QPen currentPen(visibleLowAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleLowAirways.size(); i++)
        {
            for(int j = 0; j < visibleLowAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleLowAirways.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleAirwaysHigh()
{
    if(!visibleHighAirways.empty())
    {
        QPen currentPen(visibleHighAirways.at(0)->getLine(0)->pen());
        currentPen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

        for(int i = 0; i < visibleHighAirways.size(); i++)
        {
            for(int j = 0; j < visibleHighAirways.at(i)->getCoordsVectorSize(); j++)
            {
                visibleHighAirways.at(i)->getLine(j)->setPen(currentPen);
            }
        }
    }
}

void ATCSituationalDisplay::rescaleSectorARTCCLow(ATCSectorARTCCLow *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSectorARTCCHigh(ATCSectorARTCCHigh *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSectorARTCC(ATCSectorARTCC *object)
{
    QPen currentPen(object->getPolygon(0)->pen());
    currentPen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getPolygonsVectorSize(); i++)
    {
        object->getPolygon(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleFix(ATCNavFix *object)
{
    qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

    QGraphicsPolygonItem *currentPolygonItem = dynamic_cast<QGraphicsPolygonItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    QVector<QPointF> polygonVertex(4);

    QPointF upperVertex(currentPosition->x(), currentPosition->y() - sideLength * qSqrt(3) / 3);
    QPointF lowerLeftVertex(currentPosition->x() - sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);
    QPointF lowerRightVertex(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength * qSqrt(3) / 6);

    polygonVertex[0] = upperVertex;
    polygonVertex[1] = lowerLeftVertex;
    polygonVertex[2] = lowerRightVertex;
    polygonVertex[3] = upperVertex;

    QPolygonF symbolPolygon(polygonVertex);

    currentPolygonItem->setPolygon(symbolPolygon);
    currentPolygonItem->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                         positionY + settings->FIX_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleVOR(ATCBeaconVOR *object)
{
    qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

    QGraphicsRectItem *currentRectItem = dynamic_cast<QGraphicsRectItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    QPointF topLeft(currentPosition->x() - sideLength / 2, currentPosition->y() - sideLength / 2);
    QPointF bottomRight(currentPosition->x() + sideLength / 2, currentPosition->y() + sideLength / 2);

    QRectF rect(topLeft, bottomRight);

    currentRectItem->setRect(rect);
    currentRectItem->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                         positionY + settings->VOR_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleNDB(ATCBeaconNDB *object)
{
    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

    QGraphicsEllipseItem *currentSymbol = dynamic_cast<QGraphicsEllipseItem*>(object->getSymbol());
    QPointF *currentPosition = object->getScenePosition();

    currentSymbol->setRect(currentPosition->x() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                           currentPosition->y() - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                           settings->NDB_SYMBOL_DIA / currentScale,
                           settings->NDB_SYMBOL_DIA / currentScale);
    currentSymbol->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                         positionY + settings->NDB_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleAirport(ATCAirport *object)
{
    QPen currentPen(object->getSymbol()->pen());
    currentPen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

    QGraphicsEllipseItem *currentSymbol = object->getSymbol();
    QPointF *currentPosition = object->getScenePosition();

    currentSymbol->setRect(currentPosition->x() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                           currentPosition->y() - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                           settings->AIRPORT_SYMBOL_DIA / currentScale,
                           settings->AIRPORT_SYMBOL_DIA / currentScale);
    currentSymbol->setPen(currentPen);

    QFont textFont(object->getLabel()->font());
    textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

    QGraphicsSimpleTextItem *currentLabel = object->getLabel();

    currentLabel->setFont(textFont);

    double positionX = object->getScenePosition()->x();
    double positionY = object->getScenePosition()->y();

    currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                         positionY + settings->AIRPORT_LABEL_DY / currentScale);
}

void ATCSituationalDisplay::rescaleExtendedCentreline(ATCRunwayExtendedCentreline *object)
{
    QPen currentPen(object->getCentreline()->pen());
    currentPen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

    object->getCentreline()->setPen(currentPen);

    QVector<QGraphicsLineItem*> ticks(object->getTicksVector());
    for(int i = 0; i < ticks.size(); i++)
    {
        ticks.at(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSTAR(ATCProcedureSTARSymbol *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleSID(ATCProcedureSIDSymbol *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleAirwayLow(ATCAirwayLow *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::rescaleAirwayHigh(ATCAirwayHigh *object)
{
    QPen currentPen(object->getLine(0)->pen());
    currentPen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < object->getCoordsVectorSize(); i++)
    {
        object->getLine(i)->setPen(currentPen);
    }
}

void ATCSituationalDisplay::projectSectorsARTCCLow()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCCLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCCLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCCLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCCLow(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCCLow(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::projectSectorsARTCCHigh()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCCHigh(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCCHigh(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCCHigh(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCCHigh(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCCHigh(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::projectSectorsARTCC()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSectorARTCC(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSectorARTCC(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSectorARTCC(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSectorARTCC(i)->getCoords2(j)->latitude());

            coordsPair projected;

            projected.x1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.y1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            projected.x2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            projected.y2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            airspaceData->getSectorARTCC(i)->appendCoordsPair(projected);
        }
    }
}

void ATCSituationalDisplay::calculateSectorsARTCCLow()
{
    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCCLow(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCCLow(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
       }

    }

//Display ARTCC Low lines on scene
    QPen pen(settings->ARTCC_LOW_COLOR);
    pen.setWidthF(settings->ARTCC_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCLow(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCCLow(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSectorsARTCCHigh()
{
    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCCHigh(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCCHigh(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
        }
    }

//Display ARTCC High lines on scene
    QPen pen(settings->ARTCC_HIGH_COLOR);
    pen.setWidthF(settings->ARTCC_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCCHigh(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCCHigh(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSectorsARTCC()
{
    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        QVector<QPointF> vertexVector;
        bool flagCreateNewPolygon = true;

        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            bool flagLastLineOfCoords;

            if(j == (airspaceData->getSectorARTCC(i)->getCoordsVectorSize() - 1))
            {
                flagLastLineOfCoords = true;
            }
            else
            {
                flagLastLineOfCoords = false;
            }

            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x1;
            currentCoords1.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y1;
            currentCoords2.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x2;
            currentCoords2.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y2;

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

//TEST FOR POLYGON DATA ARRANGEMENT
            if(flagCreateNewPolygon)
            {
                vertexVector.clear();

                QPointF point(currentCoords1.x, currentCoords1.y);
                vertexVector.append(point);

                flagCreateNewPolygon = false;
            }

            QPointF point(currentCoords2.x, currentCoords2.y);
            vertexVector.append(point);

            if(!flagLastLineOfCoords)
            {
                coord nextCoords;

                nextCoords.x = airspaceData->getSectorARTCC(i)->getCoordsPair(j + 1).x1;
                nextCoords.y = airspaceData->getSectorARTCC(i)->getCoordsPair(j + 1).y1;

                if((currentCoords2.x != nextCoords.x) || (currentCoords2.y != nextCoords.y))
                {
                    QPolygonF polygon(vertexVector);
                    airspaceData->getSectorARTCC(i)->appendPolygon(new QGraphicsPolygonItem(polygon));

                    flagCreateNewPolygon = true;
                }
            }
            else
            {
                QPolygonF polygon(vertexVector);
                airspaceData->getSectorARTCC(i)->appendPolygon(new QGraphicsPolygonItem(polygon));
            }
        }
    }

//Display ARTCC lines on scene
    QPen pen(settings->ARTCC_COLOR);
    pen.setWidthF(settings->ARTCC_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getPolygonsVectorSize(); j++)
        {
            QGraphicsPolygonItem *currentSymbol = airspaceData->getSectorARTCC(i)->getPolygon(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        airspaceData->getSectorARTCC(i)->hide();
    }
}

void ATCSituationalDisplay::calculateFixes()
{
    QVector<coord> tempFixes;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of fixes + rotation
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        coord currentFix;

        currentFix.x = mercatorProjectionLon(airspaceData->getFix(i)->longitude());
        currentFix.y = mercatorProjectionLat(airspaceData->getFix(i)->latitude());

        double xRotated = rotateX(currentFix.x, currentFix.y, rotationDeg);
        double yRotated = rotateY(currentFix.x, currentFix.y, rotationDeg);

        currentFix.x = xRotated;
        currentFix.y = yRotated;

        tempFixes.append(currentFix);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        tempFixes[i].x = translateToLocalX(tempFixes.at(i).x);
        tempFixes[i].y = translateToLocalY(tempFixes.at(i).y);

        airspaceData->getFix(i)->setScenePosition(new QPointF(tempFixes.at(i).x, tempFixes.at(i).y));
    }

//Build and position symbol polygons
    int vertexNumber = 3;
    qreal sideLength = settings->FIX_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QVector<QPointF> polygonVertex(vertexNumber + 1);

        QPointF upperVertex(tempFixes.at(i).x, tempFixes.at(i).y - sideLength * qSqrt(3) / 3);
        QPointF lowerLeftVertex(tempFixes.at(i).x - sideLength / 2, tempFixes.at(i).y + sideLength * qSqrt(3) / 6);
        QPointF lowerRightVertex(tempFixes.at(i).x + sideLength / 2, tempFixes.at(i).y + sideLength * qSqrt(3) / 6);

        polygonVertex[0] = upperVertex;
        polygonVertex[1] = lowerLeftVertex;
        polygonVertex[2] = lowerRightVertex;
        polygonVertex[3] = upperVertex;

        QPolygonF symbolPolygon(polygonVertex);
        airspaceData->getFix(i)->setSymbol(new QGraphicsPolygonItem(symbolPolygon));
    }

//Display fix symbols on scene
    QPen pen(settings->FIX_COLOR);
    pen.setWidthF(settings->FIX_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol = airspaceData->getFix(i)->getSymbol();

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->FIX_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getFixesVectorSize(); i++)
    {
        ATCNavFix *currentFix = airspaceData->getFix(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentFix->getName());
        currentFix->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentFix->getScenePosition()->x();
        double positionY = currentFix->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->FIX_LABEL_DX / currentScale,
                             positionY + settings->FIX_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
        currentFix->hide();
    }
}

void ATCSituationalDisplay::calculateAirports()
{
    QVector<coord> tempAirports;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of airports + rotation
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        coord currentAirport;

        currentAirport.x = mercatorProjectionLon(airspaceData->getAirport(i)->longitude());
        currentAirport.y = mercatorProjectionLat(airspaceData->getAirport(i)->latitude());

        double xRotated = rotateX(currentAirport.x, currentAirport.y, rotationDeg);
        double yRotated = rotateY(currentAirport.x, currentAirport.y, rotationDeg);

        currentAirport.x = xRotated;
        currentAirport.y = yRotated;

        tempAirports.append(currentAirport);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        tempAirports[i].x = translateToLocalX(tempAirports.at(i).x);
        tempAirports[i].y = translateToLocalY(tempAirports.at(i).y);

        airspaceData->getAirport(i)->setScenePosition(new QPointF(tempAirports.at(i).x, tempAirports.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        airspaceData->getAirport(i)->setSymbol(new QGraphicsEllipseItem(tempAirports.at(i).x - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        tempAirports.at(i).y - settings->AIRPORT_SYMBOL_DIA / 2 / currentScale,
                                                                        settings->AIRPORT_SYMBOL_DIA / currentScale,
                                                                        settings->AIRPORT_SYMBOL_DIA / currentScale));
    }

//Display airport symbols on scene
    QPen pen(settings->AIRPORT_COLOR);
    pen.setWidthF(settings->AIRPORT_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        QGraphicsEllipseItem *currentSymbol(airspaceData->getAirport(i)->getSymbol());

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->AIRPORT_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        ATCAirport *currentAirport = airspaceData->getAirport(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentAirport->getName());
        currentAirport->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentAirport->getScenePosition()->x();
        double positionY = currentAirport->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->AIRPORT_LABEL_DX / currentScale,
                             positionY + settings->AIRPORT_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
        currentAirport->hide();
    }
}

void ATCSituationalDisplay::calculateExtendedCentrelines()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    QVector<coord> rwyCoords1;
    QVector<coord> rwyCoords2;
    QVector<coord> centrelineEnd1;
    QVector<coord> centrelineEnd2;

//calculate lat and lon of extended centreline ends
    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);

                coord rwy1;
                coord rwy2;

                rwy1.x = currentRunway->getStartPoint().longitude();
                rwy1.y = currentRunway->getStartPoint().latitude();
                rwy2.x = currentRunway->getEndPoint().longitude();
                rwy2.y = currentRunway->getEndPoint().latitude();

                coord centreEnd1;
                coord centreEnd2;

                centreEnd1.x = rwy1.x + settings->RUNWAY_CENTELINE_LENGTH * ATCConst::NM_2_M / (ATCConst::WGS84_RADIUS * qCos(rwy1.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                centreEnd1.y = rwy1.y;
                centreEnd2.x = rwy2.x + settings->RUNWAY_CENTELINE_LENGTH * ATCConst::NM_2_M / (ATCConst::WGS84_RADIUS * qCos(rwy2.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                centreEnd2.y = rwy2.y;

                rwyCoords1.append(rwy1);
                rwyCoords2.append(rwy2);
                centrelineEnd1.append(centreEnd1);
                centrelineEnd2.append(centreEnd2);
            }
        }
    }

//calculate Mercator projection and rotation of points
    for(int i = 0; i < rwyCoords1.size(); i++)
    {
        //Project to 2D
        rwyCoords1[i].x = mercatorProjectionLon(rwyCoords1.at(i).x);
        rwyCoords1[i].y = mercatorProjectionLat(rwyCoords1.at(i).y);

        rwyCoords2[i].x = mercatorProjectionLon(rwyCoords2.at(i).x);
        rwyCoords2[i].y = mercatorProjectionLat(rwyCoords2.at(i).y);

        centrelineEnd1[i].x = mercatorProjectionLon(centrelineEnd1.at(i).x);
        centrelineEnd1[i].y = mercatorProjectionLat(centrelineEnd1.at(i).y);

        centrelineEnd2[i].x = mercatorProjectionLon(centrelineEnd2.at(i).x);
        centrelineEnd2[i].y = mercatorProjectionLat(centrelineEnd2.at(i).y);

        //Translate to local csys and rotate extended centrelines to match runway azimuth
        double deltaProjectedLon1 = centrelineEnd1.at(i).x - rwyCoords1.at(i).x;
        double deltaProjectedLon2 = centrelineEnd2.at(i).x - rwyCoords2.at(i).x;

        double azimuth = qAtan2(rwyCoords2.at(i).x - rwyCoords1.at(i).x, rwyCoords2.at(i).y - rwyCoords1.at(i).y);

        centrelineEnd1[i].x = deltaProjectedLon1 * qCos(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwyCoords1.at(i).x;
        centrelineEnd1[i].y = deltaProjectedLon1 * qSin(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwyCoords1.at(i).y;

        centrelineEnd2[i].x = deltaProjectedLon2 * qCos(ATCConst::PI / 2 - azimuth) + rwyCoords2.at(i).x;
        centrelineEnd2[i].y = deltaProjectedLon2 * qSin(ATCConst::PI / 2 - azimuth) + rwyCoords2.at(i).y;

        //Rotate whole map so up matches magnetic north
        double rwyCoords1xRot = rotateX(rwyCoords1.at(i).x, rwyCoords1.at(i).y, rotationDeg);
        double rwyCoords1yRot = rotateY(rwyCoords1.at(i).x, rwyCoords1.at(i).y, rotationDeg);

        double rwyCoords2xRot = rotateX(rwyCoords2.at(i).x, rwyCoords2.at(i).y, rotationDeg);
        double rwyCoords2yRot = rotateY(rwyCoords2.at(i).x, rwyCoords2.at(i).y, rotationDeg);

        double centrelineEnd1xRot = rotateX(centrelineEnd1.at(i).x, centrelineEnd1.at(i).y, rotationDeg);
        double centrelineEnd1yRot = rotateY(centrelineEnd1.at(i).x, centrelineEnd1.at(i).y, rotationDeg);

        double centrelineEnd2xRot = rotateX(centrelineEnd2.at(i).x, centrelineEnd2.at(i).y, rotationDeg);
        double centrelineEnd2yRot = rotateY(centrelineEnd2.at(i).x, centrelineEnd2.at(i).y, rotationDeg);

        rwyCoords1[i].x = rwyCoords1xRot;
        rwyCoords1[i].y = rwyCoords1yRot;

        rwyCoords2[i].x = rwyCoords2xRot;
        rwyCoords2[i].y = rwyCoords2yRot;

        centrelineEnd1[i].x = centrelineEnd1xRot;
        centrelineEnd1[i].y = centrelineEnd1yRot;

        centrelineEnd2[i].x = centrelineEnd2xRot;
        centrelineEnd2[i].y = centrelineEnd2yRot;
    }

//Translate to local & scene coords
    for(int i = 0; i < rwyCoords1.size(); i++)
    {
        rwyCoords1[i].x = translateToLocalX(rwyCoords1.at(i).x);
        rwyCoords1[i].y = translateToLocalY(rwyCoords1.at(i).y);

        rwyCoords2[i].x = translateToLocalX(rwyCoords2.at(i).x);
        rwyCoords2[i].y = translateToLocalY(rwyCoords2.at(i).y);

        centrelineEnd1[i].x = translateToLocalX(centrelineEnd1.at(i).x);
        centrelineEnd1[i].y = translateToLocalY(centrelineEnd1.at(i).y);

        centrelineEnd2[i].x = translateToLocalX(centrelineEnd2.at(i).x);
        centrelineEnd2[i].y = translateToLocalY(centrelineEnd2.at(i).y);
    }

//Construct and assign symbols
    int k = 0;

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);

                QGraphicsLineItem *centreline1 = new QGraphicsLineItem(rwyCoords1.at(k).x, rwyCoords1.at(k).y, centrelineEnd1.at(k).x, centrelineEnd1.at(k).y);
                QGraphicsLineItem *centreline2 = new QGraphicsLineItem(rwyCoords2.at(k).x, rwyCoords2.at(k).y, centrelineEnd2.at(k).x, centrelineEnd2.at(k).y);

                currentRunway->setExtendedCentreline1(new ATCRunwayExtendedCentreline(centreline1));
                currentRunway->setExtendedCentreline2(new ATCRunwayExtendedCentreline(centreline2));

                k++;
            }
        }
    }

//Display extended centrelines
    QPen pen(settings->RUNWAY_COLOR);
    pen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *currentRunway = airspaceData->getAirport(i)->getRunway(j);
                ATCRunwayExtendedCentreline *currentCentreline1 = currentRunway->getExtendedCentreline1();
                ATCRunwayExtendedCentreline *currentCentreline2 = currentRunway->getExtendedCentreline2();

                currentCentreline1->getCentreline()->setPen(pen);
                currentCentreline2->getCentreline()->setPen(pen);

                scene->addItem(currentCentreline1->getCentreline());
                scene->addItem(currentCentreline2->getCentreline());
            }
        }
    }
}

void ATCSituationalDisplay::calculateCentrelineTicks()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    for(int i = 0; i < airspaceData->getAirportsVectorSize(); i++)
    {
        if(!airspaceData->getAirport(i)->isRunwayVectorEmpty())
        {
            for(int j = 0; j < airspaceData->getAirport(i)->getRunwayVectorSize(); j++)
            {
                ATCRunway *current(airspaceData->getAirport(i)->getRunway(j));

                for(int k = 0; k < (settings->RUNWAY_CENTELINE_LENGTH - settings->TICK_FIRST_DISTANCE) / settings->TICK_SEPARATION + 1; k++)
                {
                    //Initialize runway tresholds
                    coord rwy1;
                    coord rwy2;

                    rwy1.x = current->getStartPoint().longitude();
                    rwy1.y = current->getStartPoint().latitude();
                    rwy2.x = current->getEndPoint().longitude();
                    rwy2.y = current->getEndPoint().latitude();

                    //Calculate position of tick middle
                    coord tick1mid;
                    coord tick2mid;

                    double localScale = qCos((rwy1.y + rwy2.y) / 2 * ATCConst::DEG_2_RAD);

                    tick1mid.x = rwy1.x + (settings->TICK_FIRST_DISTANCE + k * settings->TICK_SEPARATION) * ATCConst::NM_2_M / (ATCConst::WGS84_RADIUS * qCos(rwy1.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                    tick1mid.y = rwy1.y;
                    tick2mid.x = rwy2.x + (settings->TICK_FIRST_DISTANCE + k * settings->TICK_SEPARATION) * ATCConst::NM_2_M / (ATCConst::WGS84_RADIUS * qCos(rwy2.y * ATCConst::DEG_2_RAD)) * ATCConst::RAD_2_DEG;
                    tick2mid.y = rwy2.y;

                    //Project to 2D
                    rwy1.x = mercatorProjectionLon(rwy1.x);
                    rwy1.y = mercatorProjectionLat(rwy1.y);

                    rwy2.x = mercatorProjectionLon(rwy2.x);
                    rwy2.y = mercatorProjectionLat(rwy2.y);

                    tick1mid.x = mercatorProjectionLon(tick1mid.x);
                    tick1mid.y = mercatorProjectionLat(tick1mid.y);

                    tick2mid.x = mercatorProjectionLon(tick2mid.x);
                    tick2mid.y = mercatorProjectionLat(tick2mid.y);

                    //Translate to local csys and rotate to match runway azimuth
                    double deltaTickLon1 = tick1mid.x - rwy1.x;
                    double deltaTickLon2 = tick2mid.x - rwy2.x;

                    double azimuth = qAtan2(rwy2.x - rwy1.x, rwy2.y - rwy1.y);

                    tick1mid.x = deltaTickLon1 * qCos(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwy1.x;
                    tick1mid.y = deltaTickLon1 * qSin(ATCConst::PI / 2 - azimuth + ATCConst::PI) + rwy1.y;

                    tick2mid.x = deltaTickLon2 * qCos(ATCConst::PI / 2 - azimuth) + rwy2.x;
                    tick2mid.y = deltaTickLon2 * qSin(ATCConst::PI / 2 - azimuth) + rwy2.y;

                    //Calculate tick section ends using localScale
                    coord tick1start;
                    coord tick1end;
                    coord tick2start;
                    coord tick2end;

                    double deltaTickLon;

                    if (((k % static_cast<int>(settings->TICK_MAJOR_SEPARATION / settings->TICK_SEPARATION)) == 0) && (k >= (settings->TICK_FIRST_MAJOR_AT - settings->TICK_FIRST_DISTANCE) / settings->TICK_SEPARATION))
                    {
                        deltaTickLon = mercatorProjectionLon(settings->TICK_MAJOR_LENGTH * ATCConst::NM_2_M / 2 / (ATCConst::WGS84_RADIUS * localScale) * ATCConst::RAD_2_DEG);
                    }
                    else
                    {
                        deltaTickLon = mercatorProjectionLon(settings->TICK_MINOR_LENGTH * ATCConst::NM_2_M / 2 / (ATCConst::WGS84_RADIUS * localScale) * ATCConst::RAD_2_DEG);
                    }

                    tick1start.x = deltaTickLon * qCos(ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.x;
                    tick1start.y = deltaTickLon * qSin(ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.y;

                    tick1end.x = deltaTickLon * qCos(2 * ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.x;
                    tick1end.y = deltaTickLon * qSin(2 * ATCConst::PI - azimuth + ATCConst::PI) + tick1mid.y;

                    tick2start.x = deltaTickLon * qCos(ATCConst::PI - azimuth) + tick2mid.x;
                    tick2start.y = deltaTickLon * qSin(ATCConst::PI - azimuth) + tick2mid.y;

                    tick2end.x = deltaTickLon * qCos(2 * ATCConst::PI - azimuth) + tick2mid.x;
                    tick2end.y = deltaTickLon * qSin(2 * ATCConst::PI - azimuth) + tick2mid.y;

                    //Rotate tick centre to match magnetic north
                    coord rwy1rot;
                    coord rwy2rot;
                    coord tick1rot;
                    coord tick2rot;

                    rwy1rot.x = rotateX(rwy1.x, rwy1.y, rotationDeg);
                    rwy1rot.y = rotateY(rwy1.x, rwy1.y, rotationDeg);

                    rwy2rot.x = rotateX(rwy2.x, rwy2.y, rotationDeg);
                    rwy2rot.y = rotateY(rwy2.x, rwy2.y, rotationDeg);

                    tick1rot.x = rotateX(tick1mid.x, tick1mid.y, rotationDeg);
                    tick1rot.y = rotateY(tick1mid.x, tick1mid.y, rotationDeg);

                    tick2rot.x = rotateX(tick2mid.x, tick2mid.y, rotationDeg);
                    tick2rot.y = rotateY(tick2mid.x, tick2mid.y, rotationDeg);

                    rwy1.x = rwy1rot.x;
                    rwy1.y = rwy1rot.y;

                    rwy2.x = rwy2rot.x;
                    rwy2.y = rwy2rot.y;

                    tick1mid.x = tick1rot.x;
                    tick1mid.y = tick1rot.y;

                    tick2mid.x = tick2rot.x;
                    tick2mid.y = tick2rot.y;

                    //Rotate tick ends to match magnetick north
                    coord tick1startRot;
                    coord tick1endRot;
                    coord tick2startRot;
                    coord tick2endRot;

                    tick1startRot.x = rotateX(tick1start.x, tick1start.y, rotationDeg);
                    tick1startRot.y = rotateY(tick1start.x, tick1start.y, rotationDeg);

                    tick1endRot.x = rotateX(tick1end.x, tick1end.y, rotationDeg);
                    tick1endRot.y = rotateY(tick1end.x, tick1end.y, rotationDeg);

                    tick2startRot.x = rotateX(tick2start.x, tick2start.y, rotationDeg);
                    tick2startRot.y = rotateY(tick2start.x, tick2start.y, rotationDeg);

                    tick2endRot.x = rotateX(tick2end.x, tick2end.y, rotationDeg);
                    tick2endRot.y = rotateY(tick2end.x, tick2end.y, rotationDeg);

                    tick1start.x = tick1startRot.x;
                    tick1start.y = tick1startRot.y;

                    tick1end.x = tick1endRot.x;
                    tick1end.y = tick1endRot.y;

                    tick2start.x = tick2startRot.x;
                    tick2start.y = tick2startRot.y;

                    tick2end.x = tick2endRot.x;
                    tick2end.y = tick2endRot.y;

                    //Translate to local scene coords
                    rwy1.x = translateToLocalX(rwy1.x);
                    rwy1.y = translateToLocalY(rwy1.y);

                    rwy2.x = translateToLocalX(rwy2.x);
                    rwy2.y = translateToLocalY(rwy2.y);

                    tick1mid.x = translateToLocalX(tick1mid.x);
                    tick1mid.y = translateToLocalY(tick1mid.y);

                    tick2mid.x = translateToLocalX(tick2mid.x);
                    tick2mid.y = translateToLocalY(tick2mid.y);

                    tick1start.x = translateToLocalX(tick1start.x);
                    tick1start.y = translateToLocalY(tick1start.y);

                    tick1end.x = translateToLocalX(tick1end.x);
                    tick1end.y = translateToLocalY(tick1end.y);

                    tick2start.x = translateToLocalX(tick2start.x);
                    tick2start.y = translateToLocalY(tick2start.y);

                    tick2end.x = translateToLocalX(tick2end.x);
                    tick2end.y = translateToLocalY(tick2end.y);

                    //Construct tick symbols
                    QGraphicsLineItem *tickLine1 = new QGraphicsLineItem(tick1start.x, tick1start.y, tick1end.x, tick1end.y);
                    QGraphicsLineItem *tickLine2 = new QGraphicsLineItem(tick2start.x, tick2start.y, tick2end.x, tick2end.y);

                    current->getExtendedCentreline1()->appendTick(tickLine1);
                    current->getExtendedCentreline2()->appendTick(tickLine2);

                    //Display ticks
                    QPen pen(settings->RUNWAY_COLOR);
                    pen.setWidthF(settings->RUNWAY_LINE_WIDTH / currentScale);

                    tickLine1->setPen(pen);
                    tickLine2->setPen(pen);

                    scene->addItem(tickLine1);
                    scene->addItem(tickLine2);
                }

                current->getExtendedCentreline1()->hide();
                current->getExtendedCentreline2()->hide();
            }
        }
    }
}

void ATCSituationalDisplay::calculateVORs()
{
    QVector<coord> tempVORs;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of vors + rotation
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        coord currentVOR;

        currentVOR.x = mercatorProjectionLon(airspaceData->getVOR(i)->longitude());
        currentVOR.y = mercatorProjectionLat(airspaceData->getVOR(i)->latitude());

        double xRotated = rotateX(currentVOR.x, currentVOR.y, rotationDeg);
        double yRotated = rotateY(currentVOR.x, currentVOR.y, rotationDeg);

        currentVOR.x = xRotated;
        currentVOR.y = yRotated;

        tempVORs.append(currentVOR);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        tempVORs[i].x = translateToLocalX(tempVORs.at(i).x);
        tempVORs[i].y = translateToLocalY(tempVORs.at(i).y);

        airspaceData->getVOR(i)->setScenePosition(new QPointF(tempVORs.at(i).x, tempVORs.at(i).y));
    }

//Build and position symbol polygons
    qreal sideLength = settings->VOR_SIDE_LENGTH / currentScale;

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QPointF topLeft(tempVORs.at(i).x - sideLength / 2, tempVORs.at(i).y - sideLength / 2);
        QPointF bottomRight(tempVORs.at(i).x + sideLength / 2, tempVORs.at(i).y + sideLength / 2);

        QRectF rect(topLeft, bottomRight);
        airspaceData->getVOR(i)->setSymbol(new QGraphicsRectItem(rect));
    }

//Display VOR symbols on scene
    QPen pen(settings->VOR_COLOR);
    pen.setWidthF(settings->VOR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentRect(airspaceData->getVOR(i)->getSymbol());

        currentRect->setPen(pen);
        scene->addItem(currentRect);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->VOR_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getVORsVectorSize(); i++)
    {
        ATCBeaconVOR *currentVOR = airspaceData->getVOR(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentVOR->getName());
        currentVOR->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentVOR->getScenePosition()->x();
        double positionY = currentVOR->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->VOR_LABEL_DX / currentScale,
                             positionY + settings->VOR_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
        currentVOR->hide();
    }
}

void ATCSituationalDisplay::calculateNDBs()
{
    QVector<coord> tempNDBs;
    double rotationDeg = ATCConst::AVG_DECLINATION;

//Mercator projection of NDBs + rotation
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        coord currentNDB;

        currentNDB.x = mercatorProjectionLon(airspaceData->getNDB(i)->longitude());
        currentNDB.y = mercatorProjectionLat(airspaceData->getNDB(i)->latitude());

        double xRotated = rotateX(currentNDB.x, currentNDB.y, rotationDeg);
        double yRotated = rotateY(currentNDB.x, currentNDB.y, rotationDeg);

        currentNDB.x = xRotated;
        currentNDB.y = yRotated;

        tempNDBs.append(currentNDB);
    }

//Translate to local & scene coords
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        tempNDBs[i].x = translateToLocalX(tempNDBs.at(i).x);
        tempNDBs[i].y = translateToLocalY(tempNDBs.at(i).y);

        airspaceData->getNDB(i)->setScenePosition(new QPointF(tempNDBs.at(i).x, tempNDBs.at(i).y));
    }

//Build and position symbol polygons
    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        airspaceData->getNDB(i)->setSymbol(new QGraphicsEllipseItem(tempNDBs.at(i).x - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    tempNDBs.at(i).y - settings->NDB_SYMBOL_DIA / 2 / currentScale,
                                                                    settings->NDB_SYMBOL_DIA / currentScale,
                                                                    settings->NDB_SYMBOL_DIA / currentScale));
    }

//Display NDB symbols on scene
    QPen pen(settings->NDB_COLOR);
    pen.setWidthF(settings->NDB_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        QAbstractGraphicsShapeItem *currentSymbol(airspaceData->getNDB(i)->getSymbol());

        currentSymbol->setPen(pen);
        scene->addItem(currentSymbol);
    }

//Calculate labels
    QBrush textBrush(Qt::white);

    QFont textFont("Arial");
    textFont.setPointSizeF(settings->NDB_LABEL_HEIGHT / currentScale);

    for(int i = 0; i < airspaceData->getNDBsVectorSize(); i++)
    {
        ATCBeaconNDB *currentNDB = airspaceData->getNDB(i);
        QGraphicsSimpleTextItem *currentLabel = new QGraphicsSimpleTextItem(currentNDB->getName());
        currentNDB->setLabel(currentLabel);

        currentLabel->setBrush(textBrush);
        currentLabel->setFont(textFont);

        double positionX = currentNDB->getScenePosition()->x();
        double positionY = currentNDB->getScenePosition()->y();

        currentLabel->setPos(positionX + settings->NDB_LABEL_DX / currentScale,
                             positionY + settings->NDB_LABEL_DY / currentScale);

        scene->addItem(currentLabel);
        currentNDB->hide();
    }
}

void ATCSituationalDisplay::calculateSTARs()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of STAR symbols + rotation
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSTARSymbol(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSTARSymbol(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSTARSymbol(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSTARSymbol(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getSTARSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display STAR symbol lines on scene
    QPen pen(settings->STAR_COLOR);
    pen.setWidthF(settings->STAR_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSTARSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSTARSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSTARSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        airspaceData->getSTARSymbol(i)->hide();
    }
}

void ATCSituationalDisplay::calculateSIDs()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of SID symbols + rotation
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getSIDSymbol(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getSIDSymbol(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getSIDSymbol(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getSIDSymbol(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getSIDSymbol(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display SID symbol lines on scene
    QPen pen(settings->SID_COLOR);
    pen.setWidthF(settings->SID_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getSIDSymbolsVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSIDSymbol(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getSIDSymbol(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);
        }

        airspaceData->getSIDSymbol(i)->hide();
    }
}

void ATCSituationalDisplay::calculateAirwayLow()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of Low Airways + rotation
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getAirwayLow(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display Low Airway lines on scene
    QPen pen(settings->AIRWAY_LOW_COLOR);
    pen.setWidthF(settings->AIRWAY_LOW_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayLow(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayLow(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        airspaceData->getAirwayLow(i)->hide();
    }
}

void ATCSituationalDisplay::calculateAirwayHigh()
{
    double rotationDeg = ATCConst::AVG_DECLINATION;

    struct lineSegments
    {
        QVector<coord> coords1;
        QVector<coord> coords2;
    };

    QVector<lineSegments> lineSegmentsVector;

//Mercator projection of High Airways + rotation
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        lineSegments currentSegment;

        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1;
            coord currentCoords2;

            currentCoords1.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords1(j)->longitude());
            currentCoords1.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords1(j)->latitude());
            currentCoords2.x = mercatorProjectionLon(airspaceData->getAirwayLow(i)->getCoords2(j)->longitude());
            currentCoords2.y = mercatorProjectionLat(airspaceData->getAirwayLow(i)->getCoords2(j)->latitude());

            double xRotated1 = rotateX(currentCoords1.x, currentCoords1.y, rotationDeg);
            double yRotated1 = rotateY(currentCoords1.x, currentCoords1.y, rotationDeg);
            double xRotated2 = rotateX(currentCoords2.x, currentCoords2.y, rotationDeg);
            double yRotated2 = rotateY(currentCoords2.x, currentCoords2.y, rotationDeg);

            currentCoords1.x = xRotated1;
            currentCoords1.y = yRotated1;
            currentCoords2.x = xRotated2;
            currentCoords2.y = yRotated2;

            currentSegment.coords1.append(currentCoords1);
            currentSegment.coords2.append(currentCoords2);
        }

        lineSegmentsVector.append(currentSegment);
    }

//Translate to local & scene coords, build lines
    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        lineSegments currentSegment(lineSegmentsVector.at(i));

        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            coord currentCoords1 = currentSegment.coords1.at(j);
            coord currentCoords2 = currentSegment.coords2.at(j);

            currentCoords1.x = translateToLocalX(currentCoords1.x);
            currentCoords1.y = translateToLocalY(currentCoords1.y);
            currentCoords2.x = translateToLocalX(currentCoords2.x);
            currentCoords2.y = translateToLocalY(currentCoords2.y);

            airspaceData->getAirwayHigh(i)->appendLine(new QGraphicsLineItem(currentCoords1.x, currentCoords1.y,
                                                                             currentCoords2.x, currentCoords2.y));
        }
    }

//Display High Airway lines on scene
    QPen pen(settings->AIRWAY_HIGH_COLOR);
    pen.setWidthF(settings->AIRWAY_HIGH_LINE_WIDTH / currentScale);

    for(int i = 0; i < airspaceData->getAirwayHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getAirwayHigh(i)->getCoordsVectorSize(); j++)
        {
            QGraphicsLineItem *currentSymbol = airspaceData->getAirwayHigh(i)->getLine(j);

            currentSymbol->setPen(pen);
            scene->addItem(currentSymbol);            
        }

        airspaceData->getAirwayHigh(i)->hide();
    }
}

void ATCSituationalDisplay::hideAll()
{
    for(int i = 0; i < visibleSectorsARTCCLow.size(); i++)
    {
        visibleSectorsARTCCLow.at(i)->hide();
    }

    for(int i = 0; i < visibleSectorsARTCCHigh.size(); i++)
    {
        visibleSectorsARTCCHigh.at(i)->hide();
    }

    for(int i = 0; i < visibleSectorsARTCC.size(); i++)
    {
        visibleSectorsARTCC.at(i)->hide();
    }

    for(int i = 0; i < visibleCentrelines.size(); i++)
    {
        visibleCentrelines.at(i)->hide();
    }

    for(int i = 0; i < visibleFixes.size(); i++)
    {
        visibleFixes.at(i)->hide();
    }

    for(int i = 0; i < visibleAirports.size(); i++)
    {
        visibleAirports.at(i)->hide();
    }

    for(int i = 0; i < visibleVORs.size(); i++)
    {
        visibleVORs.at(i)->hide();
    }

    for(int i = 0; i < visibleNDBs.size(); i++)
    {
        visibleNDBs.at(i)->hide();
    }

    for(int i = 0; i < visibleSTARs.size(); i++)
    {
        visibleSTARs.at(i)->hide();
    }

    for(int i = 0; i < visibleSIDs.size(); i++)
    {
        visibleSIDs.at(i)->hide();
    }

    for(int i = 0; i < visibleLowAirways.size(); i++)
    {
        visibleLowAirways.at(i)->hide();
    }

    for(int i = 0; i < visibleHighAirways.size(); i++)
    {
        visibleHighAirways.at(i)->hide();
    }

    visibleSectorsARTCCLow.clear();
    visibleSectorsARTCCHigh.clear();
    visibleSectorsARTCC.clear();
    visibleCentrelines.clear();
    visibleFixes.clear();
    visibleAirports.clear();
    visibleVORs.clear();
    visibleNDBs.clear();
    visibleSTARs.clear();
    visibleSIDs.clear();
    visibleLowAirways.clear();
    visibleHighAirways.clear();
}

void ATCSituationalDisplay::loadDisplay(QString path)
{
    hideAll();
    interpretDisplayFile(path);

    settings->DISPLAY_ACTIVE_PATH = path;
}

void ATCSituationalDisplay::loadInitialDisplay(QString path)
{
    interpretDisplayFile(path);

    settings->DISPLAY_ACTIVE_PATH = path;
}

void ATCSituationalDisplay::interpretDisplayFile(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Error while opening file " << path;
        return;
    }

    bool flagINFO = false;
    bool flagARTCCLow = false;
    bool flagARTCCHigh = false;
    bool flagARTCC = false;
    bool flagVOR = false;
    bool flagNDB = false;
    bool flagFixes = false;
    bool flagAirport = false;
    bool flagRunway = false;
    bool flagSTAR = false;
    bool flagSID = false;
    bool flagLowAirway = false;
    bool flagHighAirway = false;

    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        QString textLine = stream.readLine();
        textLine = textLine.trimmed();

        if(!textLine.isEmpty())
        {
            if(textLine.contains("[INFO]", Qt::CaseInsensitive))
            {
                flagINFO = true;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC LOW]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = true;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC HIGH]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = true;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[ARTCC]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = true;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[VOR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = true;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[NDB]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = true;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[FIXES]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = true;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[AIRPORT]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = true;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[RUNWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = true;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[STAR]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = true;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[SID]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = true;
                flagLowAirway = false;
                flagHighAirway = false;
            }
            else if(textLine.contains("[LOW AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = true;
                flagHighAirway = false;
            }
            else if(textLine.contains("[HIGH AIRWAY]", Qt::CaseInsensitive))
            {
                flagINFO = false;
                flagARTCCLow = false;
                flagARTCCHigh = false;
                flagARTCC = false;
                flagVOR = false;
                flagNDB = false;
                flagFixes = false;
                flagAirport = false;
                flagRunway = false;
                flagSTAR = false;
                flagSID = false;
                flagLowAirway = false;
                flagHighAirway = true;
            }
            else if(flagINFO)
            {
                QStringList stringList = textLine.split("=", QString::SkipEmptyParts);

                if(stringList.at(0).trimmed() == "NAME")
                {
                    settings->DISPLAY_NAME = stringList.at(1).trimmed();
                }
                else if(stringList.at(0).trimmed() == "CENTRE")
                {
                    QStringList commaStringList = stringList.at(1).trimmed().split(",", QString::SkipEmptyParts);

                    double centreX = commaStringList.at(0).trimmed().toDouble();
                    double centreY = commaStringList.at(1).trimmed().toDouble();

                    centerOn(centreX, centreY);
                }
                else if(stringList.at(0).trimmed() == "SCALE")
                {
                    QStringList commaStringList = stringList.at(1).trimmed().split(",", QString::SkipEmptyParts);

                    double scaleX = commaStringList.at(0).trimmed().toDouble();
                    double scaleY = commaStringList.at(1).trimmed().toDouble();

                    QTransform currentTransformation(transform());

                    double m11 = scaleX;
                    double m12 = currentTransformation.m12();
                    double m13 = currentTransformation.m13();
                    double m21 = currentTransformation.m21();
                    double m22 = scaleY;
                    double m23 = currentTransformation.m23();
                    double m31 = currentTransformation.m31();
                    double m32 = currentTransformation.m32();
                    double m33 = currentTransformation.m33();

                    currentTransformation.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
                    setTransform(currentTransformation);

                    currentScale = m11;

                    rescaleAll();
                }

            }
            else if(flagARTCCLow)
            {
                slotShowSectorARTCCLow(textLine);
            }
            else if(flagARTCCHigh)
            {
                slotShowSectorARTCCHigh(textLine);
            }
            else if(flagARTCC)
            {
                slotShowSectorARTCC(textLine);
            }
            else if(flagVOR)
            {
                slotShowVOR(textLine);
            }
            else if(flagNDB)
            {
                slotShowNDB(textLine);
            }
            else if(flagFixes)
            {
                slotShowFix(textLine);
            }
            else if(flagAirport)
            {
                slotShowAirport(textLine);
            }
            else if(flagRunway)
            {
                slotShowCentreline(textLine);
            }
            else if(flagSTAR)
            {
                slotShowSTAR(textLine);
            }
            else if(flagSID)
            {
                slotShowSID(textLine);
            }
            else if(flagLowAirway)
            {
                slotShowAirwayLow(textLine);
            }
            else if(flagHighAirway)
            {
                slotShowAirwayHigh(textLine);
            }
        }
    }

    file.close();
}

double ATCSituationalDisplay::mercatorProjectionLon(double longitudeDeg, double referenceLongitudeDeg, double scale)
{
    return scale * (longitudeDeg - referenceLongitudeDeg);
}

double ATCSituationalDisplay::mercatorProjectionLat(double latitudeDeg, double scale)
{
    return scale * qLn(qTan(ATCConst::PI / 4 + latitudeDeg * ATCConst::DEG_2_RAD / 2)
               * qPow((1 - ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeDeg * ATCConst::DEG_2_RAD)) /
               (1 + ATCConst::WGS84_FIRST_ECCENTRICITY * qSin(latitudeDeg * ATCConst::DEG_2_RAD)) ,
                      ATCConst::WGS84_FIRST_ECCENTRICITY / 2)) * ATCConst::RAD_2_DEG;
}

double ATCSituationalDisplay::rotateX(double coordX, double coordY, double angleDeg)
{
    return coordX * qCos(angleDeg * ATCConst::DEG_2_RAD) - coordY * qSin(angleDeg * ATCConst::DEG_2_RAD);
}

double ATCSituationalDisplay::rotateY(double coordX, double coordY, double angleDeg)
{
    return coordX * qSin(angleDeg * ATCConst::DEG_2_RAD) + coordY * qCos(angleDeg * ATCConst::DEG_2_RAD);
}

double ATCSituationalDisplay::translateToLocalX(double coordX)
{
    return (coordX - sectorCentreX) * scaleFactor;
}

double ATCSituationalDisplay::translateToLocalY(double coordY)
{
    return (-1 * (coordY - sectorCentreY) * scaleFactor);
}

void ATCSituationalDisplay::calculateSectorParameters()
{
    double sectorXmin = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).x1;
    double sectorXmax = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).x1;
    double sectorYmin = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).y1;
    double sectorYmax = airspaceData->getSectorARTCCLow(0)->getCoordsPair(0).y1;

    for(int i = 0; i < airspaceData->getSectorARTCCLowVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCLow(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCCLow(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    for(int i = 0; i < airspaceData->getSectorARTCCHighVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCCHigh(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCCHigh(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    for(int i = 0; i < airspaceData->getSectorARTCCVectorSize(); i++)
    {
        for(int j = 0; j < airspaceData->getSectorARTCC(i)->getCoordsVectorSize(); j++)
        {
            double currentX = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x1;
            double currentY = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y1;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;

            currentX = airspaceData->getSectorARTCC(i)->getCoordsPair(j).x2;
            currentY = airspaceData->getSectorARTCC(i)->getCoordsPair(j).y2;

            if(currentX < sectorXmin)
                sectorXmin = currentX;
            else if(currentX > sectorXmax)
                sectorXmax = currentX;

            if(currentY < sectorYmin)
                sectorYmin = currentY;
            else if(currentY > sectorYmax)
                sectorYmax = currentY;
        }
    }

    sectorCentreX = (sectorXmin + sectorXmax) / 2;
    sectorCentreY = (sectorYmin + sectorYmax) / 2;

    double spanX = sectorXmax - sectorXmin;
    double spanY = sectorYmax - sectorYmin;

    double spanXperPixel = spanX / ATCConst::SCENE_WIDTH;
    double spanYperPixel = spanY / ATCConst::SCENE_HEIGHT;

    if(spanYperPixel >= spanXperPixel)
    {
        scaleFactor = ATCConst::SCENE_HEIGHT / ATCConst::SECTOR_SHRINK_FACTOR / spanY;
    }
    else
    {
        scaleFactor = ATCConst::SCENE_WIDTH / ATCConst::SECTOR_SHRINK_FACTOR / spanX;
    }
}

void ATCSituationalDisplay::connectSlots()
{
    connect(settings, SIGNAL(signalColorARTCCLow(QColor)), this, SLOT(slotSetColorSectorARTCCLow(QColor)));
    connect(settings, SIGNAL(signalColorARTCCHigh(QColor)), this, SLOT(slotSetColorSectorARTCCHigh(QColor)));
    connect(settings, SIGNAL(signalColorARTCC(QColor)), this, SLOT(slotSetColorSectorARTCC(QColor)));
    connect(settings, SIGNAL(signalColorVOR(QColor)), this, SLOT(slotSetColorVOR(QColor)));
    connect(settings, SIGNAL(signalColorNDB(QColor)), this, SLOT(slotSetColorNDB(QColor)));
    connect(settings, SIGNAL(signalColorFix(QColor)), this, SLOT(slotSetColorFix(QColor)));
    connect(settings, SIGNAL(signalColorAirport(QColor)), this, SLOT(slotSetColorAirport(QColor)));
    connect(settings, SIGNAL(signalColorCentreline(QColor)), this, SLOT(slotSetColorCentreline(QColor)));
    connect(settings, SIGNAL(signalColorSTAR(QColor)), this, SLOT(slotSetColorSTAR(QColor)));
    connect(settings, SIGNAL(signalColorSID(QColor)), this, SLOT(slotSetColorSID(QColor)));
    connect(settings, SIGNAL(signalColorAirwayLow(QColor)), this, SLOT(slotSetColorAirwayLow(QColor)));
    connect(settings, SIGNAL(signalColorAirwayHigh(QColor)), this, SLOT(slotSetColorAirwayHigh(QColor)));
}

void ATCSituationalDisplay::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QPoint numDegrees = event->angleDelta();
        qreal increment = (numDegrees.y() / 120) * scaleResolution;

        qreal newScale = baseScale + increment;
        currentScale = currentScale * newScale;

        scale(newScale, newScale);

        rescaleSectorsARTCCLow();
        rescaleSectorsARTCCHigh();
        rescaleSectorsARTCC();
        rescaleFixes();
        rescaleFixLabels();
        rescaleAirports();
        rescaleAirportLabels();
        rescaleExtendedCentrelines();
        rescaleVORs();
        rescaleVORLabels();
        rescaleNDBs();
        rescaleNDBLabels();
        rescaleSTARs();
        rescaleSIDs();
        rescaleAirwaysLow();
        rescaleAirwaysHigh();
    }

    event->accept();
}

void ATCSituationalDisplay::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void ATCSituationalDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        QGraphicsView::mouseMoveEvent(event);
    }
    event->accept();
}

void ATCSituationalDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void ATCSituationalDisplay::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    event->accept();
}

void ATCSituationalDisplay::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        this->setDragMode(QGraphicsView::NoDrag);
        viewport()->setCursor(Qt::CrossCursor);        
    }
    event->accept();
}

template<class T> void ATCSituationalDisplay::removeFromVisible(T *object, QVector<T*> &vector)
{
    for(int i = vector.size() - 1; i >= 0; i--)
    {
        if(object == vector.at(i))
        {
            vector.remove(i);
        }
    }
}
