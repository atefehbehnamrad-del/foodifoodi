#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <sqlite3.h>
#include <string>

using namespace std;

class Database {
private:
    sqlite3* db;

public:
    Database(const string& dbName);
    ~Database();

    sqlite3* getConnection() const;
    void execute(const string& sql);
    void createTables();
};

#endif 