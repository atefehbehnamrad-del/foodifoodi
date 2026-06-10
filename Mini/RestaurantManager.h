#ifndef RESTAURANTMANAGER_H
#define RESTAURANTMANAGER_H

#include "User.h"

class RestaurantManager : public User {
private:
    int restaurantId;

public:

    RestaurantManager(int id,const std::string& username,  const std::string& password, const std::string& name,
                      int restaurantId);

    int getRestaurantId() const;

    void showMenu() override;

    void showRestaurantInfo();
    void editRestaurantInfo();

    void manageMenu();

    void viewOrders();
    void updateOrderStatus();
};

#endif