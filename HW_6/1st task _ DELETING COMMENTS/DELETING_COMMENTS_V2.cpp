#include <fstream>
#include <vector>
#include <regex>
#include <iostream>

int main() {
    std::fstream testIn ("../HW_6/1st task _ DELETING COMMENTS/test.txt", std::ios::in);
    std::ofstream testOut ("../HW_6/1st task _ DELETING COMMENTS/result.txt");

    std::string code(
            (std::istreambuf_iterator <char> (testIn)),
            std::istreambuf_iterator <char> ()
            );

    for (auto it = std::begin(code); it != std::end(code); ++it) {
        if (*it == '/') {
            if (*std::next(it++) == '/') {
                for (; *it != '\n'; ++it);
            } else if (*it == '*') {
                for (; !((*std::prev(it) == '*') && (*it == '/')); ++it);
                ++it;
            }
        }

        if (*it == '"') {
            testOut << *(it++);
            for(; ((*it != '"') || ((*it == '"') && (*std::prev(it) == '\\'))); )
                testOut << *(it++);
        }

        if (it == std::end(code))
            break;

        testOut << *it;
    }

    return EXIT_SUCCESS;
}