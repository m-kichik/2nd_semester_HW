#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

auto preCalculatePi(std::size_t numberOfPoints) {
    std::uniform_real_distribution urd(-1., 1.);
    std::mt19937_64 mte(std::chrono::system_clock::now().time_since_epoch().count());

    auto circlePointCounter = 0u;
    const auto R = 1.;

    for (auto i = 0u; i < numberOfPoints; ++i) {
        auto x = urd(mte);
        auto y = urd(mte);
        circlePointCounter += (x * x + y * y <= R) ? 1u : 0u;
    }

    return circlePointCounter;
}

auto calculatePi_parallel(std::size_t numberOfPoints, std::size_t numberOfThreads) {
    std::vector <std::future<unsigned>> futures;
    futures.reserve(numberOfThreads);

    for (; std::size(futures) < numberOfThreads;
           futures.push_back(std::async(std::launch::async, preCalculatePi, numberOfPoints)));

    auto result = 0u;
    std::for_each(std::begin(futures), std::end(futures), [&result](auto& x){result += x.get();});

    return 4 * static_cast<long double>(result) / numberOfPoints / numberOfThreads;
}

int main() {
    const auto numberOfPoints = 100000000u;
    const auto numberOfThreads = 6u;

    std::cout << "result : " << std::fixed << std::setprecision(10) << calculatePi_parallel(numberOfPoints, numberOfThreads) << std::endl;

    return 0;
}