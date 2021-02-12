#include <array>
#include <chrono>
#include <deque>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <vector>

#include "TIMER.h"

int main()
{
    const auto N = 100000;
    std::uniform_int_distribution rule(0, 1000);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::vector <int> randomNumbers{};

    std::array <int, N> arr{};
    std::vector <int> vec;
    vec.reserve(N);
    std::deque <int> deq;
    std::list <int> listik;
    std::forward_list <int> fwListik;

    std::multimap <std::size_t, std::string> results;

    for (auto i = 0u; i < N; ++i) {
            randomNumbers.push_back(rule(e));
        }

    for (auto i = 0u; i < N; ++i) {
        arr[i] = randomNumbers[i];
        vec.push_back(randomNumbers[i]);
        deq.push_back(randomNumbers[i]);
        listik.push_back(randomNumbers[i]);
        fwListik.push_front(randomNumbers[i]);
        }

    Timer ta("array, std sort");
    std::sort(arr.begin(), arr.end());
    results.insert({static_cast <std::size_t> (ta.getTime()), ta.getName()});

    Timer tv("vector, std sort");
    std::sort(vec.begin(), vec.end());
    results.insert({static_cast <std::size_t> (tv.getTime()), tv.getName()});

    Timer td("deque, std sort");
    std::sort(deq.begin(), deq.end());
    results.insert({static_cast <std::size_t> (td.getTime()), td.getName()});

    Timer tlSelf("list, self-sort");
    listik.sort();
    results.insert({static_cast <std::size_t> (tlSelf.getTime()), tlSelf.getName()});

    Timer ftlSelf("forward list, self-sort");
    fwListik.sort();
    results.insert({static_cast <std::size_t> (ftlSelf.getTime()), ftlSelf.getName()});

    for (const auto& [result, name] : results)
    {
        std::cout << std::left << std::setfill(' ') << std::setw(25) << name << ' ' << std::right << std::setfill(' ') << std::setw(5) << result << std::endl;
    }

    return 0;
}