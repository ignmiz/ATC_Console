#ifndef ATCCOMBODELEGATE_H
#define ATCCOMBODELEGATE_H

#include "atcactiverunways.h"
#include "atcairspace.h"
#include "atcsimulation.h"

#include <QComboBox>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QStringList>
#include <QDebug>

enum class DelegateLocation
{
    DialogFlight, DialogFlightList
};

class ATCComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ATCComboDelegate(ATCAirspace *airspace, ATCSimulation *simulation, ATCActiveRunways *runways, DelegateLocation location, QObject *parent = 0);
    ~ATCComboDelegate();

    void setSimulation(ATCSimulation *sim);

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

private slots:
    void slotCloseDelegate(QString text);

private:
    ATCAirspace *airspace;
    ATCActiveRunways *runways;
    ATCSimulation *simulation;
    DelegateLocation location;
};

#endif // ATCCOMBODELEGATE_H
