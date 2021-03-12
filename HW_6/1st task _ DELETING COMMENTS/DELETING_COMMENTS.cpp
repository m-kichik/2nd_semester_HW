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

    std::regex strLitPattern (R"([\"]([*[ *!\".,:;()\\\/]*\w*)*[\"])");
    std::regex commentPattern (R"((/{2}([ !\".,:;()\\\/]*\w*)*)|(/\*([![:space:]\".,:;()\\\/]*\w*)*\*/))");

    std::vector <std::string> expressions;

    std::copy(
            std::sregex_token_iterator(std::begin(code), std::end(code), strLitPattern, { 0 }),
            std::sregex_token_iterator(),
            std::back_inserter(expressions));

    const std::string toReplaceStrLit = "IT_IS_A_STRING_LITERAL";

    code = std::regex_replace(code, strLitPattern, toReplaceStrLit);

    code = std::regex_replace(code, commentPattern, "");

    for (const auto& expression : expressions)
        code.replace(code.find(toReplaceStrLit), std::size(toReplaceStrLit), expression);

    testOut << code << std::endl;

    std::cout << "code executed successfully";

    return EXIT_SUCCESS;
}