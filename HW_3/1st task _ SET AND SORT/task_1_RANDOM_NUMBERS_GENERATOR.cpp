#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

int main() {
    std::uniform_int_distribution rule(0, 20000);
    std::default_random_engine e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::ofstream outf("randomNumbers.txt", std::ios::app);

    for (auto i = 0; i < 200000; ++i) {
        outf << rule(e) << std::endl;
    }

    return 0;
}

