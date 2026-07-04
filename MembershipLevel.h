#ifndef MEMBERSHIPLEVEL_H
#define MEMBERSHIPLEVEL_H

#include <string>
using namespace std;

class MembershipLevel{
public:

    virtual ~MembershipLevel(){}

    virtual string getLevelName() const = 0;
    virtual double getPointZarib() const = 0;
    virtual double getDiscount() const = 0;
    virtual int getErsalCost() const = 0;
    virtual int getNextLevel() const = 0;
};

#endif