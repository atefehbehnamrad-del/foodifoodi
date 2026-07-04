#include "SilverLevel.h"

using namespace std;

string SilverLevel::getLevelName() const{
    return "Silver";
}

double SilverLevel::getPointZarib() const{
    return 1.2;
}

double SilverLevel::getDiscount() const{
    return 0.05;
}

int SilverLevel::getErsalCost() const{
    return 15000;
}

int SilverLevel::getNextLevel() const{
    return 300;
}