#ifndef ORDERITEM_H
#define ORDERITEM_H

#include "MenuItem.h"
#include <iostream>

class OrderItem {
private:
    MenuItem* item;
    double price;
    int count;

public:
    OrderItem(MenuItem* item, int count);
    OrderItem(MenuItem* item, double price, int count);

    MenuItem* getItem() const;
    int getCount() const;
    double getPrice() const;

    double getTotal() const;

    void display() const;
};

#endif