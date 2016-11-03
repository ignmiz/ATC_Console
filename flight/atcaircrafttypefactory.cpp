
#include "atcaircrafttypefactory.h"

ATCAircraftTypeFactory::ATCAircraftTypeFactory(QString DBdir)
{
    QDir dir(DBdir);
    dir.setFilter(QDir::Files);

    QStringList nameFilter;
    nameFilter << "*.OPF";
    dir.setNameFilters(nameFilter);

    QFileInfoList fileList = dir.entryInfoList();

    for(int i = 0; i < fileList.size(); i++)
    {
        QString OPFpath = fileList.at(i).absoluteFilePath();
        QString APFpath = OPFpath.left(OPFpath.size() - 4) + ".APF";

        QFileInfo file(APFpath);

        if(file.exists())
        {
            ATCAircraftType *type = new ATCAircraftType(OPFpath, APFpath);
            types.append(type);
        }
    }

}

ATCAircraftTypeFactory::~ATCAircraftTypeFactory()
{
    for(int i = 0; i < types.size(); i++)
    {
        delete types.at(i);
    }

    types.clear();
}

ATCAircraftType* ATCAircraftTypeFactory::getType()
{
    if(types.size() > 1)
    {
        int vectorStart = 0;
        int vectorEnd = types.size() - 1;

        qsrand(QDateTime::currentMSecsSinceEpoch());
        int iter = vectorStart + qrand() % (vectorEnd - vectorStart);

        return types.at(iter);
    }
    else if(types.size() == 1)
    {
        return types.at(0);
    }
    else
    {
        return nullptr;
    }
}

ATCAircraftType* ATCAircraftTypeFactory::getType(int i)
{
    return types.at(i);
}

ATCAircraftType* ATCAircraftTypeFactory::getType(QString ICAOcode)
{
    ATCAircraftType *type = nullptr;

    for(int i = 0; i < types.size(); i++)
    {
        if(types.at(i)->getAcType().ICAOcode == ICAOcode)
        {
            type = types.at(i);
        }
    }

    return type;
}

