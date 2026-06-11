#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "DatabaseConnection.h" 
#include "UserDAO.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "Customer.h"
#include "RestaurantManager.h"
#include "SystemAdmin.h"
#include "Cart.h"

using namespace std;

void customerMenu(Customer& user, RestaurantDAO& rDAO, MenuItemDAO& mDAO, OrderDAO& oDAO);
void managerMenu(RestaurantManager& user, RestaurantDAO& rDAO, MenuItemDAO& mDAO, OrderDAO& oDAO);
void adminMenu(SystemAdmin& user, RestaurantDAO& rDAO, UserDAO& uDAO);

int readInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

User* login(UserDAO& uDAO) {
    cout << "\n____________________________" << endl;
    cout << "  Food Order System  " << endl;
    cout << "______________________________ " << endl;
    cout << "Username: ";
    string username; getline(cin, username);
    cout << "Password: ";
    string password; getline(cin, password);

    User* user = uDAO.findByUsername(username);
    if (!user) {
        cout << "User not found." << endl;
        return nullptr;
    }
    if (!user->checkPassword(password)) {
        cout << "Wrong password." << endl;
        delete user;
        return nullptr;
    }
    cout << "WELLCOME, " << user->getName() << "!HAVE A GOOD TIME:)" << endl;
    return user;
}

int main() {
    Database db("food_delivery.db");
    db.createTables();

    UserDAO       uDAO(db.getConnection());
    RestaurantDAO rDAO(db.getConnection());
    MenuItemDAO   mDAO(db.getConnection());
    OrderDAO      oDAO(db.getConnection());

    if (!uDAO.findByUsername("admin")) {
        SystemAdmin defaultAdmin(1, "admin", "admin123", "System Admin");
        uDAO.insertUser(defaultAdmin);
        cout << "Default admin created. (Admin / Admin123)\n";
    }

    while (true) {
        User* user = login(uDAO);
        if (!user) continue;

       if (user->getRole() == UserRole::CUSTOMER) {
            customerMenu(dynamic_cast<Customer&>(*user), rDAO, mDAO, oDAO);
        } else if (user->getRole() == UserRole::RESTAURANT_MANAGER) {
            managerMenu(dynamic_cast<RestaurantManager&>(*user), rDAO, mDAO, oDAO);
        } else if (user->getRole() == UserRole::SYSTEM_ADMIN) {
            adminMenu(dynamic_cast<SystemAdmin&>(*user), rDAO, uDAO);
        }

            delete user;
        cout << "\n1. Login again\n0. Exit\nChoice: ";
        int ch = readInt("");
        if (ch == 0) break;
    }

    cout << "Goodbye!We wil miss you:(" << endl;
    return 0;
}
void CustomerMenu(Customer& user ,  RestaurantDAO& rDAO , MenuItemDAO& mDAO , OrderDAO& oDAO){
    Cart cart;
    while(true){
        user.showMenu();
        int choice = readInt("Choice: ");
        if(choice == 0){
            break;
        } 
        else if(choice == 1){
        vector<Restaurant> restaurants = rDAO.getAllRestaurants();
        if(restaurants.empty()){
            cout << "No restaurant is available" << endl;
            continue;
        }
        cout << "_____Avaiable restaurant_____" << endl;
        for(auto& r : restaurants){
            if(r.getStatus()){
               r.display();
            }
        }
        int rId = readInt("Select restaurantId: ");
        if(rId == 0){
            continue;
        }

        Restaurant* restaurant = rDAO.getRestaurantById(rId);
        if(restaurant == nullptr || !restaurant->getStatus()){
            cout << "Restaurant is not available or not found" << endl;
            delete restaurant;
            continue;
        }

        vector<MenuItem*> items = mDAO.getMenuByRestaurant(rId);
        if(items.empty()){
            cout << "the menu is empty" << endl;
        } else {
            cout << "\n_____ Restaurant menu _____\n" << endl;
            for(auto* item : items){
                if(item->isAvailable()){
                    item->display();
                }
            }
        }
        delete restaurant;
    }
}
}