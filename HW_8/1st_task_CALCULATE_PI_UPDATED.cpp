#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <vector>

class parallelPiCalculator {
private:
    auto safeAddPoints(std::size_t points, std::size_t& accumulator) {
        std::scoped_lock lock(m_mutex);
        accumulator += points;
    }

    auto addCirclePoints(std::size_t numberOfPoints, std::size_t& pointAccumulator) {
        std::uniform_real_distribution urd(-1., 1.);
        std::mt19937_64 mte(std::chrono::system_clock::now().time_since_epoch().count());

        auto circlePointCounter = 0u;
        const auto R = 1.;

        for (auto i = 0u; i < numberOfPoints; ++i) {
            auto x = urd(mte);
            auto y = urd(mte);
            circlePointCounter += (x * x + y * y <= R) ? 1u : 0u;
        }

        safeAddPoints(circlePointCounter, pointAccumulator);
    }

public:
    auto operator()(std::size_t numPoints, std::size_t numThreads) {
        auto result = 0u;
        std::vector <std::thread> threads(numThreads - 1);
        for (auto it = 0u; it < std::size(threads); ++it) {
            threads[it] = std::thread(&parallelPiCalculator::addCirclePoints, this, numPoints, std::ref(result));
        }
        addCirclePoints(numPoints, result);
        for(auto& x : threads)
            x.join();

        return 4 * static_cast<long double>(result) / numPoints / numThreads;
    }

private:
    std::mutex m_mutex;
};

int main() {
    const auto numPoints = 100000000u;
    const auto numThreads = (std::thread::hardware_concurrency()) ? std::thread::hardware_concurrency() : 1u;

    std::cout << "result : " << std::fixed << std::setprecision(10) << parallelPiCalculator()(numPoints, numThreads) << std::endl;

    return 0;
}