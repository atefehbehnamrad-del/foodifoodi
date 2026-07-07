#include "OrderDAO.h"
#include "MenuItemDAO.h"
#include <iostream>
#include <sstream>

using namespace std;

static string getText(sqlite3_stmt* stmt, int col) {
    const char* txt = (const char*)sqlite3_column_text(stmt, col);
    return txt ? txt : "";
}

OrderDAO::OrderDAO(sqlite3* database) : db(database), menuItemDAO(database) {}

OrderDAO::~OrderDAO() {}

string OrderDAO::getCurrentTime() {
    time_t now = time(nullptr);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

int OrderDAO::insertOrder(const Order& order) {
    string sql = "INSERT INTO orders (user_id, restaurant_id, status, total, order_time) "
                 "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cout << "Insert order prepare failed: " << sqlite3_errmsg(db) << endl;
        return -1;
    }

    sqlite3_bind_int(stmt, 1, order.getUserId());
    sqlite3_bind_int(stmt, 2, order.getRestaurantId());
    sqlite3_bind_text(stmt, 3, "AWAITING", -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, order.getTotal());
    sqlite3_bind_int64(stmt, 5, static_cast<sqlite3_int64>(order.getOrderTime()));

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Insert order failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return -1;
    }

    int generatedOrderId = static_cast<int>(sqlite3_last_insert_rowid(db));
    sqlite3_finalize(stmt);

    string itemSql = "INSERT INTO order_items (order_id, menu_item_id, count, price) "
                     "VALUES (?, ?, ?, ?);";

    for (const auto& item : order.getItems()) {
        sqlite3_stmt* itemStmt = nullptr;

        if (sqlite3_prepare_v2(db, itemSql.c_str(), -1, &itemStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(itemStmt, 1, generatedOrderId);
            sqlite3_bind_int(itemStmt, 2, item.getItem()->getId());
            sqlite3_bind_int(itemStmt, 3, item.getCount());
            sqlite3_bind_double(itemStmt, 4, item.getPrice());

            if (sqlite3_step(itemStmt) != SQLITE_DONE) {
                cout << "Insert order item failed: " << sqlite3_errmsg(db) << endl;
            }
        } else {
            cout << "Prepare order item failed: " << sqlite3_errmsg(db) << endl;
        }

        sqlite3_finalize(itemStmt);
    }

    return generatedOrderId;
}

vector<Order> OrderDAO::getAllOrders() {
    vector<Order> orders;

    string sql = "SELECT id, user_id, restaurant_id, status, total, order_time "
                 "FROM orders;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            int userId = sqlite3_column_int(stmt, 1);
            int restaurantId = sqlite3_column_int(stmt, 2);

            string statusText = getText(stmt, 3);
            double total = sqlite3_column_double(stmt, 4);
            time_t orderTime = sqlite3_column_int64(stmt, 5);

            OrderStatus status = OrderStatus::AWAITING;
            if (statusText == "PREPARING") status = OrderStatus::PREPARING;
            else if (statusText == "READY_TO_SEND") status = OrderStatus::READY_TO_SEND;
            else if (statusText == "DELIVERED") status = OrderStatus::DELIVERED;

            orders.push_back(Order(id, userId, restaurantId, status, {}, total, orderTime));
        }
    } else {
        cout << "Get all orders prepare failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return orders;
}

Order* OrderDAO::getOrderById(int id) {
    string sql = "SELECT id, user_id, restaurant_id, status, total, order_time "
                 "FROM orders WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    Order* order = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int orderId = sqlite3_column_int(stmt, 0);
            int userId = sqlite3_column_int(stmt, 1);
            int restaurantId = sqlite3_column_int(stmt, 2);

            string statusText = getText(stmt, 3);
            double total = sqlite3_column_double(stmt, 4);
            time_t orderTime = sqlite3_column_int64(stmt, 5);

            OrderStatus status = OrderStatus::AWAITING;
            if (statusText == "PREPARING") status = OrderStatus::PREPARING;
            else if (statusText == "READY_TO_SEND") status = OrderStatus::READY_TO_SEND;
            else if (statusText == "DELIVERED") status = OrderStatus::DELIVERED;

            order = new Order(orderId, userId, restaurantId, status, {}, total, orderTime);
        }
    } else {
        cout << "Get order by id prepare failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return order;
}

vector<Order> OrderDAO::getOrdersByUserId(int userId) {
    vector<Order> result;
    vector<Order> all = getAllOrders();

    for (const auto& order : all) {
        if (order.getUserId() == userId) {
            result.push_back(order);
        }
    }

    return result;
}

vector<Order> OrderDAO::getOrdersByRestaurantId(int restaurantId) {
    vector<Order> result;
    vector<Order> all = getAllOrders();

    for (const auto& order : all) {
        if (order.getRestaurantId() == restaurantId) {
            result.push_back(order);
        }
    }

    return result;
}

void OrderDAO::updateOrderStatus(int orderId, OrderStatus status) {
    string statusText;

    switch (status) {
        case OrderStatus::AWAITING: statusText = "AWAITING"; break;
        case OrderStatus::PREPARING: statusText = "PREPARING"; break;
        case OrderStatus::READY_TO_SEND: statusText = "READY_TO_SEND"; break;
        case OrderStatus::DELIVERED: statusText = "DELIVERED"; break;
        case OrderStatus::CANCELLED: statusText = "CANCELLED"; break;
    }

    string sql = "UPDATE orders SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, statusText.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, orderId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Update failed: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        cout << "Update prepare failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

void OrderDAO::deleteOrder(int id) {
    string sql = "DELETE FROM orders WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << "Delete failed: " << sqlite3_errmsg(db) << endl;
        }
    } else {
        cout << "Delete prepare failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

bool OrderDAO::updateOrder(const Order& order) {
    string statusText;
    switch (order.getStatus()) {
        case OrderStatus::AWAITING: statusText = "AWAITING"; break;
        case OrderStatus::PREPARING: statusText = "PREPARING"; break;
        case OrderStatus::READY_TO_SEND: statusText = "READY_TO_SEND"; break;
        case OrderStatus::DELIVERED: statusText = "DELIVERED"; break;
        case OrderStatus::CANCELLED: statusText = "CANCELLED"; break;
    }

    string sql = "UPDATE orders SET status = ?, total = ?, final_price = ?, earned_points = ?, cancelled = ? "
                 "WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "updateOrder prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, statusText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, order.getTotal());
    sqlite3_bind_double(stmt, 3, order.getFinalPrice());
    sqlite3_bind_int(stmt, 4, order.getEarnedPoints());
    sqlite3_bind_int(stmt, 5, order.isCancelled() ? 1 : 0);
    sqlite3_bind_int(stmt, 6, order.getId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool OrderDAO::cancelOrder(int orderId) {
    string sql = "UPDATE orders SET status = 'CANCELLED', cancelled = 1 WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "cancelOrder prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_int(stmt, 1, orderId);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

vector<Order> OrderDAO::getActiveOrders() {
    vector<Order> orders;

    string sql = "SELECT id, user_id, restaurant_id, status, total, final_price, "
                 "earned_points, order_time, cancelled "
                 "FROM orders WHERE cancelled = 0 AND status != 'DELIVERED' "
                 "ORDER BY order_time DESC;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getActiveOrders prepare failed: " << sqlite3_errmsg(db) << "\n";
        return orders;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int userId = sqlite3_column_int(stmt, 1);
        int restaurantId = sqlite3_column_int(stmt, 2);
        string statusText = getText(stmt, 3);
        double total = sqlite3_column_double(stmt, 4);
        double finalPrice = sqlite3_column_double(stmt, 5);
        int earnedPoints = sqlite3_column_int(stmt, 6);
        time_t orderTime = sqlite3_column_int64(stmt, 7);
        int cancelled = sqlite3_column_int(stmt, 8);

        OrderStatus status = OrderStatus::AWAITING;
        if (statusText == "PREPARING") status = OrderStatus::PREPARING;
        else if (statusText == "READY_TO_SEND") status = OrderStatus::READY_TO_SEND;
        else if (statusText == "DELIVERED") status = OrderStatus::DELIVERED;
        else if (statusText == "CANCELLED") status = OrderStatus::CANCELLED;

        Order order(id, userId, restaurantId, status, {}, total, orderTime);
        order.setFinalPrice(finalPrice);
        order.setEarnedPoints(earnedPoints);
        order.setCancelled(cancelled == 1);
        orders.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orders;
}

double OrderDAO::getTotalRevenue() const {
    string sql = "SELECT SUM(final_price) FROM orders WHERE cancelled = 0;";

    sqlite3_stmt* stmt = nullptr;
    double total = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return total;
}

int OrderDAO::getActiveOrdersCount() const {
    string sql = "SELECT COUNT(*) FROM orders WHERE cancelled = 0 AND status != 'DELIVERED';";

    sqlite3_stmt* stmt = nullptr;
    int count = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return count;
}

double OrderDAO::getCustomerTotalSpent(int customerId) const {
    string sql = "SELECT SUM(final_price) FROM orders WHERE user_id = ? AND cancelled = 0;";

    sqlite3_stmt* stmt = nullptr;
    double total = 0;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, customerId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    return total;
}

bool OrderDAO::saveOrderItems(int orderId, const vector<OrderItem>& items) {
    string sql = "INSERT INTO order_items (order_id, item_name, quantity, price) "
                 "VALUES (?, ?, ?, ?);";

    for (const auto& item : items) {
        sqlite3_stmt* stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "saveOrderItems prepare failed: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        sqlite3_bind_int(stmt, 1, orderId);
        sqlite3_bind_text(stmt, 2, item.name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, item.quantity);
        sqlite3_bind_double(stmt, 4, item.price);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    return true;
}

vector<OrderItem> OrderDAO::getOrderItems(int orderId) {
    vector<OrderItem> items;

    string sql = "SELECT item_name, quantity, price FROM order_items WHERE order_id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "getOrderItems prepare failed: " << sqlite3_errmsg(db) << "\n";
        return items;
    }

    sqlite3_bind_int(stmt, 1, orderId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        OrderItem item;
        item.name = getText(stmt, 0);
        item.quantity = sqlite3_column_int(stmt, 1);
        item.price = sqlite3_column_double(stmt, 2);
        items.push_back(item);
    }

    sqlite3_finalize(stmt);
    return items;
}