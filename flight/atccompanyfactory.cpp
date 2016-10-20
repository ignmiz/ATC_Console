
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

            codes.append(code);
            callsigns.append(callsign);
            names.append(name);
        }
    }

    file.close();
}

ATCCompanyFactory::~ATCCompanyFactory()
{

}

ATCCompany* ATCCompanyFactory::newCompany()
{
    int vectorStart = 0;
    int vectorEnd = codes.size() - 1;

    int rowNumber = vectorStart + qrand() % (vectorEnd - vectorStart);

    return new ATCCompany(codes.at(rowNumber), callsigns.at(rowNumber), names.at(rowNumber));
}

QString ATCCompanyFactory::getCode(int i)
{
    return codes.at(i);
}

QString ATCCompanyFactory::getCallsign(int i)
{
    return callsigns.at(i);
}

QString ATCCompanyFactory::getName(int i)
{
    return names.at(i);
}
