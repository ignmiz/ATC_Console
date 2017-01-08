#ifndef ATCSETTINGSTABLEVIEW_H
#define ATCSETTINGSTABLEVIEW_H

#include <QTableView>

class ATCTableViewSettings : public QTableView
{
public:
    explicit ATCTableViewSettings(QWidget *parent = 0);
    ~ATCTableViewSettings();
};

#endif // ATCSETTINGSTABLEVIEW_H
