#include "MenuItem.h"

// ==================== MenuItem Implementation ====================
MenuItem::MenuItem(int i, const string& n, double p, bool a, const string& d, const string& t)
    : id(i), name(n), price(p), available(a), description(d), type(t) {}

MenuItem::~MenuItem() {}

int MenuItem::getId() const { return id; }
string MenuItem::getName() const { return name; }
string MenuItem::getDescription() const { return description; }
double MenuItem::getPrice() const { return price; }
bool MenuItem::getAvailability() const { return available; }

void MenuItem::setDescription(string d) { description = d; }
void MenuItem::setPrice(double p) {
    if (p >= 0) price = p;
    else cout << "Error: the price should be positive" << endl;
}
void MenuItem::setAvailability(bool a) { available = a; }
string MenuItem::getType() const { return type; }
void MenuItem::setType(const string& t) { type = t; }

void MenuItem::display() const {
    cout << "[" << id << "] Name: " << name << " - Price: " << price << endl
         << "Description: " << description << endl
         << "Status: " << (available ? "Available" : "Unavailable") << endl
         << "Type: " << type << endl;
}

// ==================== Food Implementation ====================
Food::Food(int r_time, int i, const string& n, const string& d, double p, bool a)
    : MenuItem(i, n, p, a, d, "Food"), readytime(r_time) {
    if (readytime < 0) {
        cout << "Error: Please enter the positive time" << endl;
        readytime = 0;
    }
}

int Food::getReadytime() const { return readytime; }

void Food::setReadytime(int r) {
    if (r < 0) cout << "Error: The time is incorrect. Please enter a positive time." << endl;
    else readytime = r;
}

void Food::display() const {
    MenuItem::display();
    cout << "The time you need to wait for your food to be ready is " << readytime << " minutes." << endl;
}

// ==================== Drink Implementation ====================
Drink::Drink(int c, int i, const string& n, const string& d, double p, bool a)
    : MenuItem(i, n, p, a, d, "Drink"), capacity(c) {
    if (capacity < 0) {
        cout << "Error: Please enter the positive number." << endl;
        capacity = 250;
    }
}

int Drink::getCapacity() const { return capacity; }

void Drink::setCapacity(int c) {
    if (c < 0) cout << "Error: The capacity is not valid. Please enter a positive number." << endl;
    else capacity = c;
}

void Drink::display() const {
    MenuItem::display();
    cout << "The capacity you want for your drink is " << capacity << " mL" << endl;
}