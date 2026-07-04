#ifndef CART_H
#define CART_H

#include <vector>
#include "MenuItem.h"
#include "Order.h"

class Cart {
private:
    std::vector<MenuItem*> items;
    std::vector<int> counts;
    std::vector<int> restaurantIds;

public:
    Cart() = default;

    void addItem(MenuItem* item, int count, int restaurantId);

    void removeItem(int itemId);

    void update(int itemId, int newCount);

    double getTotal() const;

    bool isEmpty() const;

    void display() const;

    void checkout(int userId, std::vector<Order>& orders);
};

#endif