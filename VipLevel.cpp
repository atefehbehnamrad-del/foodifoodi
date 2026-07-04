#include "VipLevel.h"

using namespace std;

string VipLevel::getLevelName() const{
    return "VIP";
}

double VipLevel::getPointZarib() const{
    return 2.0;
}

double VipLevel::getDiscount() const{
    return 0.15;
}

int VipLevel::getErsalCost() const{
    return 0;
}

int VipLevel::getNextLevel() const{
    return -1;
}

int VipLevel::getCoupons() const{
    return 3;
}

string VipLevel::getStage() const{
    return "V";
}