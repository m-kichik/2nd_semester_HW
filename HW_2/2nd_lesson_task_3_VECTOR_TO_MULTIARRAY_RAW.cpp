#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

#include <boost/multi_array.hpp>

template < auto N, typename Container, typename Forward_Iterator >
void fill_shape(const Container & container, Forward_Iterator shape)
{
	*shape = std::size(container);

	if constexpr (N > 1)
	{
		fill_shape < N - 1 > (*(std::begin(container)), ++shape);
	}
}

template < auto N, typename Container, typename Forward_Iterator >
void fill_multi_array(const Container & container, Forward_Iterator multi_array)
{
	// Write your code here ...

	if constexpr (N > 1)
    {
	    for(auto i = 0u; i < container.size(); i++)
	    {
	        fill_multi_array< N - 1 >(container[i], std::next(multi_array, i) -> begin());
	    }
    }
	else
    {
	    for(auto& item : container)
	    {
	        *(multi_array++) = item;
	    }
    }

	// USE: recursive template instantiation of fill_multi_array
	// USE: std::next(multi_array, i)->begin() in recursion
	// USE: if constexpr (N > 1) { ... } else { ... }

	// Enjoy debugging!
}

template < typename T, auto N, typename Container >
auto make_multi_array(const Container & container)
{
	using multi_array_t = boost::multi_array < T, N > ;

	std::vector < typename multi_array_t::index > shape(N, typename multi_array_t::index(0)); // vector made of N nulls

	fill_shape < N > (container, std::begin(shape)); // fill shape with "size" of directions

	multi_array_t multi_array(shape); // create multi_array

	fill_multi_array < N > (container, std::begin(multi_array));

	return multi_array;
}

int main()
{
	const auto size_1 = 3U;
	const auto size_2 = 4U;
	const auto size_3 = 5U;

	// make vector of vectors of vectors
	std::vector < std::vector < std::vector < int > > > v(size_1, // initialize it with size1 and items
                                                                  // -- vectors of vectors
		std::vector < std::vector < int > > (size_2,         // initialize each vector of vectors with items
                                                                  // -- vectors of integer
			std::vector < int > (size_3, 0)));          // initialize them with nulls

	std::cout << "std::vector < std::vector < std::vector < int > > >\n" << std::endl;

    // fill vector using counter... and print it
	auto counter = 0;

	for (auto i = 0U; i < size_1; ++i)
	{
		for (auto j = 0U; j < size_2; ++j)
		{
			for (auto k = 0U; k < size_3; ++k)
			{
				std::cout << std::setw(2) << std::right << (v[i][j][k] = ++counter) << " ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	// make multy_array using multy_vector
	auto multi_array = make_multi_array < int, 3U > (v);

	std::cout << "boost::multi_array\n" << std::endl;

	for (auto i = 0U; i < size_1; ++i)
	{
		for (auto j = 0U; j < size_2; ++j)
		{
			for (auto k = 0U; k < size_3; ++k)
			{
				std::cout << std::setw(2) << std::right << multi_array[i][j][k] << " ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}