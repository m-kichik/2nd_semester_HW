#include <array>
#include <chrono>
#include <fstream>
#include "TIMER.h"
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <random>
#include <vector>

void printInFile(std::size_t N, const std::pair<std::string, long long>& stInf,
                 const std::pair<std::string, long long>& vecInf, const std::pair<std::string, long long>& arrInf);

void printInLine(std::size_t N, const std::pair<std::string, long long>& stInf,
                 const std::pair<std::string, long long>& vecInf, const std::pair<std::string, long long>& arrInf);

int main() {

    const auto N = 5200u;
    std::uniform_int_distribution rule(0, 1000);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <int> st;
    Timer tSt("generate and insert in set");
    for (auto i = 0u; i < N; ++i) {
        st.insert(e());
    }
    std::pair <std::string, long long> stInf(tSt.getName(), tSt.getTime());

    Timer tVec("copy and sort in vector");
    std::vector <int> vec(st.begin(), st.end());
    std::sort(std::begin(vec), std::end(vec));
    std::pair <std::string, long long> vecInf(tVec.getName(), tVec.getTime());

    std::array <int, N> arr{};
    auto stIterator = std::begin(st);
    Timer tArr("copy and sort in array");
    for (auto& x: arr) {
        x = *(stIterator++);
    }
    std::sort(std::begin(arr), std::end(arr));
    std::pair <std::string, long long> arrInf(tArr.getName(), tArr.getTime());

    printInFile(N, stInf, vecInf, arrInf);
    // printInLine(N, stInf, vecInf, arrInf);

    return 0;
}

void printInFile(std::size_t N, const std::pair<std::string, long long>& stInf,
                 const std::pair<std::string, long long>& vecInf, const std::pair<std::string, long long>& arrInf) {
    std::ofstream outf("results.txt", std::ios::app);

    outf << "number of elements: " << N << std::endl;

    outf << std::left << std::setfill(' ') << std::setw(30) << stInf.first
         << vecInf.first << std::right << std::setfill(' ') << std::setw(30)
         << arrInf.first << std::endl;

    outf << "time in nanoseconds: " << std::endl;

    outf << std::left << std::setfill(' ') << std::setw(30) << stInf.second
         << vecInf.second << std::right << std::setfill(' ') << std::setw(30)
         << arrInf.second << std::endl;

    outf << "normalized: " << std::endl;

    auto normCoeff = std::min({stInf.second, vecInf.second, arrInf.second});

    outf << std::left << std::setfill(' ') << std::setw(30)
         << static_cast <double> (stInf.second) / normCoeff
         << static_cast <double> (vecInf.second) / normCoeff
         << std::right << std::setfill(' ') << std::setw(30)
         << static_cast <double> (arrInf.second) / normCoeff << std::endl << std::endl;
}

void printInLine(std::size_t N, const std::pair<std::string, long long>& stInf,
                 const std::pair<std::string, long long>& vecInf, const std::pair<std::string, long long>&  arrInf) {
    std::ofstream outf("results.txt", std::ios::app);

    std::cout << "number of elements: " << N << std::endl;

    std::cout << std::left << std::setfill(' ') << std::setw(30) << stInf.first
         << vecInf.first << std::right << std::setfill(' ') << std::setw(30)
         << arrInf.first << std::endl;

    std::cout << "time in nanoseconds: " << std::endl;

    std::cout << std::left << std::setfill(' ') << std::setw(30) << stInf.second
         << vecInf.second << std::right << std::setfill(' ') << std::setw(30)
         << arrInf.second << std::endl;

    std::cout << "normalized: " << std::endl;

    auto normCoeff = std::min({stInf.second, vecInf.second, arrInf.second});

    std::cout << std::left << std::setfill(' ') << std::setw(30)
         << static_cast <double> (stInf.second) / normCoeff
         << static_cast <double> (vecInf.second) / normCoeff
         << std::right << std::setfill(' ') << std::setw(30)
         << static_cast <double> (arrInf.second) / normCoeff << std::endl << std::endl;
}