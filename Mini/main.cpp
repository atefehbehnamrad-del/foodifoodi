#include <iostream>
#include <limits>
#include <string>

#include "DatabaseConnection.h"
#include "UserDAO.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "CustomerUI.h"
#include "ManagerUI.h"
#include "AdminUI.h"

using namespace std;

static int readInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error:enter failed." << endl;
    }
}

User* login(UserDAO& uDAO) {
    cout << "\n_______________________\n" << endl;
    cout << " Order food system" <<  endl;
    cout << "_______________________" << endl;
    cout << "username:"; string username; getline(cin, username);
    cout << "password:"; string password; getline(cin, password);

    User* user = uDAO.findByUsername(username);
    if (!user) {
        cout << "Error:user didnt find." << endl;
        return nullptr;
    } else if (!user->checkPassword(password)) {
        cout << "Error:wrong password" << endl;
        delete user;
        return nullptr;
    }
    cout << "Wellcome " << user->getName() << "!Enjoy your time here:)";
    return user;
}

int main() {
    Database db("food_delivery.db");
    db.createTables();

    UserDAO       uDAO(db.getConnection());
    RestaurantDAO rDAO(db.getConnection());
    MenuItemDAO   mDAO(db.getConnection());
    OrderDAO      oDAO(db.getConnection());

    if (!uDAO.findByUsername("admin")) {
        SystemAdmin defaultAdmin(1, "admin", "admin123", "Admin system");
        uDAO.insertUser(defaultAdmin);
        cout << "Here id default admin for you: admin / admin123\n";
    }

    while (true) {
        User* user = login(uDAO);
        if (!user) continue;

        if (user->getRole() == UserRole::CUSTOMER) {
            CustomerUI ui(dynamic_cast<Customer&>(*user), rDAO, mDAO, oDAO);
            ui.run();
        } else if (user->getRole() == UserRole::RESTAURANT_MANAGER) {
            ManagerUI ui(dynamic_cast<RestaurantManager&>(*user), rDAO, mDAO, oDAO);
            ui.run();
        } else if (user->getRole() == UserRole::SYSTEM_ADMIN) {
            AdminUI ui(dynamic_cast<SystemAdmin&>(*user), rDAO, uDAO, oDAO);
            ui.run();
        }

        delete user;

        int ch = readInt("\n1.Exit:(  0.Enter again:)\n choice: ");
        if (ch == 0) break;
    }

    cout << "GoodBye:)Thank for coming here." << endl;
    return 0;
}