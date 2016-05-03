#ifndef SECTORSETUP_H
#define SECTORSETUP_H

#include "atcdialog.h"

namespace Ui {
class SectorSetup;
}

class SectorSetup : public ATCDialog
{
    Q_OBJECT

public:
    explicit SectorSetup(QWidget *parent = 0);
    ~SectorSetup();

private:
    Ui::SectorSetup *ui;
};

#endif // SECTORSETUP_H
