#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

using namespace boost::multi_index;

struct PhoneNumber {
    std::string countryCode;
    std::string operatorCode;
    std::string abonentCode;
};

std::ostream& operator << (std::ostream& stream, const PhoneNumber& number)
{
    stream << number.countryCode << " (" << number.operatorCode << ") " << number.abonentCode;
    return stream;
}

struct Abonent {
    std::string surname;
    std::string name;
    PhoneNumber number;
};

using abonents_multi_index = multi_index_container <
        Abonent, indexed_by <
                // for typography
                ordered_non_unique < member < Abonent, std::string, &Abonent::surname >>,
                // for modeling agency
                random_access < >,
                // for random user
                hashed_non_unique < member < Abonent, std::string, &Abonent::surname >>
                        >>;

int main() {
    abonents_multi_index abonents;
    abonents.insert({"Makarov", "Ivan", {"8", "985", "9634545"}});
    abonents.insert({"Popov", "Nikita", {"7", "999", "5858539"}});
    abonents.insert({"Munitsina", "Maria", {"7", "916", "0636969"}});
    abonents.insert({"Glukhov", "Ilya", {"8", "916", "7568383"}});
    abonents.insert({"Judin", "Ivan", {"7", "917", "7550197"}});
    abonents.insert({"Golovko", "Andrew", {"7", "920", "8873401"}});
    abonents.insert({"Popov", "Pavel", {"8", "932", "5576211"}});

    auto& orderedSurnamesIndex = abonents.get < 0 > ();
    for (const auto& abonent : orderedSurnamesIndex)
        std::cout << abonent.surname << " " << abonent.name << ": " << abonent.number << std::endl;
    std::cout << std::endl;

    auto& randomAccessIndex = abonents.get < 1 > ();
    for (auto i = 0; i < randomAccessIndex.size(); i += 2)
        std::cout << randomAccessIndex[i].surname << " " << randomAccessIndex[i].name << ": " << randomAccessIndex[i].number << std::endl;
    std::cout << std::endl;

    auto& hashedSurnamesIndex = abonents.get < 2 > ();
    std::cout << hashedSurnamesIndex.find("Munitsina") -> number << std::endl;

    return 0;
}