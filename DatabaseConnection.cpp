#include "DatabaseConnection.h"
#include <iostream>

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
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "role TEXT NOT NULL,"
        "username TEXT NOT NULL UNIQUE,"
        "password TEXT NOT NULL,"
        "restaurant_id INTEGER DEFAULT 0,"
        "points INTEGER DEFAULT 0,"
        "level TEXT DEFAULT 'Normal',"
        "last_order_date TEXT,"
        "consecutive_orders INTEGER DEFAULT 0,"
        "registration_date TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    string restaurantsTable =
        "CREATE TABLE IF NOT EXISTS restaurants ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "address TEXT NOT NULL,"
        "phone TEXT NOT NULL,"
        "status INTEGER NOT NULL,"
        "prepare_time INTEGER NOT NULL,"
        "description TEXT"
        ");";

    string menuItemsTable =
        "CREATE TABLE IF NOT EXISTS menu_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
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
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "restaurant_id INTEGER NOT NULL,"
        "status TEXT NOT NULL,"
        "total REAL NOT NULL,"
        "final_price REAL DEFAULT 0,"
        "earned_points INTEGER DEFAULT 0,"
        "order_time TEXT,"
        "cancelled INTEGER DEFAULT 0,"
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

    string levelHistoryTable =
        "CREATE TABLE IF NOT EXISTS level_history ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "customer_id INTEGER NOT NULL,"
        "old_level TEXT NOT NULL,"
        "new_level TEXT NOT NULL,"
        "change_date TEXT DEFAULT CURRENT_TIMESTAMP,"
        "reason TEXT,"
        "FOREIGN KEY (customer_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    string couponsTable =
        "CREATE TABLE IF NOT EXISTS coupons ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "customer_id INTEGER NOT NULL,"
        "code TEXT NOT NULL UNIQUE,"
        "discount INTEGER DEFAULT 5,"
        "expiry_date TEXT NOT NULL,"
        "is_used INTEGER DEFAULT 0,"
        "description TEXT,"
        "FOREIGN KEY (customer_id) REFERENCES users(id) ON DELETE CASCADE"
        ");";

    string indexLevel = "CREATE INDEX IF NOT EXISTS idx_user_level ON users(level);";
    string indexOrderCustomer = "CREATE INDEX IF NOT EXISTS idx_order_user ON orders(user_id);";
    string indexOrderDate = "CREATE INDEX IF NOT EXISTS idx_order_date ON orders(order_time);";
    string indexCouponCode = "CREATE INDEX IF NOT EXISTS idx_coupon_code ON coupons(code);";
    string indexLevelHistory = "CREATE INDEX IF NOT EXISTS idx_level_history ON level_history(customer_id);";

    execute(usersTable);
    execute(restaurantsTable);
    execute(menuItemsTable);
    execute(ordersTable);
    execute(orderItemsTable);
    execute(levelHistoryTable);
    execute(couponsTable);
    execute(indexLevel);
    execute(indexOrderCustomer);
    execute(indexOrderDate);
    execute(indexCouponCode);
    execute(indexLevelHistory);

    cout << "tables are ready" << endl;
}