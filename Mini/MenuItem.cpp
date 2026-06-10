#include "MenuItem.h"
#include <iostream>

using namespace std;

MenuItem::MenuItem(int id, const string& name, const string& description, double price, bool available,
                   ItemType type)
    : id(id), name(name), description(description), price(price), available(available),
      type(type){}

int MenuItem::getId() const {
    return id;
}

const string& MenuItem::getName() const {
    return name;
}

const string& MenuItem::getDescription() const {
    return description;
}

double MenuItem::getPrice() const {
    return price;
}

bool MenuItem::isAvailable() const {
    return available;
}

ItemType MenuItem::getType() const {
    return type;
}

void MenuItem::setDescription(const string& description) {
    this->description = description;
}

void MenuItem::setPrice(double price) {
    if (price >= 0)
        this->price = price;
    else
        cout << "Error: Price must be positive." << endl;
}

void MenuItem::setAvailability(bool available) {
    this->available = available;
}

void MenuItem::display() const {
    cout << "[" << id << "] " << name << endl;
    cout << "Description: " << description << endl;
    cout << "Price: " << price << endl;

    cout << "Type: ";

    if (type == ItemType::FOOD) {
    cout << "Food";
    }else if (type == ItemType::DRINK) {
        cout << "Drink";
    }else if (type == ItemType::OTHER) {
        cout << "Other";
    }

    cout << endl;

    cout << "Status: " << (available ? "Available" : "Unavailable") << endl;
}

Food :: Food(int readyTime, int id, const string& name, const string& description, double price,
           bool available)
    : MenuItem(id, name, description, price, available, ItemType :: FOOD),
        readyTime(readyTime)
{
    if (this->readyTime < 0){
        this->readyTime = 0;
    }
}

int Food :: getReadyTime() const {
    return readyTime;
}

void Food :: setReadyTime(int readyTime) {
    if (readyTime >= 0){
        this->readyTime = readyTime;
    }else{
        cout << "Error: Ready time must be positive." << endl;
    }
}

void Food :: display() const {
    MenuItem :: display();
    cout << "Ready Time: " << readyTime << "(min)" << endl;
}

Drink :: Drink(int capacity, int id, const string& name, const string& description,double price,
             bool available)
    : MenuItem(id,name,description,price,available,ItemType::DRINK),
        capacity(capacity)
{
    if (this->capacity <= 0)
        this->capacity = 200;
}

int Drink::getCapacity() const {
    return capacity;
}

void Drink::setCapacity(int capacity) {
    if (capacity > 0){
        this->capacity = capacity;
    }else{
        cout << "Error: Capacity must be positive." << endl;
    }
}

void Drink::display() const {
    MenuItem::display();
    cout << "Capacity: " << capacity << " mL" << endl;
}


Other::Other(const string& group, int id, const string& name, const string& description, double price,
             bool available)
    : MenuItem(id, name, description, price, available, ItemType::OTHER),
         group(group){}

const string& Other::getGroup() const {
    return group;
}

void Other::setGroup(const string& group) {
    this->group = group;
}

void Other::display() const {
    MenuItem::display();
    cout << "Group: " << group << endl;
}