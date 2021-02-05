#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

class Timer
{
public:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    Timer() : m_begin(clock_t::now()) {}

public:
    auto stop()
    {
        m_full_time += std::chrono::duration_cast
                <std::chrono::milliseconds > (clock_t::now() - m_begin);
        m_is_stopped = true;
    }

    auto getTime()
    {
        if(!m_is_stopped)
            stop();

        return m_full_time.count();
    }

    auto goOn()
    {
        m_begin = clock_t::now();
        m_is_stopped = false;
    }

public:
    ~Timer()
    {
        if(! m_is_stopped)
            stop();
        std::cout << "milliseconds: " << m_full_time.count();
    }

private:
    time_point_t m_begin;
    std::chrono::milliseconds m_full_time{0};
    bool m_is_stopped = false;
};

int main() {

    std::vector v1(100000u, 0u);

    for (auto i = 0u; i < std::size(v1); ++i)
        v1[i] = 100000u - i;

    Timer t;

    std::sort(std::begin(v1), std::end(v1));

    t.stop();

    std::cout << "time passed (ms): " << t.getTime() << std::endl;

    std::vector v2(100000u, 0u);

    for (auto i = 0u; i < std::size(v2); ++i)
        v2[i] = 100000u - i;

    t.goOn();

    std::sort(std::begin(v2), std::end(v2));

    return 0;
}
