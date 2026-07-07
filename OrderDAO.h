#ifndef ORDERDAO_H
#define ORDERDAO_H

#include <vector>
#include <string>
#include <sqlite3.h>
#include <ctime>

#include "Order.h"
#include "MenuItemDAO.h"

using namespace std;

class OrderDAO {
private:
    sqlite3* db;
    MenuItemDAO menuItemDAO;

public:
    OrderDAO(sqlite3* database);
    ~OrderDAO();

    int insertOrder(const Order& order);
    vector<Order> getAllOrders();
    Order* getOrderById(int id);
    vector<Order> getOrdersByUserId(int userId);
    vector<Order> getOrdersByRestaurantId(int restaurantId);
    void updateOrderStatus(int orderId, OrderStatus status);
    void deleteOrder(int id);

    bool updateOrder(const Order& order);
    bool cancelOrder(int orderId);
    vector<Order> getActiveOrders();
    double getTotalRevenue() const;
    int getActiveOrdersCount() const;
    double getCustomerTotalSpent(int customerId) const;

    bool saveOrderItems(int orderId, const vector<OrderItem>& items);
    vector<OrderItem> getOrderItems(int orderId);

    string getCurrentTime();
};

#endif