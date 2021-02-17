#include <iostream>
#include <vector>
#include <array>

int main()
{
    std::array <int, 10> arr;
    std::vector <int> v(10);
    auto n1 = v.capacity();
    for(auto i = 0u; i <= n1; ++i)
    {
        v.push_back(1);
    }
    for (auto x : v)
        std::cout << x << ' ';
    std::cout << std::endl;
    auto n2 = v.capacity();
    std::cout << n1 << ' ' << n2 << std::endl;
}
