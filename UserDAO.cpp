#include "UserDAO.h"
#include <iostream>

UserDAO::UserDAO(sqlite3* database) : db(database) {}

void UserDAO::insertUser(const User& user) {
    string sql = "INSERT INTO users (id, role, username, password) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, user.getId());
        sqlite3_bind_text(stmt, 2, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Insert user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}

vector<User> UserDAO::getAllUsers() {
    vector<User> users;
    string sql = "SELECT id, role, username, password FROM users;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string username = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            users.push_back(User(id, role, username, password));
        }
    }

    sqlite3_finalize(stmt);
    return users;
}

User* UserDAO::getUserById(int id) {
    string sql = "SELECT id, role, username, password FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string username = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            user = new User(userId, role, username, password);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

User* UserDAO::findByUsername(const string& username) {
    string sql = "SELECT id, role, username, password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt = nullptr;
    User* user = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string role = (const char*)sqlite3_column_text(stmt, 1);
            string uname = (const char*)sqlite3_column_text(stmt, 2);
            string password = (const char*)sqlite3_column_text(stmt, 3);

            user = new User(id, role, uname, password);
        }
    }

    sqlite3_finalize(stmt);
    return user;
}

void UserDAO::updateUser(const User& user) {
    string sql = "UPDATE users SET role = ?, username = ?, password = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, user.getRole().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, user.getUsername().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, user.getPassword().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, user.getId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Update user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}

void UserDAO::deleteUser(int id) {
    string sql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Delete user failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}
