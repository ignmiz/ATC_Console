#ifndef MOCK_ATCABSTRACTPROFILE_H
#define MOCK_ATCABSTRACTPROFILE_H

#include "atcabstractprofile.h"

class Mock_ATCAbstractProfile : public ATCAbstractProfile
{
public:
    explicit Mock_ATCAbstractProfile(QVector<double> &levels, QVector<double> &time, QVector<double> &distance, ExType type = ExType::Tangent);
    ~Mock_ATCAbstractProfile();
};

#endif // MOCK_ATCABSTRACTPROFILE_H
