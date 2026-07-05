#ifndef VIPLEVEL_H
#define VIPLEVEL_H

#include "MembershipLevel.h"

#include <string>


class VipLevel : public MembershipLevel
{
public:

    std::string getLevelName() const override;
    double getPointZarib() const override;
    double getDiscount() const override;
    int getErsalCost() const override;
    int getNextLevel() const override;
    int getCoupons() const override;
    std::string getStage() const override;
};

#endif