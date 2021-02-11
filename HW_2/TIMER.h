#ifndef CHRONO_TIMER_H
#define CHRONO_TIMER_H

#include <iostream>
#include <chrono>
#include <vector>
#include <string>

class Timer
{
public:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    using interval_t = std::chrono::milliseconds;

public:
    explicit Timer(const std::string &name) : m_name(name), m_begin(clock_t::now()) {}
    Timer() : m_name("unknownTimer_"), m_begin(clock_t::now())
    {
        nameless_timer_counter ++;
        m_name += std::to_string(nameless_timer_counter);
    }

public:
    void stop()
    {
        m_full_time += std::chrono::duration_cast
                < interval_t > (clock_t::now() - m_begin);
        m_is_stopped = true;
    }

    void printTime()
    {
        if(!m_is_stopped)
            stop();

        std::cout << m_name <<  ", milliseconds: " << m_full_time.count() << std::endl;
    }

    void goOn()
    {
        m_begin = clock_t::now();
        m_is_stopped = false;
    }

    std::string getName()
    {
        return m_name;
    }

    auto getTime()
    {
        if(!m_is_stopped)
            stop();

        return m_full_time.count();
    }

public:
    ~Timer() noexcept
    {
//        try
//        {
//            if(! m_is_stopped)
//                stop();
//            std::cout << m_name << ", milliseconds: " << m_full_time.count() << std::endl;
//        }
//        catch (std::exception& exception)
//        {
//            std::cerr << "timer crashed: " << exception.what() << std::endl;
//        }
    }

private:
    time_point_t m_begin;
    interval_t m_full_time{0};
    std::string m_name;
    bool m_is_stopped = false;
    inline static std::size_t nameless_timer_counter = 0u;
};

#endif //CHRONO_TIMER_H