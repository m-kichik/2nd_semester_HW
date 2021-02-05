#include <iostream>
#include <chrono>
#include <utility>
#include <vector>
#include <algorithm>
#include <string>

std::string makeNumber(std::size_t num);

class Timer
{
public:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    using ms_t = std::chrono::milliseconds;

public:
    explicit Timer(std::string  name) : m_name(name), m_begin(clock_t::now()) {}
    Timer() : m_name("unknownTimer_"), m_begin(clock_t::now())
    {
        nameless_timer_counter ++;
        m_name += makeNumber(nameless_timer_counter);
    }

public:
    auto stop()
    {
        m_full_time += std::chrono::duration_cast
                < ms_t > (clock_t::now() - m_begin);
        m_is_stopped = true;
    }

    auto printTime()
    {
        if(!m_is_stopped)
            stop();

        std::cout << m_name <<  ", milliseconds: " << m_full_time.count() << std::endl;
    }

    auto goOn()
    {
        m_begin = clock_t::now();
        m_is_stopped = false;
    }

public:
    ~Timer() noexcept
    {
        if(! m_is_stopped)
            stop();
        std::cout << m_name << ", milliseconds: " << m_full_time.count() << std::endl;
    }

private:
    time_point_t m_begin;
    ms_t m_full_time{0};
    std::string m_name;
    bool m_is_stopped = false;
    static std::size_t nameless_timer_counter;
};

std::size_t Timer::nameless_timer_counter = 0u;

std::string makeNumber(std::size_t num)
{
    std::string result;
    for(; num / 10 > 0 || num != 0;)
    {
        result += static_cast <char> (static_cast<std::size_t>('0') + num % 10);
        num /= 10;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int main() {

    std::vector v1(100000u, 0u);

    for (auto i = 0u; i < std::size(v1); ++i)
        v1[i] = 100000u - i;

    Timer t;
    Timer tt("timer");

    std::sort(std::begin(v1), std::end(v1));

    t.stop();

    t.printTime();

    std::vector v2(100000u, 0u);

    for (auto i = 0u; i < std::size(v2); ++i)
        v2[i] = 100000u - i;

    t.goOn();

    std::sort(std::begin(v2), std::end(v2));

    return 0;
}
