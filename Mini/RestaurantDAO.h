#ifndef RESTAURANTDAO_H
#define RESTAURANTDAO_H

#include <vector>
#include <string>
#include "sqlite3.h"
#include "Restaurant.h"


class RestaurantDAO {
private:
    sqlite3* db;

public:
    RestaurantDAO(sqlite3* database);

    void insertRestaurant(const Restaurant& restaurant);
    std::vector<Restaurant> getAllRestaurants();
    Restaurant* getRestaurantById(int id);
    void updateRestaurant(const Restaurant& restaurant);
    void deleteRestaurant(int id);
};

#endif
