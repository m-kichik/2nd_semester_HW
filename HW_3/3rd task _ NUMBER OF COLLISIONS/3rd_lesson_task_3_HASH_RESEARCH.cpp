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

std::vector <std::string> makeRandomWords (std::size_t N) {
    std::uniform_int_distribution length(0, 10);
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> marker;
    std::vector <std::string> words;
    auto size = marker.size();

    for (; marker.size() <= 2000000u;) {
        auto len = length(e);
        std::string str;
        for (; str.size() < static_cast<std::size_t>(len);) {
            str.push_back(static_cast<char>(letter(e)));
        }
        marker.insert(str);
        if (marker.size() != size) {
            words.push_back(str);
            size = marker.size();
        }
    }

    return words;
}

std::vector <Object> makeRandomObjects (std::size_t N) {
    std::ifstream inf("randomStrings.txt");
    std::uniform_int_distribution number(0, 20000);
    std::uniform_int_distribution expression(0, 1);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::vector <Object> container;
    std::vector <std::string> words;
    words = makeRandomWords(N);

    for (auto i = 0u; i < N; ++i) {
        container.push_back({number(e), words[i], static_cast<bool>(expression(e))});
    }

    std::cout << "Vector of objects built succesfully!" << std::endl;

    return container;
}

template < typename T >
void hash_combine(std::size_t & seed, const T & value) noexcept
{
    seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t & seed, const T & value) noexcept
{
    hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t & seed, const T & value, const Types & ... args) noexcept
{
    hash_combine(seed, value);
    hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
    std::size_t seed = 0;
    hash_value(seed, args...);
    return seed;
}

void testHashFunction (std::size_t N) {
    auto randomObjects = makeRandomObjects(N);
    std::set <std::size_t> marker;
    auto numberOfCollisions = 0u;
    auto size = marker.size();
    std::ofstream outf("numberOfCollisions.txt", std::ios::app);
    for (auto i = 0u; i < N; ++i) {
        marker.insert(hash_value(randomObjects[i].number, randomObjects[i].word, randomObjects[i].expression));
        if (size == marker.size())
            numberOfCollisions ++;
        else
            size = marker.size();
        if(i % 10000 == 0)
            outf << i << ' ' << numberOfCollisions << std::endl;
    }
}

int main() {
    const std::size_t N = 2000000;

    testHashFunction(N);

    return 0;
}