#include <algorithm>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::mutex mutex;

template <typename T>
void safePushBack(std::vector <T> & vec, T value) {
    std::lock_guard <std::mutex> lock(mutex);
    vec.push_back(value);
}

void safeFindOccurences(const std::string& str, std::size_t pos, std::size_t last, std::vector <std::size_t> & vec, const std::string& occ) {
    for(auto value = str.find(occ, pos);
        (value != std::string::npos) && (value <= last);
                pos = value + std::size(occ), value = str.find(occ, pos)) {
        safePushBack(vec, value);
    }
}

auto getOccurrenceIndices(const std::string& occ, const std::string& dna) {
    std::vector <std::size_t> indexes;

    const auto numOfThreads = std::thread::hardware_concurrency();
    auto step = (std::size(dna) + numOfThreads - 1) / numOfThreads;

    std::vector <std::thread> threads(numOfThreads - 1);

    auto n = 0u;
    std::vector <std::size_t> ind;
    std::generate_n(std::back_inserter(ind), numOfThreads - 1, [step, &n](){return step * n++;});

    for(auto it = 0u; it < std::size(threads); ++it) {
        threads[it] = std::thread(safeFindOccurences, std::cref(dna),
                                  ind[it], ind[it] + step + std::size(occ) - 1,
                                  std::ref(indexes), std::ref(occ));
    }

    safeFindOccurences(dna, ind[std::size(ind) - 1] + step + std::size(occ), std::size(dna) - 1,
                       std::ref(indexes), std::ref(occ));

    for(auto& x : threads)
        x.join();

    std::sort(std::begin(indexes), std::end(indexes));
    return indexes;
}

template <typename T>
auto printIndexes (const std::vector <T> & vec, std::ostream& stream) {
    if(!std::size(vec)) {
        std::cout << std::endl << "there are no entries" << std::endl;
        return;
    }

    for (auto x : vec)
        stream << x << ' ';
    stream << std::endl;
}

int main() {
    std::ifstream ifstream("../HW_8/2nd task _ DNA SEARCH/DNA_sequence.txt");
    std::string DNA;
    std::getline(ifstream, DNA);

    std::string occurence;
    std::cout << "enter subsequence: ";
    std::cin >> occurence;

    std::string subseq(std::size(occurence), '0');
    DNA += subseq;

    auto result = getOccurrenceIndices(occurence, DNA);

    printIndexes(result, std::cout);

    return 0;
}