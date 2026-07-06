#include "OrderManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

OrderManager::OrderManager() : nextIdOrder(1){}

        bool OrderManager::orderpross(Customer* customer , Order& order){
            if(customer == nullptr){
                cout << "no customer has found" << endl;
                return false;
            }
            if(order.getItem().empty()){
                cout << "order is empty" << endl;
                return false;
            }
            double finalPrice = calFinalPrice(customer , order);
            order.setFinalPrice(finalPrice);
            int earnPoints = calEarnPoints(customer , order);
            order.setEarnPoints(earnPoints);

            customer->addPoints(earnPoints);
            customer->updateLAstOrderdate();
            customer->incrementConsecutiveOrders();

            order.push_back(order);
            customerOrder[customer->getId()].push_back(order.getId());

            cout << "----- FAINAL ORDER -----" << endl;
            cout << "Item: " << order.getTotal() << endl;
            cout << "discount:" << (order.getTotal() - finalPrice + customer->calculateErsalCost()) << endl;
            cout << "sending:" << customer->calculateErsalCost() << endl;
            cout << "Final price :" << finalPrice << endl;
            cout << "Level:" << customer->getLevel()->getBadge() << customer->getLevel()->getLevelName() << endl;
            cout << "Add point:" << earnPoints << endl;
            cout << "Totall points" << customer->getPoints() << endl;
            
            return true;
        }
        bool OrderManager::orderCancel(Customer* customer , int orderId){
            if(customer == nullptr){
                cout << "no customer has found" << endl;
                return false;
            }
            for(auto& order : orders){
                if(order.getId() == orderId && order.getUserId() == customer->getId()){
                    if (order.isCancelled()) {
                    cout << "order cancelled!" << endl;
                    return false;
                }
                if (order.getStatus() != OrderStatus::AWAITING) {
                    cout << "status od order: ";
                    switch (order.getStatus()) {
                        case OrderStatus::PREPARING: cout << "Preparing"; break;
                        case OrderStatus::READY_TO_SEND: cout << "Ready to Send"; break;
                        case OrderStatus::DELIVERED: cout << "Delivered"; break;
                        default: break;
                    }
                return false;
            }

            int payingPoints = order.getEarnPoints();
            if(payingPoints > 0){
                coustomer->removePoints(payingPoints);
            }
            
        }
    } 
}
        double OrderManager::calFinalPrice(Customer* customer ,const Order& order);
        int OrderManager::calEarnPoints(Customer* customer , const Order& order);
        std::vector<Order> getCustomerOrder(int customerId) const;
        void OrderManager::showTheOrder(int customerId) const;
        Order* getOrderById(int orderId);
        int OrderManager::getactiveOrder() const;
        int OrderManager::getTotalCount() const{
            return orders.size();
        }
        double OrderManager::getTotalAllPrice() const;
        void OrderManager::updateStatus(int orderId , OrderStatus , newStatus);