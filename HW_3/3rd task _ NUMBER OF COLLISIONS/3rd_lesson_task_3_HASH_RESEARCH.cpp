#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <vector>

struct Object {
    int number;
    std::string word;
    bool expression;
};

std::set <std::string> makeRandomWords (std::size_t N) {
    auto length = 10u;
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (; words.size() <= 2000000u;) {
        std::string str;
        for (; str.size() < length;) {
            str.push_back(static_cast<char>(letter(e)));
        }
        words.insert(str);
    }

    return words;
}

std::vector <Object> makeRandomObjects (std::size_t N) {
    std::uniform_int_distribution number(0, 20000);
    std::bernoulli_distribution expression(0.5);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::vector <Object> container;
    std::set <std::string> randWords = makeRandomWords(N);
    std::vector <std::string> words(randWords.begin(), randWords.end());

    container.reserve(N);
    for (auto i = 0u; i < N; ++i) {
        container.push_back({number(e), words[i], expression(e)});
    }

    std::cout << "Vector of objects built succesfully!" << std::endl;

    return container;
}

template < typename T >
void hash_combine (std::size_t & seed, const T & value) noexcept {
    seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value (std::size_t & seed, const T & value) noexcept {
    hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value (std::size_t & seed, const T & value, const Types & ... args) noexcept {
    hash_combine(seed, value);
    hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value (const Types & ... args) noexcept {
    std::size_t seed = 0;
    hash_value(seed, args...);
    return seed;
}

void testHashFunction (std::size_t N) {
    auto randomObjects = makeRandomObjects(N);
    std::set <std::size_t> marker;
    auto numberOfCollisions = 0u;
    std::ofstream outf("numberOfCollisions_VS_64.txt"/*, std::ios::app | std::ios::trunc*/);
    for (auto i = 0u; i < N; ++i) {
        if (!marker.insert(hash_value(randomObjects[i].number, randomObjects[i].word, randomObjects[i].expression)).second)
            numberOfCollisions ++;
        if(i % 10000 == 0)
            outf << i << ' ' << numberOfCollisions << std::endl;
    }
}

int main() {
    const std::size_t N = 2000001;

    testHashFunction(N);

    return 0;
}