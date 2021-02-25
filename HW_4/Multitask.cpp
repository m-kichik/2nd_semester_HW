#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

void printS(const std::string& str, const std::vector <int>& vec) {
    std::cout << str;
    std::for_each(std::begin(vec), std::end(vec),
                  [](auto x) {std::cout << x << ' ';});
    std::cout << std::endl;
}

int main () {
    std::uniform_int_distribution <int> generator (1, 100);
    std::default_random_engine engine(static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count()));

    // MAKE 1ST SEQUENCE (#1)
    std::vector <int> seq1(10);

    std::iota(std::begin(seq1), std::end(seq1), 1);

    printS("Seq1: ", seq1);

    // ADD MULTIPLE NUMBERS (#2)
    std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(),
         std::back_inserter(seq1));

    printS("Seq1 advanced: ", seq1);

    // SHUFFLE 1ST SEQUENCE (#3)
    std::shuffle(std::begin(seq1), std::end(seq1), engine);

    printS("Seq1 shuffled: ", seq1);

    // REMOVE DUPLICATES FROM SEQ1 (#4)
    std::unordered_set <int> container (std::begin(seq1), std::end(seq1));
    seq1.clear();
    for (const auto &x : container) seq1.push_back(x);
    std::reverse(seq1.begin(), seq1.end());

    // next variant works with sorted sequences, but it is not interesting to shuffle sequence and than sort it
    // seq1.erase(std::unique(std::begin(seq1), std::end(seq1)), std::end(seq1));

    printS("Seq1 without duplicates: ", seq1);

    // ODD NUMBERS IN SEQ1 (#5)
    auto oddNumbers = std::count_if(std::begin(seq1), std::end(seq1),
                                    [](auto x) { return (x % 2 == 1); });

    std::cout << "Number of odd numbers in seq1: " << oddNumbers << std::endl;

    // MIN AND MAX VALUE IN SEQ1 (#6)
    auto minElSeq1 = *std::min_element(std::begin(seq1), std::end(seq1));
    auto maxElSeq1 = *std::max_element(std::begin(seq1), std::end(seq1));

    // we also can use std::minmax_element

    std::cout << "Min in seq1: " << minElSeq1 << std::endl << "Max in seq1: " << maxElSeq1 << std::endl;

    // PRIME IN SEQ1 (#7)

    auto primeNumSeq1 = std::find_if(std::begin(seq1), std::end(seq1),
                                     [](auto x) {
                                         if (x <= 1)
                                             return false;

                                         for (auto i = 2; i < std::sqrt(x); ++i)
                                             if (x % i == 0)
                                                 return false;

                                         return true;
    });

    (primeNumSeq1 != std::end(seq1) ? std::cout << "Prime number in seq1: " << *primeNumSeq1 : std::cout
            << "There are no prime numbers in seq1 :(") << std::endl;

    // SQUARE EACH ELEMENT IN SEQ1 (#8)

    std::transform (std::begin(seq1), std::end(seq1), seq1.begin(),
                    [](auto x) { return x * x; });

    printS("Seq1 squared: ", seq1);

    // CREATE 2ND SEQUENCE (#9)
    std::vector <int> seq2;
    seq2.resize(std::size(seq1));
    std::generate(std::begin(seq2), std::end(seq2),
                  [&]() {return generator(engine);});

    printS("Seq2: ", seq2);

    // SUM OF ALL NUMBERS IN SEQ2 (#10)
    auto sumSeq2 = std::accumulate(std::begin(seq2), std::end(seq2), 0);
    std::cout << "Sum of seq2: " << sumSeq2 << std::endl;

    // REPLACE SEVERAL NUMBERS IN SEQ2 WITH 1 (#11)
    std::fill(std::begin(seq2), std::next(std::begin(seq2), 3), 1);

    printS("Seq2 with 1s: ", seq2);

    // MAKE SEQUENCE 3 AS DIFFERENCE BETWEEN SEQ1 AND SEQ2 (#12)
    std::vector <int> seq3;
    seq3.resize(std::size(seq1));
    std::transform(std::begin(seq1), std::end(seq1), std::begin(seq2), std::begin(seq3),
                   [](auto x, auto y) {return x - y;});

    printS("Seq3: ", seq3);

    // NEGATIVE -> NULL IN SEQ3 (#13)
    std::replace_if(std::begin(seq3), std::end(seq3),
                    [](auto x) { return x < 0; }, 0);

    printS("Seq3 without negative numbers: ", seq3);

    // REMOVE NULLS FROM SEQ3 (#14)
    seq3.erase(std::remove_if(std::begin(seq3), std::end(seq3),
                              [](auto x) { return x == 0; }), seq3.end());

    printS("Seq3 without nulls: ", seq3);

    // REVERSE SEQ3 (#15)
    std::reverse(std::begin(seq3), std::end(seq3));

    printS("Seq3 reversed: ", seq3);

    // TOP-3 MAX NUMBERS IN SEQ3 (#16)
    std::nth_element(std::begin(seq3), std::prev(std::end(seq3), 4), std::end(seq3));
    std::cout << "Top-3 max numbers in seq3: " << *std::prev(std::end(seq3), 1) << ' '
              << *std::prev(std::end(seq3), 2) << ' ' << *std::prev(std::end(seq3), 3) << std::endl;

    // SORT SEQ1 AND SEQ2 (#17)
    std::sort(std::begin(seq1), std::end(seq1));
    std::sort(std::begin(seq2), std::end(seq2));

    printS("Seq1 sorted: ", seq1);
    printS("Seq2 sorted: ", seq2);

    // MERGE SEQ1 AND SEQ2 (#18)
    std::vector<int> seq4;
    std::merge(std::begin(seq1), std::end(seq1), std::begin(seq2), std::end(seq2), std::back_inserter(seq4));

    printS("Seq4: ", seq4);

    // (#19)
    auto rangeFor1 = std::equal_range(std::begin(seq4), std::end(seq4), 1);

    (*std::begin(seq4) != 1 ? std::cout << "The element preceding the range for 1: " << *std::prev(rangeFor1.first, 1) : std::cout
            << "First element in seq4 is 1") << std::endl;

    (*std::end(seq4) != 1 ? std::cout << "The element following the range for 1: " << *rangeFor1.second : std::cout
            << "Last element in seq4 is 1") << std::endl;

    // PRINT ALL SEQUENCES (#20)
    std::cout << std::endl << "FINALLY:" << std::endl;
    printS("Seq1: ", seq1);
    printS("Seq2: ", seq2);
    printS("Seq3: ", seq3);
    printS("Seq4: ", seq4);

    return 0;
}