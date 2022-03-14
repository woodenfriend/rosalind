#include "../common/error.hpp"
#include "../common/fasta.hpp"
#include <iostream>
#include <iterator>

const size_t min_length = 4;
const size_t max_length = 12;

char complement ( char c )
{
	switch ( c )
	{
        default: fail ( "bad nucleotide" );
		case 'A': return 'T';
		case 'T': return 'A';
		case 'G': return 'C';
		case 'C': return 'G';
	}
}

struct Location
{
	size_t position;
	size_t length;
};
std::ostream& operator<< ( std::ostream& s, const Location& location )
{
	s << ( location.position + 1 ) << ' ' << location.length;
	return s;
}

template<class OutputIterator>
void findReversePalindromes ( const std::string& s, OutputIterator output )
{
	const size_t min_expand = min_length / 2 + min_length % 2;
	const size_t max_expand = max_length / 2;

	const size_t n = s.length ( );
	for ( size_t i = 0, j = 1; j < n; ++i, ++j )
	{
		const size_t expand_limit = std::min( std::min( i + 1, n - j ), max_expand );
		if ( expand_limit < min_expand )
			continue;

		for ( size_t expand = 0; expand < expand_limit; )
		{
			if ( s[i-expand] != complement( s[j+expand] ) )
				break;
			if ( ++expand < min_expand )
				continue;
			*output = Location{ j - expand, expand * 2 };
		}
	}
}

void run ( )
{
	fasta::Entry f;
	fasta::readSingleEntry ( std::cin, f );

	std::ostream_iterator<Location> it ( std::cout, "\n" );
	findReversePalindromes ( f.data, it );
}

int main ( )
{
	try
	{
		run ( );
	}
	catch ( const std::exception& s )
	{
		std::cout << "error: " << s.what() << std::endl;
		return 1;
	}
    return 0;
}
