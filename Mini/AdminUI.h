#ifndef ADMINUI_H
#define ADMINUI_H

#include "SystemAdmin.h"
#include "RestaurantDAO.h"
#include "UserDAO.h"
#include "OrderDAO.h"

class AdminUI {
public:
    AdminUI(SystemAdmin& user, RestaurantDAO& rDAO,
            UserDAO& uDAO, OrderDAO& oDAO);

    void run();

private:
    SystemAdmin&   user_;
    RestaurantDAO& rDAO_;
    UserDAO&       uDAO_;
    OrderDAO&      oDAO_;

    void addRestaurant();
    void toggleRestaurant();
    void showReports();
};

#endif