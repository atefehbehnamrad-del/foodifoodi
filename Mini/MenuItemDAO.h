#ifndef MENUITEMDAO_H
#define MENUITEMDAO_H

#include <vector>
#include "sqlite3.h"
#include "MenuItem.h"

class MenuItemDAO {
private:
    sqlite3* db;

public:
    MenuItemDAO(sqlite3* database);
    void insertMenuItem(int restaurantId, MenuItem* item);
    std::vector<MenuItem*> getMenuByRestaurant(int restaurantId);
    MenuItem* getMenuItemById(int id);
    void updateMenuItem(int restaurantId, MenuItem* item);
    void deleteMenuItem(int id);
};

#endif