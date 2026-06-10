#ifndef DRINK_H
#define DRINK_H

#include "MenuItem.h"

class Drink : public MenuItem {
private:
    int capacity;

public:
    // Constructor
    Drink(int c, int i, const string& n, const string& d, double p, bool a);

    // Getter
    int getCapacity() const;

    // Setter
    void setCapacity(int c);

    // Polymorphism: overriding display method
    void display() const override;
};

#endif // DRINK_H