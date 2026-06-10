#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include <ctime>

#include "OrderItem.h"

enum class OrderStatus {
    AWAITING,
    PREPARING,
    READY_TO_SEND,
    DELIVERED
};

class Order {
private:
    int id;
    int userId;
    int restaurantId;

    OrderStatus status;

    std::vector<OrderItem> items;

    double total;
    time_t orderTime;

public:
    Order(int id,
          int userId,
          int restaurantId);

    Order(int id,
          int userId,
          int restaurantId,
          OrderStatus status,
          const std::vector<OrderItem>& items,
          double total,
          time_t orderTime);

    int getId() const;
    int getUserId() const;
    int getRestaurantId() const;

    OrderStatus getStatus() const;

    double getTotal() const;

    time_t getOrderTime() const;

    const std::vector<OrderItem>& getItems() const;

    void setStatus(OrderStatus status);

    void addItem(MenuItem* item,
                 int count);

    void removeItem(int itemId);

    double calculateTotal() const;

    void display() const;
};

#endif