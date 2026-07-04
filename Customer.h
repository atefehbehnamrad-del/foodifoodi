#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "MembershipLevel.h"
#include "NormalLevel.h"
#include <string>

class Customer : public User {
private:
    MembershipLevel* level;
    int points;

public:
    Customer(int id, const std::string& username, const std::string& password,
             const std::string& name);

    ~Customer();

    int getPoints() const;
    void addPoints(int point);
    void removePoints(int point);

    MembershipLevel* getLevel() const;
    void setLevel(MembershipLevel* newLevel);
    void showMenu() override;
};

#endif