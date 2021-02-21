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

std::set <double> makeRandomDouble (std::size_t N) {
    std::uniform_real_distribution <double> number(0., 1000.);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <double> numbers;

    for (; numbers.size() <= N; numbers.insert(number(e)));

    std::cout << "Set of random doubles built!" << std::endl;

    return numbers;
}

std::size_t hashForDoubleV1 (double num) {
    auto result = 0u;

    for (; num >= 1.;)
        num /= 10;

    for(; num < 0.1;)
        num *= 10;

    for (auto k = 1u; k <= 100000000u; k *= 10) {
        num *= 10;
        auto e = static_cast<std::size_t>(num);
        result += e * k;
        num -= static_cast<double>(e);
    }

    return result;
}

std::size_t hashForDoubleV2 (double num) {
    auto * pNum = (std::size_t*)(&num);
    return * pNum;
}

void testHashFunction(const std::string& nameOfHashFunction,
                      const std::function <std::size_t(double)>& hashFunction, const std::set<double>& randNumbers) {

    HashFunctionsData data;

    std::ofstream outfc(nameOfHashFunction + "_Collisions.txt");

    auto counter = 0u;

    for (auto number : randNumbers) {
        auto hashedNumber = hashFunction(number);
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
    const auto N = 2000000u;

    std::set <double> randNumbers = makeRandomDouble(N);

    testHashFunction("hashForDoubleV1", hashForDoubleV1, randNumbers);
    testHashFunction("hashForDoubleV2", hashForDoubleV2, randNumbers);

    return 0;
}