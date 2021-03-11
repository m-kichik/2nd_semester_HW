#ifndef _HAS_AUTO_PTR_ETC
#define _HAS_AUTO_PTR_ETC 1
#endif

#include <codecvt>
#include <iostream>
#include <map>
#include <string>

#include <boost/locale.hpp>

#include <windows.h>

std::string convert_locale_to_utf(const std::string & string) {
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::to_utf < char >(string, locale);
}

std::string convert_utf_to_locale(const std::string & string) {
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::from_utf < char >(string, locale);
}

std::unordered_map <char32_t, std::u32string> makeTranslitDictionary () {
        std::unordered_map <char32_t, std::u32string> translitDictionary = {{U'А', U"A"}, {U'а', U"a"},
                                                                            {U'Б', U"B"}, {U'б', U"b"},
                                                                            {U'В', U"V"}, {U'в', U"v"},
                                                                            {U'Г', U"G"}, {U'г', U"g"},
                                                                            {U'Д', U"D"}, {U'д', U"d"},
                                                                            {U'Е', U"E"}, {U'е', U"e"},
                                                                            {U'Ё', U"Yo"}, {U'ё', U"yo"},
                                                                            {U'Ж', U"Zh"}, {U'ж', U"zh"},
                                                                            {U'З', U"Z"}, {U'з', U"z"},
                                                                            {U'И', U"I"}, {U'и', U"i"},
                                                                            {U'Й', U"J"}, {U'й', U"j"},
                                                                            {U'К', U"K"}, {U'к', U"k"},
                                                                            {U'Л', U"L"}, {U'л', U"l"},
                                                                            {U'М', U"M"}, {U'м', U"m"},
                                                                            {U'Н', U"N"}, {U'н', U"n"},
                                                                            {U'О', U"O"}, {U'о', U"o"},
                                                                            {U'П', U"P"}, {U'п', U"p"},
                                                                            {U'Р', U"R"}, {U'р', U"r"},
                                                                            {U'С', U"S"}, {U'с', U"s"},
                                                                            {U'Т', U"T"}, {U'т', U"t"},
                                                                            {U'У', U"U"}, {U'у', U"u"},
                                                                            {U'Ф', U"Ph"}, {U'ф', U"ph"},
                                                                            {U'Х', U"H"}, {U'х', U"h"},
                                                                            {U'Ц', U"Ts"}, {U'ц', U"ts"},
                                                                            {U'Ч', U"Ch"}, {U'ч', U"ch"},
                                                                            {U'Ш', U"Sh"}, {U'ш', U"sh"},
                                                                            {U'Щ', U"Sch"}, {U'щ', U"sch"},
                                                                            {U'Ы', U"I"}, {U'ы', U"i"},
                                                                            {U'ъ', U""}, {U'ь', U"'"},
                                                                            {U'Э', U"E"}, {U'э', U"e"},
                                                                            {U'Ю', U"Yu"}, {U'ю', U"yu"},
                                                                            {U'Я', U"Ya"}, {U'я', U"ya"},
                                                                            {U' ', U" "}, {U'.', U"."},
                                                                            {U',', U","}, {U'!', U"!"},
                                                                            {U'?', U"?"}, {U':', U":"},
                                                                            {U';', U";"}, {U'"', U"\""}};

        return translitDictionary;
};

int main() {
    //system("chcp 1251");
    std::string expression;
    std::getline(std::cin, expression);
    //std::cin >> expression;

    std::string u8expression = convert_locale_to_utf(expression);
    std::u32string u32expression = boost::locale::conv::utf_to_utf<char32_t>(u8expression);
    //std::u32string u32expression = boost::locale::conv::utf_to_utf<char32_t>(expression);

    auto translitDictionary = makeTranslitDictionary();

    std::u32string u32translittedExpression;

    for (auto x: u32expression)
        u32translittedExpression += translitDictionary[x];

    std::string u8translittedExpression = boost::locale::conv::utf_to_utf<char>(u32translittedExpression);
    //std::string translittedExpression = boost::locale::conv::utf_to_utf<char>(u32translittedExpression);

    std::string translittedExpression = convert_utf_to_locale(u8translittedExpression);

    std::cout << translittedExpression;

    return 0;
}