#include "atcsettings.h"

ATCSettings::ATCSettings()
{
    ARTCC_LOW_COLOR = qRgb(90, 230, 230);
    ARTCC_HIGH_COLOR =  qRgb(255, 255, 255);
    ARTCC_COLOR = qRgb(255, 0, 0);
}

ATCSettings::~ATCSettings()
{

}
