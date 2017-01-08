#include "dialogflight.h"
#include "ui_dialogflight.h"

DialogFlight::DialogFlight(ATCSimulation *simulation, ATCAirspace *airspace, QWidget *parent) :
    simulation(simulation),
    airspace(airspace),
    ATCDialog(parent, "Flight Creator", 600, 650),
    uiInner(new Ui::DialogFlight)
{
    uiInner->setupUi(this);
    windowSetup();

    dialogFlightSetup();
}

DialogFlight::~DialogFlight()
{
    if(model != nullptr) delete model;
    if(procedureDelegate != nullptr) delete procedureDelegate;
    delete uiInner;
}

void DialogFlight::slotUpdateFlightList(ATCFlight *flight)
{
    if(rowToEdit == -1)
    {
        appendRow(flight, model);
    }
    else
    {
        modifyRow(flight, rowToEdit, model);
        rowToEdit = -1;
    }
}

void DialogFlight::on_buttonCreateFlight_clicked()
{
    emit signalConstructDialogFlightCreator();
}

void DialogFlight::on_buttonReady_clicked()
{
    //TO BE IMPLEMENTED
}

void DialogFlight::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogFlight::dialogFlightSetup()
{
    model = new QStandardItemModel(this);

    model->setColumnCount(12);

    QStringList labelList;
    labelList << "T+" << "CS" << "TYPE" << "W" << "ADEP" << "R" << "SID" << "ADES" << "R" << "STAR" << "AFL" << "TAS";
    model->setHorizontalHeaderLabels(labelList);

    uiInner->tableViewFlights->setModel(model);
    uiInner->tableViewFlights->setGridStyle(Qt::NoPen);
    uiInner->tableViewFlights->setFocusPolicy(Qt::NoFocus);

    uiInner->tableViewFlights->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    uiInner->tableViewFlights->horizontalHeader()->setHighlightSections(false);
    uiInner->tableViewFlights->verticalHeader()->setHidden(true);

    uiInner->tableViewFlights->setColumnWidth(0, 60);       //Start time
    uiInner->tableViewFlights->setColumnWidth(1, 80);       //Callsign
    uiInner->tableViewFlights->setColumnWidth(2, 40);       //Type
    uiInner->tableViewFlights->setColumnWidth(3, 30);       //Weight category
    uiInner->tableViewFlights->setColumnWidth(4, 40);       //ADEP
    uiInner->tableViewFlights->setColumnWidth(5, 33);       //DEP RWY
    uiInner->tableViewFlights->setColumnWidth(6, 70);       //SID ID
    uiInner->tableViewFlights->setColumnWidth(7, 40);       //ADES
    uiInner->tableViewFlights->setColumnWidth(8, 33);       //ARR RWY
    uiInner->tableViewFlights->setColumnWidth(9, 70);       //STAR ID
    uiInner->tableViewFlights->setColumnWidth(10, 40);      //AFL
    uiInner->tableViewFlights->setColumnWidth(11, 40);      //TAS

    uiInner->tableViewFlights->setSelectionBehavior(QAbstractItemView::SelectRows);

    procedureDelegate = new ATCComboDelegate(airspace, simulation->getActiveRunways(), uiInner->tableViewFlights);
    uiInner->tableViewFlights->setItemDelegate(procedureDelegate);

    uiInner->tableViewFlights->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(uiInner->tableViewFlights, SIGNAL(clicked(QModelIndex)), this, SLOT(slotEdit(QModelIndex)));
}

void DialogFlight::appendRow(ATCFlight *flight, QStandardItemModel *model)
{
    QList<QStandardItem*> row;

    QStandardItem *startTime = new QStandardItem(flight->getSimStartTime().toString("HH:mm:ss"));
    startTime->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    startTime->setTextAlignment(Qt::AlignCenter);
    row.append(startTime);

    QStandardItem *callsign = new QStandardItem(flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber());
    callsign->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    callsign->setTextAlignment(Qt::AlignCenter);
    row.append(callsign);

    QStandardItem *type = new QStandardItem(flight->getFlightPlan()->getType()->getAcType().ICAOcode);
    type->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    type->setTextAlignment(Qt::AlignCenter);
    row.append(type);

    QStandardItem *wtc;
    ATC::WakeCategory wake = flight->getFlightPlan()->getType()->getAcType().wake;

    switch(wake)
    {
        case ATC::L:
            wtc = new QStandardItem("L");
            break;

        case ATC::M:
            wtc = new QStandardItem("M");
            break;

        case ATC::H:
            wtc = new QStandardItem("H");
            break;

        case ATC::J:
            wtc = new QStandardItem("J");
            break;

        default:
            wtc = new QStandardItem();
            break;
    }

    wtc->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    wtc->setTextAlignment(Qt::AlignCenter);
    row.append(wtc);

    QStandardItem *adep = new QStandardItem(flight->getFlightPlan()->getRoute().getDeparture());
    adep->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    adep->setTextAlignment(Qt::AlignCenter);
    row.append(adep);

    QStandardItem *depRwy = new QStandardItem("29"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    depRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    depRwy->setTextAlignment(Qt::AlignCenter);
    row.append(depRwy);

    QStandardItem *sid = new QStandardItem("EVINA5K"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    sid->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    sid->setTextAlignment(Qt::AlignCenter);
    row.append(sid);

    QStandardItem *ades = new QStandardItem(flight->getFlightPlan()->getRoute().getDestination());
    ades->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ades->setTextAlignment(Qt::AlignCenter);
    row.append(ades);

    QStandardItem *desRwy = new QStandardItem("15"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    desRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    desRwy->setTextAlignment(Qt::AlignCenter);
    row.append(desRwy);

    QStandardItem *star = new QStandardItem("SOXER3G"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    star->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    star->setTextAlignment(Qt::AlignCenter);
    row.append(star);

    QStandardItem *afl;
    double height = ATCMath::m2ft(flight->getState().h) / 100;

    if(height < 10)
    {
        afl = new QStandardItem("00" + QString::number(height).left(3));
    }
    else if(height < 100)
    {
        afl = new QStandardItem("0" + QString::number(height).left(3));
    }
    else
    {
        afl = new QStandardItem(QString::number(height).left(3));
    }

    afl->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    afl->setTextAlignment(Qt::AlignCenter);
    row.append(afl);

    QStandardItem *tas = new QStandardItem(QString::number(ATCMath::mps2kt(flight->getState().v)));
    tas->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    tas->setTextAlignment(Qt::AlignCenter);
    row.append(tas);

    model->appendRow(row);

    for(int i = 0; i < model->rowCount(); i++)
    {
        uiInner->tableViewFlights->setRowHeight(i, 20);
    }
}

void DialogFlight::modifyRow(ATCFlight *flight, int row, QStandardItemModel *model)
{
    model->item(row, 0)->setText(flight->getSimStartTime().toString("HH:mm:ss"));
    model->item(row, 1)->setText(flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber());
    model->item(row, 2)->setText(flight->getFlightPlan()->getType()->getAcType().ICAOcode);

    switch(flight->getFlightPlan()->getType()->getAcType().wake)
    {
    case ATC::L:
        model->item(row, 3)->setText("L");
        break;

    case ATC::M:
        model->item(row, 3)->setText("M");
        break;

    case ATC::H:
        model->item(row, 3)->setText("H");
        break;

    case ATC::J:
        model->item(row, 3)->setText("J");
        break;
    }

    model->item(row, 4)->setText(flight->getFlightPlan()->getRoute().getDeparture());
    model->item(row, 5)->setText("29");           //DEP RWY
    model->item(row, 6)->setText("EVINA5K");      //SID
    model->item(row, 7)->setText(flight->getFlightPlan()->getRoute().getDestination());
    model->item(row, 8)->setText("15");           //DEP RWY
    model->item(row, 9)->setText("SOXER3G");      //SID

    double height = ATCMath::m2ft(flight->getState().h) / 100;

    if(height < 10)
    {
        model->item(row, 10)->setText("00" + QString::number(height).left(3));
    }
    else if(height < 100)
    {
        model->item(row, 10)->setText("0" + QString::number(height).left(3));
    }
    else
    {
        model->item(row, 10)->setText(QString::number(height).left(3));
    }

    model->item(row, 11)->setText(QString::number(ATCMath::mps2kt(flight->getState().v)));
}

void DialogFlight::on_buttonEditFlight_clicked()
{
    QModelIndexList selectionList = uiInner->tableViewFlights->selectionModel()->selectedRows();

    if(selectionList.size() > 0)
    {
        emit signalConstructDialogFlightCreator(simulation->getFlight(model->index(selectionList.at(0).row(), 1).data().toString()));

        rowToEdit = selectionList.at(0).row();
    }
}

void DialogFlight::on_buttonDeleteFlight_clicked()
{
    QModelIndexList selectionList = uiInner->tableViewFlights->selectionModel()->selectedRows();

    for(int i = selectionList.size() - 1; i >= 0; i--)
    {
        QString callsign = model->index(selectionList.at(i).row(), 1).data().toString();
        simulation->removeFlight(callsign);
        model->removeRow(selectionList.at(i).row());
    }
}

void DialogFlight::on_buttonDeleteAll_clicked()
{
    QVector<ATCFlight*> flights = simulation->getFlightsVector();

    for(int i = 0; i < flights.size(); i++)
    {
        delete flights.at(i);
    }

    simulation->clearFlights();
    model->removeRows(0, model->rowCount());
}

void DialogFlight::on_buttonActiveRunways_clicked()
{
    emit signalConstructDialogActiveRunways();
}

void DialogFlight::slotEdit(QModelIndex index)
{
    if((index.column() == 5) ||
       (index.column() == 6) ||
       (index.column() == 8) ||
       (index.column() == 9))
    {
        uiInner->tableViewFlights->edit(index);
    }
}
