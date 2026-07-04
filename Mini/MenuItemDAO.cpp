#include "MenuItemDAO.h"
#include <iostream>

using namespace std;

MenuItemDAO::MenuItemDAO(sqlite3* database)
    : db(database) {}
void MenuItemDAO::insertMenuItem(int restaurantId, MenuItem* item)
{
    string sql =
        "INSERT INTO menu_items "
        "(id, restaurant_id, name, description, price, available, type, ready_time, capacity) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, item->getId());
    sqlite3_bind_int(stmt, 2, restaurantId);
    sqlite3_bind_text(stmt, 3, item->getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, item->getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, item->getPrice());
    sqlite3_bind_int(stmt, 6, item->isAvailable());

    string typeStr;

    if (item->getType() == ItemType::FOOD)
        typeStr = "Food";
    else if (item->getType() == ItemType::DRINK)
        typeStr = "Drink";
    else
        typeStr = "Other";

    sqlite3_bind_text(stmt, 7, typeStr.c_str(), -1, SQLITE_TRANSIENT);

    if (auto food = dynamic_cast<Food*>(item))
        sqlite3_bind_int(stmt, 8, food->getReadyTime());
    else
        sqlite3_bind_null(stmt, 8);

    if (auto drink = dynamic_cast<Drink*>(item))
        sqlite3_bind_int(stmt, 9, drink->getCapacity());
    else
        sqlite3_bind_null(stmt, 9);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        cout << "Insert failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

MenuItem* MenuItemDAO::getMenuItemById(int id)
{
    string sql =
        "SELECT id,name,description,price,available,type,ready_time,capacity "
        "FROM menu_items WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;
    MenuItem* item = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int itemId = sqlite3_column_int(stmt, 0);
            string name = (const char*)sqlite3_column_text(stmt, 1);
            string description = (const char*)sqlite3_column_text(stmt, 2);
            double price = sqlite3_column_double(stmt, 3);
            bool available = sqlite3_column_int(stmt, 4);

            string type =
                (const char*)sqlite3_column_text(stmt, 5);

            if (type == "Food") {
                int readyTime =
                    sqlite3_column_int(stmt, 6);

                item = new Food(readyTime,itemId,name,description,price,available);
            }else if (type == "Drink"){
                int capacity =
                    sqlite3_column_int(stmt, 7);

                item = new Drink(capacity,itemId,name,description,price,available);
            }else{
                item = new Other("General", itemId,name,description,price,available);
            }
        }
    }

    sqlite3_finalize(stmt);

    return item;
}

vector<MenuItem*> MenuItemDAO::getMenuByRestaurant(int restaurantId)
{
    vector<MenuItem*> items;

    string sql =
        "SELECT id FROM menu_items "
        "WHERE restaurant_id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, restaurantId);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int itemId =
                sqlite3_column_int(stmt, 0);

            MenuItem* item =
                getMenuItemById(itemId);

            if (item != nullptr)
                items.push_back(item);
        }
    }

    sqlite3_finalize(stmt);

    return items;
}



void MenuItemDAO::updateMenuItem(int restaurantId, MenuItem* item){
    string sql =
        "UPDATE menu_items SET "
        "restaurant_id=?,"
        "name=?,"
        "description=?,"
        "price=?,"
        "available=?,"
        "type=?,"
        "ready_time=?,"
        "capacity=? "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cout << "Prepare failed: "
             << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, restaurantId);
    sqlite3_bind_text(stmt, 2, item->getName().c_str(),-1,SQLITE_TRANSIENT);

    sqlite3_bind_text(stmt, 3,item->getDescription().c_str(),-1,SQLITE_TRANSIENT);

    sqlite3_bind_double(stmt, 4, item->getPrice());

    sqlite3_bind_int(stmt, 5,item->isAvailable());

    string typeStr;

    if (item->getType() == ItemType::FOOD)
        typeStr = "Food";
    else if (item->getType() == ItemType::DRINK)
        typeStr = "Drink";
    else
        typeStr = "Other";

    sqlite3_bind_text(stmt, 6, typeStr.c_str(), -1, SQLITE_TRANSIENT);

    if (auto food = dynamic_cast<Food*>(item))
        sqlite3_bind_int(stmt, 7, food->getReadyTime());
    else
        sqlite3_bind_null(stmt, 7);

    if (auto drink = dynamic_cast<Drink*>(item))
        sqlite3_bind_int(stmt, 8, drink->getCapacity());
    else
        sqlite3_bind_null(stmt, 8);

    sqlite3_bind_int(stmt, 9,item->getId());

    if (sqlite3_step(stmt) != SQLITE_DONE){
        cout << "Update failed: "<< sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

void MenuItemDAO::deleteMenuItem(int id){
    string sql =
        "DELETE FROM menu_items "
        "WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE){
            cout << "Delete failed: " << sqlite3_errmsg(db)  << endl;
        }
    }

    sqlite3_finalize(stmt);
}