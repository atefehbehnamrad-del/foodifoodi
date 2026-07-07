#ifndef USERDAO_H
#define USERDAO_H

#include "User.h"
#include "Customer.h"
#include "RestaurantManager.h"
#include "SystemAdmin.h"

#include <sqlite3.h>
#include <vector>
#include <string>

using namespace std;

struct LevelChangeRecord {
    string pastLevel;
    string newLevel;
    time_t dateChange;
    string reason;
};

struct Coupons {
    string code;
    int discount;
    time_t date;
    string description;
    bool isUsed;
};

class UserDAO {
public:
    UserDAO(sqlite3* db);
    ~UserDAO();

    void insertUser(const User& user);
    void updateUser(const User& user);
    void deleteUser(int id);
    
    User* getUserById(int id);
    User* findByUsername(const std::string& username);

    vector<User*> getAllUsers();

    Customer* getCustomerById(int id);
    Customer* getCustomerByUsername(const string& username);

    vector<Customer*> getAllCustomer();

    bool updateCustomer(Customer* customer);
    bool updatePoints(int customerId , int newPoint);
    bool updateLevel(int customerId , const string& namelevel);
    bool saveLevelHistory(int customerId , const string& oldLevel ,const string& newLevel ,
                    const string& couse);
    vector<SaveLevelChange> getLevelHistory(int customerId);
    bool saveCoupon(int customerId , const Coupons& coupon);

    vector<Coupons> getCoupons(int customerId);

    bool updateCouponStatus(int customerId, const string& couponCode, bool used);
    bool deleteExpiredCoupons(int customerId);

    int getAllcustomer() const;
    double getAvrage() const;

    vector<pair<string , int>> getLevel;

    string getTime();
    

private:
    sqlite3* db;

    User* createUserObject(
        int id,
        const string& username,
        const string& password,
        const string& name,
        const string& role,
        int restaurantId,
        int points,
        const string& level
    );
};

#endif