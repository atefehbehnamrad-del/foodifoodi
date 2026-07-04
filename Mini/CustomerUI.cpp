#include "CustomerUI.h"
#include <iostream>
#include <limits>

using namespace std;

static int readInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: enter correct number" << endl;
    }
}

CustomerUI::CustomerUI(Customer& user, RestaurantDAO& rDAO, MenuItemDAO& mDAO, OrderDAO& oDAO)
    : user_(user), rDAO_(rDAO), mDAO_(mDAO), oDAO_(oDAO) {}

void CustomerUI::run() {
    while (true) {
        user_.showMenu();
        int choice = readInt("");

        if      (choice == 0) break;
        else if (choice == 1) showRestaurants();
        else if (choice == 2) showCart();
        else if (choice == 3) showHistory();
        else cout << "Error:choose the number between 0-3." << endl;
    }
}

void CustomerUI::showRestaurants() {
    vector<Restaurant> restaurants = rDAO_.getAllRestaurants();
    if (restaurants.empty()) {
        cout << "No restaurant had found" << endl;
        return;
    }

    cout << "\n_____ available retaurant _____\n" << endl;
    for (auto& r : restaurants) {
        if (r.getStatus()) r.display();
    }

    int rId = readInt("retaurant ID(0 return): ");
    if (rId == 0) return;

    Restaurant* restaurant = rDAO_.getRestaurantById(rId);
    if (!restaurant || !restaurant->getStatus()) {
        cout << "Restuarant is not founded or it is unavailable" << endl;
        delete restaurant;
        return;
    }

    vector<MenuItem*> items = mDAO_.getItemsByRestaurant(rId);
    if (items.empty()) {
        cout << "Menu is empty." << endl;
        delete restaurant;
        return;
    }

    cout << "Menu" << restaurant->getName() << endl;
    for (auto* item : items) {
        if (item->isAvailable()) item->display();
    }

    while (true) {
        int itemId = readInt("");
        if (itemId == 0) break;

        MenuItem* chosen = nullptr;
        for (auto* item : items)
            if (item->getId() == itemId) { chosen = item; break; }

        if (!chosen) {
            cout << "item didnt found." << endl;
            continue;
        }

        int count = readInt("count: ");
        cart_.addItem(chosen, count, rId);

        cout << "new cart" << endl;
        cart_.display();
    }

    for (auto* item : items) delete item;
    delete restaurant;
}

void CustomerUI::showCart() {
    cart_.display();
    if (cart_.isEmpty()) return;

    cout << "1. add the order to cart" << endl;
    cout << "2. remove item" << endl;
    cout << "3. update count" << endl;
    cout << "0. return" << endl;
    int ch = readInt("choice:");

    if (ch == 1) {
        vector<Order> orders;
        cart_.checkout(user_.getId(), orders);
        for (auto& order : orders)
            oDAO_.insertOrder(order);
        cout << "order recorded." << endl;
        for (auto& order : orders)
            cout << "order ID: " << order.getId() << endl;

    } else if (ch == 2) {
        int itemId = readInt("item ID for deleting ");
        cart_.removeItem(itemId);

    } else if (ch == 3) {
        int itemId = readInt("item ID: ");
        int newCount = readInt("new count: ");
        cart_.update(itemId, newCount);
        cart_.display();
    }
}

void CustomerUI::showHistory() {
    vector<Order> history = oDAO_.getOrdersByUser(user_.getId());
    if (history.empty()) {
        cout << "NO order" << endl;
        return;
    }

    cout << "\n_____ History order _____\n";
    for (auto& order : history)
        order.display();
}