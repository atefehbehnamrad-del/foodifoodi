#include "GoldLevel.h"

using namespace std;

string GoldLevel::getLevelName() const{
    return "Gold";
}

double GoldLevel::getPointZarib() const{
    return 1.5 ;
}

double GoldLevel::getDiscount() const{
    return 0.10;
}

int GoldLevel::getErsalCost() const{
    return 15000;
}

int GoldLevel::getNextLevel() const{
    return 700;
}