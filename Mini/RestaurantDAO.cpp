#include "RestaurantDAO.h"
#include <iostream>

using namespace std;

RestaurantDAO::RestaurantDAO(sqlite3* database) : db(database) {}
void RestaurantDAO::insertRestaurant(const Restaurant& restaurant) {
    string sql = "INSERT INTO restaurants (id, name, address, phone, status, prepare_time, description) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, restaurant.getId());
        sqlite3_bind_text(stmt, 2, restaurant.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, restaurant.getAddress().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, restaurant.getPhone().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, restaurant.getStatus());
        sqlite3_bind_int(stmt, 6, restaurant.getAvgPrepare());
        sqlite3_bind_text(stmt, 7, restaurant.getDescription().c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Insert restaurant failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}
vector<Restaurant> RestaurantDAO::getAllRestaurants() {
    vector<Restaurant> restaurants;
    string sql = "SELECT id, name, address, phone, status, prepare_time, description FROM restaurants;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string name = (const char*)sqlite3_column_text(stmt, 1);
            string address = (const char*)sqlite3_column_text(stmt, 2);
            string phone = (const char*)sqlite3_column_text(stmt, 3);
            int status = sqlite3_column_int(stmt, 4);
            int prepareTime = sqlite3_column_int(stmt, 5);
            string description = (const char*)sqlite3_column_text(stmt, 6);

            restaurants.push_back(
                Restaurant(id, name, address, phone, status, prepareTime, description)
            );
        }
    }

    sqlite3_finalize(stmt);
    return restaurants;
}
Restaurant* RestaurantDAO::getRestaurantById(int id) {
    string sql = "SELECT id, name, address, phone, status, prepare_time, description FROM restaurants WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    Restaurant* restaurant = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int rid = sqlite3_column_int(stmt, 0);
            string name = (const char*)sqlite3_column_text(stmt, 1);
            string address = (const char*)sqlite3_column_text(stmt, 2);
            string phone = (const char*)sqlite3_column_text(stmt, 3);
            int status = sqlite3_column_int(stmt, 4);
            int prepareTime = sqlite3_column_int(stmt, 5);
            string description = (const char*)sqlite3_column_text(stmt, 6);

            restaurant = new Restaurant(rid, name, address, phone, status, prepareTime, description);
        }
    }

    sqlite3_finalize(stmt);
    return restaurant;
}
void RestaurantDAO::updateRestaurant(const Restaurant& restaurant) {
    string sql = "UPDATE restaurants SET name = ?, address = ?, phone = ?, status = ?, prepare_time = ?, description = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, restaurant.getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, restaurant.getAddress().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, restaurant.getPhone().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, restaurant.getStatus());
        sqlite3_bind_int(stmt, 5, restaurant.getAvgPrepare());
        sqlite3_bind_text(stmt, 6, restaurant.getDescription().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 7, restaurant.getId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Update restaurant failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}
void RestaurantDAO::deleteRestaurant(int id) {
    string sql = "DELETE FROM restaurants WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Delete restaurant failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}
