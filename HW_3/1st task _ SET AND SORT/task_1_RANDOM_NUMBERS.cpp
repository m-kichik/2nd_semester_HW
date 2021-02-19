#include_next <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <vector>
#include <algorithm>

int main() {
    std::uniform_int_distribution rule(0, 4000000);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::vector < int > v(4000000, 0);
    std::iota(std::begin(v), std::end(v), 0);
    std::shuffle(std::begin(v), std::end(v), e);

    std::ofstream outf("randWithShuffle4M.txt", std::ios::app);

    for (auto item : v)
        outf << item << std::endl;

//    std::vector <int> veryRandomNumbers;
//    std::set <int> testingSet;
//    auto tSetSize = testingSet.size();
//
//    for (auto i = 0; veryRandomNumbers.size() < 200000; ++i) {
//        auto value = rule(e);
//        testingSet.insert(value);
//        if (testingSet.size() != tSetSize) {
//            tSetSize = testingSet.size();
//            veryRandomNumbers.push_back(value);
//        }
//    }

//    std::ofstream outf("veryRandomNumbers.txt", std::ios::app);
//
//    for (auto item : veryRandomNumbers)
//        outf << item << std::endl;

    return 0;
}