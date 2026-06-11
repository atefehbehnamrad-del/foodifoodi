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
        cout << "Default admin created. (Admin / Admin123)" << endl;
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
void customerMenu(Customer& user ,  RestaurantDAO& rDAO , MenuItemDAO& mDAO , OrderDAO& oDAO){
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

double readDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number." << endl;
    }
}

void managerMenu(RestaurantManager& user, RestaurantDAO& rDAO, MenuItemDAO& mDAO, OrderDAO& oDAO) {
    int rId = user.getRestaurantId();
    while (true) {
        user.showMenu();
        int choice = readInt("Choice: ");
        if (choice == 0)
            break;
        
        else if (choice == 1) {
            Restaurant* r = rDAO.getRestaurantById(rId);
            if (!r) { 
                cout << "Restaurant not found.\n"; 
                continue; 
            }
            r->display();

            int ch = readInt("1. Edit information   0. Back\nChoice: ");
            if (ch == 1) {
                cout << "New name: "; 
                string name; 
                getline(cin, name);
                cout << "New address: "; 
                string addr; 
                getline(cin, addr);
                cout << "New phone: "; 
                string phone; 
                getline(cin, phone);
                cout << "Description: "; 
                string desc; 
                getline(cin, desc);
                int prep = readInt("Preparation time (minutes): ");

                Restaurant updatedR(r->getId(), name, addr, phone, r->getStatus(), prep, desc);
                rDAO.updateRestaurant(updatedR);
                cout << "Information updated.\n";
            }
            delete r;
        }

        else if (choice == 2) {
            vector<MenuItem*> items = mDAO.getMenuByRestaurant(rId);  
            cout << "\n_____ Current Menu _____" << endl;
            for (auto* item : items) {
                if (item) item->display();
            }
            int ch = readInt("1. Add item   2. Edit   3. Delete   0. Back\nChoice: ");
            
            if (ch == 1) {
                string name, desc;
                cout << "Name: "; 
                getline(cin, name);
                cout << "Description: "; 
                getline(cin, desc);
                
                double price = readDouble("Price: ");
                
                cout << "Item type (1: Food, 2: Drink, 3: Other): ";
                int typeChoice = readInt("");
                
                MenuItem* newItem = nullptr;
                if (typeChoice == 1) {
                    int readyTime = readInt("Ready time (minutes): ");
                    newItem = new Food(readyTime, 0, name, desc, price, true);
                } else if (typeChoice == 2) {
                    int capacity = readInt("Capacity (mL): ");
                    newItem = new Drink(capacity, 0, name, desc, price, true);
                } else {
                    string group;
                    cout << "Group: ";
                    getline(cin, group);
                    newItem = new Other(group, 0, name, desc, price, true);
                }
                
                mDAO.insertMenuItem(rId, newItem);
                cout << "Item added successfully!\n";
                delete newItem;
            }
            else if (ch == 2) {
                int itemId = readInt("Item ID for editing: ");
                cout << "Edit item " << itemId << " (implementation needed)\n";
            }
            else if (ch == 3) {
                int itemId = readInt("Item ID for deleting: ");
                mDAO.deleteMenuItem(itemId);
                cout << "Item deleted :(\n";
            }
            
            for (auto* item : items) {
                delete item;
            }
        }

        else if (choice == 3) {
            vector<Order> orders = oDAO.getOrdersByRestaurantId(rId);
            if (orders.empty()) { 
                cout << "No orders available." << endl; 
                continue; 
            }
            for (auto& order : orders) {
                order.display();
            }
            int orderId = readInt("Order ID to update: ");
            if (orderId == 0) {s
                continue;
            }
            cout << "1. Preparing\n2. Ready to send\n3. Delivered" << endl;
            int s = readInt("Choice: ");

            OrderStatus newStatus = OrderStatus::PREPARING;
            if (s == 2) newStatus = OrderStatus::READY_TO_SEND;
            else if (s == 3) {
                newStatus = OrderStatus::DELIVERED;
            }
            oDAO.updateOrderStatus(orderId, newStatus);
            cout << "Status updated." << endl;
        }
    }
}