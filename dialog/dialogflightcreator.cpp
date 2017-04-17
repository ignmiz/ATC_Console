#include "dialogflightcreator.h"
#include "ui_dialogflightcreator.h"

DialogFlightCreator::DialogFlightCreator(ATCFlight *flight, ATCAirspace *airspace, ATCSettings *settings, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 600, 700),
    uiInner(new Ui::DialogFlightCreator),
    flight(flight),
    airspace(airspace),
    settings(settings),
    flightFactory(flightFactory),
    simulation(simulation),
    model(new QStandardItemModel(this))
{
    uiInner->setupUi(this);
    windowSetup();
    formSetup(flight);
}

DialogFlightCreator::DialogFlightCreator(ATCAirspace *airspace, ATCSettings *settings, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 600, 700),
    uiInner(new Ui::DialogFlightCreator),
    airspace(airspace),
    settings(settings),
    flightFactory(flightFactory),
    simulation(simulation),
    model(new QStandardItemModel(this))
{
    uiInner->setupUi(this);
    windowSetup();
    formSetup();
}

DialogFlightCreator::~DialogFlightCreator()
{
    if(model != nullptr) delete model;
    delete uiInner;
}

void DialogFlightCreator::on_buttonOK_clicked()
{
    bool filledOK = verifyForm();

    if(filledOK && (flight == nullptr)) //Create new flight
    {
        //Initial state assignment
        State state;

        state.x = ATCMath::deg2rad(uiInner->lineEditLongitude->text().toDouble());
        state.y = ATCMath::deg2rad(uiInner->lineEditLatitude->text().toDouble());

        state.h = ATCMath::ft2m(uiInner->lineEditAltitudeCurrent->text().right(3).toDouble() * 100);

        state.v = ATCMath::kt2mps(uiInner->lineEditTASCurrent->text().toDouble());
        state.hdg = ATCMath::deg2rad(uiInner->spinBoxTrueHDG->text().toDouble());

        //Flight plan
        ATCFlightPlan *fpl = flightFactory->getFlightPlanFactory().newFlightPlan();

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

        fpl->setFlightRules(rules);

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

        fpl->setCompany(company);
        fpl->setFlightNumber(flightNo);

        //Flight plan - aircraft type
        QString typeStr = uiInner->comboBoxAcftType->currentText();
        ATCAircraftType *type = flightFactory->getAircraftTypeFactory().getType(typeStr);

        fpl->setType(type);

        //Flight plan - route
        QString departure = uiInner->lineEditDeparture->text();
        QString destination = uiInner->lineEditDestination->text();
        QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().toUpper().split(" ", QString::SkipEmptyParts);
        QString alternate = uiInner->lineEditAlternate->text();

        ATCRoute route(departure, routeStr, destination);
        route.setAlternate(alternate);

        fpl->setRoute(route);

        //Flight plan - tas
        int tas = uiInner->lineEditTAS->text().toInt();
        fpl->setTAS(tas);

        //Flight plan - altitude
        QString altitude = uiInner->lineEditAltitude->text();
        fpl->setAltitude(altitude);

        //Flight plan - departure time
        QTime depTime = uiInner->timeEditDepTime->time();
        fpl->setDepartureTime(depTime);

        //Flight plan - enroute time
        QTime enrTime = uiInner->timeEditEnrTime->time();
        fpl->setEnrouteTime(enrTime);

        //Flight plan - fuel time
        QTime fuelTime = uiInner->timeEditFuelTime->time();
        fpl->setFuelTime(fuelTime);

        //Create flight
        ATCFlight *flight = flightFactory->newFlight(state);
        flight->setFlightPlan(fpl);

        //Flight - fix list
        QStringList fixList;
        ATCProcedureSID *sid = airspace->findSID(uiInner->comboBoxSID->currentText());
        ATCProcedureSTAR *star = airspace->findSTAR(uiInner->comboBoxSTAR->currentText());

        fixList.append(departure);

        if(sid != nullptr)
        {
            for(int i = 0; i < sid->getFixListSize(); i++)
            {
                if(sid->getFixName(i) != routeStr.at(0)) fixList.append(sid->getFixName(i));
            }
        }

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") fixList.append(fix);
        }

        if(star != nullptr)
        {
            for(int i = 0; i < star->getFixListSize(); i++)
            {
                if(star->getFixName(i) != routeStr.at(routeStr.size() - 1)) fixList.append(star->getFixName(i));
            }
        }

        fixList.append(destination);
        if(!alternate.isEmpty()) fixList.append(alternate);

        flight->setFixList(fixList);

        //Flight - main fix list
        QStringList mainFixList;

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") mainFixList.append(fix);
        }

        flight->setMainFixList(mainFixList);

        //Assign selected SSR
        flight->setSquawk(uiInner->lineEditSquawkCurrent->text());

        //Assign assigned SSR
        flight->setAssignedSquawk(uiInner->lineEditSquawk->text());

        //Set initial navigation type
        if(uiInner->radioButtonOwnNav->isChecked())
        {
            flight->setNavMode(ATC::Nav);
        }
        else if(uiInner->radioButtonHDG->isChecked())
        {
            flight->setNavMode(ATC::Hdg);
        }

        //Set altitude restrictions
        if(!uiInner->lineEditAltitudeRes->text().isEmpty())
        {
            flight->setTargetAltitude(uiInner->lineEditAltitudeRes->text());
        }
        else
        {
            flight->setTargetAltitude(uiInner->lineEditAltitude->text());
        }

        //Set speed restrictions
        if(!uiInner->lineEditSpeedRes->text().isEmpty())
        {
            flight->setTargetSpeed(uiInner->lineEditSpeedRes->text());
        }

        //Set heading restrictions
        if(uiInner->radioButtonHDG->isChecked())
        {
            if(!uiInner->spinBoxHeadingRes->text().isEmpty())
            {
                flight->setHdgRestriction(uiInner->spinBoxHeadingRes->text().toInt());
            }
            else
            {
                flight->setHdgRestriction(ATCMath::normalizeAngle(uiInner->spinBoxTrueHDG->text().toInt() - qFloor(ATCConst::AVG_DECLINATION), ATC::Deg));
            }
        }
        else
        {
            flight->setHdgRestriction(-1);
        }

        //Set next fix
        if(uiInner->radioButtonOwnNav->isChecked())
        {
            flight->setNextFix(uiInner->comboBoxNextFix->currentText());
        }

        //Set start time of flight simulation
        flight->setSimStartTime(uiInner->timeEditSimulationStart->time());

        //Set procedures
        flight->setRunwayDeparture(uiInner->comboBoxRwyDep->currentText());
        flight->setRunwayDestination(uiInner->comboBoxRwyDes->currentText());
        flight->setSID(uiInner->comboBoxSID->currentText());
        flight->setSTAR(uiInner->comboBoxSTAR->currentText());

        //Set prediction phase
        QString phase = uiInner->comboBoxPredictionPhase->currentText();

        if(phase == "CLIMB")
        {
            flight->setPredictionPhase(PredictionPhase::Climb);
        }
        else if(phase == "CRUISE")
        {
            flight->setPredictionPhase(PredictionPhase::Cruise);
        }
        else
        {
            flight->setPredictionPhase(PredictionPhase::Descent);
        }

        //Append flight to simulation
        simulation->appendFlight(flight);

        emit signalCreateFlightTag(flight);
        emit signalUpdateFlightList(flight);
    }
    else if(filledOK && (flight != nullptr)) //Edit existing flight
    {
        //Initial state assignment
        State state;

        state.x = ATCMath::deg2rad(uiInner->lineEditLongitude->text().toDouble());
        state.y = ATCMath::deg2rad(uiInner->lineEditLatitude->text().toDouble());

        state.h = ATCMath::ft2m(uiInner->lineEditAltitudeCurrent->text().right(3).toDouble() * 100);

        state.v = ATCMath::kt2mps(uiInner->lineEditTASCurrent->text().toDouble());
        state.hdg = ATCMath::deg2rad(uiInner->spinBoxTrueHDG->text().toDouble());

        flight->setState(state);

        //Flight plan
        ATCFlightPlan *fpl = flight->getFlightPlan();

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

        fpl->setFlightRules(rules);

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

        fpl->setCompany(company);
        fpl->setFlightNumber(flightNo);

        //Flight plan - aircraft type
        QString typeStr = uiInner->comboBoxAcftType->currentText();
        ATCAircraftType *type = flightFactory->getAircraftTypeFactory().getType(typeStr);

        fpl->setType(type);

        //Flight plan - route
        QString departure = uiInner->lineEditDeparture->text();
        QString destination = uiInner->lineEditDestination->text();
        QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().toUpper().split(" ", QString::SkipEmptyParts);
        QString alternate = uiInner->lineEditAlternate->text();

        ATCRoute route(departure, routeStr, destination);
        route.setAlternate(alternate);

        fpl->setRoute(route);

        //Flight plan - tas
        int tas = uiInner->lineEditTAS->text().toInt();
        fpl->setTAS(tas);

        //Flight plan - altitude
        QString altitude = uiInner->lineEditAltitude->text();
        fpl->setAltitude(altitude);

        //Flight plan - departure time
        QTime depTime = uiInner->timeEditDepTime->time();
        fpl->setDepartureTime(depTime);

        //Flight plan - enroute time
        QTime enrTime = uiInner->timeEditEnrTime->time();
        fpl->setEnrouteTime(enrTime);

        //Flight plan - fuel time
        QTime fuelTime = uiInner->timeEditFuelTime->time();
        fpl->setFuelTime(fuelTime);

        //Flight - fix list
        QStringList fixList;
        ATCProcedureSID *sid = airspace->findSID(uiInner->comboBoxSID->currentText());
        ATCProcedureSTAR *star = airspace->findSTAR(uiInner->comboBoxSTAR->currentText());

        fixList.append(departure);

        if(sid != nullptr)
        {
            for(int i = 0; i < sid->getFixListSize(); i++)
            {
                if(sid->getFixName(i) != routeStr.at(0)) fixList.append(sid->getFixName(i));
            }
        }

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") fixList.append(fix);
        }

        if(star != nullptr)
        {
            for(int i = 0; i < star->getFixListSize(); i++)
            {
                if(star->getFixName(i) != routeStr.at(routeStr.size() - 1)) fixList.append(star->getFixName(i));
            }
        }

        fixList.append(destination);
        if(!alternate.isEmpty()) fixList.append(alternate);

        flight->setFixList(fixList);

        //Flight - main fix list
        QStringList mainFixList;

        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") mainFixList.append(fix);
        }

        flight->setMainFixList(mainFixList);

        //Assign selected SSR
        flight->setSquawk(uiInner->lineEditSquawkCurrent->text());

        //Assign assigned SSR
        flight->setAssignedSquawk(uiInner->lineEditSquawk->text());

        //Set initial navigation type
        if(uiInner->radioButtonOwnNav->isChecked())
        {
            flight->setNavMode(ATC::Nav);
        }
        else if(uiInner->radioButtonHDG->isChecked())
        {
            flight->setNavMode(ATC::Hdg);
        }

        //Set altitude restrictions
        if(!uiInner->lineEditAltitudeRes->text().isEmpty())
        {
            flight->setTargetAltitude(uiInner->lineEditAltitudeRes->text());
        }
        else
        {
            flight->setTargetAltitude(uiInner->lineEditAltitude->text());
        }

        //Set speed restrictions
        if(!uiInner->lineEditSpeedRes->text().isEmpty())
        {
            flight->setTargetSpeed(uiInner->lineEditSpeedRes->text());
        }

        //Set heading restrictions
        if(uiInner->radioButtonHDG->isChecked())
        {
            if(!uiInner->spinBoxHeadingRes->text().isEmpty())
            {
                flight->setHdgRestriction(uiInner->spinBoxHeadingRes->text().toInt());
            }
            else
            {
                flight->setHdgRestriction(ATCMath::normalizeAngle(uiInner->spinBoxTrueHDG->text().toInt() - qFloor(ATCConst::AVG_DECLINATION), ATC::Deg));
            }
        }
        else
        {
            flight->setHdgRestriction(-1);
        }

        //Set next fix
        if(uiInner->radioButtonOwnNav->isChecked())
        {
            flight->setNextFix(uiInner->comboBoxNextFix->currentText());
        }

        //Set start time of flight simulation
        flight->setSimStartTime(uiInner->timeEditSimulationStart->time());

        //Set procedures
        flight->setRunwayDeparture(uiInner->comboBoxRwyDep->currentText());
        flight->setRunwayDestination(uiInner->comboBoxRwyDes->currentText());
        flight->setSID(uiInner->comboBoxSID->currentText());
        flight->setSTAR(uiInner->comboBoxSTAR->currentText());

        //Set prediction phase
        QString phase = uiInner->comboBoxPredictionPhase->currentText();

        if(phase == "CLIMB")
        {
            flight->setPredictionPhase(PredictionPhase::Climb);
        }
        else if(phase == "CRUISE")
        {
            flight->setPredictionPhase(PredictionPhase::Cruise);
        }
        else
        {
            flight->setPredictionPhase(PredictionPhase::Descent);
        }

        emit signalUpdateFlightTag(flight);
        emit signalUpdateFlightList(flight);
    }

    if(filledOK)
    {
        emit closed();
        close();
    }
}

void DialogFlightCreator::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogFlightCreator::on_buttonSetRoute_clicked()
{
    setRoute();
}

void DialogFlightCreator::on_buttonGetLocation_clicked()
{
    if(flight != nullptr)
    {
        if(flight->getRoutePrediction() != nullptr) flight->getRoutePrediction()->hideRoute();
    }
    emit signalGetLocation(getFixList());
    hide();
}

void DialogFlightCreator::on_buttonFillAll_clicked()
{
    setCallsign();
    setTAS();
    setRoute();
    setSquawk();
}

void DialogFlightCreator::setRoute()
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

void DialogFlightCreator::setCallsign()
{
    QString company = flightFactory->getCompanyFactory().getCompany()->getCode();
    QString flightNumber = ATCFlightNumberFactory::getFlightNumber();
    QString callsign = company + flightNumber;

    uiInner->lineEditCallsign->setText(callsign);
}

void DialogFlightCreator::setSquawk()
{
    QString squawk  = ATCFlightFactory::generateSquawk();
    if((squawk != "7500") || (squawk != "7500") || (squawk != "7600") || (squawk != "7700"))
    {
        uiInner->lineEditSquawk->setText(squawk);
        uiInner->lineEditSquawkCurrent->setText(squawk);
    }
    else
    {
        setSquawk();
    }
}

void DialogFlightCreator::setTAS()
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

bool DialogFlightCreator::validateRoute()
{
    bool error = false;

    model->clear();

    QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().toUpper().split(" ", QString::SkipEmptyParts);

    if(!routeStr.isEmpty())
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
    else
    {
        error = true;
    }

    uiInner->tableViewValidator->setColumnWidth(0, 75);
    uiInner->tableViewValidator->setColumnWidth(1, 75);
    uiInner->tableViewValidator->setColumnWidth(2, 260);

    return !error;
}

bool DialogFlightCreator::verifyForm()
{
    if(!uiInner->lineEditCallsign->text().isEmpty() && (flight == nullptr))
    {
        ATCFlight *flight = simulation->getFlight(uiInner->lineEditCallsign->text());

        if(flight != nullptr)
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
    else if(uiInner->lineEditLatitude->text().isEmpty())
    {
        errorMessage("ERROR: Initial latitude not specified!");
        return false;
    }
    else if((uiInner->lineEditLatitude->text().toDouble() < -90) ||
            (uiInner->lineEditLatitude->text().toDouble() > 90))
    {
        errorMessage("ERROR: Initial latitude incorrect value!");
        return false;
    }
    else if(uiInner->lineEditLongitude->text().isEmpty())
    {
        errorMessage("ERROR: Initial longitude not specified!");
        return false;
    }
    else if((uiInner->lineEditLongitude->text().toDouble() < -180) ||
            (uiInner->lineEditLongitude->text().toDouble() > 180))
    {
        errorMessage("ERROR: Initial longitude incorrect value!");
        return false;
    }
    else if(uiInner->lineEditAltitudeCurrent->text().isEmpty())
    {
        errorMessage("ERROR: Initial altitude not specified!");
        return false;
    }
    else if((uiInner->lineEditAltitudeCurrent->text().left(1) != "A") &&
            (uiInner->lineEditAltitudeCurrent->text().left(1) != "F"))
    {
        errorMessage("ERROR: Incorrect initial altitude fomat!");
        return false;
    }
    else if(uiInner->lineEditTASCurrent->text().isEmpty())
    {
        errorMessage("ERROR: Initial TAS not specified!");
        return false;
    }
    else if(uiInner->spinBoxTrueHDG->text().isEmpty())
    {
        errorMessage("ERROR: Initial true heading not specified!");
        return false;
    }
    else if((uiInner->lineEditSquawkCurrent->text().at(0) == '8') ||
            (uiInner->lineEditSquawkCurrent->text().at(1) == '8') ||
            (uiInner->lineEditSquawkCurrent->text().at(2) == '8') ||
            (uiInner->lineEditSquawkCurrent->text().at(3) == '8') ||
            (uiInner->lineEditSquawkCurrent->text().at(0) == '9') ||
            (uiInner->lineEditSquawkCurrent->text().at(1) == '9') ||
            (uiInner->lineEditSquawkCurrent->text().at(2) == '9') ||
            (uiInner->lineEditSquawkCurrent->text().at(3) == '9'))
    {
        errorMessage("ERROR: Incorrect selected squawk format!");
        return false;
    }
    else if(!uiInner->radioButtonHDG->isChecked() && !uiInner->radioButtonOwnNav->isChecked())
    {
        errorMessage("ERROR: Navigation type not specified!");
        return false;
    }
    else if(!uiInner->lineEditAltitudeRes->text().isEmpty() &&
            (uiInner->lineEditAltitudeRes->text().left(1) != "A") &&
            (uiInner->lineEditAltitudeRes->text().left(1) != "F"))
    {
        errorMessage("ERROR: Incorrect altitude restriction fomat!");
        return false;
    }
    else if(!uiInner->lineEditSpeedRes->text().isEmpty() &&
            (uiInner->lineEditSpeedRes->text().at(1) != '.') &&
            (!uiInner->lineEditSpeedRes->text().at(1).isNumber()))
    {
        errorMessage("ERROR: Incorrect speed restriction format!");
        return false;
    }
    else if((uiInner->radioButtonOwnNav->isChecked()) &&
            (uiInner->comboBoxNextFix->currentText().isEmpty()))
    {
        errorMessage("ERROR: Next fix not selected!");
        return false;
    }
    else
    {
        uiInner->labelError->setVisible(false);

        return true;
    }
}

void DialogFlightCreator::errorMessage(QString msg)
{
    uiInner->labelError->setText(msg);
    uiInner->labelError->setVisible(true);
}

void DialogFlightCreator::formSetup()
{
    for(int i = 0; i < flightFactory->getAircraftTypeFactory().getTypeVectorSize(); i++)
    {
        QString type = flightFactory->getAircraftTypeFactory().getType(i)->getAcType().ICAOcode;
        uiInner->comboBoxAcftType->addItem(type, Qt::DisplayRole);
    }

    uiInner->spinBoxHeadingRes->clear();
    uiInner->spinBoxTrueHDG->clear();

    uiInner->spinBoxHeadingRes->setEnabled(false);
    uiInner->comboBoxNextFix->setEnabled(false);

    uiInner->lineEditCallsign->setInputMask(">AAAxxxxxx");
    uiInner->lineEditDeparture->setInputMask(">AAAA");
    uiInner->lineEditDestination->setInputMask(">AAAA");
    uiInner->lineEditAlternate->setInputMask(">AAAA");
    uiInner->lineEditAltitude->setInputMask(">A999");
    uiInner->lineEditTAS->setInputMask("9990");
    uiInner->lineEditSquawk->setInputMask("9999");

    uiInner->lineEditSquawkCurrent->setInputMask("9999");
    uiInner->lineEditAltitudeCurrent->setInputMask(">A999");
    uiInner->lineEditTASCurrent->setInputMask("9990");

    uiInner->lineEditAltitudeRes->setInputMask(">A999");
    uiInner->lineEditSpeedRes->setInputMask("9X90");

    uiInner->labelError->setVisible(false);

    uiInner->timeEditDepTime->setEnabled(false);
    uiInner->timeEditEnrTime->setEnabled(false);
    uiInner->timeEditFuelTime->setEnabled(false);

    model->setColumnCount(3);

    uiInner->tableViewValidator->setModel(model);
    uiInner->tableViewValidator->setGridStyle(Qt::NoPen);

    uiInner->tableViewValidator->horizontalHeader()->setHidden(true);
    uiInner->tableViewValidator->verticalHeader()->setHidden(true);
}

void DialogFlightCreator::formSetup(ATCFlight *flight)
{
    formSetup();

    if(flight != nullptr)
    {
        uiInner->lineEditCallsign->setText(flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber());

        switch(flight->getFlightPlan()->getFlightRules())
        {
            case ATC::IFR:
                uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("IFR"));
                break;

            case ATC::VFR:
                uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("VFR"));
                break;

            case ATC::SVFR:
                uiInner->comboBoxFlightRules->setCurrentIndex(uiInner->comboBoxFlightRules->findText("SVFR"));
                break;
        }

        QString acftType = flight->getFlightPlan()->getType()->getAcType().ICAOcode;
        uiInner->comboBoxAcftType->setCurrentIndex(uiInner->comboBoxAcftType->findText(acftType));

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

        uiInner->lineEditLatitude->setText(QString::number(ATCMath::rad2deg(flight->getState().y)));
        uiInner->lineEditLongitude->setText(QString::number(ATCMath::rad2deg(flight->getState().x)));

        double altitude = ATCMath::m2ft(flight->getState().h) / 100;
        if(altitude >= settings->TRANSITION_LEVEL)
        {
            if(altitude < 10)
            {
                uiInner->lineEditAltitudeCurrent->setText("F00" + QString::number(altitude).left(3));
            }
            else if(altitude < 100)
            {
                uiInner->lineEditAltitudeCurrent->setText("F0" + QString::number(altitude).left(3));
            }
            else
            {
                uiInner->lineEditAltitudeCurrent->setText("F" + QString::number(altitude).left(3));
            }
        }
        else
        {
            if(altitude < 10)
            {
                uiInner->lineEditAltitudeCurrent->setText("A00" + QString::number(altitude).left(3));
            }
            else if(altitude < 100)
            {
                uiInner->lineEditAltitudeCurrent->setText("A0" + QString::number(altitude).left(3));
            }
            else
            {
                uiInner->lineEditAltitudeCurrent->setText("A" + QString::number(altitude).left(3));
            }
        }

        uiInner->lineEditTASCurrent->setText(QString::number(qRound(ATCMath::mps2kt(flight->getState().v))));
        uiInner->lineEditSquawkCurrent->setText(flight->getSquawk());
        uiInner->spinBoxTrueHDG->setValue(qRound(ATCMath::rad2deg(flight->getState().hdg)));

        uiInner->lineEditAltitudeRes->setText(flight->getTargetAltitude());
        if(flight->getTargetSpeed() != "---") uiInner->lineEditSpeedRes->setText(flight->getTargetSpeed());

        switch(flight->getNavMode())
        {
            case ATC::Nav:
                uiInner->radioButtonOwnNav->setChecked(true);
                uiInner->radioButtonHDG->setChecked(false);

                uiInner->comboBoxNextFix->setEnabled(true);
                uiInner->spinBoxHeadingRes->setEnabled(false);

                on_tabWidget_tabBarClicked(1);
                break;

            case ATC::Hdg:
                uiInner->radioButtonOwnNav->setChecked(false);
                uiInner->radioButtonHDG->setChecked(true);

                uiInner->comboBoxNextFix->setEnabled(false);
                uiInner->spinBoxHeadingRes->setEnabled(true);

                uiInner->spinBoxHeadingRes->setValue(flight->getHdgRestriction());
                break;
        }

        uiInner->timeEditSimulationStart->setTime(flight->getSimStartTime());

        setupProcedureBoxes(flight);

        uiInner->comboBoxRwyDep->setCurrentText(flight->getRunwayDeparture());
        uiInner->comboBoxRwyDes->setCurrentText(flight->getRunwayDestination());
        uiInner->comboBoxSID->setCurrentText(flight->getSID());
        uiInner->comboBoxSTAR->setCurrentText(flight->getSTAR());

        switch(flight->getPredictionPhase())
        {
            case PredictionPhase::Climb:
                uiInner->comboBoxPredictionPhase->setCurrentText("CLIMB");
                break;

            case PredictionPhase::Cruise:
                uiInner->comboBoxPredictionPhase->setCurrentText("CRUISE");
                break;

            case PredictionPhase::Descent:
                uiInner->comboBoxPredictionPhase->setCurrentText("DESCENT");
                break;
        }
    }
}

void DialogFlightCreator::setupProcedureBoxes(ATCFlight *flight)
{
    QVector<ActiveAirport> activeAirports = simulation->getActiveRunways()->getActiveAirports();

    if(departureChanged)
    {
        uiInner->comboBoxRwyDep->clear();
        uiInner->comboBoxSID->clear();

        //Setup departure runways
        ATCAirport *adep = nullptr;
        QString activeDep = "";

        if(flight != nullptr)
        {
            adep = airspace->findAirport(flight->getFlightPlan()->getRoute().getDeparture());
        }
        else
        {
            adep = airspace->findAirport(uiInner->lineEditDeparture->text());
        }

        if(adep != nullptr)
        {
            for(int i = 0; i < activeAirports.size(); i++)
            {
                if(activeAirports.at(i).airportCode == adep->getName())
                {
                    QStringList depRwys = activeAirports.at(i).depRwys;
                    if(!depRwys.isEmpty()) activeDep = depRwys.at(0);
                }
            }

            QStringList runways;

            for(int i = 0; i < adep->getRunwayVectorSize(); i++)
            {
                runways.append(adep->getRunway(i)->getRunwayID1());
                runways.append(adep->getRunway(i)->getRunwayID2());
            }

            runways.sort();

            for(int i = 0; i < runways.size(); i++)
            {
                uiInner->comboBoxRwyDep->addItem(runways.at(i));
            }
        }

        uiInner->comboBoxRwyDep->addItem("");
        uiInner->comboBoxRwyDep->setCurrentText(activeDep);

        departureChanged = false;
    }

    if(destinationChanged)
    {
        uiInner->comboBoxRwyDes->clear();
        uiInner->comboBoxSTAR->clear();

        //Setup destination runways
        ATCAirport *ades = nullptr;
        QString activeArr = "";

        if(flight != nullptr)
        {
            ades = airspace->findAirport(flight->getFlightPlan()->getRoute().getDestination());
        }
        else
        {
            ades = airspace->findAirport(uiInner->lineEditDestination->text());
        }

        if(ades != nullptr)
        {
            for(int i = 0; i < activeAirports.size(); i++)
            {
                if(activeAirports.at(i).airportCode == ades->getName())
                {
                    QStringList arrRwys = activeAirports.at(i).arrRwys;
                    if(!arrRwys.isEmpty()) activeArr = arrRwys.at(0);
                }
            }

            QStringList runways;

            for(int i = 0; i < ades->getRunwayVectorSize(); i++)
            {
                runways.append(ades->getRunway(i)->getRunwayID1());
                runways.append(ades->getRunway(i)->getRunwayID2());
            }

            runways.sort();

            for(int i = 0; i < runways.size(); i++)
            {
                uiInner->comboBoxRwyDes->addItem(runways.at(i));
            }
        }

        uiInner->comboBoxRwyDes->addItem("");
        uiInner->comboBoxRwyDes->setCurrentText(activeArr);

        destinationChanged = false;
    }
}

void DialogFlightCreator::refreshFixList()
{
    ATCProcedureSID *sid = airspace->findSID(uiInner->comboBoxSID->currentText());
    ATCProcedureSTAR *star = airspace->findSTAR(uiInner->comboBoxSTAR->currentText());
    QStringList route = uiInner->plainTextEditRoute->toPlainText().split(" ", QString::SkipEmptyParts);

    uiInner->comboBoxNextFix->clear();
    uiInner->comboBoxNextFix->addItem(uiInner->lineEditDeparture->text());

    if(sid != nullptr)
    {
        for(int i = 0; i < sid->getFixListSize(); i++)
        {
            if(!route.isEmpty())
            {
                if(sid->getFixName(i) != route.at(0)) uiInner->comboBoxNextFix->addItem(sid->getFixName(i));
            }
            else
            {
                uiInner->comboBoxNextFix->addItem(sid->getFixName(i));
            }
        }
    }

    for(int i = 0; i < model->rowCount(); i++)
    {
        QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
        if(fix != "DCT") uiInner->comboBoxNextFix->addItem(fix, Qt::DisplayRole);
    }

    if(star != nullptr)
    {
        for(int i = 0; i < star->getFixListSize(); i++)
        {
            if(!route.empty())
            {
                if(star->getFixName(i) != route.at(route.size() - 1)) uiInner->comboBoxNextFix->addItem(star->getFixName(i));
            }
            else
            {
                uiInner->comboBoxNextFix->addItem(star->getFixName(i));
            }
        }
    }

    uiInner->comboBoxNextFix->addItem(uiInner->lineEditDestination->text());
    if(!uiInner->lineEditAlternate->text().isEmpty()) uiInner->comboBoxNextFix->addItem(uiInner->lineEditAlternate->text());
}

QStringList DialogFlightCreator::getFixList()
{
    QStringList fixList;
    ATCProcedureSID *sid = airspace->findSID(uiInner->comboBoxSID->currentText());
    ATCProcedureSTAR *star = airspace->findSTAR(uiInner->comboBoxSTAR->currentText());
    QString departure = uiInner->lineEditDeparture->text();
    QString destination = uiInner->lineEditDestination->text();
    QString alternate = uiInner->lineEditAlternate->text();
    QStringList routeStr = uiInner->plainTextEditRoute->toPlainText().split(" ", QString::SkipEmptyParts);

    fixList.append(departure);

    if(sid != nullptr)
    {
        for(int i = 0; i < sid->getFixListSize(); i++)
        {
            if(sid->getFixName(i) != routeStr.at(0)) fixList.append(sid->getFixName(i));
        }
    }

    for(int i = 0; i < model->rowCount(); i++)
    {
        QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
        if(fix != "DCT") fixList.append(fix);
    }

    if(star != nullptr)
    {
        for(int i = 0; i < star->getFixListSize(); i++)
        {
            if(star->getFixName(i) != routeStr.at(routeStr.size() - 1)) fixList.append(star->getFixName(i));
        }
    }

    fixList.append(destination);
    if(!alternate.isEmpty()) fixList.append(alternate);

    return fixList;
}

void DialogFlightCreator::on_buttonSetCallsign_clicked()
{
    setCallsign();
}

void DialogFlightCreator::on_buttonSetSquawk_clicked()
{
    setSquawk();
}

void DialogFlightCreator::on_buttonSetTAS_clicked()
{
    setTAS();
}

void DialogFlightCreator::slotShowFlightCreator()
{
    if(flight != nullptr)
    {
        if(flight->getRoutePrediction() != nullptr) flight->getRoutePrediction()->showRoute();
    }

    show();
}

void DialogFlightCreator::slotDisplayClicked(double x, double y)
{
    uiInner->lineEditLongitude->setText(QString::number(x));
    uiInner->lineEditLatitude->setText(QString::number(y));
}

void DialogFlightCreator::on_radioButtonOwnNav_clicked()
{
    uiInner->spinBoxHeadingRes->setEnabled(false);
    uiInner->spinBoxHeadingRes->clear();

    uiInner->comboBoxNextFix->setEnabled(true);
    refreshFixList();
}

void DialogFlightCreator::on_radioButtonHDG_clicked()
{
    uiInner->spinBoxHeadingRes->setEnabled(true);

    uiInner->comboBoxNextFix->setEnabled(false);
    uiInner->comboBoxNextFix->clear();
}

void DialogFlightCreator::on_tabWidget_tabBarClicked(int index)
{
    if(index == 1)
    {
        setupProcedureBoxes(nullptr);

        if(uiInner->radioButtonOwnNav->isChecked())
        {
            refreshFixList();

            if(flight != nullptr) uiInner->comboBoxNextFix->setCurrentIndex(uiInner->comboBoxNextFix->findText(flight->getNextFix()));
        }
    }
}

void DialogFlightCreator::on_plainTextEditRoute_textChanged()
{
    routeValid = validateRoute();
}

void DialogFlightCreator::on_lineEditDeparture_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    departureChanged = true;
}

void DialogFlightCreator::on_lineEditDestination_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    destinationChanged = true;
}

void DialogFlightCreator::on_comboBoxRwyDep_currentTextChanged(const QString &arg1)
{
    uiInner->comboBoxSID->clear();
    QString airportCode = uiInner->lineEditDeparture->text();

    QStringList route = uiInner->plainTextEditRoute->toPlainText().split(" ", QString::SkipEmptyParts);
    QString firstFix;
    if(!route.isEmpty()) firstFix = route.at(0);

    QString preferredSID;
    QStringList items;

    for(int i = 0; i < airspace->getSIDsVectorSize(); i++)
    {
        ATCProcedureSID *sid = airspace->getSID(i);
        if((airportCode == sid->getAirport()) && (arg1 == sid->getRunwayID())) items.append(sid->getName());
        if((airportCode == sid->getAirport()) && (arg1 == sid->getRunwayID()) && (firstFix == sid->getFixName(sid->getFixListSize() - 1))) preferredSID = sid->getName();
    }

    items.sort();
    uiInner->comboBoxSID->addItems(items);
    uiInner->comboBoxSID->addItem("");

    if(uiInner->comboBoxSID->findText(preferredSID) >= 0)
    {
        uiInner->comboBoxSID->setCurrentText(preferredSID);
    }
    else
    {
        uiInner->comboBoxSID->setCurrentText("");
    }

    if(uiInner->comboBoxNextFix->isEnabled()) refreshFixList();
}

void DialogFlightCreator::on_comboBoxRwyDes_currentTextChanged(const QString &arg1)
{
    uiInner->comboBoxSTAR->clear();
    QString airportCode = uiInner->lineEditDestination->text();

    QStringList route = uiInner->plainTextEditRoute->toPlainText().split(" ", QString::SkipEmptyParts);
    QString lastFix;
    if(!route.isEmpty()) lastFix = route.at(route.size() - 1);

    QString preferredSTAR;
    QStringList items;

    for(int i = 0; i < airspace->getSTARsVectorSize(); i++)
    {
        ATCProcedureSTAR *star = airspace->getSTAR(i);
        if((airportCode == star->getAirport()) && (arg1 == star->getRunwayID())) items.append(star->getName());
        if((airportCode == star->getAirport()) && (arg1 == star->getRunwayID()) && (lastFix == star->getFixName(0))) preferredSTAR = star->getName();
    }

    items.sort();
    uiInner->comboBoxSTAR->addItems(items);
    uiInner->comboBoxSTAR->addItem("");

    if(uiInner->comboBoxSTAR->findText(preferredSTAR) >= 0)
    {
        uiInner->comboBoxSTAR->setCurrentText(preferredSTAR);
    }
    else
    {
        uiInner->comboBoxSTAR->setCurrentText("");
    }

    if(uiInner->comboBoxNextFix->isEnabled()) refreshFixList();
}

void DialogFlightCreator::on_comboBoxSID_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    if(uiInner->comboBoxNextFix->isEnabled()) refreshFixList();
}

void DialogFlightCreator::on_comboBoxSTAR_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    if(uiInner->comboBoxNextFix->isEnabled()) refreshFixList();
}
