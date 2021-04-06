#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>

#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

class ChatMaker {
private:
    using manager_t = boost::interprocess::managed_shared_memory::segment_manager;
    using stringAllocator = boost::interprocess::allocator < char, manager_t >;
    using sharedString_t = boost::interprocess::basic_string < char, std::char_traits<char>, stringAllocator >;
    using listAllocator = boost::interprocess::allocator < sharedString_t, manager_t >;
    using sharedStringList_t = boost::interprocess::list < sharedString_t, listAllocator >;

private:
    const std::string sharedMemoryName = "sharedMemory";
    const std::string storageName = "storage";
    const std::string numUsersName = "numberUsers";
    const std::string numMessagesName = "numberMessages";
    const std::string mutexName = "mutex";
    const std::string conditionName = "condition";

public:
    ChatMaker(std::string name = "user") : sharedMemory(boost::interprocess::open_or_create,
                   sharedMemoryName.c_str(), memSize),
                   strAlloc(sharedMemory.get_segment_manager()),
                   userName(std::move(name)) {
        storage = sharedMemory.find_or_construct< sharedStringList_t >(storageName.c_str())(strAlloc);
        mutex = sharedMemory.find_or_construct< boost::interprocess::interprocess_mutex >(mutexName.c_str())();
        condition = sharedMemory.find_or_construct< boost::interprocess::interprocess_condition >(conditionName.c_str())();
        numberUsers = sharedMemory.find_or_construct< std::atomic < std::size_t > >(numUsersName.c_str())();
        numberMessages = sharedMemory.find_or_construct< std::atomic < std::size_t > >(numMessagesName.c_str())();
        boost::interprocess::scoped_lock lock(*mutex);
        localNumMessages = *numberMessages;
        (*numberUsers) ++;
        leftChat = false;
    }

    ~ ChatMaker() = default;

private:
    void printPreviuosMesages() {
        std::cout << "Previous messages:" << std::endl;
        boost::interprocess::scoped_lock lock(*mutex);
        for (const auto& prevMes : *storage)
            std::cout << prevMes.c_str() << std::endl;
    }

    void addMessage(const std::string& prepMessage) {
        sharedString_t message(prepMessage.c_str(), strAlloc);

        boost::interprocess::scoped_lock lock(*mutex);
        if (std::size(*storage) == largestNumberMessages)
            storage->pop_front();
        storage -> push_back(message);
        (*numberMessages) ++;
        localNumMessages ++;
        condition->notify_all();
    }

    void messaging() {
        std::string rowMessage;
        const char* sep = ": ";
        for (; std::getline(std::cin, rowMessage);) {
            std::string prepMessage = userName + sep + rowMessage;
            addMessage(prepMessage);
        }
    }

    void reading() {
        if (!storage->empty())
            printPreviuosMesages();
        addMessage(userName + " started messaging!");
        std::cout << "Start messaging: " << std::endl;
        for (;;) {
            std::unique_lock lock(*mutex);
            condition->wait(lock, [this]() { return localNumMessages != *numberMessages || leftChat; });
            if (leftChat) break;
            localNumMessages = *numberMessages;
            std::string message = storage->back().c_str();
            std::cout << message << std::endl;
        }
    }

    void leaveChat(std::thread& reader) {
        leftChat = true;
        condition->notify_all();
        reader.join();
        (*numberUsers) --;
        addMessage(userName + " left chat");
        if (!(*numberUsers))
            boost::interprocess::shared_memory_object::remove(sharedMemoryName.c_str());
    }

public:
    void startMessaging() {
        auto reader = std::thread(&ChatMaker::reading, this);
        messaging();
        leaveChat(reader);
    }

private:
    const std::size_t memSize = 8000u;
    const std::size_t largestNumberMessages = 100u;
    boost::interprocess::managed_shared_memory sharedMemory;
    std::atomic < std::size_t >* numberUsers;
    sharedStringList_t* storage;
    std::atomic < std::size_t >* numberMessages;
    std::size_t localNumMessages;
    stringAllocator strAlloc;
    boost::interprocess::interprocess_mutex* mutex;
    boost::interprocess::interprocess_condition* condition;
    std::string userName;
    std::atomic < bool > leftChat;
};

int main() {
    std::string name;
    std::cout << "Enter your name:" << std::endl;
    std::getline(std::cin, name);
    ChatMaker(name).startMessaging();

    system("pause");

    return 0;
}