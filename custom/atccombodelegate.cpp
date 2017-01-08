
#include "atccombodelegate.h"


ATCComboDelegate::ATCComboDelegate(ATCAirspace *airspace, ATCActiveRunways *runways, QObject *parent) :
    airspace(airspace),
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
            cb->addItem("1");
            cb->addItem("2");
            cb->addItem("3");
            break;

        case 6:     //SID
//            cb = createComboSID();
            break;

        case 8:     //ARR RWY
//            cb = createComboArrRwy();
            break;

        case 9:     //STAR
//            cb = createComboSTAR();
            break;

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
        // get the index of the text in the combobox that matches the current value of the item
        QString currentText = index.data(Qt::EditRole).toString();
        int cbIndex = cb->findText(currentText);
        // if it is valid, adjust the combobox
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
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentText(), Qt::EditRole);
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
