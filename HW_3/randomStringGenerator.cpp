#include <fstream>
#include <chrono>
#include <random>
#include <set>

int main() {
    std::uniform_int_distribution length(0, 10);
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> marker;
    auto size = marker.size();

    std::ofstream outf("randomString", std::ios::app);

    for (; marker.size() <= 2000000u;) {
        auto len = length(e);
        std::string str;
        for (; str.size() < static_cast<std::size_t>(len);) {
            str.push_back(static_cast<char>(letter(e)));
        }
        marker.insert(str);
        if (marker.size() != size) {
            outf << str << std::endl;
            size = marker.size();
        }
    }

    return 0;
}