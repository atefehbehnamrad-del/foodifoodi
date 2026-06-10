#include "Customer.h"
#include <iostream>
#include <string>

using namespace std;

Customer::Customer(int id, const string& username,
                   const string& password, const string& name)
    : User(id, username, password, name, UserRole::CUSTOMER) {}

void Customer::showMenu() {
    cout << "\n_____ customer menu _____\n";
    cout << "1.View all restaurant" << endl;
    cout << "2.Cart" << endl;
    cout << "3.History of the orders" << endl;
    cout << "0.logout" << endl;
    cout << "choice" << endl;
}