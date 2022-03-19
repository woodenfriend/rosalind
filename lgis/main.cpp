#include "../common/error.hpp"
#include <iostream>
#include <vector>
#include <optional>
#include <iterator>
#include <algorithm>

template<typename T, class Comparer>
void findLongestSubsequence ( const std::vector<T>& s, Comparer&& comparer, std::vector<T>& subs )
{
	subs.clear ( );
	if ( s.empty() )
		return;

	struct Link
	{
		std::optional<size_t> i_prev;
		size_t max_steps = 0;
	};
	std::vector<Link> links ( s.size() );

	for ( size_t i_prev = 0; i_prev < s.size(); ++i_prev )
		for ( size_t i = i_prev + 1; i < s.size(); ++i )
		{
			if ( ! comparer( s[i_prev], s[i] ) )
				continue;

			const size_t steps = links[i_prev].max_steps + 1;
			if ( steps <= links[i].max_steps )
				continue;

			links[i].i_prev = i_prev;
			links[i].max_steps = steps;
		}

	size_t i_last = 0;
	for ( size_t i = 1; i < links.size(); ++i )
		if ( links[i].max_steps > links[i_last].max_steps )
			i_last = i;
	subs.reserve ( links[i_last].max_steps + 1 );
	for ( size_t i = i_last; ; )
	{
		subs.push_back ( s[i] );
		const auto& i_prev = links[i].i_prev;
		if ( ! i_prev )
			break;
		i = *i_prev;
	}
	std::reverse ( subs.begin(), subs.end() );
}

template<typename T>
void showSequence ( const std::vector<T>& s, std::ostream& os )
{
	std::ostream_iterator<T> i ( os, " " );
	for ( auto& v : s )
		*i = v;
	os << std::endl;
}

void run ( std::istream& is, std::ostream& os )
{
	int n;
	is >> n;
	check ( ! is.fail() && n > 0 );

	std::vector<int> v;
	v.resize ( n );
	for ( int& i : v )
		is >> i;
	check ( ! is.fail() );

	std::vector<int> inc;
	findLongestSubsequence ( v, std::less<int>(), inc );
	showSequence ( inc, os );

	std::vector<int> dec;
	findLongestSubsequence ( v, std::greater<int>(), dec );
	showSequence ( dec, os );
}

int main ( )
{
	try
	{
		run ( std::cin, std::cout );
	}
	catch ( std::exception& ex )
	{
		std::cout << "Error: " << ex.what() << std::endl;
		return 1;
	}
	return 0;
}
