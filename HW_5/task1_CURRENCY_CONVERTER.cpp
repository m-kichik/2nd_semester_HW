#include <iostream>
#include <iomanip>

#include <Windows.h>

int main() {

    std::cin.imbue(std::locale("ru_RU.UTF8"));
    std::cout.imbue(std::locale("en_US.UTF8"));
    long double money;
    std::cin >> std::get_money(money);
    money /= 74.69;
    std::cout << std::showbase << std::put_money(money, false);

    return 0;
}