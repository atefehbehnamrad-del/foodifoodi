#ifndef FOOD_H
#define FOOD_H

#include "MenuItem.h"

class Food : public MenuItem {
private:
    int readytime;

public:
    // Constructor
    Food(int r_time, int i, const string& n, const string& d, double p, bool a);

    // Getter
    int getReadytime() const;

    // Setter
    void setReadytime(int r);

    // Polymorphism: overriding display method
    void display() const override;
};

#endif // FOOD_H