#include "Restaurant.h"
#include <iostream>

using namespace std;

Restaurant::Restaurant(int id, const string& name, const string& address, const string& phone, bool status, int prepare,
                       const string& description)
    : id(id),name(name),address(address),phone(phone),status(status),description(description)
{
    this->prepare = (prepare > 0) ? prepare : 0;
}

Restaurant::~Restaurant() {
    for (MenuItem* item : menu) {
        delete item;
    }
}

int Restaurant::getId() const {
    return id;
}

const string& Restaurant::getName() const {
    return name;
}

const string& Restaurant::getAddress() const {
    return address;
}

const string& Restaurant::getPhone() const {
    return phone;
}

bool Restaurant::getStatus() const {
    return status;
}

const string& Restaurant::getDescription() const {
    return description;
}

int Restaurant::getAvgPrepare() const {
    return prepare;
}

void Restaurant::setStatus(bool status) {
    this->status = status;
}

void Restaurant::setDescription(const string& description) {
    this->description = description;
}

void Restaurant::setPrepare(int prepare) {
    if (prepare > 0) {
        this->prepare = prepare;
    } else {
        cout << "Error: Prepare time must be positive." << endl;
    }
}

void Restaurant::addMenuItem(MenuItem* item) {
    menu.push_back(item);
}

void Restaurant::removeMenuItem(int itemId) {
    for (size_t i = 0; i < menu.size(); i++) {

        if (menu[i]->getId() == itemId) {

            delete menu[i];

            menu.erase(menu.begin() + i);

            cout << "Item removed successfully." << endl;
            return;
        }
    }

    cout << "Item not found." << endl;
}

MenuItem* Restaurant::findMenuItem(int itemId) const {

    for (MenuItem* item : menu) {

        if (item->getId() == itemId) {
            return item;
        }
    }

    return nullptr;
}

void Restaurant::displayMenu() const {

    if (menu.empty()) {
        cout << "Menu is empty." << endl;
        return;
    }

    cout << "\n_____ MENU _____\n";

    for (MenuItem* item : menu) {

        item->display();

        cout << "________________" << endl;
    }
}

void Restaurant::display() const {

    cout << "\n_____ Restaurant information _____\n";

    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Phone: " << phone << endl;

    cout << "Status: " << (status ? "Open" : "Closed") << endl;

    cout << "Description: " << description << endl;

    cout << "Average Prepare Time: " << prepare << "(min)" << endl;
}