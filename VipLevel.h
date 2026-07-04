#ifndef VIPLEVEL_H
#define VIPLEVEL_H

#include "MembershipLevel.h"

class VipLevel : public MembershipLevel
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