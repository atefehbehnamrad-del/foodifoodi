#include "OrderDAO.h"
#include "MenuItemDAO.h"
#include <iostream>

using namespace std;

OrderDAO::OrderDAO(sqlite3* database)
    : db(database),
      menuItemDAO(database){}

void OrderDAO::insertOrder(const Order& order) {

    string status;

    switch(order.getStatus()){
        case OrderStatus::AWAITING:
            status = "AWAITING";
            break;
        case OrderStatus::PREPARING:
            status = "PREPARING";
            break;
        case OrderStatus::READY_TO_SEND:
            status = "READY_TO_SEND";
            break;
        case OrderStatus::DELIVERED:
            status = "DELIVERED";
            break;
    }

    string sql =
    "INSERT INTO orders "
    "(id,user_id,restaurant_id,status,total,order_time) "
    "VALUES(?,?,?,?,?,?);";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){

        sqlite3_bind_int(stmt,1,order.getId());
        sqlite3_bind_int(stmt,2,order.getUserId());
        sqlite3_bind_int(stmt,3,order.getRestaurantId());

        sqlite3_bind_text(
            stmt,
            4,
            status.c_str(),
            -1,
            SQLITE_TRANSIENT);

        sqlite3_bind_double(stmt,5,order.getTotal());

        sqlite3_bind_int64(
            stmt,
            6,
            static_cast<sqlite3_int64>(order.getOrderTime()));

        if(sqlite3_step(stmt)!=SQLITE_DONE){
            cout << "Insert order failed: "
                 << sqlite3_errmsg(db)
                 << endl;
        }
    }

    sqlite3_finalize(stmt);

    // ذخیره آیتم‌های سفارش
    for(const auto& item : order.getItems()){

        string itemSql =
        "INSERT INTO order_items "
        "(order_id, menu_item_id, count, price) "
        "VALUES (?, ?, ?, ?);";

        sqlite3_stmt* itemStmt = nullptr;

        if(sqlite3_prepare_v2(db,itemSql.c_str(),-1,&itemStmt,nullptr) == SQLITE_OK){
            sqlite3_bind_int(itemStmt,1,order.getId());
            sqlite3_bind_int(itemStmt,2,item.getItem()->getId());
            sqlite3_bind_int(itemStmt,3,item.getCount());
            sqlite3_bind_double(itemStmt,4,item.getPrice());

            if(sqlite3_step(itemStmt)!=SQLITE_DONE){
                cout << "Insert order item failed: " << sqlite3_errmsg(db) << endl;
            }
        }

        sqlite3_finalize(itemStmt);
    }
}
vector<Order> OrderDAO::getAllOrders() {

    vector<Order> orders;

    string sql =
    "SELECT id,user_id,restaurant_id,status,total,order_time "
    "FROM orders;";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(db,sql.c_str(),
       -1,&stmt,nullptr)==SQLITE_OK){

        while(sqlite3_step(stmt)==SQLITE_ROW){

            int id = sqlite3_column_int(stmt,0);

            int userId = sqlite3_column_int(stmt,1);

            int restaurantId = sqlite3_column_int(stmt,2);

            string statusText =
            (const char*)sqlite3_column_text(stmt,3);

            double total =
            sqlite3_column_double(stmt,4);

            time_t orderTime =
            sqlite3_column_int64(stmt,5);

            OrderStatus status =
            OrderStatus::AWAITING;

            if(statusText=="PREPARING")
                status = OrderStatus::PREPARING;

            else if(statusText=="READY_TO_SEND")
                status = OrderStatus::READY_TO_SEND;

            else if(statusText=="DELIVERED")
                status = OrderStatus::DELIVERED;

            orders.push_back(Order(id, userId,restaurantId,status,{},total,orderTime));
        }
    }

    sqlite3_finalize(stmt);

    return orders;
}

Order* OrderDAO::getOrderById(int id) {

    string sql =
    "SELECT id,user_id,restaurant_id,status,total,order_time "
    "FROM orders WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    Order* order = nullptr;

    if(sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){

        sqlite3_bind_int(stmt,1,id);

        if(sqlite3_step(stmt)==SQLITE_ROW){

            int orderId =
            sqlite3_column_int(stmt,0);

            int userId =
            sqlite3_column_int(stmt,1);

            int restaurantId =
            sqlite3_column_int(stmt,2);

            string statusText =
            (const char*)sqlite3_column_text(stmt,3);

            double total =
            sqlite3_column_double(stmt,4);

            time_t orderTime =
            sqlite3_column_int64(stmt,5);

            OrderStatus status =
            OrderStatus::AWAITING;

            if(statusText=="PREPARING")
                status = OrderStatus::PREPARING;

            else if(statusText=="READY_TO_SEND")
                status = OrderStatus::READY_TO_SEND;

            else if(statusText=="DELIVERED")
                status = OrderStatus::DELIVERED;

            order = new Order(orderId,userId,restaurantId,status,{},total,orderTime);
        }
    }

    sqlite3_finalize(stmt);

    return order;
}

vector<Order> OrderDAO::getOrdersByUserId(int userId) {
    vector<Order> result;
    vector<Order> all = getAllOrders();
    for(const auto& order : all){
        if(order.getUserId()==userId){
            result.push_back(order);
        }
    }
    return result;
}

vector<Order> OrderDAO::getOrdersByRestaurantId(int restaurantId) {
    vector<Order> result;
    vector<Order> all = getAllOrders();
    for(const auto& order : all){
        if(order.getRestaurantId()==restaurantId){
            result.push_back(order);
        }
    }

    return result;
}

void OrderDAO::updateOrderStatus(int orderId, OrderStatus status) {
    string statusText;
    switch(status){
        case OrderStatus::AWAITING:
            statusText="AWAITING";
            break;

        case OrderStatus::PREPARING:
            statusText="PREPARING";
            break;

        case OrderStatus::READY_TO_SEND:
            statusText="READY_TO_SEND";
            break;

        case OrderStatus::DELIVERED:
            statusText="DELIVERED";
            break;
    }

    string sql = "UPDATE orders SET status=? WHERE id=?;";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){
        sqlite3_bind_text(stmt,1,statusText.c_str(),-1,SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt,2,orderId);
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            cout<<"Update failed.\n";
        }
    }

    sqlite3_finalize(stmt);
}

void OrderDAO::deleteOrder(int id) {
    string sql ="DELETE FROM orders WHERE id=?;";
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare_v2(db,sql.c_str(),-1,&stmt,nullptr)==SQLITE_OK){

        sqlite3_bind_int(stmt,1,id);

        if(sqlite3_step(stmt)!=SQLITE_DONE){
            cout<<"Delete failed." << endl;
        }
    }

    sqlite3_finalize(stmt);
}

