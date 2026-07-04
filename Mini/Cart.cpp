#include "Cart.h"
#include <iostream>
#include <map>

using namespace std;

void Cart::addItem(MenuItem* item, int count, int restaurantId)
{
    if (count <= 0) {
        cout << "Count must be positive." << endl;
        return;
    }

    if (!item->isAvailable()) {
        cout << "This item is unavailable." << endl;
        return;
    }

    for (size_t i = 0; i < items.size(); i++) {

        if (items[i]->getId() == item->getId()) {
            counts[i] += count;
            cout << item->getName() << " updated. Total count: " << counts[i] << endl;
            return;
        }
    }

    items.push_back(item);
    counts.push_back(count);
    restaurantIds.push_back(restaurantId);

    cout << item->getName() << " added to cart:)" << endl;
}

void Cart::removeItem(int itemId)
{
    for (size_t i = 0; i < items.size(); i++) {

        if (items[i]->getId() == itemId) {

            items.erase(items.begin() + i);
            counts.erase(counts.begin() + i);
            restaurantIds.erase(restaurantIds.begin() + i);

            cout << "Item removed:(" << endl;
            return;
        }
    }

    cout << "Item not found:(" << endl;
}

void Cart::update(int itemId, int newCount)
{
    if (newCount <= 0) {
        removeItem(itemId);
        return;
    }

    for (size_t i = 0; i < items.size(); i++) {

        if (items[i]->getId() == itemId) {
            counts[i] = newCount;
            cout << "Item updated." << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

double Cart::getTotal() const
{
    double total = 0;

    for (size_t i = 0; i < items.size(); i++) {

        total += items[i]->getPrice() * counts[i];
    }

    return total;
}

bool Cart::isEmpty() const
{
    return items.empty();
}

void Cart::display() const
{
    if (items.empty()) {

        cout << "Cart is empty." << endl;
        return;
    }

    cout << "\n_____ Cart _____\n";
    for (size_t i = 0; i < items.size(); i++) {
        cout << counts[i] << " x " << items[i]->getName() << " = " << items[i]->getPrice() * counts[i] << "$" << endl;
    }

    cout << "\nTotal: " << getTotal() << "$" << endl;
}

void Cart::checkout(int userId, vector<Order>& orders)
{
    if (items.empty()) {
        cout << "Cart is empty." << endl;
        return;
    }

    map<int, vector<pair<MenuItem*, int>>> groupedItems;

    for (size_t i = 0; i < items.size(); i++) {
        groupedItems[restaurantIds[i]]
            .push_back({items[i], counts[i]});
    }

    for (auto& group : groupedItems) {
        int restaurantId = group.first;
        int orderId = orders.size() + 1;

        Order newOrder(orderId,userId,restaurantId);

        for (auto& itemPair : group.second) {

            newOrder.addItem(itemPair.first, itemPair.second);
        }
        orders.push_back(newOrder);

        cout << "Order " << orderId << " record :0"
             << endl;
    }
    items.clear();
    counts.clear();
    restaurantIds.clear();

    cout << "Checkout completed." << endl;
}