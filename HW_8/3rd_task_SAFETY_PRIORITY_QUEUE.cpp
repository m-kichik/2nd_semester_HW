#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

template <typename Type, typename ContainerType = std::vector<Type>, typename Compare = std::less<typename ContainerType::value_type>>
class  ThreadsafePriorityQueue {
public:
    ThreadsafePriorityQueue() = default;

    ThreadsafePriorityQueue(const ThreadsafePriorityQueue & other)
    {
        std::scoped_lock lock(other.m_mutex);
        m_priorityQueue = other.m_priorityQueue;
    }

    ThreadsafePriorityQueue & operator=(const ThreadsafePriorityQueue & other)
    {
        std::scoped_lock lock(m_mutex, other.m_mutex);
        m_priorityQueue = other.m_priorityQueue;
    }

public:
    Type tryTop() {
        if (m_priorityQueue.empty()) {
            return Type();
        }

        std::scoped_lock lock(m_mutex);
        return m_priorityQueue.top();
    }

public:
    bool empty() {
        std::scoped_lock lock(m_mutex);
        return m_priorityQueue.empty();
    }

    std::size_t size() {
        std::scoped_lock lock(m_mutex);
        return m_priorityQueue.size();
    }

public:
    void push(const Type& value) {
        std::scoped_lock lock(m_mutex);
        m_priorityQueue.push(value);
        m_conditionVariable.notify_one();
    }

    void push(Type&& value) {
        std::scoped_lock lock(m_mutex);
        m_priorityQueue.push(std::move(value));
        m_conditionVariable.notify_one();
    }

    template <typename... Args>
    void emplace(Args&&... value) {
        std::scoped_lock lock(m_mutex);
        m_priorityQueue.emplace(value...);
    }

    void waitPop(Type & value) {
        std::unique_lock < std::mutex > lock(m_mutex);

        m_conditionVariable.wait(lock, [this] {return !m_priorityQueue.empty(); });
        value = m_priorityQueue.top();
        m_priorityQueue.pop();
    }

    std::shared_ptr < Type > waitPop() {
        std::unique_lock < std::mutex > lock(m_mutex);

        m_conditionVariable.wait(lock, [this] {return !m_priorityQueue.empty(); });
        auto result = std::make_shared < Type > (m_priorityQueue.top());
        m_priorityQueue.pop();

        return result;
    }

    bool tryPop(Type & value) {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_priorityQueue.empty()) {
            return false;
        }

        value = m_priorityQueue.top();
        m_priorityQueue.pop();

        return true;
    }

    std::shared_ptr < Type > tryPop() {
        std::lock_guard < std::mutex > lock(m_mutex);

        if (m_priorityQueue.empty()) {
            return std::shared_ptr < Type > ();
        }

        auto result = std::make_shared < Type > (m_priorityQueue.top());
        m_priorityQueue.pop();

        return result;
    }

    void swap(ThreadsafePriorityQueue & other) {
        std::scoped_lock (m_mutex, other.m_mutex);
        m_priorityQueue.swap(other.m_priorityQueue);
    }

private:
    std::condition_variable m_conditionVariable;
    mutable std::mutex m_mutex;
    std::priority_queue<Type, ContainerType, Compare> m_priorityQueue;
};

int main() {
    ThreadsafePriorityQueue <int, std::vector<int>> tsq;
    auto b = tsq.empty();
    tsq.emplace(4);
    tsq.push(2);

    ThreadsafePriorityQueue <int, std::vector<int>> tsq2(tsq);

    auto s = tsq.size();
    auto n = tsq.tryTop();

    int val = 0;
    tsq2.tryPop(val);
    tsq2.waitPop();
    auto tp = tsq2.tryPop(val);

    tsq.swap(tsq2);

    std::cout << std::boolalpha << b << ' ' << s << ' ' << n << ' ' << val << ' ' << tp << std::endl;

    return 0;
}