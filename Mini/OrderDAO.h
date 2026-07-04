#ifndef ORDERDAO_H
#define ORDERDAO_H

#include <vector>
#include <string>
#include <sqlite3.h>

#include "Order.h"
#include "MenuItemDAO.h"

class OrderDAO {
private:
    sqlite3* db;
    MenuItemDAO menuItemDAO;

public:
    OrderDAO(sqlite3* database);
    void insertOrder(const Order& order);
    std::vector<Order> getAllOrders();
    Order* getOrderById(int id);
    std::vector<Order> getOrdersByUserId(int userId);
    std::vector<Order> getOrdersByRestaurantId(int restaurantId);
    void updateOrderStatus(int orderId,OrderStatus status);
    void deleteOrder(int id);
    
};

#endif