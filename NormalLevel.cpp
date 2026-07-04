#include "NormalLevel.h"

using namespace std;

string NormalLevel::getLevelName() const{
    return "Normal";
}

double NormalLevel::getPointZarib() const{
    return 1.0;
}

double NormalLevel::getDiscount() const{
    return 0;
}

int NormalLevel::getErsalCost() const{
    return 30000;
}

int NormalLevel::getNextLevel() const{
    return 100;
}