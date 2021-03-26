#include <chrono>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <vector>

class ParallelPiCalculator {
private:
    /*auto safeAddPoints(std::size_t points) {
        std::scoped_lock lock(m_mutex);
        m_result += points;
    }*/

    auto addCirclePoints(std::size_t numberOfPoints) {
        std::uniform_real_distribution urd(-1., 1.);
        std::mt19937_64 mte(std::chrono::system_clock::now().time_since_epoch().count());

//        auto circlePointCounter = 0u;
        const auto R = 1.;

        for (auto i = 0u; i < numberOfPoints; ++i) {
            auto x = urd(mte);
            auto y = urd(mte);
//            circlePointCounter += (x * x + y * y <= R) ? 1u : 0u;
            m_result += (x * x + y * y <= R) ? 1u : 0u;
        }


//        safeAddPoints(circlePointCounter);
    }

public:
    auto operator()(std::size_t numPoints, std::size_t numThreads) {
        std::vector <std::thread> threads(numThreads - 1);
        for (auto it = 0u; it < std::size(threads); ++it) {
            threads[it] = std::thread(&ParallelPiCalculator::addCirclePoints, this, numPoints);
        }
        addCirclePoints(numPoints);
        for(auto& x : threads)
            x.join();

        return 4. * m_result / numPoints / numThreads;
    }

private:
//    std::mutex m_mutex;
    std::atomic_uint m_result{0u};
};

int main() {
    const auto numPoints = 100000000u;
    const auto numThreads = (std::thread::hardware_concurrency()) ? std::thread::hardware_concurrency() : 1u;

    std::cout << "result : " << std::fixed << std::setprecision(10) << ParallelPiCalculator()(numPoints, numThreads) << std::endl;

    return 0;
}