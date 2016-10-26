
#include "atccompanyfactory.h"

ATCCompanyFactory::ATCCompanyFactory(QString path)
{
    QFile file(path);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Failed to open path: " + path);
        msgBox.exec();
    }

    QTextStream stream(&file);

    while(!stream.atEnd())
    {
        QString textLine = stream.readLine().trimmed();

        if(!textLine.isEmpty())
        {
            QStringList stringList = textLine.split('\t', QString::SkipEmptyParts);

            QString code = stringList.at(0).trimmed();
            QString callsign = stringList.at(1).trimmed();
            QString name = stringList.at(2).trimmed();

            ATCCompany *company = new ATCCompany(code, callsign, name);
            companies.append(company);
        }
    }

    file.close();
}

ATCCompanyFactory::~ATCCompanyFactory()
{
    for(int i = 0; i < companies.size(); i++)
    {
        delete companies.at(i);
    }

    companies.clear();
}

ATCCompany* ATCCompanyFactory::getCompany()
{
    if(companies.size() > 1)
    {
        int vectorStart = 0;
        int vectorEnd = companies.size() - 1;

        int iter = vectorStart + qrand() % (vectorEnd - vectorStart);

        return companies.at(iter);
    }
    else if(companies.size() == 1)
    {
        return companies.at(0);
    }
    else
    {
        return nullptr;
    }
}

ATCCompany *ATCCompanyFactory::getCompany(int i)
{
    return companies.at(i);
}

ATCCompany *ATCCompanyFactory::getCompany(QString ICAOcode)
{
    ATCCompany *company = nullptr;

    for(int i = 0; i < companies.size(); i++)
    {
        if(companies.at(i)->getCode() == ICAOcode)
        {
            company = companies.at(i);
        }
    }

    return company;
}
