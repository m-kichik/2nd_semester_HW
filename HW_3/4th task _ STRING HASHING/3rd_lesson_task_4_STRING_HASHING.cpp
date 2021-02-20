#include <chrono>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>

struct HashFunctionsData {
    std::function <unsigned int(const char*, unsigned int)> hashFunction;
    std::set <std::size_t> hashedWords;
    std::size_t numberOfCollisions = 0u;
};

std::set <std::string> makeRandomWords (std::size_t N, std::size_t length) {
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine  e(static_cast <std::size_t> (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() <= 2000000u; words.insert(str))
        for (auto &c : str)
            c = letter(e);

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

void testHashFunctions(std::size_t N, std::size_t length, std::size_t numberOfHashFunctions) {
    std::ofstream outf("CollisionsInDifferentHashFunctions.txt");

    std::set <std::string> randWords = makeRandomWords(N, length);

    std::unordered_map <std::string, HashFunctionsData> hashFunctions =
            {{"rsCollisions", {RSHash}}, {"jsCollisions", {JSHash}}, {"pjwCollisions", {PJWHash}},
             {"elfCollisions", {ELFHash}}, {"bkdrCollisions", {BKDRHash}}, {"sdbmCollisions", {SDBMHash}},
             {"djbCollisions", {DJBHash}}, {"dekCollisions", {DEKHash}}, {"apCollisions", {APHash}}};

    std::vector hashNames ({"rsCollisions", "jsCollisions", "pjwCollisions", "elfCollisions",
                            "bkdrCollisions", "sdbmCollisions", "djbCollisions", "dekCollisions", "apCollisions"});

    for (auto name : hashNames)
        outf << name << ' ';
    outf << std::endl;

    auto counter = 0u;

    for (const auto& word: randWords) {
        for (auto step = 0u; step < numberOfHashFunctions; ++step) {
            auto& hashFuncData = hashFunctions[hashNames[step]];
            if (!hashFuncData.hashedWords.insert(
                    hashFuncData.hashFunction(word.c_str(), length)).second)
                hashFuncData.numberOfCollisions++;
        }

        if (counter % 10000 == 0) {
            outf << counter;
            for (auto funcN = 0u; funcN < numberOfHashFunctions; ++funcN)
                outf << ' ' << hashFunctions[hashNames[funcN]].numberOfCollisions;
            outf << std::endl;
        }

        counter ++;
    }
}

int main() {
    testHashFunctions(2000001u, 10u, 9u);

    return 0;
}