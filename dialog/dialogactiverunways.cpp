#include "dialogactiverunways.h"
#include "ui_dialogactiverunways.h"

DialogActiveRunways::DialogActiveRunways(ATCAirspace *airspace, QWidget *parent) :
    ATCDialog(parent, "Active Runways", 600, 650),
    airspace(airspace),
    uiInner(new Ui::DialogActiveRunways)
{
    uiInner->setupUi(this);
    windowSetup();

    dialogSetup();
}

DialogActiveRunways::~DialogActiveRunways()
{
    if(model != nullptr) delete model;
    delete uiInner;
}

void DialogActiveRunways::on_buttonOK_clicked()
{

}

void DialogActiveRunways::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogActiveRunways::dialogSetup()
{
    model = new QStandardItemModel(this);

    model->setColumnCount(7);

    QStringList labelList;
    labelList << "DEP" << "ARR" << "AIRPORT CODE" << "RWY" << "DEP" << "ARR" << "";
    model->setHorizontalHeaderLabels(labelList);

    uiInner->tableViewRunways->setModel(model);
    uiInner->tableViewRunways->setGridStyle(Qt::NoPen);
    uiInner->tableViewRunways->setFocusPolicy(Qt::NoFocus);

    uiInner->tableViewRunways->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    uiInner->tableViewRunways->horizontalHeader()->setHighlightSections(false);
    uiInner->tableViewRunways->verticalHeader()->setHidden(true);

    uiInner->tableViewRunways->setColumnWidth(0, 60);       //AIRPORT ARR
    uiInner->tableViewRunways->setColumnWidth(1, 60);       //AIRPORT DEP
    uiInner->tableViewRunways->setColumnWidth(2, 150);      //AIRPORT CODE
    uiInner->tableViewRunways->setColumnWidth(3, 65);       //RUNWAY CODE
    uiInner->tableViewRunways->setColumnWidth(4, 60);       //RUNWAY ARR
    uiInner->tableViewRunways->setColumnWidth(5, 60);       //RUNWAY DEP
    uiInner->tableViewRunways->setColumnWidth(6, 119);      //AIRPORT CODE

    for(int i = 0; i < airspace->getAirportsVectorSize(); i++)
    {
        if(airspace->getAirport(i)->getRunwayVectorSize() > 0) appendAirport(airspace->getAirport(i), model);
    }

    for(int i = 0; i < airspace->getAirportsVectorSize(); i++)
    {
        if(airspace->getAirport(i)->getRunwayVectorSize() == 0) appendAirport(airspace->getAirport(i), model);
    }

    for(int i = 0; i < model->rowCount(); i++)
    {
        uiInner->tableViewRunways->setRowHeight(i, 25);
    }
}

void DialogActiveRunways::appendAirport(ATCAirport *airport, QStandardItemModel *model)
{
    if(airport->getRunwayVectorSize() > 0)
    {
        for(int i = 0; i < airport->getRunwayVectorSize(); i++)
        {
            appendRow(airport->getName(), airport->getRunway(i)->getRunwayID1(), model);
            appendRow(airport->getName(), airport->getRunway(i)->getRunwayID2(), model);
        }
    }
    else
    {
        appendRow(airport->getName(), model);
    }
}

void DialogActiveRunways::appendRow(QString airportCode, QString rwyCode, QStandardItemModel *model)
{
    QList<QStandardItem*> row;

    QStandardItem *spacer1 = new QStandardItem();
    spacer1->setFlags(Qt::NoItemFlags);
    spacer1->setTextAlignment(Qt::AlignCenter);
    row.append(spacer1);

    QStandardItem *spacer2 = new QStandardItem();
    spacer2->setFlags(Qt::NoItemFlags);
    spacer2->setTextAlignment(Qt::AlignCenter);
    row.append(spacer2);

    QStandardItem *airport = new QStandardItem(airportCode);
    airport->setFlags(Qt::NoItemFlags);
    airport->setTextAlignment(Qt::AlignCenter);
    row.append(airport);

    QStandardItem *runway = new QStandardItem(rwyCode);
    runway->setFlags(Qt::NoItemFlags);
    runway->setTextAlignment(Qt::AlignCenter);
    row.append(runway);

    QStandardItem *spacer3 = new QStandardItem();
    spacer3->setFlags(Qt::NoItemFlags);
    spacer3->setTextAlignment(Qt::AlignCenter);
    row.append(spacer3);

    QStandardItem *spacer4 = new QStandardItem();
    spacer4->setFlags(Qt::NoItemFlags);
    spacer4->setTextAlignment(Qt::AlignCenter);
    row.append(spacer4);

    QStandardItem *spacer5 = new QStandardItem();
    spacer5->setFlags(Qt::NoItemFlags);
    spacer5->setTextAlignment(Qt::AlignCenter);
    row.append(spacer5);

    model->appendRow(row);

    createCenteredCheckbox(model->indexFromItem(spacer1));
    createCenteredCheckbox(model->indexFromItem(spacer2));
    createCenteredCheckbox(model->indexFromItem(spacer3));
    createCenteredCheckbox(model->indexFromItem(spacer4));
}

void DialogActiveRunways::appendRow(QString airportCode, QStandardItemModel *model)
{
    QList<QStandardItem*> row;

    QStandardItem *spacer1 = new QStandardItem();
    spacer1->setFlags(Qt::NoItemFlags);
    spacer1->setTextAlignment(Qt::AlignCenter);
    row.append(spacer1);

    QStandardItem *spacer2 = new QStandardItem();
    spacer2->setFlags(Qt::NoItemFlags);
    spacer2->setTextAlignment(Qt::AlignCenter);
    row.append(spacer2);

    QStandardItem *airport = new QStandardItem(airportCode);
    airport->setFlags(Qt::NoItemFlags);
    airport->setTextAlignment(Qt::AlignCenter);
    row.append(airport);

    QStandardItem *runway = new QStandardItem();
    runway->setFlags(Qt::NoItemFlags);
    runway->setTextAlignment(Qt::AlignCenter);
    row.append(runway);

    QStandardItem *spacer3 = new QStandardItem();
    spacer3->setFlags(Qt::NoItemFlags);
    spacer3->setTextAlignment(Qt::AlignCenter);
    row.append(spacer3);

    QStandardItem *spacer4 = new QStandardItem();
    spacer4->setFlags(Qt::NoItemFlags);
    spacer4->setTextAlignment(Qt::AlignCenter);
    row.append(spacer4);

    QStandardItem *spacer5 = new QStandardItem();
    spacer5->setFlags(Qt::NoItemFlags);
    spacer5->setTextAlignment(Qt::AlignCenter);
    row.append(spacer5);

    model->appendRow(row);

    createCenteredCheckbox(model->indexFromItem(spacer1));
    createCenteredCheckbox(model->indexFromItem(spacer2));
}

void DialogActiveRunways::createCenteredCheckbox(QModelIndex &index)
{
    QWidget *centeredCheckBoxWidget = new QWidget(uiInner->tableViewRunways);

    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(centeredCheckBoxWidget);

    checkBoxLayout->addWidget(checkBox);
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);

    uiInner->tableViewRunways->setIndexWidget(index, centeredCheckBoxWidget);
}
