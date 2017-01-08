#ifndef ATCPROCEDURECOMBODELEGATE_H
#define ATCPROCEDURECOMBODELEGATE_H

#include "atcactiverunways.h"
#include "atcairspace.h"

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QDebug>

class ATCComboDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ATCComboDelegate(ATCAirspace *airspace, ATCActiveRunways *runways, QObject *parent = 0);
    ~ATCComboDelegate();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

private slots:
    void slotCloseDelegate(QString text);

private:
    ATCAirspace *airspace;
    ATCActiveRunways *runways;
};

#endif // ATCPROCEDURECOMBODELEGATE_H
