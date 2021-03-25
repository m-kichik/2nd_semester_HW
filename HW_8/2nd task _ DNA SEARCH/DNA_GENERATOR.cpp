#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

void generateDNA(std::size_t N) {
    std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution uniformIntDistribution(0, 3);

    std::vector letters = {'A', 'G', 'T', 'C'};

    std::ofstream ofstream("../HW_8/2nd task _ DNA SEARCH/DNA_sequence.txt", std::ios::trunc);

    for(auto i = 0u; i < N; ++i)
        ofstream << letters[uniformIntDistribution(engine)];
}

int main() {
    const auto N = 1000000u;
    generateDNA(N);

    return 0;
}