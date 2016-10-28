
#include "atcroutefactory.h"

ATCRouteFactory::ATCRouteFactory(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open path: " + path);
        msgBox.exec();

        return;
    }

    QTextStream stream(&file);

    while(!stream.atEnd())
    {
        QString textLine = stream.readLine().trimmed();

        if(!textLine.isEmpty())
        {
            QStringList stringList = textLine.split('\t', QString::SkipEmptyParts);

            QString departure = stringList.at(0).trimmed();
            QString destination = stringList.at(stringList.size() - 1).trimmed();

            QStringList routeList;

            for(int i = 1; i < stringList.size() - 1; i++)
            {
                routeList << stringList.at(i);
            }

            ATCRoute *route = new ATCRoute(departure, routeList, destination);
            routes.append(route);
        }
    }

    file.close();
}

ATCRouteFactory::~ATCRouteFactory()
{
    for(int i = 0; i < routes.size(); i++)
    {
        delete routes.at(i);
    }

    routes.clear();
}

ATCRoute *ATCRouteFactory::getRoute()
{
    if(routes.size() > 1)
    {
        int vectorStart = 0;
        int vectorEnd = routes.size() - 1;

        int iter = vectorStart + qrand() % (vectorEnd - vectorStart);

        return routes.at(iter);
    }
    else if(routes.size() == 1)
    {
        return routes.at(0);
    }
    else
    {
        return nullptr;
    }
}

ATCRoute *ATCRouteFactory::getRoute(int i)
{
    return routes.at(i);
}

ATCRoute *ATCRouteFactory::getRoute(QString ades, QString adep)
{
    for(int i = 0; i < routes.size(); i++)
    {
        if((routes.at(i)->getDeparture() == ades) && (routes.at(i)->getDestination() == adep))
        {
            return routes.at(i);
        }
    }

    return nullptr;
}

