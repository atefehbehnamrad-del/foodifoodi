#include "User.h"
#include <iostream>

using namespace std;

User::User(int id, const string& username, const string& password, const string& name,
           UserRole role)
    : id(id), username(username), password(password), name(name),
      role(role){}

void User::printInfo() const {
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Username: " << username << endl;
}

bool User::checkPassword(const string& input) const {
    return input == password;
}