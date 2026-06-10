#include "SystemAdmin.h"
#include <iostream>
#include <string>

using namespace std;

SystemAdmin :: SystemAdmin(int id, const string& username, const string& password, 
        const string& name)
    : User(id, username, password, name, UserRole::SYSTEM_ADMIN) {}

void SystemAdmin::showMenu() {
    cout << "\n_____ System Admin menu______\n";
    cout << "1.Register a new restaurant" << endl;
    cout << "2.On or off the restaurant" << endl;
    cout << "3.View report" << endl;
    cout << "0.Logout" << endl;
    cout << "Choice" << endl;
}