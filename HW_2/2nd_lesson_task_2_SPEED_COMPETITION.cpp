#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <random>
#include <chrono>
#include <map>
#include "TIMER.h"

int main()
{
    const auto N = 100000;
    std::uniform_int_distribution rule(0, 1000);
    std::default_random_engine  e(std::chrono::steady_clock::now().time_since_epoch().count());

    std::vector <int> randomNumbers{};

    std::array <int, N> arr;
    std::vector <int> vec;
    std::deque <int> deq;
    std::list <int> listik;
    std::forward_list <int> fwListik;

    std::map <std::size_t, std::string> results;

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
    results.insert({ta.getTime(), ta.getName()});

    Timer tv("vector, std sort");
    std::sort(vec.begin(), vec.end());
    results.insert({tv.getTime(), tv.getName()});

    Timer td("deque, std sort");
    std::sort(deq.begin(), deq.end());
    results.insert({td.getTime(), td.getName()});

    Timer tlSelf("list, self-sort");
    listik.sort();
    results.insert({tlSelf.getTime(), tlSelf.getName()});

    Timer ftlSelf("forward list, self-sort");
    fwListik.sort();
    results.insert({ftlSelf.getTime(), ftlSelf.getName()});

    for (const auto& [result, name] : results)
    {
        std::cout << name << ' ' << result << std::endl;
    }

    return 0;
}