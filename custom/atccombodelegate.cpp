
#include "atccombodelegate.h"


ATCComboDelegate::ATCComboDelegate(ATCAirspace *airspace, ATCSimulation *simulation, ATCActiveRunways *runways, DelegateLocation location, QObject *parent) :
    airspace(airspace),
    simulation(simulation),
    runways(runways),
    location(location),
    QStyledItemDelegate(parent)
{
}

ATCComboDelegate::~ATCComboDelegate()
{
}

void ATCComboDelegate::setSimulation(ATCSimulation *sim)
{
    simulation = sim;
}

QWidget *ATCComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *cb = new QComboBox(parent);

    switch(index.column())
    {
        case 5:     //DEP RWY
        {
            ATCAirport *airport = airspace->findAirport(index.model()->index(index.row(), 4).data().toString());

            if(airport != nullptr)
            {
                QStringList runways;

                for(int i = 0; i < airport->getRunwayVectorSize(); i++)
                {
                    runways.append(airport->getRunway(i)->getRunwayID1());
                    runways.append(airport->getRunway(i)->getRunwayID2());
                }

                runways.sort();

                for(int i = 0; i < runways.size(); i++)
                {
                    cb->addItem(runways.at(i));
                }
            }

            cb->addItem("");

            break;
        }

        case 6:     //SID
        {
            QStringList items;
            ATCAirport *airport = airspace->findAirport(index.model()->index(index.row(), 4).data().toString());
            QString rwy = index.model()->index(index.row(), 5).data().toString();

            for(int i = 0; i < airspace->getSIDsVectorSize(); i++)
            {
                ATCProcedureSID *sid = airspace->getSID(i);

                QString airportCode;
                if(airport != nullptr) airportCode = airport->getName();

                if((airportCode == sid->getAirport()) && (rwy == sid->getRunwayID())) items.append(sid->getName());
            }

            items.sort();
            items.append("");

            cb->addItems(items);

            break;
        }

        case 8:     //ARR RWY
        {
            ATCAirport *airport = airspace->findAirport(index.model()->index(index.row(), 7).data().toString());

            if(airport != nullptr)
            {
                QStringList runways;

                for(int i = 0; i < airport->getRunwayVectorSize(); i++)
                {
                    runways.append(airport->getRunway(i)->getRunwayID1());
                    runways.append(airport->getRunway(i)->getRunwayID2());
                }

                runways.sort();

                for(int i = 0; i < runways.size(); i++)
                {
                    cb->addItem(runways.at(i));
                }
            }

            cb->addItem("");

            break;
        }

        case 9:     //STAR
        {
            QStringList items;
            ATCAirport *airport = airspace->findAirport(index.model()->index(index.row(), 7).data().toString());
            QString rwy = index.model()->index(index.row(), 8).data().toString();

            for(int i = 0; i < airspace->getSTARsVectorSize(); i++)
            {
                ATCProcedureSTAR *star = airspace->getSTAR(i);

                QString airportCode;
                if(airport != nullptr) airportCode = airport->getName();

                if((airportCode == star->getAirport()) && (rwy == star->getRunwayID())) items.append(star->getName());
            }

            items.sort();
            items.append("");

            cb->addItems(items);

            break;
        }

        default:    //OTHER COLUMN
            return QStyledItemDelegate::createEditor(parent, option, index);
            break;
    }

    for(int i = 0; i < cb->count(); i++)
    {
        cb->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    connect(cb, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotCloseDelegate(QString)));

    return cb;
}

void ATCComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
    {
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);

        if (cbIndex >= 0) cb->setCurrentIndex(cbIndex);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ATCComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
    {
        model->setData(index, cb->currentText(), Qt::DisplayRole);

        ATCFlight *flight = nullptr;
        switch(location)
        {
            case DelegateLocation::DialogFlight:
                flight = simulation->getFlight(model->index(index.row(), 1).data().toString());
                break;
            case DelegateLocation::DialogFlightList:
                flight = simulation->getFlight(model->index(index.row(), 0).data().toString());
                break;
        }

        QStringList route = flight->getFlightPlan()->getRoute().getRoute();

        if(index.column() == 5)
        {
            QString rwy = index.data().toString();
            QString adep = flight->getFlightPlan()->getRoute().getDeparture();

            QString firstFix;
            if(!route.empty()) firstFix = route.at(0);

            bool found = false;

            for(int i = 0; i < airspace->getSIDsVectorSize(); i++)
            {
                ATCProcedureSID *sid = airspace->getSID(i);
                if((adep == sid->getAirport()) && (rwy == sid->getRunwayID()) && (firstFix == sid->getFixName(sid->getFixListSize() - 1)))
                {
                    model->setData(model->index(index.row(), 6), sid->getName(), Qt::DisplayRole);
                    found = true;
                }
            }

            if(!found) model->setData(model->index(index.row(), 6), "", Qt::DisplayRole);

            flight->setRunwayDeparture(model->index(index.row(), 5).data().toString());
            flight->setSID(model->index(index.row(), 6).data().toString());

            //Rebuild fix list
            QStringList fixList;
            ATCProcedureSID *sid = airspace->findSID(flight->getSID());
            ATCProcedureSTAR *star = airspace->findSTAR(flight->getSTAR());

            fixList.append(flight->getFlightPlan()->getRoute().getDeparture());

            if(sid != nullptr)
            {
                for(int i = 0; i < sid->getFixListSize(); i++)
                {
                    if(sid->getFixName(i) != route.at(0)) fixList.append(sid->getFixName(i));
                }
            }

            for(int i = 0; i < flight->getMainFixList().size(); i++)
            {
                fixList.append(flight->getMainFixList().at(i));
            }

            if(star != nullptr)
            {
                for(int i = 0; i < star->getFixListSize(); i++)
                {
                    if(star->getFixName(i) != route.at(route.size() - 1)) fixList.append(star->getFixName(i));
                }
            }

            fixList.append(flight->getFlightPlan()->getRoute().getDestination());
            if(!flight->getFlightPlan()->getRoute().getAlternate().isEmpty()) fixList.append(flight->getFlightPlan()->getRoute().getAlternate());

            flight->setFixList(fixList);

            //Rebuild waypoints list
            flight->clearWaypoints();
            fixList = flight->getFixList();
            for(int i = 0; i < fixList.size(); i++)
            {
                ATCNavFix *fix = nullptr;
                ATCBeaconVOR *vor = nullptr;
                ATCAirport *airport = nullptr;
                ATCBeaconNDB *ndb = nullptr;

                double lat;
                double lon;

                double x;
                double y;

                if((fix = airspace->findFix(fixList.at(i))) != nullptr)
                {
                    lat = fix->latitude();
                    lon = fix->longitude();
                    x = fix->getScenePosition()->x();
                    y = fix->getScenePosition()->y();
                }
                else if((airport = airspace->findAirport(fixList.at(i))) != nullptr)
                {
                    lat = airport->latitude();
                    lon = airport->longitude();
                    x = airport->getScenePosition()->x();
                    y = airport->getScenePosition()->y();
                }
                else if((vor = airspace->findVOR(fixList.at(i))) != nullptr)
                {
                    lat = vor->latitude();
                    lon = vor->longitude();
                    x = vor->getScenePosition()->x();
                    y = vor->getScenePosition()->y();
                }
                else if((ndb = airspace->findNDB(fixList.at(i))) != nullptr)
                {
                    lat = ndb->latitude();
                    lon = ndb->longitude();
                    x = ndb->getScenePosition()->x();
                    y = ndb->getScenePosition()->y();
                }

                flight->appendWaypoint(QPair<double, double>(lat, lon));
                flight->appendProjectedWaypoint(QPair<double, double>(x, y));
                if(fixList.at(i) == flight->getNextFix())
                {
                    flight->setWaypointIndex(i);
                }
                else if(i == fixList.size() - 1) flight->setWaypointIndex(-1);
            }

            //Check if waypoint index found. If not, change mode to heading & update tag
            if(flight->getWaypointIndex() == -1)
            {
                flight->setNavMode(ATC::Hdg);
                flight->setHdgRestriction(ATCMath::normalizeAngle(ATCMath::rad2deg(flight->getState().hdg) - ATCConst::AVG_DECLINATION, ATC::Deg));
                flight->setDCT(false);

                QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
                QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

                QString headingString = QString::number(flight->getHdgRestriction());
                for(int i = 0; i < headingString.size(); i++)
                {
                    longEtiquette[i + 39] = headingString.at(i);
                }

                if(flight->getFlightTag()->getTagType() == ATC::Short)
                {
                    for(int i = 0; i < 5; i++)
                    {
                        shortEtiquette[i + 24] = QChar(' ');
                        longEtiquette[i + 24] = QChar(' ');
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->rectShort2Long();
                    flight->getFlightTag()->setTagType(ATC::Full);
                    flight->getFlightTag()->getTagBox()->setLong();

                    flight->setNavMode(ATC::Hdg);
                }
                else
                {
                    if(flight->getNavMode() == ATC::Nav)
                    {
                        for(int i = 0; i < 5; i++)
                        {
                            shortEtiquette[i + 24] = QChar(' ');
                            longEtiquette[i + 24] = QChar(' ');
                        }

                        flight->setNavMode(ATC::Hdg);
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->setLong();
                }

                if(flight->isFinalApp())
                {
                    flight->setCldFinalApp(false);
                    flight->setFinalApp(false);
                    flight->setGlidePath(false);
                }

            }

            //Repaint route prediction
            if(flight->getRoutePrediction() != nullptr)
            {
                flight->slotDisplayRoute();     //First call deletes existing route prediction
                flight->slotDisplayRoute();     //Second call creates new, updated route prediction
            }
        }
        else if(index.column() == 6)
        {
            flight->setSID(model->index(index.row(), 6).data().toString());

            //Rebuild fix list
            QStringList fixList;
            ATCProcedureSID *sid = airspace->findSID(flight->getSID());
            ATCProcedureSTAR *star = airspace->findSTAR(flight->getSTAR());

            fixList.append(flight->getFlightPlan()->getRoute().getDeparture());

            if(sid != nullptr)
            {
                for(int i = 0; i < sid->getFixListSize(); i++)
                {
                    if(sid->getFixName(i) != route.at(0)) fixList.append(sid->getFixName(i));
                }
            }

            for(int i = 0; i < flight->getMainFixList().size(); i++)
            {
                fixList.append(flight->getMainFixList().at(i));
            }

            if(star != nullptr)
            {
                for(int i = 0; i < star->getFixListSize(); i++)
                {
                    if(star->getFixName(i) != route.at(route.size() - 1)) fixList.append(star->getFixName(i));
                }
            }

            fixList.append(flight->getFlightPlan()->getRoute().getDestination());
            if(!flight->getFlightPlan()->getRoute().getAlternate().isEmpty()) fixList.append(flight->getFlightPlan()->getRoute().getAlternate());

            flight->setFixList(fixList);

            //Rebuild waypoints list
            flight->clearWaypoints();
            fixList = flight->getFixList();
            for(int i = 0; i < fixList.size(); i++)
            {
                ATCNavFix *fix = nullptr;
                ATCBeaconVOR *vor = nullptr;
                ATCAirport *airport = nullptr;
                ATCBeaconNDB *ndb = nullptr;

                double lat;
                double lon;

                double x;
                double y;

                if((fix = airspace->findFix(fixList.at(i))) != nullptr)
                {
                    lat = fix->latitude();
                    lon = fix->longitude();
                    x = fix->getScenePosition()->x();
                    y = fix->getScenePosition()->y();
                }
                else if((airport = airspace->findAirport(fixList.at(i))) != nullptr)
                {
                    lat = airport->latitude();
                    lon = airport->longitude();
                    x = airport->getScenePosition()->x();
                    y = airport->getScenePosition()->y();
                }
                else if((vor = airspace->findVOR(fixList.at(i))) != nullptr)
                {
                    lat = vor->latitude();
                    lon = vor->longitude();
                    x = vor->getScenePosition()->x();
                    y = vor->getScenePosition()->y();
                }
                else if((ndb = airspace->findNDB(fixList.at(i))) != nullptr)
                {
                    lat = ndb->latitude();
                    lon = ndb->longitude();
                    x = ndb->getScenePosition()->x();
                    y = ndb->getScenePosition()->y();
                }

                flight->appendWaypoint(QPair<double, double>(lat, lon));
                flight->appendProjectedWaypoint(QPair<double, double>(x, y));
                if(fixList.at(i) == flight->getNextFix())
                {
                    flight->setWaypointIndex(i);
                }
                else if(i == fixList.size() - 1) flight->setWaypointIndex(-1);
            }

            //Check if waypoint index found. If not, change mode to heading & update tag
            if(flight->getWaypointIndex() == -1)
            {
                flight->setNavMode(ATC::Hdg);
                flight->setHdgRestriction(ATCMath::normalizeAngle(ATCMath::rad2deg(flight->getState().hdg) - ATCConst::AVG_DECLINATION, ATC::Deg));
                flight->setDCT(false);

                QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
                QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

                QString headingString = QString::number(flight->getHdgRestriction());
                for(int i = 0; i < headingString.size(); i++)
                {
                    longEtiquette[i + 39] = headingString.at(i);
                }

                if(flight->getFlightTag()->getTagType() == ATC::Short)
                {
                    for(int i = 0; i < 5; i++)
                    {
                        shortEtiquette[i + 24] = QChar(' ');
                        longEtiquette[i + 24] = QChar(' ');
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->rectShort2Long();
                    flight->getFlightTag()->setTagType(ATC::Full);
                    flight->getFlightTag()->getTagBox()->setLong();

                    flight->setNavMode(ATC::Hdg);
                }
                else
                {
                    if(flight->getNavMode() == ATC::Nav)
                    {
                        for(int i = 0; i < 5; i++)
                        {
                            shortEtiquette[i + 24] = QChar(' ');
                            longEtiquette[i + 24] = QChar(' ');
                        }

                        flight->setNavMode(ATC::Hdg);
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->setLong();
                }

                if(flight->isFinalApp())
                {
                    flight->setCldFinalApp(false);
                    flight->setFinalApp(false);
                    flight->setGlidePath(false);
                }

            }

            //Repaint route prediction
            if(flight->getRoutePrediction() != nullptr)
            {
                flight->slotDisplayRoute();     //First call deletes existing route prediction
                flight->slotDisplayRoute();     //Second call creates new, updated route prediction
            }
        }
        else if(index.column() == 8)
        {
            QString rwy = index.data().toString();
            QString ades = flight->getFlightPlan()->getRoute().getDestination();

            QString lastFix;
            if(!route.empty()) lastFix = route.at(route.size() - 1);

            bool found = false;

            for(int i = 0; i < airspace->getSTARsVectorSize(); i++)
            {
                ATCProcedureSTAR *star = airspace->getSTAR(i);
                if((ades == star->getAirport()) && (rwy == star->getRunwayID()) && (lastFix == star->getFixName(0)))
                {
                    model->setData(model->index(index.row(), 9), star->getName(), Qt::DisplayRole);
                    found = true;
                }
            }

            if(!found) model->setData(model->index(index.row(), 9), "", Qt::DisplayRole);

            flight->setRunwayDestination(model->index(index.row(), 8).data().toString());
            flight->setSTAR(model->index(index.row(), 9).data().toString());

            if(flight->isCldFinalApp() || flight->isFinalApp())
            {
                flight->setCldFinalApp(false);
                flight->setFinalApp(false);
            }

            //Update temp data
            if(!flight->getRunwayDestination().isEmpty())
            {
                ATCRunway *runway = airspace->findRunway(flight->getFlightPlan()->getRoute().getDestination(), flight->getRunwayDestination());

                if(runway != nullptr)
                {
                    Temp temp = flight->getTemp();

                    double thrLat;
                    double thrLon;
                    double azimuth;

                    if(flight->getRunwayDestination().left(2).toInt() <= 18)
                    {
                        thrLat = runway->getStartPoint().latitude();
                        thrLon = runway->getStartPoint().longitude();
                        azimuth = runway->getAzimuth();
                    }
                    else
                    {
                        thrLat = runway->getEndPoint().latitude();
                        thrLon = runway->getEndPoint().longitude();
                        azimuth = ATCMath::normalizeAngle(runway->getAzimuth() + ATCConst::PI, ATC::Deg);
                    }

                    temp.rwyDesThr = QPair<double, double>(thrLat, thrLon);
                    temp.rwyDesAzimuth = azimuth;

                    double rangeLat;
                    double rangeLon;
                    GeographicLib::Rhumb rhumb = GeographicLib::Rhumb::WGS84();
                    rhumb.Direct(thrLat, thrLon, ATCMath::rad2deg(ATCMath::normalizeAngle(azimuth + ATCConst::PI, ATC::Deg)), ATCConst::APP_RANGE, rangeLat, rangeLon);
                    temp.rwyDesAppRange = QPair<double, double>(rangeLat, rangeLon);

                    flight->setTemp(temp);
                }
            }

            //Rebuild fix list
            QStringList fixList;
            ATCProcedureSID *sid = airspace->findSID(flight->getSID());
            ATCProcedureSTAR *star = airspace->findSTAR(flight->getSTAR());

            fixList.append(flight->getFlightPlan()->getRoute().getDeparture());

            if(sid != nullptr)
            {
                for(int i = 0; i < sid->getFixListSize(); i++)
                {
                    if(sid->getFixName(i) != route.at(0)) fixList.append(sid->getFixName(i));
                }
            }

            for(int i = 0; i < flight->getMainFixList().size(); i++)
            {
                fixList.append(flight->getMainFixList().at(i));
            }

            if(star != nullptr)
            {
                for(int i = 0; i < star->getFixListSize(); i++)
                {
                    if(star->getFixName(i) != route.at(route.size() - 1)) fixList.append(star->getFixName(i));
                }
            }

            fixList.append(flight->getFlightPlan()->getRoute().getDestination());
            if(!flight->getFlightPlan()->getRoute().getAlternate().isEmpty()) fixList.append(flight->getFlightPlan()->getRoute().getAlternate());

            flight->setFixList(fixList);

            //Rebuild waypoints list
            flight->clearWaypoints();
            fixList = flight->getFixList();
            for(int i = 0; i < fixList.size(); i++)
            {
                ATCNavFix *fix = nullptr;
                ATCBeaconVOR *vor = nullptr;
                ATCAirport *airport = nullptr;
                ATCBeaconNDB *ndb = nullptr;

                double lat;
                double lon;

                double x;
                double y;

                if((fix = airspace->findFix(fixList.at(i))) != nullptr)
                {
                    lat = fix->latitude();
                    lon = fix->longitude();
                    x = fix->getScenePosition()->x();
                    y = fix->getScenePosition()->y();
                }
                else if((airport = airspace->findAirport(fixList.at(i))) != nullptr)
                {
                    lat = airport->latitude();
                    lon = airport->longitude();
                    x = airport->getScenePosition()->x();
                    y = airport->getScenePosition()->y();
                }
                else if((vor = airspace->findVOR(fixList.at(i))) != nullptr)
                {
                    lat = vor->latitude();
                    lon = vor->longitude();
                    x = vor->getScenePosition()->x();
                    y = vor->getScenePosition()->y();
                }
                else if((ndb = airspace->findNDB(fixList.at(i))) != nullptr)
                {
                    lat = ndb->latitude();
                    lon = ndb->longitude();
                    x = ndb->getScenePosition()->x();
                    y = ndb->getScenePosition()->y();
                }

                flight->appendWaypoint(QPair<double, double>(lat, lon));
                flight->appendProjectedWaypoint(QPair<double, double>(x, y));
                if(fixList.at(i) == flight->getNextFix())
                {
                    flight->setWaypointIndex(i);
                }
                else if(i == fixList.size() - 1) flight->setWaypointIndex(-1);
            }

            //Check if waypoint index found. If not, change mode to heading & update tag
            if(flight->getWaypointIndex() == -1)
            {
                flight->setNavMode(ATC::Hdg);
                flight->setHdgRestriction(ATCMath::normalizeAngle(ATCMath::rad2deg(flight->getState().hdg) - ATCConst::AVG_DECLINATION, ATC::Deg));
                flight->setDCT(false);

                QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
                QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

                QString headingString = QString::number(flight->getHdgRestriction());
                for(int i = 0; i < headingString.size(); i++)
                {
                    longEtiquette[i + 39] = headingString.at(i);
                }

                if(flight->getFlightTag()->getTagType() == ATC::Short)
                {
                    for(int i = 0; i < 5; i++)
                    {
                        shortEtiquette[i + 24] = QChar(' ');
                        longEtiquette[i + 24] = QChar(' ');
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->rectShort2Long();
                    flight->getFlightTag()->setTagType(ATC::Full);
                    flight->getFlightTag()->getTagBox()->setLong();

                    flight->setNavMode(ATC::Hdg);
                }
                else
                {
                    if(flight->getNavMode() == ATC::Nav)
                    {
                        for(int i = 0; i < 5; i++)
                        {
                            shortEtiquette[i + 24] = QChar(' ');
                            longEtiquette[i + 24] = QChar(' ');
                        }

                        flight->setNavMode(ATC::Hdg);
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->setLong();
                }

                if(flight->isFinalApp())
                {
                    flight->setCldFinalApp(false);
                    flight->setFinalApp(false);
                    flight->setGlidePath(false);
                }

            }

            //Repaint route prediction
            if(flight->getRoutePrediction() != nullptr)
            {
                flight->slotDisplayRoute();     //First call deletes existing route prediction
                flight->slotDisplayRoute();     //Second call creates new, updated route prediction
            }
        }
        else if(index.column() == 9)
        {
            flight->setSTAR(model->index(index.row(), 9).data().toString());

            //Rebuild fix list
            QStringList fixList;
            ATCProcedureSID *sid = airspace->findSID(flight->getSID());
            ATCProcedureSTAR *star = airspace->findSTAR(flight->getSTAR());

            fixList.append(flight->getFlightPlan()->getRoute().getDeparture());

            if(sid != nullptr)
            {
                for(int i = 0; i < sid->getFixListSize(); i++)
                {
                    if(sid->getFixName(i) != route.at(0)) fixList.append(sid->getFixName(i));
                }
            }

            for(int i = 0; i < flight->getMainFixList().size(); i++)
            {
                fixList.append(flight->getMainFixList().at(i));
            }

            if(star != nullptr)
            {
                for(int i = 0; i < star->getFixListSize(); i++)
                {
                    if(star->getFixName(i) != route.at(route.size() - 1)) fixList.append(star->getFixName(i));
                }
            }

            fixList.append(flight->getFlightPlan()->getRoute().getDestination());
            if(!flight->getFlightPlan()->getRoute().getAlternate().isEmpty()) fixList.append(flight->getFlightPlan()->getRoute().getAlternate());

            flight->setFixList(fixList);

            //Rebuild waypoints list
            flight->clearWaypoints();
            fixList = flight->getFixList();
            for(int i = 0; i < fixList.size(); i++)
            {
                ATCNavFix *fix = nullptr;
                ATCBeaconVOR *vor = nullptr;
                ATCAirport *airport = nullptr;
                ATCBeaconNDB *ndb = nullptr;

                double lat;
                double lon;

                double x;
                double y;

                if((fix = airspace->findFix(fixList.at(i))) != nullptr)
                {
                    lat = fix->latitude();
                    lon = fix->longitude();
                    x = fix->getScenePosition()->x();
                    y = fix->getScenePosition()->y();
                }
                else if((airport = airspace->findAirport(fixList.at(i))) != nullptr)
                {
                    lat = airport->latitude();
                    lon = airport->longitude();
                    x = airport->getScenePosition()->x();
                    y = airport->getScenePosition()->y();
                }
                else if((vor = airspace->findVOR(fixList.at(i))) != nullptr)
                {
                    lat = vor->latitude();
                    lon = vor->longitude();
                    x = vor->getScenePosition()->x();
                    y = vor->getScenePosition()->y();
                }
                else if((ndb = airspace->findNDB(fixList.at(i))) != nullptr)
                {
                    lat = ndb->latitude();
                    lon = ndb->longitude();
                    x = ndb->getScenePosition()->x();
                    y = ndb->getScenePosition()->y();
                }

                flight->appendWaypoint(QPair<double, double>(lat, lon));
                flight->appendProjectedWaypoint(QPair<double, double>(x, y));
                if(fixList.at(i) == flight->getNextFix())
                {
                    flight->setWaypointIndex(i);
                }
                else if(i == fixList.size() - 1) flight->setWaypointIndex(-1);
            }

            //Check if waypoint index found. If not, change mode to heading & update tag
            if(flight->getWaypointIndex() == -1)
            {
                flight->setNavMode(ATC::Hdg);
                flight->setHdgRestriction(ATCMath::normalizeAngle(ATCMath::rad2deg(flight->getState().hdg) - ATCConst::AVG_DECLINATION, ATC::Deg));
                flight->setDCT(false);

                QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
                QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

                QString headingString = QString::number(flight->getHdgRestriction());
                for(int i = 0; i < headingString.size(); i++)
                {
                    longEtiquette[i + 39] = headingString.at(i);
                }

                if(flight->getFlightTag()->getTagType() == ATC::Short)
                {
                    for(int i = 0; i < 5; i++)
                    {
                        shortEtiquette[i + 24] = QChar(' ');
                        longEtiquette[i + 24] = QChar(' ');
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->rectShort2Long();
                    flight->getFlightTag()->setTagType(ATC::Full);
                    flight->getFlightTag()->getTagBox()->setLong();

                    flight->setNavMode(ATC::Hdg);
                }
                else
                {
                    if(flight->getNavMode() == ATC::Nav)
                    {
                        for(int i = 0; i < 5; i++)
                        {
                            shortEtiquette[i + 24] = QChar(' ');
                            longEtiquette[i + 24] = QChar(' ');
                        }

                        flight->setNavMode(ATC::Hdg);
                    }

                    flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                    flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                    flight->getFlightTag()->getTagBox()->setLong();
                }

                if(flight->isFinalApp())
                {
                    flight->setCldFinalApp(false);
                    flight->setFinalApp(false);
                    flight->setGlidePath(false);
                }

            }

            //Repaint route prediction
            if(flight->getRoutePrediction() != nullptr)
            {
                flight->slotDisplayRoute();     //First call deletes existing route prediction
                flight->slotDisplayRoute();     //Second call creates new, updated route prediction
            }
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ATCComboDelegate::slotCloseDelegate(QString text)
{
    Q_UNUSED(text)
    QComboBox* cb = qobject_cast<QComboBox*>(sender());

    emit commitData(cb);
    emit closeEditor(cb);
}
