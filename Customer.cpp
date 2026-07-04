#include "Customer.h"
#include "NormalLevel.h"
#include "SilverLevel.h"
#include "GoldLevel.h"
#include "VipLevel.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Customer::Customer(int id, const string& username,const string& password,
                   const string& name)
    : User(id, username, password, name, UserRole::CUSTOMER){
    level = new NormalLevel();
    points = 0;
    order = 0;
    registerDate = time(nullptr);
    previousOrder = time(nullptr);
}

Customer::~Customer(){
    delete level;
}

int Customer::getPoints() const{
    return points;
}

void Customer::setPoints(int points){
    this->points = points;
}

void Customer::addPoints(int point){
    if(points <= 0){
        retrun;
    }
    this->points += point;
    cout << "points add final" << this->points << endl;
    checkAndUpgrade();
}

void Customer::removePoints(int point){
    if(points <= 0){
        return;
    }
    this->points -= point;

    if (this->points < 0)
        this->points = 0;
        cout << "points removed final " << this->points << endl;
    checkAndDowngrade();
}

MembershipLevel* Customer::getLevel() const{
    return level;
}

void Customer::setLevel(MembershipLevel* newLevel){
    if(newLevel == nullptr){
        return;
    }
    string oldLevelName = level->getLevelName;
    string newLevelNAme = newlevel->getLevelName;

    addLevelHistory(oldLevelName , newLevelName , "level change");
    delete level;
    level = newLevel;
    cout << "your new level is " << newLevelName << endl;
}

int Customer::getPointsNextLevel() const {
    if(point < 100){return 100 - points;}
    else if(points < 300){return 300 - points;}
    else if(points < 700){return 700 - points;}
    else return 0;
}

void Customer::checkAndUpgrade() {
    MembershipLevel* newLevel = nullptr;
    string newLevelName;
    
    if (points >= 700 && dynamic_cast<VipLevel*>level == nullptr) {
        newLevel = new VipLevel();
        newLevelName = "VIP";
    } else if (points >= 300 && dynamic_cast<GoldLevel*>level == nullptr) {
        newLevel = new GoldLevel();
        newLevelName = "Gold";
    } else if (points >= 100 && dynamic_cast<SilverLevel*>level == nullptr) {
        newLevel = new SilverLevel();
        newLevelName = "Silver";
    }
    
    if (newLevel != nullptr) {
        string oldLevelName = level->getLevelName();
        
        addLevelHistory(oldLevelName, newLevelName, "upgrade");
        
        delete level;
        level = newLevel;
        
        cout << "CONGRATULATIONS! UPGRADE!" << endl;
        cout << "your old level was" << oldLevelName << endl;
        cout << "your new level is" << newLevelNAme << endl;
        cout << "benefits:" << endl;
        cout << "Discount: " << level->getDiscount() << "%" << endl;
        cout << "Ersal: " << level->getErsalCost() << endl;
        cout << "Point Zarib" << level->getPointZarib() << endl;
        cout << "Coupons: " << level->getMonthlyCoupons() << endl;
        MonthlyCoupons();
    }
}

void Customer::showMenu(){
    cout << "\n===== Customer Menu =====\n";
    cout << "Level : " << level->getLevelName() << endl;
    cout << "Points: " << points << endl;
    cout << "1. View all restaurants" << endl;
    cout << "2. Cart" << endl;
    cout << "3. Order history" << endl;
    cout << "0. Logout" << endl;
    cout << "Choice: ";
}