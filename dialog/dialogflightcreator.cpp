#include "dialogflightcreator.h"
#include "ui_dialogflightcreator.h"

DialogFlightCreator::DialogFlightCreator(ATCAirspace *airspace, ATCFlightFactory *flightFactory, ATCSimulation *simulation, QWidget *parent) :
    ATCDialog(parent, "Flight Creator", 600, 700),
    uiInner(new Ui::DialogFlightCreator),
    airspace(airspace),
    flightFactory(flightFactory),
    simulation(simulation),
    model(new QStandardItemModel(this))
{
    uiInner->setupUi(this);
    windowSetup();

    for(int i = 0; i < flightFactory->getAircraftTypeFactory().getTypeVectorSize(); i++)
    {
        QString type = flightFactory->getAircraftTypeFactory().getType(i)->getAcType().ICAOcode;
        uiInner->comboBoxAcftType->addItem(type, Qt::DisplayRole);
    }

    uiInner->spinBoxHeadingRes->clear();
    uiInner->spinBoxTrueHDG->clear();

    uiInner->spinBoxHeadingRes->setEnabled(false);
    uiInner->comboBoxNextFix->setEnabled(false);

    uiInner->lineEditCallsign->setInputMask(">AAAxxxxxxx");
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

DialogFlightCreator::~DialogFlightCreator()
{
    if(model != nullptr) delete model;
    delete uiInner;
}

void DialogFlightCreator::on_buttonOK_clicked()
{
    bool filledOK = verifyForm();

    if(filledOK)
    {
        //Initial state assignment
        State state;

        state.x = uiInner->lineEditLongitude->text().toDouble();
        state.y = uiInner->lineEditLatitude->text().toDouble();

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

        fixList.append(departure);
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") fixList.append(fix);
        }
        fixList.append(destination);
        if(!alternate.isEmpty()) fixList.append(alternate);

        flight->setFixList(fixList);

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

        //Append flight to simulation
        simulation->appendFlight(flight);

        emit signalCreateFlightTag(flight);

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
    emit signalGetLocation();
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

    uiInner->tableViewValidator->setColumnWidth(0, 75);
    uiInner->tableViewValidator->setColumnWidth(1, 75);
    uiInner->tableViewValidator->setColumnWidth(2, 260);

    return !error;
}

bool DialogFlightCreator::verifyForm()
{
    if(!uiInner->lineEditCallsign->text().isEmpty())
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
    uiInner->comboBoxNextFix->clear();

    uiInner->comboBoxNextFix->addItem(uiInner->lineEditDeparture->text());
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
        if(fix != "DCT") uiInner->comboBoxNextFix->addItem(fix, Qt::DisplayRole);
    }
    uiInner->comboBoxNextFix->addItem(uiInner->lineEditDestination->text());
    if(!uiInner->lineEditAlternate->text().isEmpty()) uiInner->comboBoxNextFix->addItem(uiInner->lineEditAlternate->text());
}

void DialogFlightCreator::on_radioButtonHDG_clicked()
{
    uiInner->spinBoxHeadingRes->setEnabled(true);

    uiInner->comboBoxNextFix->setEnabled(false);
    uiInner->comboBoxNextFix->clear();
}

void DialogFlightCreator::on_tabWidget_tabBarClicked(int index)
{
    if((index == 1) && uiInner->radioButtonOwnNav->isChecked())
    {
        uiInner->comboBoxNextFix->clear();

        uiInner->comboBoxNextFix->addItem(uiInner->lineEditDeparture->text());
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString fix = model->data(model->index(i, 1), Qt::DisplayRole).toString();
            if(fix != "DCT") uiInner->comboBoxNextFix->addItem(fix, Qt::DisplayRole);
        }
        uiInner->comboBoxNextFix->addItem(uiInner->lineEditDestination->text());
        if(!uiInner->lineEditAlternate->text().isEmpty()) uiInner->comboBoxNextFix->addItem(uiInner->lineEditAlternate->text());
    }
}

void DialogFlightCreator::on_plainTextEditRoute_textChanged()
{
    routeValid = validateRoute();
}
