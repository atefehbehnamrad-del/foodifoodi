#include "OrderItem.h"

using namespace std;

OrderItem::OrderItem(MenuItem* item, int count)
    : item(item),
      count(count > 0 ? count : 1)
{
    if (item != nullptr)
        price = item->getPrice();
    else
        price = 0.0;
}

OrderItem::OrderItem(MenuItem* item, double price, int count)
    : item(item), price(price), count(count > 0 ? count : 1){}

MenuItem* OrderItem::getItem() const {
    return item;
}

int OrderItem::getCount() const {
    return count;
}

double OrderItem::getPrice() const {
    return price;
}

double OrderItem::getTotal() const {
    return price * count;
}

void OrderItem::display() const {

    if (item == nullptr) {
        cout << "Item not found." << endl;
        return;
    }

    cout << item->getName() << " x " << count << endl;

    cout << "Unit Price: " << price << "$" << endl;

    cout << "Total: " << getTotal() << "$" << endl;
}