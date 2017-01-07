#include "dialogactiverunways.h"
#include "ui_dialogactiverunways.h"

DialogActiveRunways::DialogActiveRunways(ATCAirspace *airspace, ATCActiveRunways *activeRunways, QWidget *parent) :
    ATCDialog(parent, "Active Runways", 600, 650),
    airspace(airspace),
    activeRunways(activeRunways),
    uiInner(new Ui::DialogActiveRunways)
{
    uiInner->setupUi(this);
    windowSetup();

    dialogSetup();
    fillModel();
}

DialogActiveRunways::~DialogActiveRunways()
{
    if(model != nullptr) delete model;
    delete uiInner;
}

void DialogActiveRunways::on_buttonOK_clicked()
{
    activeRunways->clearActiveAirports();

    QString tempAirport = "";

    for(int i = 0; i < model->rowCount(); i++)
    {
        QCheckBox *boxAirportDep = dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 0))->layout()->itemAt(0)->widget());
        QCheckBox *boxAirportArr = dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 1))->layout()->itemAt(0)->widget());
        QCheckBox *boxRunwayDep = nullptr;
        QCheckBox *boxRunwayArr = nullptr;

        if(uiInner->tableViewRunways->indexWidget(model->index(i, 4)) != nullptr) boxRunwayDep = dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 4))->layout()->itemAt(0)->widget());
        if(uiInner->tableViewRunways->indexWidget(model->index(i, 5)) != nullptr) boxRunwayArr = dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 5))->layout()->itemAt(0)->widget());

        bool boxAirportDepChecked = boxAirportDep->isChecked();
        bool boxAirportArrChecked = boxAirportArr->isChecked();
        bool boxRunwayDepChecked = false;
        bool boxRunwayArrChecked = false;

        if(boxRunwayDep != nullptr) boxRunwayDepChecked = boxRunwayDep->isChecked();
        if(boxRunwayArr != nullptr) boxRunwayArrChecked = boxRunwayArr->isChecked();

        if(boxAirportDepChecked || boxAirportArrChecked || boxRunwayDepChecked || boxRunwayArrChecked)
        {
            QString airport = model->index(i, 2).data().toString();

            if(airport != tempAirport) //Create new ActiveAirport
            {
                ActiveAirport activeAirport;

                activeAirport.airportCode = airport;
                if(boxAirportDepChecked) activeAirport.dep = true;
                if(boxAirportArrChecked) activeAirport.arr = true;
                if(boxRunwayDepChecked) activeAirport.depRwys.append(model->index(i, 3).data().toString());
                if(boxRunwayArrChecked) activeAirport.arrRwys.append(model->index(i, 3).data().toString());

                activeRunways->appendActiveAirport(activeAirport);

                tempAirport = airport;
            }
            else //Append data to last ActiveAirport
            {
                ActiveAirport activeAirport = activeRunways->getActiveAirport(activeRunways->getActiveAirports().size() - 1);

                if(boxRunwayDepChecked)
                {
                    activeAirport.depRwys.append(model->index(i, 3).data().toString());
                    activeRunways->getActiveAirports().replace(activeRunways->getActiveAirports().size() - 1, activeAirport);
                }

                if(boxRunwayArrChecked)
                {
                    activeAirport.arrRwys.append(model->index(i, 3).data().toString());
                    activeRunways->getActiveAirports().replace(activeRunways->getActiveAirports().size() - 1, activeAirport);
                }
            }
        }
    }

    emit closed();
    close();
}

void DialogActiveRunways::on_buttonCancel_clicked()
{
    emit closed();
    close();
}

void DialogActiveRunways::slotCheckBoxStateChanged(int i)
{
    QModelIndex checkBoxIndex = getCheckBoxIndex(dynamic_cast<QCheckBox*>(sender()));

    if((checkBoxIndex.column() == 0) || (checkBoxIndex.column() == 1))
    {
        for(int j = 0; j < model->rowCount(); j++)
        {
            if(model->index(j, 2).data().toString() == model->index(checkBoxIndex.row(), 2).data().toString())
            {
                QCheckBox *box = dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(j, checkBoxIndex.column()))->layout()->itemAt(0)->widget());

                if(i == 0) //Box unchecked
                {
                    box->setChecked(false);
                }
                else if(i == 2) //Box checked
                {
                    box->setChecked(true);
                }
            }
        }
    }
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
    uiInner->tableViewRunways->setColumnWidth(6, 119);      //SPACER

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

        connect(dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 0))->layout()->itemAt(0)->widget()), SIGNAL(stateChanged(int)), this, SLOT(slotCheckBoxStateChanged(int)));
        connect(dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 1))->layout()->itemAt(0)->widget()), SIGNAL(stateChanged(int)), this, SLOT(slotCheckBoxStateChanged(int)));

        if(uiInner->tableViewRunways->indexWidget(model->index(i, 4)) != nullptr) connect(dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 4))->layout()->itemAt(0)->widget()), SIGNAL(stateChanged(int)), this, SLOT(slotCheckBoxStateChanged(int)));
        if(uiInner->tableViewRunways->indexWidget(model->index(i, 5)) != nullptr) connect(dynamic_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(i, 5))->layout()->itemAt(0)->widget()), SIGNAL(stateChanged(int)), this, SLOT(slotCheckBoxStateChanged(int)));
    }
}

void DialogActiveRunways::fillModel()
{
    for(int i = 0; i < activeRunways->getActiveAirports().size(); i++)
    {
        ActiveAirport airport = activeRunways->getActiveAirport(i);
        setActive(airport);
    }
}

void DialogActiveRunways::setActive(ActiveAirport &airport)
{
    QString airportCode = airport.airportCode;
    bool dep = airport.dep;
    bool arr = airport.arr;
    QStringList depRwys = airport.depRwys;
    QStringList arrRwys = airport.arrRwys;

    QVector<QModelIndex> indexes;

    for(int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex currentIndex = model->index(i, 2);
        if(currentIndex.data().toString() == airportCode) indexes.append(currentIndex);
    }

    for(int i = 0; i < indexes.size(); i++)
    {
        int row = indexes.at(i).row();

        if(dep) static_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(row, 0))->layout()->itemAt(0)->widget())->setChecked(true);
        if(arr) static_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(row, 1))->layout()->itemAt(0)->widget())->setChecked(true);

        for(int j = 0; j < depRwys.size(); j++)
        {
            if(depRwys.at(j) == model->index(row, 3).data().toString()) static_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(row, 4))->layout()->itemAt(0)->widget())->setChecked(true);
        }

        for(int j = 0; j < arrRwys.size(); j++)
        {
            if(arrRwys.at(j) == model->index(row, 3).data().toString()) static_cast<QCheckBox*>(uiInner->tableViewRunways->indexWidget(model->index(row, 5))->layout()->itemAt(0)->widget())->setChecked(true);
        }
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

    centeredCheckBoxWidget->setLayout(checkBoxLayout);
    uiInner->tableViewRunways->setIndexWidget(index, centeredCheckBoxWidget);
}

QModelIndex DialogActiveRunways::getCheckBoxIndex(QCheckBox *checkBox)
{
    QModelIndex checkBoxIndex;

    for(int i = 0; i < model->rowCount(); i++)
    {
        if(uiInner->tableViewRunways->indexWidget(model->index(i, 0))->layout()->itemAt(0)->widget() == checkBox)
        {
            checkBoxIndex = model->index(i, 0);
        }
        else if(uiInner->tableViewRunways->indexWidget(model->index(i, 1))->layout()->itemAt(0)->widget() == checkBox)
        {
            checkBoxIndex = model->index(i, 1);
        }
        else if((uiInner->tableViewRunways->indexWidget(model->index(i, 4)) != nullptr) &&
                (uiInner->tableViewRunways->indexWidget(model->index(i, 5)) != nullptr))
        {
            if(uiInner->tableViewRunways->indexWidget(model->index(i, 4))->layout()->itemAt(0)->widget() == checkBox)
            {
                checkBoxIndex = model->index(i, 4);
            }
            else if(uiInner->tableViewRunways->indexWidget(model->index(i, 5))->layout()->itemAt(0)->widget() == checkBox)
            {
                checkBoxIndex = model->index(i, 5);
            }
        }
    }

    return checkBoxIndex;
}
