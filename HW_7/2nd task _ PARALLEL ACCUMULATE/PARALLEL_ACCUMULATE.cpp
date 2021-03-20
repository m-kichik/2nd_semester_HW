#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "../../Timer/TIMER.h"

template < typename Iterator, typename T >
T parallelAccumulate(Iterator first, Iterator last, T init, std::size_t numberOfThreads) {
    const std::size_t length = std::distance(first, last);

    if (numberOfThreads <= 1) {
        return std::accumulate(first, last, init);
    }
    else {
        Iterator middle = first;
        std::advance(middle, length / 2);

        std::future < T > first_half_result =
                std::async(std::launch::async, parallelAccumulate<Iterator, T>, first, middle, init,
                           numberOfThreads / 2);

        T second_half_result = parallelAccumulate(middle, last, T(), numberOfThreads - (numberOfThreads / 2));

        return first_half_result.get() + second_half_result;
    }
}

void testParallelAccumulate(std::size_t numberOfCycles) {
    std::vector <double> container;
    container.resize(100000000);
    std::iota(std::begin(container), std::end(container), 1.);

    const auto numberOfThreads = std::thread::hardware_concurrency() * 2;
    std::vector <double> times(numberOfThreads, 0.);

    for (auto num = 0u; num < numberOfCycles; ++num) {
        for (auto i = 1u; i <= numberOfThreads; ++i) {
            Timer t;
            parallelAccumulate(std::begin(container), std::end(container), 0., i);
            t.stop();
            times[i-1] += static_cast<double>(t.getTime()) / numberOfCycles;
        }
    }

    std::ofstream ofstream("../HW_7/2nd task _ PARALLEL ACCUMULATE/results.txt");
    for (auto i = 0u; i < std::size(times); ++i)
        ofstream << i + 1 << ' ' << times[i] << std::endl;
}

// time of execution = 18 min 2 s, when N = 1000

int main() {
    const auto N = 1000u;
    Timer t;
    testParallelAccumulate(N);
    t.printTime();

    return 0;
}