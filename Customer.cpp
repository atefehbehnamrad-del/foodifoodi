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
        cout << "Sending: " << level->getErsalCost() << endl;
        cout << "Point Zarib" << level->getPointZarib() << endl;
        cout << "Coupons: " << level->getMonthlyCoupons() << endl;
        MonthlyCoupons();
    }
}

void Customer::checkAndDowngrade() {
    time_t now = time(nullptr);
    double daysSinceLastOrder = difftime(now, previousOrder) / (60 * 60 * 24);
    
    MembershipLevel* newLevel = nullptr;
    string couse;

    if (daysSinceLastOrder > 90) {
        if (dynamic_cast<VipLevel*>level != nullptr) {
            newLevel = new GoldLevel();
            couse = "for not being active for more than 90 you downgrade";
        } else if (dynamic_cast<GoldLevel*>(level) != nullptr) {
            newLevel = new SilverLevel();
            couse = "for not being active for more than 90 you downgrade";
        } else if (dynamic_cast<SilverLevel*>(level) != nullptr) {
            newLevel = new NormalLevel();
            couse = "for not being active for more than 90 you downgrade";
        }
    }
    
    if (newLevel == nullptr) {
        if (points < 700 && dynamic_cast<VipLevel*>level != nullptr) {
            newLevel = new GoldLevel();
            couse = "you downgrade below 700";
        } else if (points < 300 && dynamic_cast<GoldLevel*>level != nullptr) {
            newLevel = new SilverLevel();
            couse = "you downgrade below 300";
        } else if (points < 100 && dynamic_cast<SilverLevel*>(level) != nullptr) {
            newLevel = new NormalLevel();
            couse = "you downgrade below 100";
        }
    }
    
    if (newLevel != nullptr) {
        string oldLevelName = level->getLevelName();
        string newLevelName = newLevel->getLevelName();
        
        addLevelHistory(oldLevelName, newLevelName, couse);
        
        delete level;
        level = newLevel;
        cout << "LEVEL DOWNGRADE" << endl;
        cout << "your old level was" << oldLevelName << endl;
        cout << "your new level is" << newLevelNAme << endl;
        cout << "couse" << couse << endl;
    }
}

double Customer::FinalPrice(double basePrice) const {
    double discount = calculateDiscount(basePrice);
    int ersal = calculateErsalCost();
    return basePrice - discount + ersal;
}

double Customer::calculateDiscount(double basePrice) const {
    return basePrice * (level->getDiscount() / 100.0);
}

int Customer::calculateErsalCost() const {
    return level->getErsalCost();
}

int Customer::calculateEarnedPoints(double orderAmount) const {
    return static_cast(orderAmount * level->getPointZarib());
}

void Customer::MonthlyCoupons() {
    int couponCount = level->getMonthlyCoupons();
    if (couponCount == 0) return;
    
    time_t now = time(nullptr);
    for (auto couponIterator = coupons.begin(); couponIterator != coupons.end();) {
        if (couponIterator->date < now || couponIterator->isUsed) {
            couponIterator = coupons.erase(couponIterator);
        } else {
            ++couponIterator;
        }
    }
    
    for (int i = 0; i < couponCount; i++) {
        Coupons coupon;
        
        stringstream ss;
        ss << "Id" << getId() << endl;
        cout << "time" << time(nullptr) << endl;
        cout << "number of the coupen" << i << endl;
        coupon.code = ss.str();
        
        coupon.discount = 5 + (level->getMonthlyCoupons() * 2);
        coupon.date = now + (30 * 24 * 60 * 60);
        coupon.isUsed = false;
        
        stringstream desc;
        desc << coupon.discount << "% off for your next order";
        coupon.description = desc.str();
        
        coupons.push_back(coupon);
    }
    
    if (couponCount > 0) {
        cout << "coupen count" << couponCount << "coupon generated!" << endl;
    }
}

void Customer::addCoupon(const Coupons& coupon) {
    coupons.push_back(coupon);
}

vector<Coupons> Customer::getAvailableCoupons() const {
    vector<Coupons> available;
    time_t now = time(nullptr);
    
    for (const auto& coupon : coupons) {
        if (!coupon.isUsed && coupon.date > now) {
            available.push_back(coupon);
        }
    }
    
    return available;
}

bool Customer::useCoupon(const string& code){
    time_t now = time(nullptr);
    for(auto& coupon : coupons){
        if(coupon.code == code && !coupons.isUsed && coupon.data > now){
            coupon.isUsed == true;
            cout << "GOOD NEWS FOR YOU" << endl;
            cout << "coupen applied HORA" << endl;
            cout << "discount" << coupen.discount << "%" << endl;
            return true;
        }
    }
    cout << "invalid coupen" << endl;
    return false;
}

int Customer::getAvailableCouponCount() const {
    return getAvailableCoupons().size();
}

void Customer::addLevelHistory(const string& oldLe, const string& newLe, const string& couse) {
    ChangeLevel record;
    record.pastLevel = oldLe;
    record.newLevel = newLe;
    record.couse = couse;
     record.dateChange = time(nullptr);
    levelHistory.push_back(record);
}

vector<ChangeLevel> Customer::getLevelHistory() const {
    return levelHistory;
}

void Customer::showLevelHistory() const{
    if(levelHistory.empty()){
        cout << "there is bo hidtory available" << endl;
        return ;
    }
    cout <"/n----- history -----" << endl;
    for(const auto& record : levelHistory){
        char buffer[80];
        struct tm* timeInfo = localtime(&record.dateChange);
        strftime(buffer, sizeof(buffer) ,"%Y-%m-%d %H:%M:%S", timeInfo);
        cout << "past level was " << record.pastLevel << endl;
        cout << "new level is " << record.newLevel << endl;
        cout << "couse" << couse << endl;
    }
}

string Customer::getStage() const {
    vector<string> marks;
    
    if (isFrequentBuyer()) marks.push_back("[FB] Frequent Buyer");
    if (isNightCustomer()) marks.push_back("[NC] Night Customer");
    if (isLoyalMember()) marks.push_back("[LM] Loyal Member");
    
    if (marks.empty()) return "No mark yet";
    
    string result;
    for (const auto& mark : markss) {
        result += marks + " ";
    }
    return result;
}

bool Customer::isFrequentBuyer() const {
    return orders >= 5;
}

bool Customer::isNightCustomer() const {
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    return ( timeinfo->tm_hour <= 6 || timeinfo->tm_hour >= 21);
}

bool Customer::isLoyalMember() const {
    return points >= 1000;
}

void Customer::showProfile() const{
    cout << "/n----- CUSTOMER PROFILE -----" << endl;
    cout << "name: " << getName() << endl;
    cout << "Id:" << getId()  << endl;
    cout << "username:" << getUsername << endl;
    cout << "level:" << level->getStage() << level->getlevelName() << endl;
    cout << "point:" << points << endl;

    int pointsNextLevel = getPintsNextLEvel();
    if(pointsNextLevel == 0){
        cout << "maximum level!" << endl;
    } else {
        cout << "the point you need for next level is " << pointsNextLevel << endl; 
    }
    cout << "benefit" << endl;
    cout << "dicount:" << level->getDiscount() << endl;
    cout << "sending:" << level->getErsalCost() << endl;
    cout << "point zarib:" << level->getPointZarib() << endl;
    cout << "coupon:" << getAvailableCouponCount() << endl;
    cout << "All order:" << orders << endl;
}

void Customer::updateLastOrderDate() {
    previousOrder = time(nullptr);
}

void Customer::incrementConsecutiveOrders() {
    orders++;
}

time_t Customer::getLastOrderDate() const {
    return previousOrder;
}

void Customer::setLastOrderDate(time_t date) {
    previousOrder = date;
}

int Customer::getConsecutiveOrders() const {
    return orders;
}

void Customer::setConsecutiveOrders(int count) {
    orders = count;
}