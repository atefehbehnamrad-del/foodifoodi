#ifndef NORMALLEVEL_H
#define NORMALLEVEL_H

#include "MembershipLevel.h"

class NormalLevel : public MembershipLevel
{
public:

    string getLevelName() const override;
    double getPointZarib() const override;
    double getDiscount() const override;
    int getErsalCost() const override;
    int getNextLevel() const override;
};

#endif