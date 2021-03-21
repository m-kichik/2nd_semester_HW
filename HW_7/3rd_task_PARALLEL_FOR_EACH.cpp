#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <vector>

template <typename Iterator, typename Function>
void parallelForEach(Iterator firstIt, Iterator lastIt, Function func) {
    const auto length = std::distance(firstIt, lastIt);
    const auto max_size = 25;

    if (length <= max_size) {
        std::for_each(firstIt, lastIt, func);
        return;
    } else {
        Iterator middleIt = firstIt;
        std::advance(middleIt, length / 2);

        std::future <void> firstRes = std::async(std::launch::async, parallelForEach<Iterator, Function>, firstIt, middleIt, func);
        parallelForEach(middleIt, lastIt, func);

        return;
    }

}

int main() {
    std::vector <double> container(1000);
    std::iota(std::begin(container), std::end(container), 1.);
    auto res = 0.;

    parallelForEach(std::begin(container), std::end(container), [&res](auto x){res += x;});

    std::cout << res; // res = 500500

    return 0;
}