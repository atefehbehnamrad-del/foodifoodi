#ifndef SYSTEM_ADMIN_H
#define SYSTEM_ADMIN_H

#include "User.h"

class SystemAdmin : public User {
public:
    SystemAdmin(int id, const std::string& username,
                const std::string& password, const std::string& name);

    void showMenu() override;
};

#endif