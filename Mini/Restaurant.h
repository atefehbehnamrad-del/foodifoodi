#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>
#include "MenuItem.h"

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
    Restaurant(int id,
               const std::string& name,
               const std::string& address,
               const std::string& phone,
               bool status,
               int prepare,
               const std::string& description);

    ~Restaurant();

    int getId() const;
    const std::string& getName() const;
    const std::string& getAddress() const;
    const std::string& getPhone() const;
    bool getStatus() const;
    const std::string& getDescription() const;
    int getAvgPrepare() const;

    void setStatus(bool status);
    void setDescription(const std::string& description);
    void setPrepare(int prepare);

    void addMenuItem(MenuItem* item);
    void removeMenuItem(int itemId);

    MenuItem* findMenuItem(int itemId) const;

    void displayMenu() const;
    void display() const;
};

#endif