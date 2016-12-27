#include "dialogflight.h"
#include "ui_dialogflight.h"

DialogFlight::DialogFlight(ATCSimulation *simulation, QWidget *parent) :
    simulation(simulation),
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
    delete uiInner;
}

void DialogFlight::slotUpdateFlightList(ATCFlight *flight)
{
    appendRow(flight, model);
}

void DialogFlight::on_buttonCreateFlight_clicked()
{
    emit signalConstructFlightCreator();
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
    labelList << "T+" << "CS" << "TYPE" << "W" << "ADEP" << "R" << "SID" << "ADES" << "R" << "STAR" << "AFL" << "CFL";
    model->setHorizontalHeaderLabels(labelList);

    uiInner->tableViewFlights->setModel(model);
    uiInner->tableViewFlights->setGridStyle(Qt::NoPen);
    uiInner->tableViewFlights->setFocusPolicy(Qt::NoFocus);

    uiInner->tableViewFlights->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    uiInner->tableViewFlights->horizontalHeader()->setHighlightSections(false);
    uiInner->tableViewFlights->verticalHeader()->setHidden(true);

    uiInner->tableViewFlights->setColumnWidth(0, 60);   //Start time
    uiInner->tableViewFlights->setColumnWidth(1, 80);   //Callsign
    uiInner->tableViewFlights->setColumnWidth(2, 40);   //Type
    uiInner->tableViewFlights->setColumnWidth(3, 30);   //Weight category
    uiInner->tableViewFlights->setColumnWidth(4, 40);   //ADEP
    uiInner->tableViewFlights->setColumnWidth(5, 33);   //DEP RWY
    uiInner->tableViewFlights->setColumnWidth(6, 70);   //SID ID
    uiInner->tableViewFlights->setColumnWidth(7, 40);   //ADES
    uiInner->tableViewFlights->setColumnWidth(8, 33);   //ARR RWY
    uiInner->tableViewFlights->setColumnWidth(9, 70);   //STAR ID
    uiInner->tableViewFlights->setColumnWidth(10, 40);  //AFL
    uiInner->tableViewFlights->setColumnWidth(11, 40);  //CFL

    uiInner->tableViewFlights->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    depRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    depRwy->setTextAlignment(Qt::AlignCenter);
    row.append(depRwy);

    QStandardItem *sid = new QStandardItem("EVINA5K"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    sid->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    sid->setTextAlignment(Qt::AlignCenter);
    row.append(sid);

    QStandardItem *ades = new QStandardItem(flight->getFlightPlan()->getRoute().getDestination());
    ades->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ades->setTextAlignment(Qt::AlignCenter);
    row.append(ades);

    QStandardItem *desRwy = new QStandardItem("15"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    desRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    desRwy->setTextAlignment(Qt::AlignCenter);
    row.append(desRwy);

    QStandardItem *star = new QStandardItem("SOXER3G"); //TO BE CHANGED WHEN ACTIVE RWY SYSTEM IS IMPLEMENTED
    star->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    star->setTextAlignment(Qt::AlignCenter);
    row.append(star);

    QStandardItem *afl = new QStandardItem(QString::number(ATCMath::m2ft(flight->getState().h) / 100).left(3));
    afl->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    afl->setTextAlignment(Qt::AlignCenter);
    row.append(afl);

    QStandardItem *cfl = new QStandardItem(flight->getTargetAltitude().right(3));
    cfl->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    cfl->setTextAlignment(Qt::AlignCenter);
    row.append(cfl);

    model->appendRow(row);
    uiInner->tableViewFlights->setRowHeight(model->rowCount() - 1, 20);
}

void DialogFlight::modifyRow(ATCFlight *flight, QStandardItemModel *model)
{

}

void DialogFlight::on_buttonEditFlight_clicked()
{

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
