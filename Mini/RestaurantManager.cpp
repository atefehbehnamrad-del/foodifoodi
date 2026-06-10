#include "RestaurantManager.h"
#include <iostream>
#include <string>

using namespace std;


RestaurantManager::RestaurantManager(int id,const string& username,const string& password,const string& name,
        int restaurantId)
    : User(id,username,password,name,
            UserRole::RESTAURANT_MANAGER),
     restaurantId(restaurantId)
{
}

int RestaurantManager::getRestaurantId() const {
    return restaurantId;
}

void RestaurantManager::showMenu() {
    cout << "_____Restaurant Manager Menu_____" << endl;

    cout << "1. Show Restaurant Information" << endl;
    cout << "2. Edit Restaurant Information" << endl;
    cout << "3. Manage Menu Items" << endl;
    cout << "4. View Current Orders" << endl;
    cout << "5. Update Order Status" << endl;
    cout << "0. Logout" << endl;

    cout << "Choice: " << endl;
}

void RestaurantManager::showRestaurantInfo() {
    cout << "Restaurant information section." << endl;
}

void RestaurantManager::editRestaurantInfo() {
    cout << "Edit restaurant information section." << endl;
}

void RestaurantManager::manageMenu() {
    cout << "Menu management section." << endl;
}

void RestaurantManager::viewOrders() {
    cout << "Current orders section." << endl;
}

void RestaurantManager::updateOrderStatus() {
    cout << "Order status update section." << endl;
}