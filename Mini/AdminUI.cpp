#include "AdminUI.h"
#include <iostream>
#include <limits>

using namespace std;

static int readInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "ورودی نامعتبر. لطفاً عدد وارد کنید.\n";
    }
}

AdminUI::AdminUI(SystemAdmin& user, RestaurantDAO& rDAO,
                 UserDAO& uDAO, OrderDAO& oDAO)
    : user_(user), rDAO_(rDAO), uDAO_(uDAO), oDAO_(oDAO) {}

void AdminUI::run() {
    while (true) {
        user_.showMenu();
        int choice = readInt("");

        if      (choice == 0) break;
        else if (choice == 1) addRestaurant();
        else if (choice == 2) toggleRestaurant();
        else if (choice == 3) showReports();
        else cout << "گزینه نامعتبر.\n";
    }
}

// ── ثبت رستوران جدید ──────────────────────────────────────────────────────
void AdminUI::addRestaurant() {
    cout << "نام رستوران: ";  string name;  getline(cin, name);
    cout << "آدرس: ";         string addr;  getline(cin, addr);
    cout << "تلفن: ";         string phone; getline(cin, phone);
    cout << "توضیحات: ";      string desc;  getline(cin, desc);
    int prep = readInt("زمان آماده‌سازی (دقیقه): ");

    vector<Restaurant> all = rDAO_.getAllRestaurants();
    int newId = (int)all.size() + 1;

    Restaurant newR(newId, name, addr, phone, true, prep, desc);
    rDAO_.insertRestaurant(newR);
    cout << "✓ رستوران با شناسه " << newId << " ثبت شد.\n";
}

// ── فعال/غیرفعال کردن رستوران ─────────────────────────────────────────────
void AdminUI::toggleRestaurant() {
    vector<Restaurant> all = rDAO_.getAllRestaurants();
    if (all.empty()) { cout << "هیچ رستورانی موجود نیست.\n"; return; }

    cout << "\n----- لیست رستوران‌ها -----\n";
    for (auto& r : all) {
        cout << "[" << r.getId() << "] " << r.getName()
             << " - " << (r.getStatus() ? "فعال" : "غیرفعال") << "\n";
    }

    int rId = readInt("شناسه رستوران: ");
    Restaurant* r = rDAO_.getRestaurantById(rId);
    if (!r) { cout << "رستوران یافت نشد.\n"; return; }

    r->setStatus(!r->getStatus());
    rDAO_.updateRestaurant(*r);
    cout << "✓ وضعیت رستوران به "
         << (r->getStatus() ? "فعال" : "غیرفعال")
         << " تغییر کرد.\n";
    delete r;
}

// ── گزارش‌ها ───────────────────────────────────────────────────────────────
void AdminUI::showReports() {
    vector<User*>      users       = uDAO_.getAllUsers();
    vector<Restaurant> restaurants = rDAO_.getAllRestaurants();

    int activeR = 0, inactiveR = 0;
    for (auto& r : restaurants)
        r.getStatus() ? activeR++ : inactiveR++;

    int customers = 0, managers = 0;
    for (auto* u : users) {
        if (u->getRole() == UserRole::CUSTOMER)           customers++;
        else if (u->getRole() == UserRole::RESTAURANT_MANAGER) managers++;
    }

    cout << "\n========== گزارش سامانه ==========\n";
    cout << "تعداد کل کاربران:      " << users.size()       << "\n";
    cout << "  مشتریان:             " << customers           << "\n";
    cout << "  مدیران رستوران:      " << managers            << "\n";
    cout << "تعداد کل رستوران‌ها:   " << restaurants.size()  << "\n";
    cout << "  رستوران‌های فعال:    " << activeR             << "\n";
    cout << "  رستوران‌های غیرفعال: " << inactiveR           << "\n";
    cout << "===================================\n";

    for (auto* u : users) delete u;
}