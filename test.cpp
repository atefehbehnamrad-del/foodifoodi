#include "Database.h"
#include <iostream>

using namespace std;

int main() {
    // اتصال به دیتابیس
    Database db("test.db");
    
    // ساخت جدول
    db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT)");
    
    // درج اطلاعات
    db.execute("INSERT INTO users (name) VALUES ('Ali')");
    db.execute("INSERT INTO users (name) VALUES ('Reza')");
    
    // خواندن اطلاعات
    cout << "Users in database:" << endl;
    db.query("SELECT * FROM users", [](sqlite3_stmt* stmt) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = (const char*)sqlite3_column_text(stmt, 1);
        cout << "ID: " << id << ", Name: " << name << endl;
    });
    
    return 0;
}