
#include "atccombodelegate.h"


ATCComboDelegate::ATCComboDelegate(ATCAirspace *airspace, ATCSimulation *simulation, ATCActiveRunways *runways, QObject *parent) :
    airspace(airspace),
    simulation(simulation),
    runways(runways),
    QStyledItemDelegate(parent)
{
}

ATCComboDelegate::~ATCComboDelegate()
{
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

        ATCFlight *flight = simulation->getFlight(model->index(index.row(), 1).data().toString());

        if(index.column() == 5)
        {
            QString rwy = index.data().toString();
            QString adep = flight->getFlightPlan()->getRoute().getDeparture();
            QStringList route = flight->getFlightPlan()->getRoute().getRoute();

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
        }
        else if(index.column() == 6)
        {
            flight->setSID(model->index(index.row(), 6).data().toString());
        }
        else if(index.column() == 8)
        {
            QString rwy = index.data().toString();
            QString ades = flight->getFlightPlan()->getRoute().getDestination();
            QStringList route = flight->getFlightPlan()->getRoute().getRoute();

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
        }
        else if(index.column() == 9)
        {
            flight->setSTAR(model->index(index.row(), 9).data().toString());
        }
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ATCComboDelegate::slotCloseDelegate(QString text)
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());

    emit commitData(cb);
    emit closeEditor(cb);
}
