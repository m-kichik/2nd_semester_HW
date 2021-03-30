#include <condition_variable>
#include <exception>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <thread>
#include <vector>
#include <queue>

#include <boost/lockfree/stack.hpp>
#include <boost/lockfree/queue.hpp>
#include "../../Timer/TIMER.h"

template < typename T >
class Threadsafe_Stack
{
public:

    Threadsafe_Stack() = default;

    Threadsafe_Stack(const Threadsafe_Stack & other) {
        std::lock_guard < std::mutex > lock(other.m_mutex);
        m_data = other.m_data;
    }

    Threadsafe_Stack& operator=(const Threadsafe_Stack &) = delete;

public:

    void push(T value) {
        std::lock_guard < std::mutex > lock(m_mutex);
        m_data.push(value);
    }

    std::shared_ptr < T > pop() {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_data.empty()) {
            throw std::range_error("empty stack");
        }

        const auto result = std::make_shared < T > (m_data.top());
        m_data.pop();

        return result;
    }

    void pop(T & value) {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_data.empty()) {
            throw std::range_error("empty stack");
        }

        value = m_data.top();
        m_data.pop();
    }

    bool empty() const {
        std::lock_guard < std::mutex > lock(m_mutex);
        return m_data.empty();
    }

private:

    std::stack < T > m_data;

private:

    mutable std::mutex m_mutex;
};

template < typename T >
class Threadsafe_Queue {
public:

    Threadsafe_Queue() = default;

    Threadsafe_Queue(const Threadsafe_Queue & other) {
        std::lock_guard < std::mutex > lock(other.m_mutex);
        m_queue = other.m_queue;
    }

    Threadsafe_Queue & operator=(const Threadsafe_Queue & other) {
        std::scoped_lock lock(m_mutex, other.m_mutex);
        m_queue = other.m_queue;
    }

public:

    void push(T value) {
        std::lock_guard < std::mutex > lock(m_mutex);
        m_queue.push(value);
        m_condition_variable.notify_all();
    }

    void wait_and_pop(T & value) {
        std::unique_lock < std::mutex > lock(m_mutex);

        m_condition_variable.wait(lock, [this] {return !m_queue.empty(); });
        value = m_queue.front();
        m_queue.pop();
    }

    std::shared_ptr < T > wait_and_pop() {
        std::unique_lock < std::mutex > lock(m_mutex);

        m_condition_variable.wait(lock, [this] {return !m_queue.empty(); });
        auto result = std::make_shared < T > (m_queue.front());
        m_queue.pop();

        return result;
    }

    bool /*try_*/pop(T & value) {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_queue.empty())
        {
            return false;
        }

        value = m_queue.front();
        m_queue.pop();

        return true;
    }

    std::shared_ptr < T > try_pop() {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_queue.empty())
        {
            return std::shared_ptr < T > ();
        }

        auto result = std::make_shared < T > (m_queue.front());
        m_queue.pop();

        return result;
    }

    bool empty() const
    {
        std::lock_guard < std::mutex > lock(m_mutex);
        return m_queue.empty();
    }

private:

    std::queue < T >		m_queue;
    std::condition_variable m_condition_variable;

private:

    mutable std::mutex m_mutex;
};

class ContainerTester {
public:
    template < typename ContainerT >
    void pushNumbers(std::size_t numOperations, ContainerT& stack) {
        std::unique_lock lock(m_mutex);

        m_conditionalVariable.wait(lock, [this](){return m_flag.load();});
        for (auto i = 0u; i < numOperations; ++i)
            stack.push(i);
    }

    template < typename Type, typename ContainerT >
    void popNumbers(std::size_t numOperations, ContainerT& stack) {
        Type val;
        std::unique_lock lock(m_mutex);

        m_conditionalVariable.wait(lock, [this](){return m_flag.load();});
        for (auto i = 0u; i < numOperations; ++i) {
            try {
            stack.pop(val);
        }
            catch (std::exception& exception) {}
        }
    }

    template < typename ContainerT >
    std::size_t containerStack(std::size_t numThreads, std::size_t numOperations) {
        std::vector <std::future<void>> pushThreads;
        std::vector <std::future<void>> popThreads;

        ContainerT container;

        for (auto i = 0u; i < numThreads; ++i) {
            pushThreads.push_back(std::async(std::launch::async,
                                             &ContainerTester::pushNumbers<ContainerT>, this,
                                             numOperations, std::ref(container)));
            popThreads.push_back(std::async(std::launch::async,
                                            &ContainerTester::popNumbers<std::size_t, ContainerT>, this,
                                            numOperations, std::ref(container)));
        }

        Timer timer;
            m_flag = true;
            m_conditionalVariable.notify_all();
            for (auto i = 0u; i < numThreads; ++i) {
                pushThreads[i].get();
                popThreads[i].get();
            }
        timer.stop();

        std::cout << numThreads << ' ' << "finished\n";

        return timer.getTime();
    }

    template < typename First, typename... ContainerTypes >
    void printResults(std::ostream& ostream, std::size_t numThreads, std::size_t numOperations) {
        ostream << containerStack<First>(numThreads, numOperations) << ' ';
        if constexpr (sizeof...(ContainerTypes)) {
            printResults<ContainerTypes...>(ostream, numThreads, numOperations);
        }
    }

    template < typename... ContainerTypes >
    void makeResults(std::ostream& ostream, std::size_t numThreads, std::size_t numOperations) {
        for (auto i = 1u; i <= numThreads; i += (i < 10) ? 1 : 10){
            ostream << i << ' ';
            printResults<ContainerTypes...>(ostream, i, numOperations);
            ostream << std::endl;
        }
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_conditionalVariable;
    std::atomic <bool> m_flag{false};
};

int main() {
//    const auto N = std::thread::hardware_concurrency() / 2;
    const auto N = 100u;
    const auto M = 1000000u;

    std::ofstream ofstream;

    ofstream.open("../HW_8/4th task _ COMPETITION/results boost stack and ts stack.txt");
    ContainerTester().makeResults<boost::lockfree::stack<std::size_t>, Threadsafe_Stack<std::size_t>>(ofstream, N, M);
    ofstream.close();

    ofstream.open("../HW_8/4th task _ COMPETITION/results boost queue and ts (try_pop) queue.txt");
    ContainerTester().makeResults<boost::lockfree::queue<std::size_t>, Threadsafe_Queue<std::size_t>>(ofstream, N, M);

    return 0;
}