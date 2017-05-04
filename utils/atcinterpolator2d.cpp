
#include "atcinterpolator2d.h"

ATCInterpolator2D::ATCInterpolator2D()
{

}

ATCInterpolator2D::~ATCInterpolator2D()
{
    QMap<double, ATCInterpolator*>::iterator i;
    for(i = interpolators.begin(); i != interpolators.end(); i++)
    {
        if(i.value() != nullptr) delete i.value();
    }
}

void ATCInterpolator2D::interpolate(double key, QVector<double> &sites, QVector<double> &results)
{
    //Find crossing point
    QList<double>::iterator Xpoint = ATCMath::findCrossingPoint(keys.begin(), std::prev(keys.end()), key);

    //Find appropriate interpolators
    if(*Xpoint == key) //One 1D interpolator needed
    {
        interpolators.find(key).value()->interpolate(sites, results);
    }
    else //Two 1D interpolators needed
    {
        QMap<double, ATCInterpolator*>::iterator first = interpolators.find(*Xpoint);
        QMap<double, ATCInterpolator*>::iterator second = std::next(first);

        if(second == interpolators.end()) //Move iterators back by one position
        {
            second = first;
            first = std::prev(second);
        }

        //Temporary containers for first interpolation
        QVector<double> resultsFirst(sites.size());
        QVector<double> resultsSecond(sites.size());

        //First interpolation (linear) for two closest keys
        first.value()->interpolate(sites, resultsFirst);
        second.value()->interpolate(sites, resultsSecond);

        //Second interpolation (linear) for the actual key
        for(int i = 0; i < results.size(); i++)
        {
            results.replace(i, resultsFirst.at(i) + (key - first.key()) * (resultsSecond.at(i) - resultsFirst.at(i)) / (second.key() - first.key()));
        }
    }
}

ATCInterpolator *ATCInterpolator2D::getInterpolator(double key)
{
    QMap<double, ATCInterpolator*>::iterator i = interpolators.find(key);

    if(i != interpolators.end()) return i.value();
    else return nullptr;
}

void ATCInterpolator2D::insertInterpolator(double key, ATCInterpolator *value)
{
    interpolators.insert(key, value);
    keys = interpolators.keys();
}

void ATCInterpolator2D::removeInterpolator(double key)
{
    QMap<double, ATCInterpolator*>::iterator i = interpolators.find(key);

    if(i != interpolators.end())
    {
        if(i.value() != nullptr) delete i.value();
        interpolators.remove(key);
        keys = interpolators.keys();
    }
}
