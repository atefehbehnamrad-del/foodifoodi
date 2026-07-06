#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include "Order.h"
#include "Customer.h"
#include <vector>
#include <map>

class OrderManager{
    private:
        std::vector<Order> orders;
        int nextIdOrder;
        std::map<int , std::vector<int>> customerOrder;

    public:
        OrderManager();
        bool orderpross(Customer* customer , Order& order);
        bool orderCancel(Customer* customer , int orderId);
        double calFinalPrice(Customer* customer ,const Order& order);
        int calEarnPoints(Customer* customer , const Order& order);
        std::vector<Order> getCustomerOrder(int customerId) const;
        void showTheOrder(int customerId) const;
        Order* getOrderById(int orderId);
        int getactiveOrder() const;
        int getTotalCount() const{
            return orders.size();
        }
        double getTotalAllPrice() const;
        void updateStatus(int orderId , OrderStatus , newStatus);
};
#endif