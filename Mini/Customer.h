#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <string>

class Customer : public User {
public:
    Customer(int id, const std::string& username,
             const std::string& password, const std::string& name);

    void showMenu() override;
};

#endif