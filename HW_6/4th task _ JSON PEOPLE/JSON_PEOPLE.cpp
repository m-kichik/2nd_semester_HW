#include <fstream>
#include <filesystem>
#include <iostream>

#include "../../ClassWork/2021_03_11/json.hpp"
using nlohmann::json;

struct Person {
    std::string name;
    std::string surname;
    std::size_t age = 20u;
    std::string animal;
    std::string country;
    std::string city;
    std::string street;
};

std::istream& operator >> (std::istream& in,  Person& person) {
    in >> person.name >> person.surname >> person.age >> person.animal >> person.country >> person.city >> person.street;
    return in;
}

int main() {
    std::filesystem::create_directory("../HW_6/4th task _ JSON PEOPLE/directory");
    auto path = std::filesystem::absolute("..") / "HW_6/4th task _ JSON PEOPLE/directory";

    for (auto i = 0u; i < 5; ++i) {
        Person p;
        std::cin >> p;
        std::ofstream file(path / (p.name + ' ' + p.surname + ".txt"));
        json j;
        j["Name"] = p.name;
        j["Surname"] = p.surname;
        j["Age"] = p.age;
        j["Animal"] = p.animal;
        j["Address"]["Country"] = p.country;
        j["Address"]["City"] = p.city;
        j["Address"]["Street"] = p.street;
        file << std::setw(4) << j;
    }

    return 0;
}

// Copy:
/*
Egor Egorov 20 cat Russia Chelyabinsk Lenina
Ivan Ivanov 25 fish Russia Moskow Tverskaya
Petya Petrov 24 none Ukraine Kiev Tsvetnaya
Artyom Artyomov 21 dog Russia Dolgoprudny Pervomaiskaya
Aleksandr Aleksandrov 22 none Belarus Minsk Lenina
*/