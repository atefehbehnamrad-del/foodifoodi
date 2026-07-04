#ifndef USERDAO_H
#define USERDAO_H

#include "User.h"
#include "Customer.h"
#include "RestaurantManager.h"
#include "SystemAdmin.h"

#include <sqlite3.h>
#include <vector>
#include <string>

class UserDAO {
public:
    UserDAO(sqlite3* db);

    void insertUser(const User& user);
    void updateUser(const User& user);
    void deleteUser(int id);

    User* getUserById(int id);
    User* findByUsername(const std::string& username);

    std::vector<User*> getAllUsers();

private:
    sqlite3* db;

    User* createUserObject(
        int id,
        const std::string& username,
        const std::string& password,
        const std::string& name,
        const std::string& role,
        int restaurantId
    );
};

#endif