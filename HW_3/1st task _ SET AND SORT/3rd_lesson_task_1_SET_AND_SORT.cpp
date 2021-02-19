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

long long makeTestSt (std::size_t N, std::vector <int> randNum);

long long makeTestVec (std::size_t N, std::vector <int> randNum);

int main() {

//    const auto N = 200u;

    for (auto numberOfCycle = 0u; numberOfCycle <= 5000000u; numberOfCycle += (numberOfCycle > 500000u ? 100000u : 10000u)) {
        std::ifstream inf("RandWithShuffle5M.txt");
        std::vector <int> randomNumbers(numberOfCycle, 0);
        for (auto i = 0u; i < numberOfCycle; ++i)
            inf >> randomNumbers[i];

        std::ofstream outfSt("testSetWithShuffleRelease5M.txt", std::ios::app);
        outfSt << numberOfCycle << ' ' << makeTestSt(numberOfCycle, randomNumbers) << std::endl;

        std::ofstream outfVec("testVectorWithShuffleRelease5M.txt", std::ios::app);
        outfVec << numberOfCycle << ' ' << makeTestVec(numberOfCycle, randomNumbers) << std::endl;
    }


    /*{
        std::array <int, N> arr{};
        auto randNumIterator = randomNumbers.begin();
        Timer tArr("copy and sort in array");
        for (auto& x: arr) {
            x = *(randNumIterator++);
        }
        std::sort(std::begin(arr), std::end(arr));
        std::pair <std::string, long long> arrInf(tArr.getName(), tArr.getTime());
    }*/

    //printInFile(N, stInf, vecInf, arrInf);
    // printInLine(N, stInf, vecInf, arrInf);

    return 0;
}

long long makeTestSt (std::size_t N, std::vector <int> randNum) {
    std::set <int> st;
    Timer tSt("generate and insert in set");
    for (auto i = 0u; i < N; ++i) {
        st.insert(randNum[i]);
    }
    return tSt.getTime();
}

long long makeTestVec (std::size_t N, std::vector <int> randNum) {
    Timer tVec("copy and sort in vector");
    std::vector <int> vec(randNum);
    std::sort(std::begin(vec), std::end(vec));
    return tVec.getTime();
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