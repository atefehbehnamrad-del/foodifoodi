#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <iostream>
#include <string>
#include <sqlite3.h>



class Database {
private:
    sqlite3* db;

public:
    Database(const std::string& dbName = "food_delivery.db");  
    ~Database();

    sqlite3* getConnection() const;
    void execute(const std::string& sql); 
    void createTables();
};

#endif