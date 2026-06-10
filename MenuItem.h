#ifndef MENUITEM_H
#define MENUITEM_H

#include <iostream>
#include <string>

using namespace std;

class MenuItem {
protected:
    int id;
    string name;
    double price;
    bool available;
    string description;
    string type;

public:
    MenuItem(int i, const string& n, double p, bool a, const string& d, const string& t);
    virtual ~MenuItem();

    int getId() const;
    string getName() const;
    string getDescription() const;
    double getPrice() const;
    bool getAvailability() const;

    void setDescription(string d);
    void setPrice(double p);
    void setAvailability(bool a);
    string getType() const;
    void setType(const string& t);

    virtual void display() const;
};

// ==================== Food ====================
class Food : public MenuItem {
private:
    int readytime;

public:
    Food(int r_time, int i, const string& n, const string& d, double p, bool a);

    int getReadytime() const;
    void setReadytime(int r);

    void display() const override;
};

// ==================== Drink ====================
class Drink : public MenuItem {
private:
    int capacity;

public:
    Drink(int c, int i, const string& n, const string& d, double p, bool a);

    int getCapacity() const;
    void setCapacity(int c);

    void display() const override;
};

#endif // MENUITEM_H