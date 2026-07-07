#include "UserDAO.h"
#include "NormalLevel.h"
#include "SilverLevel.h"
#include "GoldLevel.h"
#include "VipLevel.h"
#include <iostream>
#include <sstream>

using namespace std;

static string getText(sqlite3_stmt* stmt, int col) {
    const char* txt = (const char*)sqlite3_column_text(stmt, col);
    return txt ? txt : "";
}

static string roleToString(UserRole role) {
    if (role == UserRole::CUSTOMER) return "CUSTOMER";
    if (role == UserRole::RESTAURANT_MANAGER) return "RESTAURANT_MANAGER";
    return "SYSTEM_ADMIN";
}

static UserRole stringToRole(const string& role) {
    if (role == "CUSTOMER") return UserRole::CUSTOMER;
    if (role == "RESTAURANT_MANAGER") return UserRole::RESTAURANT_MANAGER;
    return UserRole::SYSTEM_ADMIN;
}

static MembershipLevel* createLevelFromString(const string& levelName) {
    if (levelName == "VIP") return new VipLevel();
    if (levelName == "Gold") return new GoldLevel();
    if (levelName == "Silver") return new SilverLevel();
    return new NormalLevel();
}

static string getLevelString(MembershipLevel* level) {
    if (dynamic_cast<VipLevel*>(level)) return "VIP";
    if (dynamic_cast<GoldLevel*>(level)) return "Gold";
    if (dynamic_cast<SilverLevel*>(level)) return "Silver";
    return "Normal";
}

UserDAO::UserDAO(sqlite3* db) : db(db) {}

UserDAO::~UserDAO() {}

string UserDAO::getCurrentTime() {
    time_t now = time(nullptr);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

User* UserDAO::createUserObject(int id, const string& username, const string& password,
                                 const string& name, const string& role, int restaurantId) {
    if (role == "CUSTOMER")
        return new Customer(id, username, password, name);
    if (role == "RESTAURANT_MANAGER")
        return new RestaurantManager(id, username, password, name, restaurantId);
    if (role == "SYSTEM_ADMIN")
        return new SystemAdmin(id, username, password, name);
    return nullptr;
}

void UserDAO::insertUser(const User& user) {
    string sql = "INSERT INTO users (id, name, role, username, password, restaurant_id) "
                 "VALUES (?, ?, ?, ?, ?, ?);";

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

    sqlite3_bind_int(stmt, 1, user.getId());
    sqlite3_bind_text(stmt, 2, user.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, restaurantId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "insertUser step failed: " << sqlite3_errmsg(db) << "\n";

    sqlite3_finalize(stmt);
}

User* UserDAO::getUserById(int id) {
    string sql = "SELECT id, name, role, username, password, restaurant_id "
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
            sqlite3_column_int(stmt, 5)
        );
    }

    sqlite3_finalize(stmt);
    return user;
}

User* UserDAO::findByUsername(const string& username) {
    string sql = "SELECT id, name, role, username, password, restaurant_id "
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
            sqlite3_column_int(stmt, 5)
        );
    }

    sqlite3_finalize(stmt);
    return user;
}

vector<User*> UserDAO::getAllUsers() {
    vector<User*> users;

    string sql = "SELECT id, name, role, username, password, restaurant_id FROM users;";

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
            sqlite3_column_int(stmt, 5)
        );
        if (u) users.push_back(u);
    }

    sqlite3_finalize(stmt);
    return users;
}

void UserDAO::updateUser(const User& user) {
    string sql = "UPDATE users SET name = ?, role = ?, username = ?, password = ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateUser prepare failed: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    string role = roleToString(user.getRole());

    sqlite3_bind_text(stmt, 1, user.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, role.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, user.getId());

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

Customer* UserDAO::getCustomerById(int id) {
    string sql = "SELECT id, name, username, password, points, level, "
                 "last_order_date, consecutive_orders, registration_date "
                 "FROM users WHERE id = ? AND role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;
    Customer* customer = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getCustomerById prepare failed: " << sqlite3_errmsg(db) << "\n";
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int uid = sqlite3_column_int(stmt, 0);
        string name = getText(stmt, 1);
        string username = getText(stmt, 2);
        string password = getText(stmt, 3);
        int points = sqlite3_column_int(stmt, 4);
        string levelName = getText(stmt, 5);
        int consecutiveOrders = sqlite3_column_int(stmt, 7);

        customer = new Customer(uid, username, password, name);
        customer->setPoints(points);
        customer->setLevel(createLevelFromString(levelName));
        customer->setConsecutiveOrders(consecutiveOrders);
    }

    sqlite3_finalize(stmt);
    return customer;
}

Customer* UserDAO::getCustomerByUsername(const string& username) {
    string sql = "SELECT id, name, username, password, points, level, "
                 "last_order_date, consecutive_orders, registration_date "
                 "FROM users WHERE username = ? AND role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;
    Customer* customer = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getCustomerByUsername prepare failed: " << sqlite3_errmsg(db) << "\n";
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int uid = sqlite3_column_int(stmt, 0);
        string name = getText(stmt, 1);
        string password = getText(stmt, 3);
        int points = sqlite3_column_int(stmt, 4);
        string levelName = getText(stmt, 5);
        int consecutiveOrders = sqlite3_column_int(stmt, 7);

        customer = new Customer(uid, username, password, name);
        customer->setPoints(points);
        customer->setLevel(createLevelFromString(levelName));
        customer->setConsecutiveOrders(consecutiveOrders);
    }

    sqlite3_finalize(stmt);
    return customer;
}

bool UserDAO::updateCustomer(Customer* customer) {
    if (customer == nullptr) return false;

    string sql = "UPDATE users SET name = ?, username = ?, password = ?, "
                 "points = ?, level = ?, consecutive_orders = ? "
                 "WHERE id = ? AND role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateCustomer prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, customer->getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, customer->getUsername().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, customer->getPassword().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, customer->getPoints());
    sqlite3_bind_text(stmt, 5, getLevelString(customer->getCurrentLevel()).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, customer->getConsecutiveOrders());
    sqlite3_bind_int(stmt, 7, customer->getId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<Customer*> UserDAO::getAllCustomers() {
    vector<Customer*> customers;

    string sql = "SELECT id, name, username, password, points, level "
                 "FROM users WHERE role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getAllCustomers prepare failed: " << sqlite3_errmsg(db) << "\n";
        return customers;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int uid = sqlite3_column_int(stmt, 0);
        string name = getText(stmt, 1);
        string username = getText(stmt, 2);
        string password = getText(stmt, 3);
        int points = sqlite3_column_int(stmt, 4);
        string levelName = getText(stmt, 5);

        Customer* customer = new Customer(uid, username, password, name);
        customer->setPoints(points);
        customer->setLevel(createLevelFromString(levelName));
        customers.push_back(customer);
    }

    sqlite3_finalize(stmt);
    return customers;
}

bool UserDAO::updatePoints(int customerId, int newPoints) {
    string sql = "UPDATE users SET points = ? WHERE id = ? AND role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updatePoints prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, newPoints);
    sqlite3_bind_int(stmt, 2, customerId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool UserDAO::updateLevel(int customerId, const string& levelName) {
    string sql = "UPDATE users SET level = ? WHERE id = ? AND role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateLevel prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, levelName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, customerId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool UserDAO::saveLevelHistory(int customerId, const string& oldLevel, 
                                const string& newLevel, const string& reason) {
    string sql = "INSERT INTO level_history (customer_id, old_level, new_level, change_date, reason) "
                 "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "saveLevelHistory prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, customerId);
    sqlite3_bind_text(stmt, 2, oldLevel.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, newLevel.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, getCurrentTime().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, reason.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<LevelChangeRecord> UserDAO::getLevelHistory(int customerId) {
    vector<LevelChangeRecord> history;

    string sql = "SELECT old_level, new_level, change_date, reason "
                 "FROM level_history WHERE customer_id = ? "
                 "ORDER BY change_date DESC;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getLevelHistory prepare failed: " << sqlite3_errmsg(db) << "\n";
        return history;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        LevelChangeRecord record;
        record.pastLevel = getText(stmt, 0);
        record.newLevel = getText(stmt, 1);
        record.dateChange = time(nullptr);
        record.reason = getText(stmt, 3);
        history.push_back(record);
    }

    sqlite3_finalize(stmt);
    return history;
}

bool UserDAO::saveCoupon(int customerId, const Coupons& coupon) {
    string sql = "INSERT INTO coupons (customer_id, code, discount, expiry_date, is_used, description) "
                 "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "saveCoupon prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    char expiryBuffer[20];
    strftime(expiryBuffer, sizeof(expiryBuffer), "%Y-%m-%d %H:%M:%S", localtime(&coupon.date));

    sqlite3_bind_int(stmt, 1, customerId);
    sqlite3_bind_text(stmt, 2, coupon.code.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, coupon.discount);
    sqlite3_bind_text(stmt, 4, expiryBuffer, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, coupon.isUsed ? 1 : 0);
    sqlite3_bind_text(stmt, 6, coupon.description.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<Coupons> UserDAO::getCoupons(int customerId) {
    vector<Coupons> coupons;

    string sql = "SELECT code, discount, expiry_date, is_used, description "
                 "FROM coupons WHERE customer_id = ? "
                 "ORDER BY expiry_date DESC;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getCoupons prepare failed: " << sqlite3_errmsg(db) << "\n";
        return coupons;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Coupons coupon;
        coupon.code = getText(stmt, 0);
        coupon.discount = sqlite3_column_int(stmt, 1);
        coupon.date = time(nullptr);
        coupon.isUsed = sqlite3_column_int(stmt, 3) == 1;
        coupon.description = getText(stmt, 4);
        coupons.push_back(coupon);
    }

    sqlite3_finalize(stmt);
    return coupons;
}

bool UserDAO::updateCouponStatus(int customerId, const string& couponCode, bool used) {
    string sql = "UPDATE coupons SET is_used = ? WHERE customer_id = ? AND code = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateCouponStatus prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, used ? 1 : 0);
    sqlite3_bind_int(stmt, 2, customerId);
    sqlite3_bind_text(stmt, 3, couponCode.c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool UserDAO::deleteExpiredCoupons(int customerId) {
    string sql = "DELETE FROM coupons WHERE customer_id = ? AND expiry_date < ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "deleteExpiredCoupons prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, customerId);
    sqlite3_bind_text(stmt, 2, getCurrentTime().c_str(), -1, SQLITE_TRANSIENT);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<pair<string, int>> UserDAO::getLevelStatistics() {
    vector<pair<string, int>> stats;

    string sql = "SELECT level, COUNT(*) FROM users WHERE role = 'CUSTOMER' GROUP BY level;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getLevelStatistics prepare failed: " << sqlite3_errmsg(db) << "\n";
        return stats;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string level = getText(stmt, 0);
        int count = sqlite3_column_int(stmt, 1);
        stats.push_back({level, count});
    }

    sqlite3_finalize(stmt);
    return stats;
}

int UserDAO::getTotalCustomers() const {
    string sql = "SELECT COUNT(*) FROM users WHERE role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return count;
}

double UserDAO::getAveragePoints() const {
    string sql = "SELECT AVG(points) FROM users WHERE role = 'CUSTOMER';";

    sqlite3_stmt* stmt = nullptr;
    double avg = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            avg = sqlite3_column_double(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return avg;
}