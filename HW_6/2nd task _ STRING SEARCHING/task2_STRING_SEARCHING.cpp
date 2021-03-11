#include <fstream>
#include <iostream>

std::size_t getStringSize (std::ifstream& file) {
    file.seekg(0);
    char buf = 0;
    auto size = 0u;
        for (; buf != '\n' & buf != '\r'; ++size)
            file.get(buf);
        for (; buf == '\n' || buf == '\r'; ++size)
            file.get(buf);
    return size;
}

int main() {
    std::ifstream file("../HW_6/2nd task _ STRING SEARCHING/file_with_random_strings.txt");

    auto sizeOfStrings = getStringSize(file);

    std::cout << "Enter line number: " << std::endl;
    auto lineNumber = 0u;
    std::cin >> lineNumber;

    auto symbolNumber = lineNumber * (sizeOfStrings);

    std::string string;
    file.seekg(symbolNumber, std::ios::beg);
    std::getline(file, string);
    std::cout << string << std::endl;

    return EXIT_SUCCESS;
}