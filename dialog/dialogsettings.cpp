#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QMessageBox>
#include <QDebug>

DialogSettings::DialogSettings(ATCSituationalDisplay *display, QWidget *parent) :
    situationalDisplay(display),
    ATCDialog(parent, "Settings", 500, 700),
    uiInner(new Ui::DialogSettings)
{
    uiInner->setupUi(this);
    windowSetup();

    createModelSymbology();
    createModelDisplay();

    setupViewSymbology();
    setupViewDisplay();

    connectSlots();

    uiInner->lineEditDefaultSettings->setReadOnly(true);
    uiInner->lineEditActiveSettings->setReadOnly(true);

    uiInner->lineEditDefaultSettings->setText(situationalDisplay->getSettings()->SETTINGS_DFLT_PATH);
    uiInner->lineEditActiveSettings->setText(situationalDisplay->getSettings()->SETTINGS_ACTIVE_PATH);
}

DialogSettings::~DialogSettings()
{
    delete uiInner;
    if(modelSymbology != nullptr) delete modelSymbology;
    if(modelDisplay != nullptr) delete modelDisplay;
}

void DialogSettings::slotColorPickerClosed()
{
    flagDialogColorPickerExists = false;
}

void DialogSettings::slotUpdateTableColorARTCCLow(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(0, 1)->setBackground(brush);
    modelSymbology->item(0, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_LOW_COLOR = color;
}

void DialogSettings::slotUpdateTableColorARTCCHigh(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(1, 1)->setBackground(brush);
    modelSymbology->item(1, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_HIGH_COLOR = color;
}

void DialogSettings::slotUpdateTableColorARTCC(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(2, 1)->setBackground(brush);
    modelSymbology->item(2, 1)->setForeground(brush);

    situationalDisplay->getSettings()->ARTCC_COLOR = color;
}

void DialogSettings::slotUpdateTableColorVOR(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(3, 1)->setBackground(brush);
    modelSymbology->item(3, 1)->setForeground(brush);

    situationalDisplay->getSettings()->VOR_COLOR = color;
}

void DialogSettings::slotUpdateTableColorNDB(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(4, 1)->setBackground(brush);
    modelSymbology->item(4, 1)->setForeground(brush);

    situationalDisplay->getSettings()->NDB_COLOR = color;
}

void DialogSettings::slotUpdateTableColorFix(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(5, 1)->setBackground(brush);
    modelSymbology->item(5, 1)->setForeground(brush);

    situationalDisplay->getSettings()->FIX_COLOR = color;
}

void DialogSettings::slotUpdateTableColorAirport(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(6, 1)->setBackground(brush);
    modelSymbology->item(6, 1)->setForeground(brush);

    situationalDisplay->getSettings()->AIRPORT_COLOR = color;
}

void DialogSettings::slotUpdateTableColorRunway(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(7, 1)->setBackground(brush);
    modelSymbology->item(7, 1)->setForeground(brush);

    situationalDisplay->getSettings()->RUNWAY_COLOR = color;
}

void DialogSettings::slotUpdateTableColorSTAR(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(8, 1)->setBackground(brush);
    modelSymbology->item(8, 1)->setForeground(brush);

    situationalDisplay->getSettings()->STAR_COLOR = color;
}

void DialogSettings::slotUpdateTableColorSID(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(9, 1)->setBackground(brush);
    modelSymbology->item(9, 1)->setForeground(brush);

    situationalDisplay->getSettings()->SID_COLOR = color;
}

void DialogSettings::slotUpdateTableColorAirwayLow(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(10, 1)->setBackground(brush);
    modelSymbology->item(10, 1)->setForeground(brush);

    situationalDisplay->getSettings()->AIRWAY_LOW_COLOR = color;
}

void DialogSettings::slotUpdateTableColorAirwayHigh(QColor color)
{
    QBrush brush(color);
    modelSymbology->item(11, 1)->setBackground(brush);
    modelSymbology->item(11, 1)->setForeground(brush);

    situationalDisplay->getSettings()->AIRWAY_HIGH_COLOR = color;
}

void DialogSettings::onViewSymbologyClicked(const QModelIndex &index)
{
    QVariant value = index.data();

    if((value.toString() == "Sectors ARTCC Low Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_LOW_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCLow(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCCLow(QColor)));
    }
    else if((value.toString() == "Sectors ARTCC High Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_HIGH_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCCHigh(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCCHigh(QColor)));
    }
    else if((value.toString() == "Sectors ARTCC Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->ARTCC_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSectorARTCC(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorARTCC(QColor)));
    }
    else if((value.toString() == "VORs Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->VOR_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorVOR(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorVOR(QColor)));
    }
    else if((value.toString() == "NDBs Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->NDB_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorNDB(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorNDB(QColor)));
    }
    else if((value.toString() == "Fixes Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->FIX_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorFix(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorFix(QColor)));
    }
    else if((value.toString() == "Airports Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->AIRPORT_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorAirport(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorAirport(QColor)));
    }
    else if((value.toString() == "Runways Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->RUNWAY_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorRunway(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorRunway(QColor)));
    }
    else if((value.toString() == "STARs Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->STAR_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSTAR(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorSTAR(QColor)));
    }
    else if((value.toString() == "SIDs Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->SID_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorSID(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorSID(QColor)));
    }
    else if((value.toString() == "Low Airways Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->AIRWAY_LOW_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorAirwayLow(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorAirwayLow(QColor)));
    }
    else if((value.toString() == "High Airways Color") && !flagDialogColorPickerExists)
    {
        constructColorPicker(situationalDisplay->getSettings()->AIRWAY_HIGH_COLOR);

        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), situationalDisplay, SLOT(slotSetColorAirwayHigh(QColor)));
        connect(dialogColorPicker, SIGNAL(colorSelected(QColor)), this, SLOT(slotUpdateTableColorAirwayHigh(QColor)));
    }
}

void DialogSettings::onViewDisplayClicked(const QModelIndex &index)
{
    bool isNotHeader(index.parent().isValid());

    if(isNotHeader)
    {
        QModelIndex checkboxIndex(index.model()->index(index.row(), 1, index.parent()));
        QModelIndex checkboxHeaderIndex(index.model()->index(index.parent().row(), 1, index.parent().parent()));
        QModelIndex nameIndex(index.model()->index(index.row(), 0, index.parent()));
        QModelIndex nameHeaderIndex(index.model()->index(index.parent().row(), 0, index.parent().parent()));

        QStandardItem *checkboxItem(modelDisplay->itemFromIndex(checkboxIndex));
        QStandardItem *checkboxHeaderItem(modelDisplay->itemFromIndex(checkboxHeaderIndex));
        QStandardItem *nameHeaderItem(modelDisplay->itemFromIndex(nameHeaderIndex));

        bool isChecked((checkboxItem->checkState()) == Qt::Checked);

        QString headerName(nameHeaderIndex.data().toString());
        QString childName(nameIndex.data().toString());

        disconnect(modelDisplay, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotHeaderStateChanged(QStandardItem*)));

        if(isChecked)
        {
            checkboxItem->setCheckState(Qt::Unchecked);

            if(headerName == "Sectors: ARTCC Low")
            {
                emit signalHideSectorARTCCLow(childName);
                decrementChildCounter(visibleSectorsARTCCLow, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Sectors: ARTCC High")
            {
                emit signalHideSectorARTCCHigh(childName);
                decrementChildCounter(visibleSectorsARTCCHigh, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Sectors: ARTCC")
            {
                emit signalHideSectorARTCC(childName);
                decrementChildCounter(visibleSectorsARTCC, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Fixes")
            {
                emit signalHideFix(childName);
                decrementChildCounter(visibleFixes, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Beacons: NDB")
            {
                emit signalHideNDB(childName);
                decrementChildCounter(visibleNDBs, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Beacons: VOR")
            {
                emit signalHideVOR(childName);
                decrementChildCounter(visibleVORs, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airports")
            {
                emit signalHideAirport(childName);
                decrementChildCounter(visibleAirports, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Procedures: SID")
            {
                emit signalHideSID(childName);
                decrementChildCounter(visibleSIDSymbols, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Procedures: STAR")
            {
                emit signalHideSTAR(childName);
                decrementChildCounter(visibleSTARSymbols, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airways: Low")
            {
                emit signalHideAirwayLow(childName);
                decrementChildCounter(visibleAirwaysLow, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airways: High")
            {
                emit signalHideAirwayHigh(childName);
                decrementChildCounter(visibleAirwaysHigh, nameHeaderItem, checkboxHeaderItem);
            }
        }
        else
        {
            checkboxItem->setCheckState(Qt::Checked);

            if(headerName == "Sectors: ARTCC Low")
            {
                emit signalShowSectorARTCCLow(childName);
                incrementChildCounter(visibleSectorsARTCCLow, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Sectors: ARTCC High")
            {
                emit signalShowSectorARTCCHigh(childName);
                incrementChildCounter(visibleSectorsARTCCHigh, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Sectors: ARTCC")
            {
                emit signalShowSectorARTCC(childName);
                incrementChildCounter(visibleSectorsARTCC, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Fixes")
            {
                emit signalShowFix(childName);
                incrementChildCounter(visibleFixes, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Beacons: NDB")
            {
                emit signalShowNDB(childName);
                incrementChildCounter(visibleNDBs, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Beacons: VOR")
            {
                emit signalShowVOR(childName);
                incrementChildCounter(visibleVORs, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airports")
            {
                emit signalShowAirport(childName);
                incrementChildCounter(visibleAirports, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Procedures: SID")
            {
                emit signalShowSID(childName);
                incrementChildCounter(visibleSIDSymbols, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Procedures: STAR")
            {
                emit signalShowSTAR(childName);
                incrementChildCounter(visibleSTARSymbols, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airways: Low")
            {
                emit signalShowAirwayLow(childName);
                incrementChildCounter(visibleAirwaysLow, nameHeaderItem, checkboxHeaderItem);
            }
            else if(headerName == "Airways: High")
            {
                emit signalShowAirwayHigh(childName);
                incrementChildCounter(visibleAirwaysHigh, nameHeaderItem, checkboxHeaderItem);
            }
        }

        connect(modelDisplay, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotHeaderStateChanged(QStandardItem*)));
    }
}

void DialogSettings::slotHeaderStateChanged(QStandardItem *item)
{
    QModelIndex checkboxIndex(item->index());
    QModelIndex nameIndex(checkboxIndex.model()->index(checkboxIndex.row(), 0, checkboxIndex.parent()));

    QStandardItem *headerItem(modelDisplay->itemFromIndex(nameIndex));

    QString headerName(nameIndex.data().toString());
    bool isChecked(item->checkState() == Qt::Unchecked);

    int childrenCount = headerItem->rowCount();

    if(isChecked)
    {
        for(int i = 0; i < childrenCount; i++)
        {
            QStandardItem *childNameItem(headerItem->child(i));

            QModelIndex childNameIndex(childNameItem->index());
            QModelIndex childCheckboxIndex(childNameIndex.model()->index(childNameIndex.row(), 1, childNameIndex.parent()));

            QStandardItem *childCheckboxItem(modelDisplay->itemFromIndex(childCheckboxIndex));

            QString childName(childNameIndex.data().toString());

            if(headerName == "Sectors: ARTCC Low")
            {
                emit signalHideSectorARTCCLow(childName);
            }
            else if(headerName == "Sectors: ARTCC High")
            {
                emit signalHideSectorARTCCHigh(childName);
            }
            else if(headerName == "Sectors: ARTCC")
            {
                emit signalHideSectorARTCC(childName);
            }
            else if(headerName == "Fixes")
            {
                emit signalHideFix(childName);
            }
            else if(headerName == "Beacons: NDB")
            {
                emit signalHideNDB(childName);
            }
            else if(headerName == "Beacons: VOR")
            {
                emit signalHideVOR(childName);
            }
            else if(headerName == "Airports")
            {
                emit signalHideAirport(childName);
            }
            else if(headerName == "Procedures: SID")
            {
                emit signalHideSID(childName);
            }
            else if(headerName == "Procedures: STAR")
            {
                emit signalHideSTAR(childName);
            }
            else if(headerName == "Airways: Low")
            {
                emit signalHideAirwayLow(childName);
            }
            else if(headerName == "Airways: High")
            {
                emit signalHideAirwayHigh(childName);
            }

            childCheckboxItem->setCheckState(Qt::Unchecked);
        }

        if(headerName == "Sectors: ARTCC Low")
        {
            visibleSectorsARTCCLow = 0;
        }
        else if(headerName == "Sectors: ARTCC High")
        {
            visibleSectorsARTCCHigh = 0;
        }
        else if(headerName == "Sectors: ARTCC")
        {
            visibleSectorsARTCC = 0;
        }
        else if(headerName == "Fixes")
        {
            visibleFixes = 0;
        }
        else if(headerName == "Beacons: NDB")
        {
            visibleNDBs = 0;
        }
        else if(headerName == "Beacons: VOR")
        {
            visibleVORs = 0;
        }
        else if(headerName == "Airports")
        {
            visibleAirports = 0;
        }
        else if(headerName == "Procedures: SID")
        {
            visibleSIDSymbols = 0;
        }
        else if(headerName == "Procedures: STAR")
        {
            visibleSTARSymbols = 0;
        }
        else if(headerName == "Airways: Low")
        {
            visibleAirwaysLow = 0;
        }
        else if(headerName == "Airways: High")
        {
            visibleAirwaysHigh = 0;
        }
    }
    else
    {
        for(int i = 0; i < childrenCount; i++)
        {
            QStandardItem *childNameItem(headerItem->child(i));

            QModelIndex childNameIndex(childNameItem->index());
            QModelIndex childCheckboxIndex(childNameIndex.model()->index(childNameIndex.row(), 1, childNameIndex.parent()));

            QStandardItem *childCheckboxItem(modelDisplay->itemFromIndex(childCheckboxIndex));

            QString childName(childNameIndex.data().toString());

            if(headerName == "Sectors: ARTCC Low")
            {
                emit signalShowSectorARTCCLow(childName);
            }
            else if(headerName == "Sectors: ARTCC High")
            {
                emit signalShowSectorARTCCHigh(childName);
            }
            else if(headerName == "Sectors: ARTCC")
            {
                emit signalShowSectorARTCC(childName);
            }
            else if(headerName == "Fixes")
            {
                emit signalShowFix(childName);
            }
            else if(headerName == "Beacons: NDB")
            {
                emit signalShowNDB(childName);
            }
            else if(headerName == "Beacons: VOR")
            {
                emit signalShowVOR(childName);
            }
            else if(headerName == "Airports")
            {
                emit signalShowAirport(childName);
            }
            else if(headerName == "Procedures: SID")
            {
                emit signalShowSID(childName);
            }
            else if(headerName == "Procedures: STAR")
            {
                emit signalShowSTAR(childName);
            }
            else if(headerName == "Airways: Low")
            {
                emit signalShowAirwayLow(childName);
            }
            else if(headerName == "Airways: High")
            {
                emit signalShowAirwayHigh(childName);
            }

            childCheckboxItem->setCheckState(Qt::Checked);
        }

        if(headerName == "Sectors: ARTCC Low")
        {
            visibleSectorsARTCCLow = childrenCount;
        }
        else if(headerName == "Sectors: ARTCC High")
        {
            visibleSectorsARTCCHigh = childrenCount;
        }
        else if(headerName == "Sectors: ARTCC")
        {
            visibleSectorsARTCC = childrenCount;
        }
        else if(headerName == "Fixes")
        {
            visibleFixes = childrenCount;
        }
        else if(headerName == "Beacons: NDB")
        {
            visibleNDBs = childrenCount;
        }
        else if(headerName == "Beacons: VOR")
        {
            visibleVORs = childrenCount;
        }
        else if(headerName == "Airports")
        {
            visibleAirports = childrenCount;
        }
        else if(headerName == "Procedures: SID")
        {
            visibleSIDSymbols = childrenCount;
        }
        else if(headerName == "Procedures: STAR")
        {
            visibleSTARSymbols = childrenCount;
        }
        else if(headerName == "Airways: Low")
        {
            visibleAirwaysLow = childrenCount;
        }
        else if(headerName == "Airways: High")
        {
            visibleAirwaysHigh = childrenCount;
        }
    }
}

void DialogSettings::on_buttonExportSymbology_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export to..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->exportSettings(filePath);

    QMessageBox msgBox(this);
    msgBox.setText("Settings successfuly exported to: " + filePath);
    msgBox.exec();
}

void DialogSettings::on_buttonLoadSymbology_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Load settings..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->loadSettings(filePath);

    uiInner->lineEditActiveSettings->setText(situationalDisplay->getSettings()->SETTINGS_ACTIVE_PATH);
}

void DialogSettings::on_buttonSetDefaultSymbology_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Set as default config..."), situationalDisplay->getSettings()->SETTINGS_EXPORT_PATH, tr("Text files(*.txt)"));
    if(filePath.isEmpty()) return;

    situationalDisplay->getSettings()->setDefaultPath(filePath);

    QMessageBox msgBox(this);
    msgBox.setText("Default settings set to: " + filePath);
    msgBox.exec();

    uiInner->lineEditDefaultSettings->setText(situationalDisplay->getSettings()->SETTINGS_DFLT_PATH);
}

void DialogSettings::setupViewSymbology()
{
    uiInner->viewSymbology->setModel(modelSymbology);
    uiInner->viewSymbology->setGridStyle(Qt::NoPen);

    int rowHeight = 25;
    for(int i = 0; i < modelSymbology->rowCount(); i++)
    {
        uiInner->viewSymbology->setRowHeight(i, rowHeight);
    }

    uiInner->viewSymbology->setColumnWidth(0, 300);
    uiInner->viewSymbology->setColumnWidth(1, 176);

    uiInner->viewSymbology->horizontalHeader()->setHidden(true);
    uiInner->viewSymbology->verticalHeader()->setHidden(true);
}

void DialogSettings::setupViewDisplay()
{
    uiInner->viewDisplay->setModel(modelDisplay);
    uiInner->viewDisplay->setHeaderHidden(true);
    uiInner->viewDisplay->sortByColumn(0, Qt::AscendingOrder);

    uiInner->viewDisplay->setColumnWidth(0, 375);
    uiInner->viewDisplay->setColumnWidth(1, 25);
}

void DialogSettings::createModelSymbology()
{
    modelSymbology = new QStandardItemModel(0, 2, this);

    modelSymbology->appendRow(createSymbologyRow("Sectors ARTCC Low", situationalDisplay->getSettings()->ARTCC_LOW_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Sectors ARTCC High", situationalDisplay->getSettings()->ARTCC_HIGH_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Sectors ARTCC", situationalDisplay->getSettings()->ARTCC_COLOR));
    modelSymbology->appendRow(createSymbologyRow("VORs", situationalDisplay->getSettings()->VOR_COLOR));
    modelSymbology->appendRow(createSymbologyRow("NDBs", situationalDisplay->getSettings()->NDB_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Fixes", situationalDisplay->getSettings()->FIX_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Airports", situationalDisplay->getSettings()->AIRPORT_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Runways", situationalDisplay->getSettings()->RUNWAY_COLOR));
    modelSymbology->appendRow(createSymbologyRow("STARs", situationalDisplay->getSettings()->STAR_COLOR));
    modelSymbology->appendRow(createSymbologyRow("SIDs", situationalDisplay->getSettings()->SID_COLOR));
    modelSymbology->appendRow(createSymbologyRow("Low Airways", situationalDisplay->getSettings()->AIRWAY_LOW_COLOR));
    modelSymbology->appendRow(createSymbologyRow("High Airways", situationalDisplay->getSettings()->AIRWAY_HIGH_COLOR));
}

void DialogSettings::createModelDisplay()
{
    ATCAirspace* airspaceData = situationalDisplay->getAirspaceData();

    modelDisplay = new QStandardItemModel(0, 2, this);

    populateModelDisplay("Sectors: ARTCC Low", airspaceData->getSectorARTCCLowVector(), visibleSectorsARTCCLow, modelDisplay);
    populateModelDisplay("Sectors: ARTCC High", airspaceData->getSectorARTCCHighVector(), visibleSectorsARTCCHigh, modelDisplay);
    populateModelDisplay("Sectors: ARTCC", airspaceData->getSectorARTCCVector(), visibleSectorsARTCC, modelDisplay);
    populateModelDisplay("Fixes", airspaceData->getFixesVector(), visibleFixes, modelDisplay);
    populateModelDisplay("Beacons: NDB", airspaceData->getNDBsVector(), visibleNDBs, modelDisplay);
    populateModelDisplay("Beacons: VOR", airspaceData->getVORsVector(), visibleVORs, modelDisplay);
    populateModelDisplay("Airports", airspaceData->getAirportsVector(), visibleAirports, modelDisplay);
    populateModelDisplay("Procedures: SID", airspaceData->getSIDSymbolsVector(), visibleSIDSymbols, modelDisplay);
    populateModelDisplay("Procedures: STAR", airspaceData->getSTARSymbolsVector(), visibleSTARSymbols, modelDisplay);
    populateModelDisplay("Airways: Low", airspaceData->getAirwayLowVector(), visibleAirwaysLow, modelDisplay);
    populateModelDisplay("Airways: High", airspaceData->getAirwayHighVector(), visibleAirwaysHigh, modelDisplay);
}

void DialogSettings::connectSlots()
{
    connect(uiInner->viewSymbology, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onViewSymbologyClicked(const QModelIndex &)));
    connect(uiInner->viewDisplay, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onViewDisplayClicked(const QModelIndex &)));

    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCLow(QColor)), this, SLOT(slotUpdateTableColorARTCCLow(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCCHigh(QColor)), this, SLOT(slotUpdateTableColorARTCCHigh(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorARTCC(QColor)), this, SLOT(slotUpdateTableColorARTCC(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorVOR(QColor)), this, SLOT(slotUpdateTableColorVOR(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorNDB(QColor)), this, SLOT(slotUpdateTableColorNDB(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorFix(QColor)), this, SLOT(slotUpdateTableColorFix(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorAirport(QColor)), this, SLOT(slotUpdateTableColorAirport(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorRunway(QColor)), this, SLOT(slotUpdateTableColorRunway(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorSTAR(QColor)), this, SLOT(slotUpdateTableColorSTAR(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorSID(QColor)), this, SLOT(slotUpdateTableColorSID(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorAirwayLow(QColor)), this, SLOT(slotUpdateTableColorAirwayLow(QColor)));
    connect(situationalDisplay->getSettings(), SIGNAL(signalColorAirwayHigh(QColor)), this, SLOT(slotUpdateTableColorAirwayHigh(QColor)));

    connect(situationalDisplay->getSettings(), SIGNAL(signalApplySettings()), situationalDisplay, SLOT(slotApplySettings()));

    connect(this, SIGNAL(signalHideSectorARTCCLow(QString)), situationalDisplay, SLOT(slotHideSectorARTCCLow(QString)));
    connect(this, SIGNAL(signalHideSectorARTCCHigh(QString)), situationalDisplay, SLOT(slotHideSectorARTCCHigh(QString)));
    connect(this, SIGNAL(signalHideSectorARTCC(QString)), situationalDisplay, SLOT(slotHideSectorARTCC(QString)));
    connect(this, SIGNAL(signalHideFix(QString)), situationalDisplay, SLOT(slotHideFix(QString)));
    connect(this, SIGNAL(signalHideNDB(QString)), situationalDisplay, SLOT(slotHideNDB(QString)));
    connect(this, SIGNAL(signalHideVOR(QString)), situationalDisplay, SLOT(slotHideVOR(QString)));
    connect(this, SIGNAL(signalHideAirport(QString)), situationalDisplay, SLOT(slotHideAirport(QString)));
    connect(this, SIGNAL(signalHideSID(QString)), situationalDisplay, SLOT(slotHideSID(QString)));
    connect(this, SIGNAL(signalHideSTAR(QString)), situationalDisplay, SLOT(slotHideSTAR(QString)));
    connect(this, SIGNAL(signalHideAirwayLow(QString)), situationalDisplay, SLOT(slotHideAirwayLow(QString)));
    connect(this, SIGNAL(signalHideAirwayHigh(QString)), situationalDisplay, SLOT(slotHideSTAR(QString)));

    connect(this, SIGNAL(signalShowSectorARTCCLow(QString)), situationalDisplay, SLOT(slotShowSectorARTCCLow(QString)));
    connect(this, SIGNAL(signalShowSectorARTCCHigh(QString)), situationalDisplay, SLOT(slotShowSectorARTCCHigh(QString)));
    connect(this, SIGNAL(signalShowSectorARTCC(QString)), situationalDisplay, SLOT(slotShowSectorARTCC(QString)));
    connect(this, SIGNAL(signalShowFix(QString)), situationalDisplay, SLOT(slotShowFix(QString)));
    connect(this, SIGNAL(signalShowNDB(QString)), situationalDisplay, SLOT(slotShowNDB(QString)));
    connect(this, SIGNAL(signalShowVOR(QString)), situationalDisplay, SLOT(slotShowVOR(QString)));
    connect(this, SIGNAL(signalShowAirport(QString)), situationalDisplay, SLOT(slotShowAirport(QString)));
    connect(this, SIGNAL(signalShowSID(QString)), situationalDisplay, SLOT(slotShowSID(QString)));
    connect(this, SIGNAL(signalShowSTAR(QString)), situationalDisplay, SLOT(slotShowSTAR(QString)));
    connect(this, SIGNAL(signalShowAirwayLow(QString)), situationalDisplay, SLOT(slotShowAirwayLow(QString)));
    connect(this, SIGNAL(signalShowAirwayHigh(QString)), situationalDisplay, SLOT(slotShowSTAR(QString)));

    connect(modelDisplay, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotHeaderStateChanged(QStandardItem*)));
}

void DialogSettings::constructColorPicker(QColor &initColor)
{
    QColor currentColor(initColor);

    dialogColorPicker = new DialogColorPicker(currentColor, this);
    dialogColorPicker->show();

    connect(dialogColorPicker, SIGNAL(signalColorPickerClosed()), this, SLOT(slotColorPickerClosed()));

    flagDialogColorPickerExists = true;
}

QList<QStandardItem *> DialogSettings::createSymbologyRow(QString text, QColor color)
{
    QList<QStandardItem*> items;

    QStandardItem *name = new QStandardItem(text);
    name->setFlags(Qt::NoItemFlags);
    items.append(name);

    QStandardItem *colorField = new QStandardItem(text + " Color");
    colorField->setFlags(Qt::NoItemFlags);
    items.append(colorField);

    QBrush brush(color);
    colorField->setBackground(brush);
    colorField->setForeground(brush);

    return items;
}

QList<QStandardItem*> DialogSettings::createDisplayHeader(QString text)
{
    QList<QStandardItem*> rowHeader;

    QStandardItem *headerName = new QStandardItem(text);
    headerName->setFlags(Qt::NoItemFlags);
    rowHeader.append(headerName);

    QStandardItem *headerCheckbox = new QStandardItem();
    headerCheckbox->setEditable(false);
    headerCheckbox->setSelectable(false);
    headerCheckbox->setCheckable(true);
    headerCheckbox->setUserTristate(false);
    headerCheckbox->setTextAlignment(Qt::AlignCenter);
    rowHeader.append(headerCheckbox);

    return rowHeader;
}

QList<QStandardItem *> DialogSettings::createDisplayRow(QString text, bool checked, int &counter)
{
    QList<QStandardItem*> rowDisplay;

    QStandardItem *name = new QStandardItem(text);
    name->setFlags(Qt::NoItemFlags);
    rowDisplay.append(name);

    QStandardItem *checkbox = new QStandardItem();
    checkbox->setEditable(false);
    checkbox->setSelectable(false);
    checkbox->setCheckable(false);

    if(checked)
    {
        checkbox->setCheckState(Qt::Checked);
        counter++;
    }
    else
    {
        checkbox->setCheckState(Qt::Unchecked);
    }

    rowDisplay.append(checkbox);

    return rowDisplay;
}

void DialogSettings::decrementChildCounter(int &counter, QStandardItem *headerName ,QStandardItem *headerCheckbox)
{
    counter--;

    if(counter == 0)
    {
        headerCheckbox->setCheckState(Qt::Unchecked);
    }
    else if(counter < headerName->rowCount())
    {
        headerCheckbox->setCheckState(Qt::PartiallyChecked);
    }
}

void DialogSettings::incrementChildCounter(int &counter, QStandardItem *headerName, QStandardItem *headerCheckbox)
{
    counter++;

    if(counter == headerName->rowCount())
    {
        headerCheckbox->setCheckState(Qt::Checked);
    }
    else if(counter < headerName->rowCount())
    {
        headerCheckbox->setCheckState(Qt::PartiallyChecked);
    }
}

template<class T> void DialogSettings::populateModelDisplay(QString headerName, QVector<T*> const &vector, int &counter, QStandardItemModel *model)
{
    QList<QStandardItem*> categoryHeader(createDisplayHeader(headerName));

    for(int i = 0; i < vector.size(); i++)
    {
        categoryHeader.at(0)->appendRow(createDisplayRow(vector.at(i)->getName(), vector.at(i)->isVisible(), counter));
    }

    if((vector.size() == counter) && (counter != 0))
    {
        categoryHeader.at(1)->setCheckState(Qt::Checked);
    }
    else if(counter == 0)
    {
        categoryHeader.at(1)->setCheckState(Qt::Unchecked);
    }
    else
    {
        categoryHeader.at(1)->setCheckState(Qt::PartiallyChecked);
    }

    model->appendRow(categoryHeader);
}
