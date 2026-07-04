#ifndef SILVERLEVEL_H
#define SILVERLEVEL_H

#include "MembershipLevel.h"

class SilverLevel : public MembershipLevel
{
public:

    string getLevelName() const override;
    double getPointZarib() const override;
    double getDiscount() const override;
    int getErsalCost() const override;
    int getNextLevel() const override;
    int getCoupons() const override;
    string getStage() const override;
};

#endif