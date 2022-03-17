#include "../common/error.hpp"
#include "../common/protein.hpp"
#include "../common/fasta.hpp"
#include <boost/container/static_vector.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <optional>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>

template<class Iterator, class OutIterator>
void buildProteins ( Iterator begin, Iterator end, OutIterator output )
{
	const auto& start = aa2codon ( AA::START );
	const auto& stop  = aa2codon ( AA::STOP );

	for ( ; begin != end; ++begin )
	{
		boost::container::static_vector<char, 3> buf;
		Iterator i = begin;

		std::optional<Codon> start_c;
		for ( ; i != end; ++i )
		{
			buf.push_back ( *i );

			bool match = false;
			for ( const auto& c : start )
				if ( std::equal( buf.begin(), buf.end(), c.n ) )
				{
					match = true;
					break;
				}
			if ( ! match )
				break;
			if ( buf.size() < 3 )
				continue;

			start_c.emplace ( buf.data() );
			buf.clear ( );

			++i;
			break;
		}
		if ( ! start_c )
			continue;

		std::string protein;
		protein.push_back ( aa2symbol( codon2aa( *start_c ) ) );

		std::optional<Codon> stop_c;
		for ( ; i != end; ++i )
		{
			buf.push_back ( *i );

			if ( buf.size() < 3 )
				continue;

			Codon c ( buf.data() );
			buf.clear ( );
			if ( std::find( stop.begin(), stop.end(), c ) != stop.end() )
			{
				stop_c = c;
				break;
			}

			protein.push_back ( aa2symbol( codon2aa( c ) ) );
		}
		if ( ! stop_c )
			continue;

		if ( ! protein.empty() )
			*output = protein;
	}
}

void run ( )
{
	fasta::Entry f;
	fasta::readSingleEntry ( std::cin, f );

	std::set<std::string> proteins;

	buildProteins (
		f.data.begin(),
		f.data.end(),
		std::inserter( proteins, proteins.end() )
	);

	auto complement = []( char c )
	{
		switch ( c )
		{
			default: fail ( "bad nucleotide" );
			case 'A': return 'T';
			case 'T': return 'A';
			case 'G': return 'C';
			case 'C': return 'G';
		}
	};
	buildProteins (
		boost::make_transform_iterator( f.data.rbegin(), complement ),
		boost::make_transform_iterator( f.data.rend(), complement ),
		std::inserter( proteins, proteins.end() )
	);

	for ( const auto& protein : proteins )
		std::cout << protein << std::endl;
}

int main ( )
{
	try
	{
		run ( );
	}
	catch ( std::exception& s )
	{
		std::cout << "error: " << s.what() << std::endl;
		return 1;
	}
    return 0;
}
