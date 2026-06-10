#include "Food.h"

// پیاده‌سازی سازنده و ارسال پارامترها به کلاس پایه (MenuItem)
Food::Food(int r_time, int i, const string& n, const string& d, double p, bool a)
    : MenuItem(i, n, p, a, d, "Food") {
    if (r_time < 0) {
        std::cout << "Error: Please enter the positive time" << std::endl;
        readytime = 0;
    } else {
        readytime = r_time;
    }
}

int Food::getReadytime() const {
    return readytime;
}

void Food::setReadytime(int r) {
    if (r < 0) {
        std::cout << "Error: The time is incorrect. Please enter a positive time." << std::endl;
    } else {
        readytime = r;
    }
}

void Food::display() const {
    // فراخوانی متد display از کلاس پایه برای نمایش اطلاعات عمومی
    MenuItem::display();
    std::cout << "The time you need to wait for your food to be ready is " 
              << readytime << " minutes." << std::endl;
}