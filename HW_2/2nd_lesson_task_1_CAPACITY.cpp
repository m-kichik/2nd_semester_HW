#include <iostream>
#include <utility>
#include <vector>

int main()
{
    // OVERFLOW OF THE INT AND BOOL ARRAYS: capacity increase coefficient in MinGW is 2, in MS VS it is 1,5
    {
        std::vector<std::size_t> sizeV;
        for (auto i = 1u, j = 10u; i <= 7; ++i) {
            sizeV.push_back(j);
            j *= 10;
        }

        std::cout << std::endl << "INT:" << std::endl;

        for (unsigned int i : sizeV)
        {
            std::vector<int> v(i);
            auto capB = v.capacity();
            v.push_back(42);
            auto capA = v.capacity();
            std::cout << i << ' ' << static_cast <double> (capA) / capB << std::endl;
        }

        std::cout << std::endl << "BOOL:" << std::endl;

        for (unsigned int i : sizeV)
        {
            std::vector<bool> v(i);
            auto capB = v.capacity();
            v.push_back(true);
            auto capA = v.capacity();
            std::cout << i << ' ' << static_cast <double> (capA) / capB << std::endl;
        }

        std::cout << std::endl;
    }

    // SOME EFFECT OF THE BOOL ARRAY OVERFLOW: least capacity is 32 (?)
    {
        std::vector<bool> v;
        std::vector<int> vv;

        for (auto i = 0u; i <= 5; ++i)
        {
            std::cout << i << "    " << v.capacity() << "    " << vv.capacity() << std::endl;
            v.push_back(true);
            vv.push_back(42);
        }

        std::cout << std::endl;
    }

    // RESERVE OVERFLOW: nothing changed...
    {
        const auto N = 1000u;
        std::vector <int> v;
        v.reserve(N);
        std::cout << "size: " << v.size() << " capacity: " << v.capacity() << std::endl;
        for (auto i = 0u; i <= N; ++i)
            v.push_back(42);
        std::cout << "size: " << v.size() << " capacity: " << v.capacity() << std::endl;

        std::cout << std::endl;
    }

    // HEAP OVERFLOW (bad alloc): the memory allocation mechanism has not changed...
    {
        const auto N = 90811045u; // 67108864 - max MinGW, 90811045 - max MS Visual Studio
        std::vector <long long> v;
        std::vector <double> capacities;
        capacities.push_back(static_cast<double>(v.capacity()));

        for (auto i = 0u; i < N; ++i)
        {
            if(capacities.back() < v.capacity())
            {
                std::cout << v.size() << ' ' << v.capacity() << std::endl;
                capacities.push_back(static_cast<double>(v.capacity()));
            }
            v.push_back(42);
        }

        std::vector normCapacities(capacities);

        for(auto i = normCapacities.size() - 1; i > 0; --i)
        {
            normCapacities.at(i) = normCapacities.at(i) / normCapacities.at(i - 1);
        }

        std::cout << std::endl;

        for (auto x : normCapacities)
        std::cout << x << std::endl;
    }

    return 0;
}