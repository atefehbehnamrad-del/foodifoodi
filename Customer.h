#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "MembershipLevel.h"
#include "NormalLevel.h"
#include "SilverLevel.h"
#include "GoldLevel.h"
#include "VipLevel.h"
#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct Coupons{
    string code;
    int discount;
    time_t date;
    string description;
    bool isUsed;
};

struct ChangeLevel{
    string pastLevel;
    string newLevel;
    time_t dateChange;
    string reason;
};

class Customer : public User {
private:
    MembershipLevel* level;
    int points;
    vector<ChangeLevel> levelHistory;
    vector<Coupons> coupons;
    time_t previousOrder; 
    time_t registerDate;
    int orders;
public:
    Customer(int id, const std::string& username, const std::string& password,
             const std::string& name);

    ~Customer();

    int getPoints() const;
    void setPoints(int points);
    void addPoints(int points);
    void removePoints(int points);
    
    MembershipLevel* getLevel() const;
    void setLevel(MembershipLevel* newLevel);
    void showMenu() override;
    
    string getCurrentLevel() const;
    void checkAndUpgrade();
    void checkAndDowngrade();
    int getPointsNextLevel() const;
    
    double FinalPrice(double basePrice) const;
    int calculateErsalCost() const;
    int calculateEarnedPoints(double orderAmount) const;
    double calculateDiscount(double basePrice) const;
    
    void MonthlyCoupons();
    vector<Coupons> getAvailableCoupons() const;
    bool useCoupon(const string& code);
    int getAvailableCouponCount() const;
    void addCoupon(const Coupons& coupon);
    
    void addLevelHistory(const string& oldLevel, const string& newLevel, const string& reason);
    vector<ChangeLevel> getLevelHistory() const;
    void showLevelHistory() const;
    
    string getStage() const;
    bool isFrequentBuyer() const;
    bool isNightCustomer() const;
    bool isLoyalMember() const;
    
    void showProfile() const;
    void updateLastOrderDate();
    void incrementConsecutiveOrders();
    time_t getLastOrderDate() const;
    void setLastOrderDate(time_t date);
    int getConsecutiveOrders() const;
    void setConsecutiveOrders(int count);
};

#endif