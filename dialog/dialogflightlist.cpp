#include "dialogflightlist.h"
#include "ui_dialogflightlist.h"

DialogFlightList::DialogFlightList(ATCAirspace *airspace, ATCSimulation *simulation, QWidget *parent) :
    ATCDialog(parent, "Flight List", 530, 300),
    airspace(airspace),
    simulation(simulation),
    uiInner(new Ui::DialogFlightList)
{
    uiInner->setupUi(this);
    windowSetup();

    dialogFlightListSetup();
}

DialogFlightList::~DialogFlightList()
{
    delete uiInner;
    if(procedureDelegate != nullptr) delete procedureDelegate;
}

void DialogFlightList::clearFlightList()
{
    model->removeRows(0, model->rowCount());
}

void DialogFlightList::setSimulation(ATCSimulation *sim)
{
    simulation = sim;
    initializeComboDelegate();
}

void DialogFlightList::fillFlightList()
{
    if(simulation != nullptr)
    {
        for(int i = 0; i < simulation->getFlightsVectorSize(); i++)
        {
            ATCFlight *flight = simulation->getFlight(i);
            if(flight->isSimulated()) appendRow(simulation->getFlight(i), model);
        }

        model->sort(0, Qt::AscendingOrder);
    }
}

void DialogFlightList::slotUpdateFlightList()
{
    clearFlightList();
    fillFlightList();
}

void DialogFlightList::on_tableViewList_clicked(const QModelIndex &index)
{
    ATCFlight *flight = simulation->getFlight(model->index(index.row(), 0).data().toString());

    if(index.column() == 0)
    {
        emit signalCreateDialogFlightPlan(flight);
    }
    if(index.column() == 1)
    {
        flight->slotDisplayRoute();
    }
}

void DialogFlightList::slotEdit(QModelIndex index)
{
    if((index.column() == 5) ||
       (index.column() == 6) ||
       (index.column() == 8) ||
       (index.column() == 9))
    {
        uiInner->tableViewList->edit(index);
    }
}

void DialogFlightList::dialogFlightListSetup()
{
    model = new QStandardItemModel(this);

    formatList();
    createComboDelegate();
    fillFlightList();
}

void DialogFlightList::formatList()
{
    model->setColumnCount(11);

    QStringList labelList;
    labelList << "CS" << "TYPE" << "CFL" << "RFL" << "ADEP" << "R" << "SID" << "ADES" << "R" << "STAR" << "W";
    model->setHorizontalHeaderLabels(labelList);

    uiInner->tableViewList->setModel(model);
    uiInner->tableViewList->setGridStyle(Qt::NoPen);
    uiInner->tableViewList->setFocusPolicy(Qt::NoFocus);

    uiInner->tableViewList->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    uiInner->tableViewList->horizontalHeader()->setHighlightSections(false);
    uiInner->tableViewList->verticalHeader()->setHidden(true);

    uiInner->tableViewList->setColumnWidth(0, 86);       //Callsign
    uiInner->tableViewList->setColumnWidth(1, 40);       //Type
    uiInner->tableViewList->setColumnWidth(2, 40);       //CFL
    uiInner->tableViewList->setColumnWidth(3, 40);       //RFL
    uiInner->tableViewList->setColumnWidth(4, 40);       //ADEP
    uiInner->tableViewList->setColumnWidth(5, 33);       //DEP RWY
    uiInner->tableViewList->setColumnWidth(6, 70);       //SID ID
    uiInner->tableViewList->setColumnWidth(7, 40);       //ADES
    uiInner->tableViewList->setColumnWidth(8, 33);       //ARR RWY
    uiInner->tableViewList->setColumnWidth(9, 70);       //STAR ID
    uiInner->tableViewList->setColumnWidth(10, 30);      //Weight category

    uiInner->tableViewList->setSelectionMode(QAbstractItemView::NoSelection);
}

void DialogFlightList::createComboDelegate()
{
    if(simulation != nullptr)
    {
        procedureDelegate = new ATCComboDelegate(airspace, simulation, simulation->getActiveRunways(), DelegateLocation::DialogFlightList, uiInner->tableViewList);
        uiInner->tableViewList->setItemDelegate(procedureDelegate);

        uiInner->tableViewList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        connect(uiInner->tableViewList, SIGNAL(clicked(QModelIndex)), this, SLOT(slotEdit(QModelIndex)));
    }
}

void DialogFlightList::initializeComboDelegate()
{
    if(procedureDelegate != nullptr)
    {
        procedureDelegate->setSimulation(simulation);
    }
    else if(simulation != nullptr)
    {
        createComboDelegate();
    }
}

void DialogFlightList::appendRow(ATCFlight *flight, QStandardItemModel *model)
{
    QList<QStandardItem*> row;

    QStandardItem *callsign = new QStandardItem(flight->getFlightPlan()->getCompany()->getCode() + flight->getFlightPlan()->getFlightNumber());
    callsign->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    callsign->setTextAlignment(Qt::AlignCenter);
    row.append(callsign);

    QStandardItem *type = new QStandardItem(flight->getFlightPlan()->getType()->getAcType().ICAOcode);
    type->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    type->setTextAlignment(Qt::AlignCenter);
    row.append(type);

    QStandardItem *cfl = new QStandardItem(flight->getTargetAltitude().right(3));
    cfl->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    cfl->setTextAlignment(Qt::AlignCenter);
    row.append(cfl);

    QStandardItem *rfl = new QStandardItem(flight->getFlightPlan()->getAltitude().right(3));
    rfl->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    rfl->setTextAlignment(Qt::AlignCenter);
    row.append(rfl);

    QStandardItem *adep = new QStandardItem(flight->getFlightPlan()->getRoute().getDeparture());
    adep->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    adep->setTextAlignment(Qt::AlignCenter);
    row.append(adep);

    QStandardItem *depRwy = new QStandardItem(flight->getRunwayDeparture());
    depRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    depRwy->setTextAlignment(Qt::AlignCenter);
    row.append(depRwy);

    QStandardItem *sid = new QStandardItem(flight->getSID());
    sid->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    sid->setTextAlignment(Qt::AlignCenter);
    row.append(sid);

    QStandardItem *ades = new QStandardItem(flight->getFlightPlan()->getRoute().getDestination());
    ades->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ades->setTextAlignment(Qt::AlignCenter);
    row.append(ades);

    QStandardItem *desRwy = new QStandardItem(flight->getRunwayDestination());
    desRwy->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    desRwy->setTextAlignment(Qt::AlignCenter);
    row.append(desRwy);

    QStandardItem *star = new QStandardItem(flight->getSTAR());
    star->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    star->setTextAlignment(Qt::AlignCenter);
    row.append(star);

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

    model->appendRow(row);

    for(int i = 0; i < model->rowCount(); i++)
    {
        uiInner->tableViewList->setRowHeight(i, 20);
    }
}
