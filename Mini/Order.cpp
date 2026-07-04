#include "Order.h"
#include <iostream>
#include <string>

using namespace std;

Order :: Order(int id, int userId, int restaurantId)
    : id(id), userId(userId), restaurantId(restaurantId), status(OrderStatus::AWAITING), total(0),
      orderTime(time(nullptr)){}

Order :: Order(int id, int userId, int restaurantId, OrderStatus status, const vector<OrderItem>& items,
             double total, time_t orderTime)
    : id(id), userId(userId), restaurantId(restaurantId), status(status), items(items), total(total),
      orderTime(orderTime){}

int Order :: getId() const {
    return id;
}

int Order :: getUserId() const {
    return userId;
}

int Order :: getRestaurantId() const {
    return restaurantId;
}

OrderStatus Order :: getStatus() const {
    return status;
}

double Order :: getTotal() const {
    return total;
}

time_t Order :: getOrderTime() const {
    return orderTime;
}

const vector<OrderItem>& Order :: getItems() const {
    return items;
}

void Order :: setStatus(OrderStatus status) {
    this->status = status;
}

void Order :: addItem(MenuItem* item,
                    int count)
{
    if (item == nullptr)
        return;

    if (!item->isAvailable())
        return;

    if (count <= 0)
        return;

    items.push_back(OrderItem(item, count));

    total = calculateTotal();
}

void Order :: removeItem(int itemId)
{
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].getItem()->getId() == itemId) {
            items.erase(items.begin() + i);
            total = calculateTotal();
            return;
        }
    }
}

double Order::calculateTotal() const
{
    double sum = 0;
    for (const auto& item : items) {
        sum += item.getTotal();
    }
    return sum;
}

void Order :: display() const
{
    cout << "\n_____ ORDER _____\n" << endl;

    cout << "Order ID: " << id << endl;

    cout << "User ID: " << userId << endl;

    cout << "Restaurant ID: " << restaurantId << endl;

    cout << "Status: " << endl;

   if (status == OrderStatus::AWAITING){
        cout << "Awaiting" << endl;
    } else if (status == OrderStatus::PREPARING){
        cout << "Preparing" << endl;
    } else if (status == OrderStatus::READY_TO_SEND){
        cout << "Ready To Send" << endl;
    }else if (status == OrderStatus::DELIVERED){
        cout << "Delivered" << endl;
    }
    cout << "Order Time: " << ctime(&orderTime);

    cout << "Items:" << endl;

    for (const auto& item : items) {
        item.display();
    }

    cout << "Total: " << total << "$" << endl;
}