#include "UserDAO.h"
#include "SilverLevel.h"
#include "GoldLevel.h"
#include "VipLevel.h"
#include "Customer.h"

#include <iostream>

using namespace std;
 
static string getText(sqlite3_stmt* stmt, int col) {
    const char* txt = (const char*)sqlite3_column_text(stmt, col);
    return txt ? txt : "";
}
 
static string roleToString(UserRole role) {
    if (role == UserRole::CUSTOMER)           return "CUSTOMER";
    if (role == UserRole::RESTAURANT_MANAGER) return "RESTAURANT_MANAGER";
    return "SYSTEM_ADMIN";
}
 
static UserRole stringToRole(const string& role) {
    if (role == "CUSTOMER")           return UserRole::CUSTOMER;
    if (role == "RESTAURANT_MANAGER") return UserRole::RESTAURANT_MANAGER;
    return UserRole::SYSTEM_ADMIN;
}
 
UserDAO::UserDAO(sqlite3* db) : db(db) {}
 
User* UserDAO::createUserObject(int id, const string& username, const string& password,
                                 const string& name, const string& role, int restaurantId,
                                int points , const string& level) {
    if (role == "CUSTOMER"){
        Customer* customer = new Customer(id, username, password, name);
        customer->addPoints(points);
        if(level == "Silver"){
            customer->setLevel(new SilverLevel());
        } else if(level == "Gold"){
            customer->setLevel(new GoldLevel());
        } else if(level == "VIP"){
            customer->setLevel(new VipLevel());
        }
        return customer;
    }
    if (role == "RESTAURANT_MANAGER")
        return new RestaurantManager(id, username, password, name, restaurantId);
 
    if (role == "SYSTEM_ADMIN")
        return new SystemAdmin(id, username, password, name);
 
    return nullptr;
}
 
 
void UserDAO::insertUser(const User& user) {
    string sql =
    "INSERT INTO users (id, name, role, username, password, restaurant_id, points, level ) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
 
    sqlite3_stmt* stmt = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "insertUser prepare failed: " << sqlite3_errmsg(db) << "\n";
        return;
    }
 
    int restaurantId = 0;
    if (user.getRole() == UserRole::RESTAURANT_MANAGER) {
        const RestaurantManager& mgr = dynamic_cast<const RestaurantManager&>(user);
        restaurantId = mgr.getRestaurantId();
    }
 
    string role = roleToString(user.getRole());
 
    sqlite3_bind_int (stmt, 1, user.getId());
    sqlite3_bind_text(stmt, 2, user.getName().c_str(),     -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, role.c_str(),               -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int (stmt, 6, restaurantId);

    int points = 0;
    string level = "Normal";
    if(user.getRole() == UserRole::CUSTOMER){
        const Customer& customer = dynamic_cast<const Customer&>(user);
        points = customer.getPoints();
        level = customer.getLevel()->getLevelName();
    }
    sqlite3_bind_int(stmt , 7 ,points);
    sqlite3_bind_text(stmt, 8, level.c_str(), -1, SQLITE_TRANSIENT);
 
    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "insertUser step failed: " << sqlite3_errmsg(db) << "\n";
 
    sqlite3_finalize(stmt);
}
 
User* UserDAO::getUserById(int id) {
    string sql =
    "SELECT id, name, role, username, password, restaurant_id, points, level "
    "FROM users WHERE id = ?;";
 
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getUserById prepare failed: " << sqlite3_errmsg(db) << "\n";
        return nullptr;
    }
 
    sqlite3_bind_int(stmt, 1, id);
 
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = createUserObject(
            sqlite3_column_int(stmt, 0),
            getText(stmt, 3),
            getText(stmt, 4),
            getText(stmt, 1),
            getText(stmt, 2),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            getText(stmt, 7)
        );
    }
 
    sqlite3_finalize(stmt);
    return user;
}
 
 
User* UserDAO::findByUsername(const string& username) {
    string sql =
        "SELECT id, name, role, username, password, restaurant_id , points , level "
        "FROM users WHERE username = ?;";
 
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "findByUsername prepare failed: " << sqlite3_errmsg(db) << "\n";
        return nullptr;
    }
 
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
 
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = createUserObject(
            sqlite3_column_int(stmt, 0),
            getText(stmt, 3),
            getText(stmt, 4),
            getText(stmt, 1),
            getText(stmt, 2),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            getText(stmt, 7)
        );
    }
 
    sqlite3_finalize(stmt);
    return user;
}

 
vector<User*> UserDAO::getAllUsers() {
    vector<User*> users;
 
    string sql =
        "SELECT id, name, role, username, password, restaurant_id, points, level "
        "FROM users;";
 
    sqlite3_stmt* stmt = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getAllUsers prepare failed: " << sqlite3_errmsg(db) << "\n";
        return users;
    }
 
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        User* u = createUserObject(
            sqlite3_column_int(stmt, 0),
            getText(stmt, 3),
            getText(stmt, 4),
            getText(stmt, 1),
            getText(stmt, 2),
            sqlite3_column_int(stmt, 5),
            sqlite3_column_int(stmt, 6),
            getText(stmt, 7)
        );

        if (u) users.push_back(u);
    }
 
    sqlite3_finalize(stmt);
    return users;
}
 
void UserDAO::updateUser(const User& user) {
    string sql =
         "UPDATE users SET "
        "name = ?, "
        "role = ?, "
        "username = ?, "
        "password = ?, "
        "points = ?, "
        "level = ? "
        "WHERE id = ?;";
 
    sqlite3_stmt* stmt = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateUser prepare failed: " << sqlite3_errmsg(db) << "\n";
        return;
    }
 
    string role = roleToString(user.getRole());

    int points = 0;
    string level = "Normal";

    if (user.getRole() == UserRole::CUSTOMER){
        const Customer& customer = dynamic_cast<const Customer&>(user);
        points = customer.getPoints();
        level = customer.getLevel()->getLevelName();
    }

    sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, role.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, points);
    sqlite3_bind_text(stmt, 6, level.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 7, user.getId());
 
    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "updateUser step failed: " << sqlite3_errmsg(db) << "\n";
 
    sqlite3_finalize(stmt);
}
 
void UserDAO::deleteUser(int id) {
    string sql = "DELETE FROM users WHERE id = ?;";
 
    sqlite3_stmt* stmt = nullptr;
 
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "deleteUser prepare failed: " << sqlite3_errmsg(db) << "\n";
        return;
    }
 
    sqlite3_bind_int(stmt, 1, id);
 
    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "deleteUser step failed: " << sqlite3_errmsg(db) << "\n";
 
    sqlite3_finalize(stmt);
}
=======
#include <iostream>

UserDAO::UserDAO(sqlite3* database) : db(database) {}

void UserDAO::insertUser(const User& user) {
    string sql = "INSERT INTO users (id, role, username, password) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user.getId());
        sqlite3_bind_text(stmt, 2, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Insert user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}

vector<User> UserDAO::getAllUsers() {
    vector<User> users;
    string sql = "SELECT id, role, username, password FROM users;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string username = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            users.push_back(User(id, role, username, password));
        }
    }

    sqlite3_finalize(stmt);
    return users;
}

User* UserDAO::getUserById(int id) {
    string sql = "SELECT id, role, username, password FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string username = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            user = new User(userId, role, username, password);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

User* UserDAO::findByUsername(const string& username) {
    string sql = "SELECT id, role, username, password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string uname = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            user = new User(id, role, uname, password);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

void UserDAO::updateUser(const User& user) {
    string sql = "UPDATE users SET role = ?, username = ?, password = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, user.getId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Update user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}

void UserDAO::deleteUser(int id) {
    string sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Delete user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}
>>>>>>> 57573a606e9fa61e6fe2ac5cefa5dac380918ad0
