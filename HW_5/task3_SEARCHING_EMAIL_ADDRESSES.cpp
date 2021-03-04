#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <streambuf>
#include <string>

int main() {
    std::ifstream inf("text.txt");
    std::string textContainer((std::istreambuf_iterator<char>(inf)), std::istreambuf_iterator<char>());

    std::regex pattern(R"(( [[:alnum:]]{1,}([[:alnum:]]*[_\.-]{0,1}[[:alnum:]]{1,})*)@([[:alnum:]]{1,}([[:alnum:]]*[_\.-]{0,1}[[:alnum:]]{1,})*\.[[:alnum:]]{2,}))");

    std::copy(
            std::sregex_token_iterator(textContainer.begin(), textContainer.end(), pattern, { 3 }),
            std::sregex_token_iterator(),
            std::ostream_iterator < std::string >(std::cout, "\n"));

    return 0;
}