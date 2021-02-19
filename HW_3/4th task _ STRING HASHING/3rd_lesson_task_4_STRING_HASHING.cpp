#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>

std::set <std::string> makeRandomWords (std::size_t N, std::size_t length) {
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (; words.size() <= 2000000u;) {
        std::string str;
        for (; str.size() < length;) {
            str.push_back(static_cast<char>(letter(e)));
        }
        words.insert(str);
    }

    return words;
}

unsigned int RSHash(const char* str, unsigned int length) {
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }

    return hash;
}

unsigned int JSHash(const char* str, unsigned int length) {
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

unsigned int PJWHash(const char* str, unsigned int length) {
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          =
            (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length) {
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << 4) + (*str);

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length) {
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length) {
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned int DJBHash(const char* str, unsigned int length) {
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}

unsigned int DEKHash(const char* str, unsigned int length) {
    unsigned int hash = length;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }

    return hash;
}

unsigned int APHash(const char* str, unsigned int length) {
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }

    return hash;
}

std::vector <std::function<unsigned int (const char*, unsigned int)>> makeVectorOfHashFunctions() {
    std::vector <std::function<unsigned int (const char*, unsigned int)>> vectorOfHashFunctions;
    vectorOfHashFunctions.emplace_back(RSHash);
    vectorOfHashFunctions.emplace_back(JSHash);
    vectorOfHashFunctions.emplace_back(PJWHash);
    vectorOfHashFunctions.emplace_back(ELFHash);
    vectorOfHashFunctions.emplace_back(BKDRHash);
    vectorOfHashFunctions.emplace_back(SDBMHash);
    vectorOfHashFunctions.emplace_back(DJBHash);
    vectorOfHashFunctions.emplace_back(DEKHash);
    vectorOfHashFunctions.emplace_back(APHash);
    return vectorOfHashFunctions;
}

void testHashFunctions(std::size_t N, std::size_t length) {
    std::ofstream outf("CollisionsInDifferentHashFunctions.txt");
    std::set <std::string> st = makeRandomWords(N, length);
    std::vector <std::string> randWords(st.begin(), st.end());

    auto vectorOfHashFunctions = makeVectorOfHashFunctions();

    std::vector <std::pair<std::string, std::size_t>> hashFunctionsAndCollisions =
            {{"rsCollisions", 0u}, {"jsCollisions", 0u}, {"pjwCollisions", 0u},
             {"elfCollisions", 0u}, {"bkdrCollisions", 0u}, {"sdbmCollisions", 0u},
             {"djbCollisions", 0u}, {"dekCollisions", 0u}, {"apCollisions", 0u}};

    for (auto element: hashFunctionsAndCollisions)
        outf << element.first << ' ';

    outf << std::endl;

    std::vector <std::set<std::size_t>> vectorOfSets(9);



        for (auto randWordIterator = 0u; randWordIterator < N; ++randWordIterator) {
                for (auto hFuncIterator = 0u; hFuncIterator < 9; ++hFuncIterator) {
                    if (!vectorOfSets[hFuncIterator].insert(vectorOfHashFunctions[hFuncIterator](randWords[randWordIterator].c_str(), length)).second)
                        hashFunctionsAndCollisions[hFuncIterator].second++;
                }
            if (randWordIterator % 10000 == 0) {
                outf << randWordIterator << ' ';
                for (auto element: hashFunctionsAndCollisions)
                    outf << element.second << ' ';
                outf << std::endl;
            }
        }

}

int main() {
    testHashFunctions(2000001, 10);

    return 0;
}