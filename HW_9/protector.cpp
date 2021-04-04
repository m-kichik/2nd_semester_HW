#include <iostream>

#include <boost/interprocess/managed_shared_memory.hpp>

int main() {
    const std::string sharedMemoryName = "sharedMemory";
    const std::string numUsersName = "numberUsers";
    const std::size_t memSize = 8000u;

    boost::interprocess::shared_memory_object::remove(sharedMemoryName.c_str());

    boost::interprocess::managed_shared_memory sharedMemory(boost::interprocess::open_or_create,
                                                            sharedMemoryName.c_str(), memSize);
    std::atomic < std::size_t >* numberUsers = sharedMemory.construct< std::atomic < std::size_t > >(numUsersName.c_str())();
    *numberUsers = 0u;

    std::cout << "numUsers " << *numberUsers << std::endl;

    std::cout << "chat protected" << std::endl;

    system("pause");

    return 0;
}
