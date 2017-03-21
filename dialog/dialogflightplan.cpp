#include "dialogflightplan.h"
#include "ui_dialogflightplan.h"

DialogFlightPlan::DialogFlightPlan(ATCFlight *flight, ATCAirspace *airspace, ATCSimulation *simulation, ATCFlightFactory *flightFactory, QWidget *parent) :
    ATCDialog(parent, "Flight Plan", 600, 650),
    flight(flight),
    airspace(airspace),
    simulation(simulation),
    flightFactory(flightFactory),
    model(new QStandardItemModel(this)),
    uiInner(new Ui::DialogFlightPlan)
{
    uiInner->setupUi(this);
    windowSetup();

    for(int i = 0; i < flightFactory->getAircraftTypeFactory().getTypeVectorSize(); i++)
    {
        QString type = flightFactory->getAircraftTypeFactory().getType(i)->getAcType().ICAOcode;
        uiInner->comboBoxAcftType->addItem(type, Qt::DisplayRole);
    }

    uiInner->lineEditCallsign->setInputMask(">AAAxxxxxxx");
    uiInner->lineEditDeparture->setInputMask(">AAAA");
    uiInner->lineEditDestination->setInputMask(">AAAA");
    uiInner->lineEditAlternate->setInputMask(">AAAA");
    uiInner->lineEditAltitude->setInputMask(">A999");
    uiInner->lineEditTAS->setInputMask("9990");
    uiInner->lineEditSquawk->setInputMask("9999");

    uiInner->lineEditCallsign->setText(flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber());

    if(flight->getFlightPlan()->getFlightRules() == ATC::IFR)
    {
        uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("IFR"));
    }
    else if(flight->getFlightPlan()->getFlightRules() == ATC::VFR)
    {
        uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("VFR"));
    }
    else
    {
        uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("SVFR"));
    }

    QString acftType = flight->getFlightPlan()->getType()->getAcType().ICAOcode;
    uiInner->comboBoxAcftType->setCurrentIndex(uiInner->comboBoxAcftType->findText(acftType));
    uiInner->comboBoxAcftType->setEnabled(false);

    uiInner->lineEditDeparture->setText(flight->getFlightPlan()->getRoute().getDeparture());
    uiInner->lineEditDestination->setText(flight->getFlightPlan()->getRoute().getDestination());
    uiInner->lineEditAlternate->setText(flight->getFlightPlan()->getRoute().getAlternate());
    uiInner->lineEditTAS->setText(QString::number(flight->getFlightPlan()->getTAS()));
    uiInner->lineEditAltitude->setText(flight->getFlightPlan()->getAltitude());
    uiInner->lineEditSquawk->setText(flight->getAssignedSquawk());

    QStringList route = flight->getFlightPlan()->getRoute().getRoute();
    QString routeStr;

    for(int i = 0; i < route.size() - 1; i++)
    {
        routeStr = routeStr + route.at(i) + " ";
    }
    if(route.size() >= 1)routeStr = routeStr + route.at(route.size() - 1);

    uiInner->plainTextEditRoute->setPlainText(routeStr);

    uiInner->timeEditDepTime->setEnabled(false);
    uiInner->timeEditEnrTime->setEnabled(false);
    uiInner->timeEditFuelTime->setEnabled(false);

    uiInner->labelError->setVisible(false);

    model->setColumnCount(3);

    uiInner->tableViewValidator->setModel(model);
    uiInner->tableViewValidator->setGridStyle(Qt::NoPen);

    uiInner->tableViewValidator->horizontalHeader()->setHidden(true);
    uiInner->tableViewValidator->verticalHeader()->setHidden(true);
}

DialogFlightPlan::~DialogFlightPlan()
{
    if(model != nullptr) delete model;
    delete uiInner;
}

void DialogFlightPlan::on_plainTextEditRoute_textChanged()
{
    routeValid = validateRoute();
}

bool DialogFlightPlan::validateRoute()
{
    bool error = false;

    model->clear();

    QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().toUpper().split(" ", QString::SkipEmptyParts);

    if(routeStr.isEmpty())
    {
        errorMessage("Route is empty");
        error = true;
    }
    else
    {
        QList<QStandardItem*> row;

        QStandardItem *emptyAirway = new QStandardItem();
        emptyAirway->setFlags(Qt::NoItemFlags);
        row.append(emptyAirway);

        QStandardItem *firstFix = new QStandardItem(routeStr.at(0));
        firstFix->setFlags(Qt::NoItemFlags);
        firstFix->setTextAlignment(Qt::AlignCenter);
        row.append(firstFix);

        if(airspace->isValidNavaid(routeStr.at(0)) || ((routeStr.at(0) == "DCT") && (routeStr.size() == 1)))
        {
            firstFix->setBackground(QBrush(Qt::darkGreen));

            QStandardItem *description = new QStandardItem("Correct");
            description->setFlags(Qt::NoItemFlags);
            description->setTextAlignment(Qt::AlignCenter);
            description->setForeground(Qt::green);
            row.append(description);
        }
        else
        {
            firstFix->setBackground(QBrush(Qt::darkRed));
            QStandardItem *description = new QStandardItem(routeStr.at(0) + " not found");
            description->setFlags(Qt::NoItemFlags);
            description->setTextAlignment(Qt::AlignCenter);
            description->setForeground(Qt::red);
            row.append(description);

            error  = true;
        }

        model->appendRow(row);
        row.clear();

        for(int i = 1; i < routeStr.size(); i++)
        {
            QStandardItem *descriptionAirway;
            QStandardItem *descriptionFix;

            if(i % 2 == 1)
            {
                QStandardItem *airway = new QStandardItem(routeStr.at(i));
                airway->setFlags(Qt::NoItemFlags);
                airway->setTextAlignment(Qt::AlignCenter);

                ATCAbstractAirway *aw = airspace->findAirway(routeStr.at(i));
                bool previousFixConnected = false;

                if(aw != nullptr)
                {
                    for(int j = 0; j < aw->getRouteFixes().size(); j++)
                    {
                        if(aw->getRouteFixes().at(j) == routeStr.at(i - 1)) previousFixConnected = true;
                    }
                }

                bool isAirway = airspace->isAirwayLow(routeStr.at(i)) || airspace->isAirwayHigh(routeStr.at(i));

                if((isAirway && previousFixConnected) || (routeStr.at(i) == "DCT"))
                {
                    descriptionAirway = new QStandardItem();

                    airway->setBackground(QBrush(Qt::darkGreen));
                }
                else
                {
                    if(!isAirway)
                    {
                        descriptionAirway = new QStandardItem(routeStr.at(i) + " not found");
                        descriptionAirway->setFlags(Qt::NoItemFlags);
                        descriptionAirway->setTextAlignment(Qt::AlignCenter);
                        descriptionAirway->setForeground(Qt::red);
                    }
                    else if(!previousFixConnected)
                    {
                        descriptionAirway = new QStandardItem(routeStr.at(i - 1) + " not connected to " + routeStr.at(i));
                        descriptionAirway->setFlags(Qt::NoItemFlags);
                        descriptionAirway->setTextAlignment(Qt::AlignCenter);
                        descriptionAirway->setForeground(Qt::red);
                    }

                    airway->setBackground(QBrush(Qt::darkRed));
                    error  = true;
                }

                row.append(airway);
            }
            else
            {
                ATCAbstractAirway *airway = airspace->findAirway(routeStr.at(i - 1));

                QStringList fixList;
                int firstFix = -1;
                int secondFix = -1;

                if(airway != nullptr)
                {
                    fixList = airway->getRouteFixes();

                    for(int j = 0; j < fixList.size(); j++)
                    {
                        if(fixList.at(j) == routeStr.at(i - 2)) firstFix = j;
                        if(fixList.at(j) == routeStr.at(i)) secondFix = j;
                    }

                    if((qFabs(firstFix - secondFix) > 1) && (firstFix >= 0) && (secondFix >= 0))
                    {
                        if(firstFix < secondFix)
                        {
                            for(int j = firstFix + 1; j < secondFix; j++)
                            {
                                QList<QStandardItem*> tempRow;

                                QStandardItem *airway = new QStandardItem(routeStr.at(i - 1));
                                airway->setFlags(Qt::NoItemFlags);
                                airway->setTextAlignment(Qt::AlignCenter);
                                airway->setBackground(QBrush(Qt::darkGreen));

                                QStandardItem *fix = new QStandardItem(fixList.at(j));
                                fix->setFlags(Qt::NoItemFlags);
                                fix->setTextAlignment(Qt::AlignCenter);
                                fix->setBackground(QBrush(Qt::darkGreen));

                                QStandardItem *description = new QStandardItem("Correct");
                                description->setFlags(Qt::NoItemFlags);
                                description->setTextAlignment(Qt::AlignCenter);
                                description->setForeground(Qt::green);

                                tempRow.append(airway);
                                tempRow.append(fix);
                                tempRow.append(description);

                                model->appendRow(tempRow);
                            }
                        }
                        else if(firstFix > secondFix)
                        {
                            for(int j = firstFix - 1; j > secondFix; j--)
                            {
                                QList<QStandardItem*> tempRow;

                                QStandardItem *airway = new QStandardItem(routeStr.at(i - 1));
                                airway->setFlags(Qt::NoItemFlags);
                                airway->setTextAlignment(Qt::AlignCenter);
                                airway->setBackground(QBrush(Qt::darkGreen));

                                QStandardItem *fix = new QStandardItem(fixList.at(j));
                                fix->setFlags(Qt::NoItemFlags);
                                fix->setTextAlignment(Qt::AlignCenter);
                                fix->setBackground(QBrush(Qt::darkGreen));

                                QStandardItem *description = new QStandardItem("Correct");
                                description->setFlags(Qt::NoItemFlags);
                                description->setTextAlignment(Qt::AlignCenter);
                                description->setForeground(Qt::green);

                                tempRow.append(airway);
                                tempRow.append(fix);
                                tempRow.append(description);

                                model->appendRow(tempRow);
                            }
                        }
                    }
                }

                QStandardItem *fix = new QStandardItem(routeStr.at(i));
                fix->setFlags(Qt::NoItemFlags);
                fix->setTextAlignment(Qt::AlignCenter);
                row.append(fix);

                bool isValidNavaid = airspace->isValidNavaid(routeStr.at(i));

                if(isValidNavaid && ((secondFix >= 0) || (routeStr.at(i - 1) == "DCT")))
                {
                    fix->setBackground(QBrush(Qt::darkGreen));

                    descriptionFix = new QStandardItem("Correct");
                    descriptionFix->setFlags(Qt::NoItemFlags);
                    descriptionFix->setTextAlignment(Qt::AlignCenter);
                    descriptionFix->setForeground(Qt::green);
                }
                else
                {
                    fix->setBackground(QBrush(Qt::darkRed));
                    error  = true;

                    if(!isValidNavaid)
                    {
                        descriptionFix = new QStandardItem(routeStr.at(i) + " not found");
                        descriptionFix->setFlags(Qt::NoItemFlags);
                        descriptionFix->setTextAlignment(Qt::AlignCenter);
                        descriptionFix->setForeground(Qt::red);
                    }
                    else if(secondFix == -1)
                    {
                        descriptionFix = new QStandardItem(routeStr.at(i) + " not connected to " + routeStr.at(i - 1));
                        descriptionFix->setFlags(Qt::NoItemFlags);
                        descriptionFix->setTextAlignment(Qt::AlignCenter);
                        descriptionFix->setForeground(Qt::red);
                    }
                }

                if(descriptionAirway->text().isEmpty())
                {
                    row.append(descriptionFix);
                }
                else
                {
                    row.append(descriptionAirway);
                }
            }

            if(i % 2 == 0)
            {
                model->appendRow(row);
                row.clear();
            }
        }

        uiInner->tableViewValidator->setModel(model);

        for(int i = 0; i < model->rowCount(); i++)
        {
            uiInner->tableViewValidator->setRowHeight(i, 20);
        }

        uiInner->tableViewValidator->scrollToBottom();
    }

    uiInner->tableViewValidator->setColumnWidth(0, 75);
    uiInner->tableViewValidator->setColumnWidth(1, 75);
    uiInner->tableViewValidator->setColumnWidth(2, 260);

    return !error;
}

void DialogFlightPlan::errorMessage(QString msg)
{
    uiInner->labelError->setText(msg);
    uiInner->labelError->setVisible(true);
}

void DialogFlightPlan::setRoute()
{
    QString departure = uiInner->lineEditDeparture->text();
    QString destination = uiInner->lineEditDestination->text();

    ATCRoute *route = flightFactory->getRouteFactory().getRoute(departure, destination);

    if(route != nullptr)
    {
        QString strRoute = route->getRoute().at(0);

        for(int i = 1; i < route->getRoute().size(); i++)
        {
            strRoute = strRoute + " " + route->getRoute().at(i);
        }

        uiInner->plainTextEditRoute->setPlainText(strRoute);
    }
    else
    {
        errorMessage("ERROR: Route not found!");
    }
}

void DialogFlightPlan::setCallsign()
{
    QString company = flightFactory->getCompanyFactory().getCompany()->getCode();
    QString flightNumber = ATCFlightNumberFactory::getFlightNumber();
    QString callsign = company + flightNumber;

    uiInner->lineEditCallsign->setText(callsign);
}

void DialogFlightPlan::setSquawk()
{
    QString squawk  = ATCFlightFactory::generateSquawk();
    if((squawk != "7500") || (squawk != "7500") || (squawk != "7600") || (squawk != "7700"))
    {
        uiInner->lineEditSquawk->setText(squawk);
    }
    else
    {
        setSquawk();
    }
}

void DialogFlightPlan::setTAS()
{
    QString altitude = uiInner->lineEditAltitude->text();

    double alt = altitude.right(altitude.size() - 1).toDouble() * 100;
    alt = ATCMath::ft2m(alt);

    ISA isa = ATCMath::atmosISA(alt);

    QString typeStr = uiInner->comboBoxAcftType->currentText();
    ATCAircraftType *type = flightFactory->getAircraftTypeFactory().getType(typeStr);

    double mach = type->getVelocity().M_CR_AV;
    double cas = type->getVelocity().V_CR2_AV;

    double crossoverAlt = ATCMath::crossoverAltitude(cas, mach);
    crossoverAlt = ATCMath::m2ft(crossoverAlt);

    int tas;

    if(alt >= crossoverAlt)
    {
        tas = qFabs(ATCMath::mps2kt(ATCMath::mach2tas(mach, isa.a)));
    }
    else
    {
        tas = qFabs(ATCMath::mps2kt(ATCMath::cas2tas(ATCMath::kt2mps(cas), isa.p, isa.rho)));
    }

    uiInner->lineEditTAS->setText(QString::number(tas));
}

void DialogFlightPlan::on_buttonFillAll_clicked()
{
    setCallsign();
    setTAS();
    setRoute();
    setSquawk();
}

void DialogFlightPlan::on_buttonSetCallsign_clicked()
{
    setCallsign();
}

void DialogFlightPlan::on_buttonSetTAS_clicked()
{
    setTAS();
}

void DialogFlightPlan::on_buttonSetSquawk_clicked()
{
    setSquawk();
}

void DialogFlightPlan::on_buttonSetRoute_clicked()
{
    setRoute();
}

void DialogFlightPlan::on_buttonOK_clicked()
{
    bool filledOK = verifyForm();

    if(filledOK)
    {
        //Flight plan - Flight rules
        QString rulesStr = uiInner->comboBoxFlightRules->currentText();
        ATC::FlightRules rules;

        if(rulesStr == "IFR")
        {
            rules = ATC::IFR;
        }
        else if(rulesStr == "VFR")
        {
            rules = ATC::VFR;
        }
        else if(rulesStr == "SVFR")
        {
            rules = ATC::SVFR;
        }

        flight->getFlightPlan()->setFlightRules(rules);

        //Flight plan - company & flight number
        QString callsign = uiInner->lineEditCallsign->text();

        ATCCompany *company;
        QString flightNo;

        if(callsign.at(3).isLetter())
        {
            company = new ATCCompany(callsign, "UNKNOWN", "Unknown Airline");
            flightFactory->getCompanyFactory().appendCompany(company);
            flightNo = "";
        }
        else
        {
            if(flightFactory->getCompanyFactory().getCompany(callsign.left(3)) != nullptr)
            {
                company = flightFactory->getCompanyFactory().getCompany(callsign.left(3));
                flightNo = callsign.right(callsign.size() - 3);
            }
            else
            {
                company = new ATCCompany(callsign.left(3), "UNKNOWN", "Unknown Airline");
                flightNo = callsign.right(callsign.size() - 3);
            }
        }

        flight->getFlightPlan()->setCompany(company);
        flight->getFlightPlan()->setFlightNumber(flightNo);

        //Flight plan - route
        QString departure = uiInner->lineEditDeparture->text();
        QString destination = uiInner->lineEditDestination->text();
        QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().toUpper().split(" ", QString::SkipEmptyParts);
        QString alternate = uiInner->lineEditAlternate->text();

        ATCRoute route(departure, routeStr, destination);
        route.setAlternate(alternate);

        flight->getFlightPlan()->setRoute(route);

        //Flight plan - tas
        int tas = uiInner->lineEditTAS->text().toInt();
        flight->getFlightPlan()->setTAS(tas);

        //Flight plan - altitude
        QString altitude = uiInner->lineEditAltitude->text();
        flight->getFlightPlan()->setAltitude(altitude);

        //Flight plan - departure time
        QTime depTime = uiInner->timeEditDepTime->time();
        flight->getFlightPlan()->setDepartureTime(depTime);

        //Flight plan - enroute time
        QTime enrTime = uiInner->timeEditEnrTime->time();
        flight->getFlightPlan()->setEnrouteTime(enrTime);

        //Flight plan - fuel time
        QTime fuelTime = uiInner->timeEditFuelTime->time();
        flight->getFlightPlan()->setFuelTime(fuelTime);

        //Reset sid and star
        flight->setSID("");
        flight->setSTAR("");

        //Flight - fix list
        QStringList fixList;

        fixList.append(departure);

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") fixList.append(fix);
        }

        fixList.append(destination);
        if(!alternate.isEmpty()) fixList.append(alternate);

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

        //Assign leg distances and angle changes
        flight->clearLegAngleChanges();
        flight->clearLegDistances();

        double previousForwardAzimuth;

        for(int i = 0; i < flight->getWaypointsVectorSize() - 1; i++)
        {
            GeographicLib::Geodesic geo = GeographicLib::Geodesic::WGS84();

            QPair<double, double> fix1 = flight->getWaypoint(i);
            QPair<double, double> fix2 = flight->getWaypoint(i + 1);

            double distance;
            double azimuth1to2;
            double azimuth2to1;
            geo.Inverse(fix1.first, fix1.second, fix2.first, fix2.second, distance, azimuth1to2, azimuth2to1);

            if(i != 0)
            {
                double hdgChange = ATCMath::normalizeHdgChange(ATCMath::deg2rad(azimuth1to2 - previousForwardAzimuth));
                flight->appendLegAngleChange(hdgChange);
            }

            previousForwardAzimuth = azimuth2to1;
            flight->appendLegDistance(distance);
        }

        //Check if waypoint index found. If not, change mode to heading & update tag
        if(flight->getWaypointIndex() == -1)
        {
            flight->setNavMode(ATC::Hdg);
            flight->setHdgRestriction(ATCMath::normalizeAngle(ATCMath::rad2deg(flight->getState().hdg) - ATCConst::AVG_DECLINATION, ATC::Deg));
            flight->setDCT(false);

            QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
            QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

            QString headingString;
            if(flight->getHdgRestriction() < 10)
            {
                headingString = "00" + QString::number(flight->getHdgRestriction());
            }
            else if(flight->getHdgRestriction() < 100)
            {
                headingString = "0" + QString::number(flight->getHdgRestriction());
            }
            else
            {
                headingString = QString::number(flight->getHdgRestriction());
            }

            for(int i = 0; i < headingString.size(); i++)
            {
                longEtiquette[i + 39] = headingString.at(i);
            }

            for(int i = 0; i < 5; i++)
            {
                shortEtiquette[i + 24] = QChar(' ');
                longEtiquette[i + 24] = QChar(' ');
            }

            if(flight->getFlightTag()->getTagType() == ATC::Short)
            {
                flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
                flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

                flight->getFlightTag()->getTagBox()->rectShort2Long();
                flight->getFlightTag()->setTagType(ATC::Full);
                flight->getFlightTag()->getTagBox()->setLong();
            }
            else
            {
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

        //Flight - main fix list
        QStringList mainFixList;

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") mainFixList.append(fix);
        }

        flight->setMainFixList(mainFixList);

        //Assign assigned SSR
        flight->setAssignedSquawk(uiInner->lineEditSquawk->text());

        //Assign departure & destination runway
        QVector<ActiveAirport> activeAirports = simulation->getActiveRunways()->getActiveAirports();

        flight->setRunwayDeparture("");
        flight->setRunwayDestination("");

        for(int i = 0; i < activeAirports.size(); i++)
        {
            ActiveAirport current = activeAirports.at(i);

            if((departure == current.airportCode) && (!current.depRwys.isEmpty())) flight->setRunwayDeparture(current.depRwys.at(0));
            if((destination == current.airportCode) && (!current.arrRwys.isEmpty())) flight->setRunwayDestination(current.arrRwys.at(0));
        }

        //Modify callsign on data tag
        QString shortEtiquette = flight->getFlightTag()->getTagBox()->getShortEtiquette();
        QString longEtiquette = flight->getFlightTag()->getTagBox()->getLongEtiquette();

        for(int i = 0; i < 9; i++)
        {
            shortEtiquette[i] = ' ';
            longEtiquette[i] = ' ';
        }

        for(int i = 0; i < callsign.left(9).size(); i++)
        {
            shortEtiquette[i] = callsign.at(i);
            longEtiquette[i] = callsign.at(i);
        }

        flight->getFlightTag()->getTagBox()->setShortEtiquette(shortEtiquette);
        flight->getFlightTag()->getTagBox()->setLongEtiquette(longEtiquette);

        if(flight->getFlightTag()->getTagType() == ATC::Short)
        {
            flight->getFlightTag()->getTagBox()->setShort();
        }
        else
        {
            flight->getFlightTag()->getTagBox()->setLong();
        }

        if(flight->getRoutePrediction() != nullptr) emit signalUpdateRoute(flight);
        emit signalUpdateFlightList();
        emit closed();
        close();
    }
}

void DialogFlightPlan::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

bool DialogFlightPlan::verifyForm()
{
    if(!uiInner->lineEditCallsign->text().isEmpty())
    {
        ATCFlight *flt = simulation->getFlight(uiInner->lineEditCallsign->text());

        if((flt != nullptr) && (flt != flight))
        {
            errorMessage("ERROR: Callsign already in use!");
            return false;
        }
    }

    if(uiInner->lineEditCallsign->text().isEmpty())
    {
        errorMessage("ERROR: Callsign not specified!");
        return false;
    }
    else if(uiInner->lineEditDeparture->text().isEmpty())
    {
        errorMessage("ERROR: Departure airfield not specified!");
        return false;
    }
    else if(uiInner->lineEditDeparture->text().size() != 4)
    {
        errorMessage("ERROR: Departure ICAO code not correct!");
        return false;
    }
    else if(uiInner->lineEditDestination->text().isEmpty())
    {
        errorMessage("ERROR: Destination airfield not specified!");
        return false;
    }
    else if(uiInner->lineEditDestination->text().size() != 4)
    {
        errorMessage("ERROR: Destination ICAO code not correct!");
        return false;
    }
    else if(uiInner->lineEditTAS->text().isEmpty())
    {
        errorMessage("ERROR: TAS not specified!");
        return false;
    }
    else if(uiInner->lineEditAltitude->text().isEmpty())
    {
        errorMessage("ERROR: Altitude not specified!");
        return false;
    }
    else if((uiInner->lineEditAltitude->text().left(1) != "A") &&
            (uiInner->lineEditAltitude->text().left(1) != "F"))
    {
        errorMessage("ERROR: Incorrect altitude fomat!");
        return false;
    }
    else if((uiInner->lineEditSquawk->text().at(0) == '8') ||
            (uiInner->lineEditSquawk->text().at(1) == '8') ||
            (uiInner->lineEditSquawk->text().at(2) == '8') ||
            (uiInner->lineEditSquawk->text().at(3) == '8') ||
            (uiInner->lineEditSquawk->text().at(0) == '9') ||
            (uiInner->lineEditSquawk->text().at(1) == '9') ||
            (uiInner->lineEditSquawk->text().at(2) == '9') ||
            (uiInner->lineEditSquawk->text().at(3) == '9'))
    {
        errorMessage("ERROR: Incorrect squawk format!");
        return false;
    }
    else if(!routeValid)
    {
        errorMessage("ERROR: Route incorrect! See route validator.");
        return false;
    }
    else
    {
        uiInner->labelError->setVisible(false);

        return true;
    }
}
