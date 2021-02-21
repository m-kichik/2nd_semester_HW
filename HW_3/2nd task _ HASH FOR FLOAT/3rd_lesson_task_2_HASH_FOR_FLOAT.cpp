#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>

struct HashFunctionsData {
    std::set <std::size_t> hashedNumbers;
    std::size_t numberOfCollisions = 0u;
    std::map <std::size_t, std::size_t> duplication;
};

std::set <double> makeRandomDouble (std::size_t N, double a, double b) {
    std::uniform_real_distribution <double> number(a, b);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <double> numbers;

    for (; numbers.size() <= N; numbers.insert(number(e)));

    std::cout << "Set of random doubles built!" << std::endl;

    return numbers;
}

std::size_t hashForDoubleV1 (double num, double a, double b, double H) {
    num -= a;
    num /= (b - a);
    num *= H;

    return static_cast<std::size_t>(num);
}

std::size_t hashForDoubleV2 (double num, double a, double b, double H) {
    auto * result = reinterpret_cast<std::size_t*>(&num);
    return (* result) / H;
}

void testHashFunction(const std::string& nameOfHashFunction,
                      const std::function <std::size_t(double, double, double, double)>& hashFunction, const std::set<double>& randNumbers,
                      double a, double b, double H) {

    HashFunctionsData data;

    std::ofstream outfc(nameOfHashFunction + "_Collisions.txt");

    auto counter = 0u;

    for (auto number : randNumbers) {
        auto hashedNumber = hashFunction(number, a, b, H);
        if(!data.hashedNumbers.insert(hashedNumber).second) {
            data.numberOfCollisions ++;
            data.duplication[hashedNumber] ++;
        } else {
            data.duplication[hashedNumber] = 1u;
        }

        if (counter % 10000 == 0) {
            outfc << counter << ' ' << data.numberOfCollisions << std::endl;
        }

        counter ++;
    }

    std::ofstream outfd(nameOfHashFunction + "_Duplications.txt");

    for (auto element : data.duplication)
        outfd << element.first << ' ' << element.second << std::endl;
}

int main() {
    const auto H = 100000u;
    const auto a = 0.;
    const auto b = 10.;
    const auto N = 2000000;

    std::set <double> randNumbers = makeRandomDouble(N, a, b);

    testHashFunction("hashForDoubleV1", hashForDoubleV1, randNumbers, a, b, H);
    testHashFunction("hashForDoubleV2", hashForDoubleV2, randNumbers, a, b, H);

    return 0;
}