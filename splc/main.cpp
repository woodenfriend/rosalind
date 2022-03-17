#include "../common/error.hpp"
#include "../common/protein.hpp"
#include "../common/fasta.hpp"
#include <boost/container/static_vector.hpp>
#include <optional>
#include <iostream>
#include <algorithm>

void translateProtein ( const std::string& s, std::string& protein )
{
	const auto& start = aa2codon ( AA::START );
	const auto& stop  = aa2codon ( AA::STOP );

	for ( auto begin = s.begin(), end = s.end(); begin != end; ++begin )
	{
		boost::container::static_vector<char, 3> buf;
		auto i = begin;

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

		break;
	}
}

void run ( )
{
	std::optional<std::string> data;
	fasta::readSeries ( std::cin,
		[&data]( fasta::Entry&& f )
		{
			if ( ! data )
				data = std::move ( f.data );
			else
				for ( size_t pos; ( pos = data->find( f.data ) ) != std::string::npos; )
					data->erase ( pos, f.data.length() );
		}
	);
	validate ( !! data, "no data" );

	std::string protein;
	translateProtein ( *data, protein );
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
