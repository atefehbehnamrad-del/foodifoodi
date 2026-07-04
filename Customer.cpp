#include "Customer.h"
#include "NormalLevel.h"

#include <iostream>

using namespace std;

Customer::Customer(int id, const string& username,const string& password,
                   const string& name)
    : User(id, username, password, name, UserRole::CUSTOMER){
    level = new NormalLevel();
    points = 0;
}

Customer::~Customer(){
    delete level;
}

int Customer::getPoints() const{
    return points;
}

void Customer::addPoints(int point){
    points += point;
}

void Customer::removePoints(int point){
    points -= point;

    if (points < 0)
        points = 0;
}

MembershipLevel* Customer::getLevel() const{
    return level;
}

void Customer::setLevel(MembershipLevel* newLevel){
    delete level;
    level = newLevel;
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