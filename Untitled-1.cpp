#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<utility>
#include<limits>

using namespace std;

// ==================== Forward Declarations ====================
class User;
class RestaurantManager;
class Cart;
class Order;
class ReviewManager;
void userMenu(User* user, RestaurantManager& rm, Cart& cart, vector<Order>& orders, ReviewManager& reviewManager);
void adminMenu(RestaurantManager& rm);

// ==================== Helper: safe line read ====================
// Discards any leftover newline in the buffer, then reads a full line.
static void readLine(const string& prompt, string& out) {
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, out);
}

// Use this variant when you know the buffer is already clean
// (i.e. right after getline, not after cin >>).
static void readLineNoIgnore(const string& prompt, string& out) {
    cout << prompt;
    getline(cin, out);
}

// ==================== MenuItem ====================
class MenuItem {
protected:
    int id;
    string name;
    double price;
    bool available;
    string description;
    string type;

public:
    MenuItem(int i, const string& n, double p, bool a, const string& d, const string& t)
        : id(i), name(n), price(p), available(a), description(d), type(t) {}

    virtual ~MenuItem() {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    bool getAvailability() const { return available; }

    void setDescription(string d) { description = d; }
    void setPrice(double p) {
        if (p >= 0) price = p;
        else cout << "Error: the price should be positive" << endl;
    }
    void setAvailability(bool a) { available = a; }
    string getType() const { return type; }
    void setType(const string& t) { type = t; }

    virtual void display() const {
        cout << "[" << id << "] Name: " << name << " - Price: " << price << endl
             << "Description: " << description << endl
             << "Status: " << (available ? "Available" : "Unavailable") << endl
             << "Type: " << type << endl;
    }
};

// ==================== Food ====================
class Food : public MenuItem {
private:
    int readytime;

public:
    Food(int r_time, int i, const string& n, const string& d, double p, bool a)
        : MenuItem(i, n, p, a, d, "Food"), readytime(r_time) {
        if (readytime < 0) {
            cout << "Error: Please enter the positive time" << endl;
            readytime = 0;
        }
    }

    int getReadytime() const { return readytime; }

    void setReadytime(int r) {
        if (r < 0) cout << "Error: The time is incorrect. Please enter a positive time." << endl;
        else readytime = r;
    }

    void display() const override {
        MenuItem::display();
        cout << "The time you need to wait for your food to be ready is " << readytime << " minutes." << endl;
    }
};

// ==================== Drink ====================
class Drink : public MenuItem {
private:
    int capacity;

public:
    Drink(int c, int i, const string& n, const string& d, double p, bool a)
        : MenuItem(i, n, p, a, d, "Drink"), capacity(c) {
        if (capacity < 0) {
            cout << "Error: Please enter the positive number." << endl;
            capacity = 250;
        }
    }

    int getCapacity() const { return capacity; }

    void setCapacity(int c) {
        if (c < 0) cout << "Error: The capacity is not valid. Please enter a positive number." << endl;
        else capacity = c;
    }

    void display() const override {
        MenuItem::display();
        cout << "The capacity you want for your drink is " << capacity << " mL" << endl;
    }
};

// ==================== Restaurant ====================
class Restaurant {
private:
    int id;
    string name;
    string address;
    string phone;
    bool status;
    int prepare;
    string description;
    vector<MenuItem*> menu;

public:
    Restaurant(int i, const string& n, const string& a, const string& ph, bool s, int pr, const string& d)
        : id(i), name(n), address(a), phone(ph), status(s), description(d) {
        prepare = (pr > 0) ? pr : 0;
    }

    ~Restaurant() {
        for (auto item : menu) delete item;
        menu.clear();
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    bool getStatus() const { return status; }
    string getDescription() const { return description; }
    int getAvgPrepare() const { return prepare; }

    void setStatus(bool s) { status = s; }
    void setDescription(string d) { description = d; }
    void setPrepare(int pr) {
        if (pr > 0) prepare = pr;
        else cout << "Error: the time should be positive" << endl;
    }

    void addToMenuItem(MenuItem* item) { menu.push_back(item); }

    void removeMenuItem(int ItemId) {
        for (int i = 0; i < (int)menu.size(); i++) {
            if (menu[i]->getId() == ItemId) {
                delete menu[i];
                menu.erase(menu.begin() + i);
                cout << "The item was removed from the menu." << endl;
                return;
            }
        }
        cout << "The item you chose was not found. Please try again." << endl;
    }

    void displayMenu() const {
        if (menu.empty()) {
            cout << "The menu is empty." << endl;
        } else {
            cout << "*** Menu ***" << endl;
            for (const auto& item : menu) {
                item->display();
                cout << endl;
            }
        }
    }

    MenuItem* findMenuItem(int itemId) const {
        for (const auto& item : menu) {
            if (item->getId() == itemId) return item;
        }
        return nullptr;
    }

    void display() const {
        cout << "Restaurant: " << name << " (ID: " << id << ")" << endl
             << "Address: " << address << endl
             << "Phone: " << phone << endl
             << "Status: " << (status ? "Open" : "Closed") << endl
             << "Description: " << description << endl
             << "Prepare time: " << prepare << " minutes" << endl;
    }
};

// ==================== User ====================
class User {
private:
    int id;
    string role;
    string username;
    string password;

public:
    User(int i, string r, string u, string p)
        : id(i), role(r), username(u), password(p) {}

    int getId() const { return id; }
    string getRole() const { return role; }
    string getUsername() const { return username; }

    bool login(string p) const { return password == p; }

    void display() const {
        cout << "Id: " << id << endl;
        cout << "Role: " << role << endl;
        cout << "Username: " << username << endl;
    }
};

// ==================== OrderItem ====================
class OrderItem {
private:
    MenuItem* item;
    double price;
    int count;

public:
    OrderItem(MenuItem* i, int c)
        : item(i), count(c), price(i->getPrice()) {}

    double getTotal() const { return count * price; }
    MenuItem* getItem() const { return item; }
    int getCount() const { return count; }
    double getPrice() const { return price; }

    void Display() const {
        cout << "Item: " << item->getName() << endl;
        cout << "Count: " << count << endl;
        cout << "Total: " << getTotal() << "$" << endl;
    }
};

// ==================== Order ====================
class Order {
private:
    int id;
    int iduser;
    int idrestaurant;
    string status;
    vector<OrderItem> items;
    double total;
    time_t ordertime;

public:
    Order(int id, int iduser, int idrestaurant)
        : id(id), iduser(iduser), idrestaurant(idrestaurant),
          status("Pending"), total(0), ordertime(time(NULL)) {}

    int getId() const { return id; }
    int getIduser() const { return iduser; }
    int getIdrestaurant() const { return idrestaurant; }
    string getStatus() const { return status; }

    void setStatus(string newS) { status = newS; }

    void addItem(MenuItem* item, int count) {
        if (!item) { cout << "Invalid item!" << endl; return; }
        if (!item->getAvailability()) { cout << "This item is not available." << endl; return; }
        items.push_back(OrderItem(item, count));
        total = calculateTotal();
    }

    void removeItem(int itemId) {
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].getItem()->getId() == itemId) {
                items.erase(items.begin() + i);
                total = calculateTotal();
                cout << "The item was removed." << endl;
                return;
            }
        }
        cout << "The item was not found." << endl;
    }

    double calculateTotal() {
        double t = 0;
        for (int i = 0; i < (int)items.size(); i++) t += items[i].getTotal();
        return t;
    }

    void display() const {
        cout << "*****ORDER*****" << endl;
        cout << "ID: " << id << endl;
        cout << "Customer ID: " << iduser << endl;
        cout << "Restaurant ID: " << idrestaurant << endl;
        cout << "Status: " << status << endl;
        cout << "Order time: " << ctime(&ordertime);
        cout << "Items:" << endl;
        for (int i = 0; i < (int)items.size(); i++) {
            cout << "  - ";
            items[i].Display();
        }
        cout << "Total: " << total << "$" << endl;
        cout << "**********" << endl;
    }
};

// ==================== Cart ====================
class Cart {
private:
    vector<pair<MenuItem*, int>> items;

public:
    void addItem(MenuItem* item, int count) {
        if (count <= 0) {
            cout << "Count should be positive." << endl;
            return;
        }
        if (!item->getAvailability()) {
            cout << "The item is not available." << endl;
            return;
        }
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].first->getId() == item->getId()) {
                items[i].second += count;
                cout << item->getName() << ": " << count << " more added." << endl;
                return;
            }
        }
        items.push_back({item, count});
        cout << item->getName() << " added. Count: " << count << endl;
    }

    void removeItem(int itemId) {
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].first->getId() == itemId) {
                items.erase(items.begin() + i);
                cout << "Item deleted from cart." << endl;
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    void update(int itemId, int newCount) {
        if (newCount <= 0) { removeItem(itemId); return; }
        for (int i = 0; i < (int)items.size(); i++) {
            if (items[i].first->getId() == itemId) {
                items[i].second = newCount;
                cout << "New count: " << newCount << endl;
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items) total += item.first->getPrice() * item.second;
        return total;
    }

    void display() const {
        if (items.empty()) { cout << "Your cart is empty." << endl; return; }
        cout << "***** Cart *****" << endl;
        for (int i = 0; i < (int)items.size(); i++) {
            cout << "   " << items[i].second << " * " << items[i].first->getName()
                 << " = " << items[i].first->getPrice() * items[i].second << "$" << endl;
        }
        cout << "Total: " << getTotal() << "$" << endl;
    }

    void checkout(int userId, int restaurantId, vector<Order>& orders) {
        if (items.empty()) {
            cout << "The cart is empty!" << endl;
            return;
        }
        Order newOrder((int)orders.size() + 1, userId, restaurantId);
        for (const auto& item : items) {
            newOrder.addItem(item.first, item.second);
        }
        orders.push_back(newOrder);
        items.clear();
        cout << "Your new order ID is " << newOrder.getId() << endl;
    }
};

// ==================== UserManager ====================
class UserManager {
private:
    vector<User> users;
    User* currentUser;

public:
    UserManager() : currentUser(nullptr) {}

    void registerUser() {
        int id = (int)users.size() + 1;
        string username, password, role;
        readLine("Enter username: ", username);
        readLineNoIgnore("Enter password: ", password);
        readLineNoIgnore("Enter role (admin/user): ", role);
        users.push_back(User(id, role, username, password));
        cout << "Registered! Your ID: " << id << endl;
    }

    bool login() {
        string username, password;
        readLine("Username: ", username);
        readLineNoIgnore("Password: ", password);
        for (int i = 0; i < (int)users.size(); i++) {
            if (users[i].getUsername() == username && users[i].login(password)) {
                currentUser = &users[i];
                cout << "Welcome, " << username << "!" << endl;
                return true;
            }
        }
        cout << "Error: Invalid username or password." << endl;
        return false;
    }

    User* getCurrentUser() { return currentUser; }
    void logout() { currentUser = nullptr; }
};

// ==================== RestaurantManager ====================
class RestaurantManager {
private:
    vector<Restaurant> restaurants;

public:
    void addRestaurant() {
        int id = (int)restaurants.size() + 1;
        string name, address, phone, description;
        int prepareTime;
        bool status;

        readLine("Restaurant name: ", name);
        readLineNoIgnore("Address: ", address);
        readLineNoIgnore("Phone: ", phone);

        cout << "Status (1=open, 0=closed): "; cin >> status;
        cout << "Prepare time: ";               cin >> prepareTime;

        readLine("Description: ", description);

        restaurants.push_back(Restaurant(id, name, address, phone, status, prepareTime, description));
        cout << "Restaurant added successfully!" << endl;
    }

    void displayAllRestaurants() {
        if (restaurants.empty()) { cout << "No restaurants yet." << endl; return; }
        for (auto& r : restaurants) { r.display(); cout << "**********" << endl; }
    }

    Restaurant* findRestaurant(int id) {
        for (auto& r : restaurants) {
            if (r.getId() == id) return &r;
        }
        return nullptr;
    }
};

// ==================== Review ====================
class Review {
private:
    int userId, restaurantId, rating;
    string comment;
    time_t reviewTime;

public:
    Review(int u, int r, int re, string c)
        : userId(u), restaurantId(r), rating(re), comment(c) {
        reviewTime = time(NULL);
    }

    int getRestaurantId() const { return restaurantId; }

    void display() {
        cout << "Comment: " << comment << endl;
        cout << "Rating: " << rating << "/5" << endl;
        cout << "Date: " << ctime(&reviewTime);
    }
};

// ==================== ReviewManager ====================
class ReviewManager {
private:
    vector<Review> reviews;

public:
    void addReview(int userId, int restaurantId) {
        int rating;
        string comment;
        cout << "Rating (1-5): "; cin >> rating;
        readLine("Your comment: ", comment);
        reviews.push_back(Review(userId, restaurantId, rating, comment));
        cout << "Thank you for your review!" << endl;
    }

    void showRestaurantReviews(int restaurantId) {
        cout << "***** Reviews *****" << endl;
        bool found = false;
        for (auto& r : reviews) {
            if (r.getRestaurantId() == restaurantId) {
                r.display(); cout << "---" << endl; found = true;
            }
        }
        if (!found) cout << "No reviews yet." << endl;
    }
};

// ==================== Payment ====================
class Payment {
private:
    int orderId;
    double amount;
    string method;
    bool status;
    time_t paymentTime;

public:
    Payment(int o, double a, string m)
        : orderId(o), amount(a), method(m), status(false) {
        paymentTime = time(NULL);
    }

    bool processPayment() {
        cout << "Payment method: " << method << ". Amount: " << amount << "$" << endl;
        status = true;
        cout << "Payment successful!" << endl;
        return status;
    }

    void display() {
        cout << "Order ID: " << orderId << endl;
        cout << "Amount: " << amount << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << (status ? "Paid" : "Pending") << endl;
    }
};

// ==================== userMenu ====================
void userMenu(User* user, RestaurantManager& rm, Cart& cart, vector<Order>& orders, ReviewManager& reviewManager) {
    int choice;
    do {
        cout << "\n***** USER MENU *****" << endl;
        cout << "1. Browse Restaurants" << endl;
        cout << "2. View Restaurant Menu" << endl;
        cout << "3. Add to Cart" << endl;
        cout << "4. View Cart" << endl;
        cout << "5. Checkout" << endl;
        cout << "6. My Orders" << endl;
        cout << "7. Add Review" << endl;
        cout << "0. Logout" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            rm.displayAllRestaurants();
        } else if (choice == 2) {
            int restaurantId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (r) r->displayMenu();
            else cout << "Restaurant not found!" << endl;
        } else if (choice == 3) {
            int restaurantId, itemId, count;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (r) {
                r->displayMenu();
                cout << "Item ID: "; cin >> itemId;
                cout << "Count: ";   cin >> count;
                MenuItem* item = r->findMenuItem(itemId);
                if (item) cart.addItem(item, count);
                else cout << "Item not found!" << endl;
            } else cout << "Restaurant not found!" << endl;
        } else if (choice == 4) {
            cart.display();
        } else if (choice == 5) {
            if (cart.getTotal() > 0) {
                int restaurantId;
                cout << "Restaurant ID: "; cin >> restaurantId;
                cart.checkout(user->getId(), restaurantId, orders);
                string method;
                readLine("How will you pay? (cash/card): ", method);
                Payment payment((int)orders.size(), orders.back().calculateTotal(), method);
                payment.processPayment();
            } else cout << "Your cart is empty!" << endl;
        } else if (choice == 6) {
            for (auto& order : orders) order.display();
        } else if (choice == 7) {
            int restaurantId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            reviewManager.addReview(user->getId(), restaurantId);
        } else if (choice == 0) {
            cout << "Logged out." << endl;
        } else {
            cout << "Error: choose a correct number." << endl;
        }
    } while (choice != 0);
}

// ==================== adminMenu ====================
void adminMenu(RestaurantManager& rm) {
    int choice;
    do {
        cout << "\n***** ADMIN MENU *****" << endl;
        cout << "1. Add Restaurant" << endl;
        cout << "2. View All Restaurants" << endl;
        cout << "3. Add Food Item to Restaurant" << endl;
        cout << "4. Add Drink Item to Restaurant" << endl;
        cout << "5. Remove Menu Item from Restaurant" << endl;
        cout << "6. View Restaurant Menu" << endl;
        cout << "0. Logout" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            rm.addRestaurant();

        } else if (choice == 2) {
            rm.displayAllRestaurants();

        } else if (choice == 3) {
            int restaurantId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (!r) { cout << "Restaurant not found!" << endl; continue; }

            string name, description;
            double price;
            int readyTime;
            bool available;

            readLine("Food name: ", name);
            readLineNoIgnore("Description: ", description);
            cout << "Price: ";              cin >> price;
            cout << "Ready time (minutes): "; cin >> readyTime;
            cout << "Available? (1=yes, 0=no): "; cin >> available;

            static int foodCounter = 1;
            r->addToMenuItem(new Food(readyTime, foodCounter++, name, description, price, available));
            cout << "Food item added to menu!" << endl;

        } else if (choice == 4) {
            int restaurantId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (!r) { cout << "Restaurant not found!" << endl; continue; }

            string name, description;
            double price;
            int capacity;
            bool available;

            readLine("Drink name: ", name);
            readLineNoIgnore("Description: ", description);
            cout << "Price: ";             cin >> price;
            cout << "Capacity (mL): ";     cin >> capacity;
            cout << "Available? (1=yes, 0=no): "; cin >> available;

            static int drinkCounter = 1001;
            r->addToMenuItem(new Drink(capacity, drinkCounter++, name, description, price, available));
            cout << "Drink item added to menu!" << endl;

        } else if (choice == 5) {
            int restaurantId, itemId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (!r) { cout << "Restaurant not found!" << endl; continue; }
            r->displayMenu();
            cout << "Item ID to remove: "; cin >> itemId;
            r->removeMenuItem(itemId);

        } else if (choice == 6) {
            int restaurantId;
            cout << "Restaurant ID: "; cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if (r) r->displayMenu();
            else cout << "Restaurant not found!" << endl;

        } else if (choice == 0) {
            cout << "Admin logged out." << endl;
        } else {
            cout << "Error: choose a correct number." << endl;
        }
    } while (choice != 0);
}

// ==================== mainMenu ====================
void mainMenu() {
    RestaurantManager restaurantManager;
    UserManager userManager;
    Cart cart;
    vector<Order> orders;
    ReviewManager reviewManager;

    int choice;
    do {
        cout << "\n***** FOOD DELIVERY SYSTEM *****" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. View Restaurants" << endl;
        cout << "4. View Cart" << endl;
        cout << "5. My Orders" << endl;
        cout << "6. Add Review" << endl;
        cout << "7. Admin" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            userManager.registerUser();
        } else if (choice == 2) {
            if (userManager.login()) {
                User* currentUser = userManager.getCurrentUser();
                if (currentUser->getRole() == "admin") {
                    cout << "Admin access granted." << endl;
                    adminMenu(restaurantManager);
                } else {
                    userMenu(currentUser, restaurantManager, cart, orders, reviewManager);
                }
            }
        } else if (choice == 3) {
            restaurantManager.displayAllRestaurants();
        } else if (choice == 4) {
            cart.display();
        } else if (choice == 5) {
            for (auto& order : orders) order.display();
        } else if (choice == 6) {
            if (userManager.getCurrentUser()) {
                int restaurantId;
                cout << "Restaurant ID: "; cin >> restaurantId;
                reviewManager.addReview(userManager.getCurrentUser()->getId(), restaurantId);
            } else cout << "Please login first." << endl;
        } else if (choice == 7) {
            if (userManager.getCurrentUser() && userManager.getCurrentUser()->getRole() == "admin") {
                adminMenu(restaurantManager);
            } else cout << "Admin access required. Please login as admin." << endl;
        } else if (choice == 0) {
            cout << "Goodbye!" << endl;
        } else {
            cout << "Error: choose a correct number." << endl;
        }
    } while (choice != 0);
}

// ==================== main ====================
int main() {
    mainMenu();
    return 0;
}
1
