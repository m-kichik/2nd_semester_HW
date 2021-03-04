#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <streambuf>
#include <string>

int main() {
    std::ifstream inD("date.txt");
    std::ifstream inT("time.txt");
    std::string dateContainer((std::istreambuf_iterator<char>(inD)), std::istreambuf_iterator<char>());
    std::string timeContainer((std::istreambuf_iterator<char>(inT)), std::istreambuf_iterator<char>());

    std::regex pattern_d(R"((\d{0,4})\.(((0{0,1}[13-9])|(1[12]))\.(([12]\d)|(3[01])|(0{0,1}[1-9])))|((0{0,1}2)\.(([012]{0,1}\d))))");

    std::regex pattern_t(R"((((1\d)|(2\[0-3])|(0{0,1}\d))(\.(([1-5]\d)|(0{0,1}\d))){2}))");

    std::cout << "date: " << std::endl;

    std::copy(
            std::sregex_token_iterator(dateContainer.begin(), dateContainer.end(), pattern_d, { 0 }),
            std::sregex_token_iterator(),
            std::ostream_iterator < std::string >(std::cout, "\n"));

   std::cout << "time: " << std::endl;

    std::copy(
            std::sregex_token_iterator(timeContainer.begin(), timeContainer.end(), pattern_t, { 0 }),
            std::sregex_token_iterator(),
            std::ostream_iterator < std::string >(std::cout, "\n"));

    return 0;
}