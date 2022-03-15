#include "../common/error.hpp"
#include <iostream>
#include <limits>
#include <cmath>
#include <tbb/parallel_reduce.h>
#include <tbb/blocked_range.h>

template<typename T>
void run ( )
{
	T n, k;
	std::cin >> n >> k;
	validate ( ! std::cin.fail() );
	validate ( k > 0 && n > 0 && k <= n );

	auto mod_mul = []( T lhs, T rhs )
	{
		const T modulo = 1000000;
		static_assert ( modulo < std::sqrt( std::numeric_limits<T>::max() ), "overflow may occur" );
		return ( lhs * rhs ) % modulo;
	};

	const size_t grainsize = 20;
	const T result = tbb::parallel_reduce (
		tbb::blocked_range<T>( n-k+1, n+1, grainsize ), T( 1 ),
		[&mod_mul]( const tbb::blocked_range<T>& r, T v )
		{
			for ( T i = r.begin(); i < r.end(); ++i )
				v = mod_mul ( v, i );
			return v;
		},
		mod_mul
	);

	std::cout << result << std::endl;
}

int main ( )
{
	try
	{
		run<long> ( );
	}
	catch ( const std::exception& s )
	{
		std::cout << "error: " << s.what() << std::endl;
		return 1;
	}
	return 0;
}
