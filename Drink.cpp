#include "Drink.h"

// پیاده‌سازی سازنده
Drink::Drink(int c, int i, const string& n, const string& d, double p, bool a)
    : MenuItem(i, n, p, a, d, "Drink") {
    if (c < 0) {
        std::cout << "Error: Please enter the positive number." << std::endl;
        capacity = 250; // مقدار پیش‌فرض در صورت ورودی اشتباه
    } else {
        capacity = c;
    }
}

int Drink::getCapacity() const {
    return capacity;
}

void Drink::setCapacity(int c) {
    if (c < 0) {
        std::cout << "Error: The capacity is not valid. Please enter a positive number." << std::endl;
    } else {
        capacity = c;
    }
}

void Drink::display() const {
    // فراخوانی نمایش اطلاعات کلی از MenuItem
    MenuItem::display();
    std::cout << "The capacity you want for your drink is " 
              << capacity << " mL" << std::endl;
}