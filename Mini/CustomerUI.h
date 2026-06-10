#ifndef CUSTOMERUI_H
#define CUSTOMERUI_H
 
#include "Customer.h"
#include "RestaurantDAO.h"
#include "MenuItemDAO.h"
#include "OrderDAO.h"
#include "Cart.h"
 
class CustomerUI {
public:
    CustomerUI(Customer& user, RestaurantDAO& rDAO,MenuItemDAO& mDAO, OrderDAO& oDAO);
 
    void run();
 
private:
    Customer&      user;
    RestaurantDAO& rDAO;
    MenuItemDAO&   mDAO;
    OrderDAO&      oDAO;
    Cart           cart;
 
    void showRestaurants();
    void showCart();
    void showHistory();
};
 
#endif