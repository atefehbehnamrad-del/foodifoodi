#include "DatabaseConnection.h"

using namespace std;

Database::Database(const string& dbName) : db(nullptr) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        cout << "Database connection failed: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Database connected successfully.\n";
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

sqlite3* Database::getConnection() const {
    return db;
}

void Database::execute(const string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        cout << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::createTables() {
    string usersTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY,"
        "role TEXT NOT NULL,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL"
        ");";

    string restaurantsTable =
        "CREATE TABLE IF NOT EXISTS restaurants ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "address TEXT NOT NULL,"
        "phone TEXT NOT NULL,"
        "status INTEGER NOT NULL,"
        "prepare_time INTEGER NOT NULL,"
        "description TEXT"
        ");";

    string menuItemsTable =
        "CREATE TABLE IF NOT EXISTS menu_items ("
        "id INTEGER PRIMARY KEY,"
        "restaurant_id INTEGER NOT NULL,"
        "name TEXT NOT NULL,"
        "description TEXT,"
        "price REAL NOT NULL,"
        "available INTEGER NOT NULL,"
        "type TEXT NOT NULL,"
        "ready_time INTEGER,"
        "capacity INTEGER,"
        "FOREIGN KEY (restaurant_id) REFERENCES restaurants(id)"
        ");";

    string ordersTable =
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "restaurant_id INTEGER NOT NULL,"
        "status TEXT NOT NULL,"
        "total REAL NOT NULL,"
        "order_time TEXT,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (restaurant_id) REFERENCES restaurants(id)"
        ");";

    string orderItemsTable =
        "CREATE TABLE IF NOT EXISTS order_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "order_id INTEGER NOT NULL,"
        "menu_item_id INTEGER NOT NULL,"
        "count INTEGER NOT NULL,"
        "price REAL NOT NULL,"
        "FOREIGN KEY (order_id) REFERENCES orders(id),"
        "FOREIGN KEY (menu_item_id) REFERENCES menu_items(id)"
        ");";

    execute(usersTable);
    execute(restaurantsTable);
    execute(menuItemsTable);
    execute(ordersTable);
    execute(orderItemsTable);
}