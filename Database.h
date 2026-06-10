#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include <string>
#include <functional>
#include <iostream>

class Database {
private:
    sqlite3* db;
    std::string dbPath;
    
public:
    Database(const std::string& path = "food_delivery.db") : db(nullptr), dbPath(path) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "Database connected successfully!" << std::endl;
        }
    }
    
    ~Database() {
        if (db) {
            sqlite3_close(db);
        }
    }
    
    bool execute(const std::string& sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        
        if (rc != SQLITE_OK) {
            std::cerr << "SQL Error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }
    
    bool query(const std::string& sql, std::function<void(sqlite3_stmt*)> rowHandler) {
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        
        if (rc != SQLITE_OK) {
            std::cerr << "Query prepare failed: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            rowHandler(stmt);
        }
        
        sqlite3_finalize(stmt);
        return true;
    }
    
    int getLastInsertId() {
        return sqlite3_last_insert_rowid(db);
    }
    
    sqlite3* getConnection() { return db; }
};

#endif