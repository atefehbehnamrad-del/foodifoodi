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
                    if (order.isCancled()) {
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
            order.setCancle(true);
            order.setStatus(orderStatus::CANCELLED);
            cout << "new point" << payingPoints << endl;
            cout << "your order cancled" << endl;
            return true;
        }
    } 
    cout << "the order didnt find" << endl;
    return false;
}
        double OrderManager::calFinalPrice(Customer* customer ,const Order& order){
            if(customer == nullptr){
                return order.getTotal();
            }
            double firstPrise = order.getTotal();
            double discount = customer->calculatediscount(firstPrice);
            int ersal = customer->calculateErsalCost();
            return firstPrice - discount + ersal;
        }
        int OrderManager::calEarnPoints(Customer* customer , const Order& order){
            if(customer == nullptr){
                return customer->calEarnPoints(order.getTotal());
            }
        }
        vector<Order> OrderManager::getCustomerOrder(int customerId) const{
            vector<Order> couse;
            auto it = coustomerOrder.find(coustomerId);
            if(it != customerOrder.end()){
                for(int orderId : it->second){
                    for(const auto& order : orders){
                        if(order.getId() == orderId){
                            couse.push_back(order);
                            break;
                        }
                    }
                }
            }
            return couse;
        }
        void OrderManager::showTheOrder(int customerId) const{
            auto customerOrderList = getCustomerOrder(customerId);
            if(customerOrderList.empty()){
                cout << "there is no order" << endl;
                return;
            }
            cout << "----- History order -----" << endl;
            for(const auto& order : customerOrderList){
                order.display();
            }
        }
        Order* OrderManager::getOrderById(int orderId){
            for(auto& order : orders){
                if(order.getId() == orderId){
                    return &order;
                }
            }
            return nullptr;
        }
        int OrderManager::getactiveOrder() const{
            int number = 0;
            for(const auto& order : orders){
                if(order.getStatus != OrderStatus::DELIVERD && !order.isCancled()){
                    number++;
                }
            }
            return number;
        }
        double OrderManager::getTotalAllPrice() const{
            double total = 0;
            for(const auto& order : orders){
                if(!order.isCancled){
                    total += order.getFinalPrice();
                }
            }
            return total;
        }
        void OrderManager::updateStatus(int orderId , OrderStatus , newStatus){
            Order* order = getOrderById(orderId);
            if(order == nullptr){
                cout << "there is no order" << endl;
                return;
            }
            if(order->isCancled()){
                cout << "the order cancled" << endl;
                return;
            }
            order->setstatus(newStatus);
            cout << "order id" << orderId << "status updated to :";
            if (newStatus == OrderStatus::AWAITING) {
                cout << "Awaiting";
            } else if (newStatus == OrderStatus::PREPARING) {
                    cout << "Preparing";
            } else if (newStatus == OrderStatus::READY_TO_SEND) {
                    cout << "Ready to Send";
            } else if (newStatus == OrderStatus::DELIVERED) {
                    cout << "Delivered";
            } else if (newStatus == OrderStatus::CANCELLED) {
                    cout << "Cancelled";
            } else {
                    cout << "Unknown";
            }
            cout << endl;
        }