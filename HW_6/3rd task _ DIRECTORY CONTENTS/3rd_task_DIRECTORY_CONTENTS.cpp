#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>

struct Entry {
    std::string name;
    std::string type;
    std::string lastWriteTime;
};

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                        + system_clock::now());
    return system_clock::to_time_t(sctp);
}

auto defineEntry (const std::filesystem::directory_entry& entry) {
    Entry result;
    result.name = entry.path().filename().string();
    result.type = (entry.is_block_file() || entry.is_character_file() || entry.is_regular_file()) ?
            "file" : entry.is_directory() ? "directory" : "other";
    auto lrt = to_time_t(entry.last_write_time());
    result.lastWriteTime = std::asctime(std::localtime(&lrt));

    return result;
}

int main() {
    auto path = std::filesystem::absolute("..") / "HW_3";

    if (std::filesystem::exists(path)) {
        std::cout << path.string() << " contains:\n" << std::endl;

        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            auto item = defineEntry(entry);
            std::cout << std::setfill('.') << std::setw(20) << std::left << "name:" <<
                      std::setfill('.') << std::setw(40) << std::right << item.name << std::endl;
            std::cout << std::setfill('.') << std::setw(20) << std::left << "type:" <<
                      std::setfill('.') << std::setw(40) << std::right << item.type << std::endl;
            std::cout << std::setfill('.') << std::setw(20) << std::left << "last write time:" <<
                      std::setfill('.') << std::setw(40) << std::right << item.lastWriteTime << std::endl;
            std::cout << std::endl;
        }
    }

    return 42;
}