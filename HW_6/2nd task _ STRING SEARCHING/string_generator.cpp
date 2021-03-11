#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <random>
#include <iomanip>
#include <iostream>

void makeFileWithRandomStrings(std::size_t N) {

    std::ofstream outf("../HW_6/2nd task _ STRING SEARCHING/file_with_random_strings.txt");

    const auto leftRangeEdge = 111111u;
    const auto rightRangeEdge = leftRangeEdge * 9;

    std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution uid(leftRangeEdge, rightRangeEdge);

    for (auto i = 0u; i <= N; ++i) {
        outf << std::setfill('0') << std::setw(std::size(std::to_string(N))) << std::right << i << "_string_" << uid(engine) << std::endl;
    }
}

int main() {
    makeFileWithRandomStrings(1000);
    return 0;
}