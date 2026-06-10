#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include <utility>

using namespace std;

// ==================== کلاس MenuItem ====================
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
        if(p >= 0) { 
            price = p;
        } else {
            cout << "Error: the price should be positive" << endl;
        }
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

// ==================== کلاس Food ====================
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

    int getReadytime() const {
        return readytime;
    }

    void setReadytime(int r) {
        if (r < 0) { 
            cout << "Error: The time is incorrect. Please enter a positive time." << endl;
        } else {
            readytime = r;
        }
    }

    void display() const override {
        MenuItem::display(); 
        cout << "The time you need to wait for your food to be ready is " << readytime << " minutes." << endl;
    }
};

// ==================== کلاس Drink ====================
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
    
    int getCapacity() const {
        return capacity;
    }
    
    void setCapacity(int c) {
        if (c < 0) {
            cout << "Error: The capacity is not true. Please enter the positive number." << endl; 
        } else {
            capacity = c;
        }
    }
    
    void display() const override {
        MenuItem::display();
        cout << "The capacity you want for your drink is " << capacity << " mL" << endl;
    } 
};

// ==================== کلاس Restaurant ====================
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
        for (auto item : menu) {
            delete item;
        }
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
        if (pr > 0) {
            prepare = pr;
        } else {
            cout << "Error: the time should be positive" << endl;
        }
    };

    void addToMenuItem(MenuItem* item) {
        menu.push_back(item);
    };

    void removeMenuItem(int ItemId) {
        for (int i = 0; i < menu.size(); i++) {
            if (menu[i]->getId() == ItemId) {
                delete menu[i];
                menu.erase(menu.begin() + i);
                cout << "The item removed from the menu" << endl;
                return;
            }
        }
        cout << "The item you choose was not found. Please try again." << endl;
    };

    void displayMenu() const {
        if (menu.empty()) {
            cout << "The menu is empty." << endl;
        } else {
            cout << "*Menu you choose*" << endl;
            for (const auto& item : menu) {
                item->display();
                cout << endl;
            }
        }
    };

    MenuItem* findMenuItem(int itemId) const {
        for (const auto& item : menu) {
            if (item->getId() == itemId) {
                return item;
            }
        }
        return nullptr;
    };

    void display() const {
        cout << "Restaurant: " << name << " (ID: " << id << ")" << endl
             << "Address: " << address << endl
             << "Phone: " << phone << endl
             << "Status: " << status  << endl
             << "Description: " << description << endl
             << "Prepare time: " << prepare << " minutes" << endl;
    }
};

// ==================== کلاس User ====================
class User{
private:
    int Id;
    string Role;
    string Username;
    string Password;

public:
    User(int id, string role, string username, string password)
        : Id(id), Role(role), Username(username), Password(password) {}
    
    int getId() const { return Id; }
    string getRole() const { return Role; }
    string getUsername() const { return Username; }
    
    bool login(string password) const {
        return Password == password;
    }
    
    void display() const {
        cout << "Id:" << Id << endl;
        cout << "Role:" << Role << endl;
        cout << "Username:" << Username << endl;
    }
};

// ==================== کلاس OrderItem ====================
class OrderItem{
private:
    MenuItem* item;
    double Price;
    int Count;
    
public:
    OrderItem(MenuItem* i, int c)
        : item(i), Count(c) {
        Price = item->getPrice();
    }

    double getTotal() const {
        return Count * Price;
    }

    MenuItem* getItem() const { return item; }
    int getCount() const { return Count; }

    void Display() const {
        cout << "Item: " << item->getName() << " - Count: " << Count 
             << " - Total: " << getTotal() << endl;
    } 
};

// ==================== کلاس Order ====================
class Order{
private:
    int Id;
    int Iduser;
    int Idrestaurant;
    string status;
    vector<OrderItem> items;
    double total;
    time_t ordertime;

public:
    Order(int id, int userid, int resid)
        : Id(id), Iduser(userid), Idrestaurant(resid) {
        status = "Nothing";
        total = 0;
        ordertime = time(NULL);
    }

    int getId() const { return Id; }
    int getIduser() const { return Iduser; }
    int getIdrestaurant() const { return Idrestaurant; }
    string getStatus() const { return status; }
    
    void setStatus(string newS) {
        status = newS;
    }
    
    void addItem(MenuItem* item, int Count) {
        if(!item->getAvailability()){
            cout << "This item is not available" << endl;
            return;
        }
        items.push_back(OrderItem(item, Count));
        total = calculateTotal();
    }
    
    void removeItem(int itemId) {
        for (int i = 0; i < items.size(); i++) {
            if (items[i].getItem()->getId() == itemId) {
                items.erase(items.begin() + i);
                total = calculateTotal();
                cout << "The item you chose was removed." << endl;
                return;
            }
        }
        cout << "The item was not found." << endl;
    }
    
    double calculateTotal() {
        double totalPrice = 0;
        for(int i = 0; i < items.size(); i++){
            totalPrice += items[i].getTotal();
        }
        return totalPrice;
    }

    void display() const {
        cout << "*****ORDER*****" << endl;
        cout << "ID:" << Id << endl;
        cout << "ID customer:" << Iduser << endl;
        cout << "ID Restaurant:" << Idrestaurant << endl;
        cout << "Status:" << status << endl;
        cout << "Order time: " << ctime(&ordertime);
        cout << "Items:" << endl;
        for (int i = 0; i < items.size(); i++) {
            cout << "  - ";
            items[i].Display();
        }
        cout << "Total: " << total << endl;
        cout << "**********" << endl;
    }
};


class Cart{
    private:
    vector<pair<MenuItem*, int>> items;
    
    public:
    void addItem(MenuItem* Item, int Count){
        if(Count <= 0){
            cout << "Count should be positive" << endl;
            return;
        }
        if(!Item->getAvailability()){
            cout << "The item is not available. Choose again." << endl;
            return;
        }
        for(int i = 0 ; i < items.size() ; i++){
            if(items[i].first->getId() == Item->getId()){
                items[i].second += Count;
                cout << Item->getName() << " added to your order. " << Count << " more item(s) added." << endl;
                return;
            }
        }
        items.push_back({Item, Count});
        cout << Item->getName() << " added to your order. Count: " << Count << endl;
    }

    void removeItem(int ItemId){
        for(int i = 0 ; i < items.size() ; i++){
            if(items[i].first->getId() == ItemId){
                items.erase(items.begin() + i);
                cout << "This item is deleted from the cart." << endl;
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    void update(int ItemId, int newCount){
        if(newCount <= 0){
            removeItem(ItemId);
            return;
        }
        for (int i = 0; i < items.size(); i++) {
            if (items[i].first->getId() == ItemId) {
                items[i].second = newCount;
                cout << "The new count of your order is " << newCount << endl;
                return;
            }
        }
        cout << "Item not found." << endl;
    }

    double getTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.first->getPrice() * item.second;
        }
        return total;
    }

    void display() const {
        if (items.empty()) {
            cout << "Your cart is empty." << endl;
            return;
        }
        
        cout << "*****🛒 Basket *****" << endl;
        for (int i = 0; i < items.size(); i++){
            cout << "   " << items[i].second << " * " << items[i].first->getName()
                 << " = " << items[i].first->getPrice() * items[i].second << "$" << endl;
        }
        cout << "Total price: " << getTotal() << "$" << endl;
    }

    void checkout(int userId, int restaurantId, vector<Order>& orders) {
        if (items.empty()) {
            cout << "The cart is empty!" << endl;
            return;
        }
        
        Order newOrder(orders.size() + 1, userId, restaurantId);
        for (const auto& item : items) {
            newOrder.addItem(item.first, item.second);
        }
        
        orders.push_back(newOrder);
        items.clear();
        cout << "Your new order ID is " << newOrder.getId() << endl;
    }
};

class UserManager {
private:
    vector<User> users;
    User* currentUser;
    
public:
    UserManager() : currentUser(nullptr) {}
    
    void registerUser() {
        int id = users.size() + 1;
        string username ;
        string password;
        string role;
        cout << "Enter username: " << endl;
        cin >> username >> endl;
        cout << "Enter password: " << endl;
        cin >> password >> endl;
        cout << "Enter role: " << endl;
        cin >> role >> endl;
        users.push_back(User(id, role, username, password));
        cout << "Horaa;) you registed." << id << endl;
    }
    
    bool login() {
        string username;
        string password;
        cout << "Username: " << endl;
        cin >> username >> endl;
        cout << "Password: " << endl;
        cin >> password >> endl;
        
        for(int i = 0; i < users.size(); i++) {
            if(users[i].getUsername() == username && users[i].login(password)) {
                PeresentUser = &users[i];
                cout << "Welcome:) " << username << endl;
                return true;
             }
        }
        cout << "Erorr" << endl;
        return false;
    }
    
    User* getPeresentUser() { return PeresentUser; }
    void logout() { PeresentUser = nullptr; }
};

class RestaurantManager {
private:
    vector<Restaurant> restaurants;
    
public:
    void addRestaurant() {
        int id = restaurants.size() + 1;
        string name;
        string address;
        string phone;
        string description;
        int prepareTime;
        bool status;
        cout << "Restaurant name: " << endl;
        cin >> name >> endl;
        cout << "Address: " << endl;
        cin >> address >> endl;
        cout << "Phone: " << endl;
        cin << phone << endl;
        cout << "Status " << endl;
        cin >> status >> endl;
        cout << "Prepare time : " << endl;
        cin >> prepareTime >> endl;
        cout << "Description: " << endl;
        cin >> description >> endl;
        restaurants.push_back(Restaurant(id, name, address, phone, status, prepareTime, description));
        cout << "Hora your restaurant adds succssesfully:)" << endl;
    }
    
    void displayAllRestaurants() {
        for(auto& r : restaurants) {
            r.display();
            cout << "**********" << endl;
        }
    }
    
    Restaurant* findRestaurant(int id) {
        for(auto& r : restaurants) {
            if(r.getId() == id){
                return &r;
        }
        return nullptr;
    }
};


class Review {
private:
    int userId;
    int restaurantId;
    int rating;
    string comment;
    time_t reviewTime;
    
public:
    Review(int u, int r, int re, string c) 
        : userId(u), restaurantId(r), rating(re), comment(c) {
        reviewTime = time(NULL);
    }
    
    void display() {
        cout << "Comment: " << comment << endl;
        cout << "Rating: " << rating << "/10" << endl;
        cout << "Date: " << ctime(&reviewTime);
    }
};

class ReviewManager {
private:
    vector<Review> reviews;
    
public:
    void addReview(int userId, int restaurantId) {
        int rating;
        string comment;
        cout << "Rating (1-5): "; cin >> rating;
        cout << "Your comment: "; cin.ignore(); getline(cin, comment);
        
        reviews.push_back(Review(userId, restaurantId, rating, comment));
        cout << "Thank you for your review:)" << endl;
    }
    
    void showRestaurantReviews(int restaurantId) {
        cout << "***** Reviews *****" << endl;
        for(auto& review : reviews) {
            review.display();
            cout << "**********" << endl;
        }
    }
};

class Payment {
private:
    int orderId;
    double amount;
    string method; 
    bool status;
    time_t paymentTime;
    
public:
    Payment(int o, double a, string m) 
        : orderId(o), amount(a), method(m) {
        status = false;
        paymentTime = time(NULL);
    }
    
    bool processPayment() {
        cout << "Method of paying is " << method << "amount of money you should pay is" << amount << endl;
        status = true;
        cout << "Payment successful:)" << endl;
        return status;
    }
    
    void display() {
        cout << "Order ID: " << orderId << endl;
        cout << "Amount: " << amount << endl;
        cout << "Method: " << method << endl;
        cout << "Status: " << status << endl;
    }
};

void mainMenu() {
    RestaurantManager restaurantManager;
    UserManager userManager;
    Cart cart;
    vector<Order> orders;
    ReviewManager reviewManager;
    
    int choice;
    do {
        cout << "***** FOOD DELIVERY SYSTEM *****" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Restaurants" << endl;
        cout << "4. Cart" << endl;
        cout << "5. My Orders" << endl;
        cout << "6. Add Review" << endl;
        cout << "7. Admin" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
    
    
        if(choice == 1) {
            rm.displayAllRestaurants();
        } else if(choice == 2) {
            int restaurantId;
            cout << "Restaurant ID: " << endl;
            cin >> restaurantId >> endl;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if(r) r->displayMenu();
        } else if(choice == 3) {
            int restaurantId;
            int itemId;
            int count;
            cout << "Restaurant ID: " << endl;
            cin >> restaurantId >> endl;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if(r) {
                r->displayMenu();
                cout << "Item ID: " << endl;
                cin >> itemId >> endl;
                cout << "Count: " << endl; 
                cin >> count >> endl;
                MenuItem* item = r->findMenuItem(itemId);
                if(item) cart.addItem(item, count);
            }
        } else if(choice == 4) {
            cart.display();
        } else if(choice == 5) {
            if(cart.getTotal() > 0) {
                int restaurantId;
                cout << "Restaurant ID: " << endl; 
                cin >> restaurantId >> endl;
                cart.checkout(user->getId(), restaurantId, orders);
            
                cout << "How you pay? with cash or card?";
                string method; cin >> method;
                Payment payment(orders.size(), cart.getTotal(), method);
                payment.processPayment();
            }
        } else if(choice == 0) {
            cout << "Logged out:(" << endl;
        } else {
            cout << "Error: choose correct number" << endl;
        }
    } while(choice != 0);
};

void userMenu(User* user, RestaurantManager& rm, Cart& cart, vector<Order>& orders, ReviewManager& reviewManager) {
    int choice;
    do {
        cout << "***** USER MENU *****" << endl;
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
    
        if(choice == 1) {
            rm.displayAllRestaurants();
        } else if(choice == 2) {
            int restaurantId;
            cout << "Restaurant ID: "; 
            cin >> restaurantId;
            Restaurant* r = rm.findRestaurant(restId);
            if(r) {
                r->displayMenu();
            }
        } else if(choice == 3) {
            int restaurantIdک
            int itemId;
            int count;
            cout << "Restaurant ID: "; 
            cin >> restaurantId >> endl;
            Restaurant* r = rm.findRestaurant(restaurantId);
            if(r) {
                r->displayMenu();
                cout << "Item ID: "; 
                cin >> itemId >> endl;
                cout << "Count: "; 
                cin >> count >> endl;
                MenuItem* item = r->findMenuItem(itemId);
                if(item) {
                    cart.addItem(item, count);
                }
            }
        } else if(choice == 4) {
            cart.display();
        } else if(choice == 5) {
            if(cart.getTotal() > 0) {
                int restaurantId;
                cout << "Restaurant ID: "; 
                cin >> restaurantId >> endl;
                cart.checkout(user->getId(), restaurantId, orders);
            
                cout << "How will you pay? with crad or cash? ";
                string method; 
                cin >> method >> endl;
                Payment payment(orders.size(), cart.getTotal(), method);
                payment.processPayment();
            } else {
                cout << "Your cart is empty!" << endl;
            }
        } else if(choice == 0) {
            cout << "Logged out:(" << endl;
        } else {
            cout << "Error:choose correct number" << endl;
        }
    } while(choice != 0);

int main() {
    // فرض کنیم یک رستوران و چند آیتم داریم
    Restaurant r1(1, "فست فود", "...", "...", true, 20, "...");
    r1.addToMenuItem(new Food(15, 101, "پیتزا", "...", 120000, true));
    
    Cart cart;  // سبد خرید خالی
    
    // 1. مشتری ۲ تا پیتزا اضافه می‌کند
    MenuItem* pizza = r1.findMenuItem(101);
    cart.addItem(pizza, 2);  // خروجی: 2 عدد پیتزا به سبد اضافه شد
    
    // 2. نمایش سبد
    cart.display();  
    // خروجی:
    // 🛒 ========== سبد خرید ==========
    //    2 x پیتزا = 240000 تومان
    //    مجموع: 240000 تومان
    
    // 3. مشتری ۱ عدد دیگر اضافه می‌کند
    cart.addItem(pizza, 1);  // خروجی: 1 عدد به پیتزا اضافه شد
    
    // 4. نمایش دوباره
    cart.display();
    // خروجی: 3 x پیتزا = 360000 تومان
    
    // 5. ثبت نهایی سفارش
    vector<Order> orders;
    cart.checkout(1, 1, orders);  // سفارش ثبت شد
    
    // 6. سبد خالی شده
    cart.display();  // خروجی: سبد خرید شما خالی است
    
    return 0;
}