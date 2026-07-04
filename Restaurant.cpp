#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <vector>
#include <string>
#include <iostream>
#include "MenuItem.h" // برای شناختن MenuItem*

class Restaurant {
private:
    int id;
    std::string name;
    std::string address;
    std::string phone;
    bool status;
    int prepare;
    std::string description;
    std::vector<MenuItem*> menu;

public:
    // Constructor & Destructor
    Restaurant(int i, const std::string& n, const std::string& a, const std::string& ph, bool s, int pr, const std::string& d);
    ~Restaurant();

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getPhone() const;
    bool getStatus() const;
    std::string getDescription() const;
    int getAvgPrepare() const;

    // Setters
    void setStatus(bool s);
    void setDescription(std::string d);
    void setPrepare(int pr);

    // Menu Management
    void addToMenuItem(MenuItem* item);
    void removeMenuItem(int ItemId);
    void displayMenu() const;
    MenuItem* findMenuItem(int itemId) const;

    // Display Info
    void display() const;
};

#endif // RESTAURANT_H