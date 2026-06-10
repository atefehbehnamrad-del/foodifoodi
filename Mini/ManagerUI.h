#ifndef MANAGERUI_H
#define MANAGERUI_H

#include "RestaurantManager.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"

class ManagerUI {
public:
    ManagerUI(RestaurantManager& user, RestaurantDAO& rDAO,MenuItemDAO& mDAO, OrderDAO& oDAO);ss
    void run();

private:
    RestaurantManager& user_;
    RestaurantDAO&     rDAO_;
    MenuItemDAO&       mDAO_;
    OrderDAO&          oDAO_;
    int                rId_;

    void editRestaurantInfo();
    void manageMenu();
    void manageOrders();
};

#endif